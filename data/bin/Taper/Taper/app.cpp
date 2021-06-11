#include "pch.h"
#include "app.h"
#include "wechat.h"

/// <summary>
/// APP单例初始化
/// </summary>
/// <returns></returns>
App& App::Instance()
{
    static App app;
    return app;
}

App::~App()
{
    Destroy_HP_TcpPackClient(pClient_);
    Destroy_HP_TcpPackClientListener(pListener_);
}

namespace {

enum class DataType :UINT
{
    kRobotLogin = 1001, //机器人登录
    kRobotMsg = 1002, //机器人消息
    kAppInstall = 1003, //应用安装
    kAppUninstall = 1004, //应用卸载
    kAppStart = 1005, //应用开启
    kAppStop = 1006, //应用停止
    kAppLog = 1007, //应用日志
    kAppMsg = 1008, //应用消息
    kAppMenu = 1009, //应用菜单
    kAppForwardMsg = 1010 //应用转发消息
};

enum class MsgType :UINT
{
    kText = 1,
    kNotifyText = 2,
    kPic = 3,
    kLink = 5,
    kFile = 6,
    kApplet = 33,
    kVoice = 34,
    kFriendReq = 37,
    kFriendCard = 42,
    kVideo = 43,
    kGif = 47,
    kLocation = 48,
    kXml = 49,
    kTransfer = 2000,
    kSystem = 10000
};

En_HP_HandleResult __HP_CALL OnConnect(HP_Client pSender, HP_CONNID dwConnID)
{
    App::Instance().Login();
    return HR_OK;
}

/// <summary>
/// 数据到达处理
/// </summary>
/// <param name="pSender"></param>
/// <param name="dwConnID"></param>
/// <param name="pData"></param>
/// <param name="iLength"></param>
/// <returns></returns>
En_HP_HandleResult __HP_CALL OnRecevice(HP_Client pSender, HP_CONNID dwConnID, const BYTE* pData, int iLength)
{
    Document document;
    //auto data = helper::A2W((const char*)pData);

    //GenericDocument<UTF16<> > document;
    document.Parse((const char*)pData, iLength);

    //document.ParseStream<0, UTF8<>>(pData);
    if (!document.HasMember("dataType") || 
        !document.HasMember("msgType") || 
        document["dataType"] != (UINT)DataType::kAppMsg)
    {
        return HR_IGNORE;
    }
    MsgType type = (MsgType)document["msgType"].GetUint();
    auto to_wxid = document.HasMember("toWxid") ?
        helper::A2W(document["toWxid"].GetString()) :
        L"";
    auto msg = document.HasMember("msg") ?
        helper::A2W(document["msg"].GetString()) :
        L"";

    switch (type)
    {
    case MsgType::kText:
        WeChat::Instance().SendText(to_wxid.c_str(), msg.c_str());
        break;
    case MsgType::kNotifyText:
        if (document.HasMember("notifyWxid"))
        {
            auto notify_wxid = helper::A2W(document["notifyWxid"].GetString());
            auto notify_name = document.HasMember("notifyName") ?
                helper::A2W(document["notifyName"].GetString()) :
                L"Taper";
            std::wstring group_msg(L"", notify_name.size() + msg.size() + 10);
            wsprintfW(&group_msg[0], L"@%s %s", notify_name.c_str(), msg.c_str());

            WeChat::Instance().SendText(to_wxid.c_str(), group_msg.c_str(), notify_wxid.c_str());
        }
        break;
    case MsgType::kPic:
        WeChat::Instance().SendPic(to_wxid.c_str(), msg.c_str());
        break;
    case MsgType::kLink:
        break;
    case MsgType::kFile:
        WeChat::Instance().SendFile(to_wxid.c_str(), msg.c_str());
        break;
    case MsgType::kApplet:
        break;
    case MsgType::kVoice:
        break;
    case MsgType::kFriendReq:
        if (document.HasMember("encryptusername") && document.HasMember("ticket"))
        {
            auto encryptusername = helper::A2W(document["encryptusername"].GetString());
            auto ticket = helper::A2W(document["ticket"].GetString());
            WeChat::Instance().AgreeFriendReq(encryptusername.c_str(), ticket.c_str());
        }
        break;
    case MsgType::kFriendCard:
        if (document.HasMember("cardWxid") && document.HasMember("nickname"))
        {
            auto cardWxid = helper::A2W(document["cardWxid"].GetString());
            auto nickname = helper::A2W(document["nickname"].GetString());
            WeChat::Instance().SendCard(to_wxid.c_str(), cardWxid.c_str(), nickname.c_str());
        }
        break;
    case MsgType::kVideo:
        WeChat::Instance().SendFile(to_wxid.c_str(), msg.c_str());
        break;
    case MsgType::kGif:
        break;
    case MsgType::kLocation:
        break;
    case MsgType::kXml:
        break;
    case MsgType::kTransfer:
        if (document.HasMember("fromWxid") && document.HasMember("transferid"))
        {
            auto from_wxid = helper::A2W(document["fromWxid"].GetString());
            auto transferid = helper::A2W(document["transferid"].GetString());
            WeChat::Instance().CollectMoney(from_wxid.c_str(), transferid.c_str());
        }
        break;
    case MsgType::kSystem:
        break;
    default:
        break;
    }
    return HR_OK;
}

/// <summary>
/// 设置定时任务 检测连接、检测微信登录
/// </summary>
void SetTimerTask()
{
    SetTimer(NULL, 0, 2000,
        [](
            HWND Param1,
            UINT Param2,
            UINT_PTR IDEVENT,
            DWORD Param4)
        {
            if (WeChat::Instance().IsLogin() && !App::Instance().IsConnect())
            {
                App::Instance().GetInfo();
                App::Instance().Start();
            }
            else if (!WeChat::Instance().IsLogin() && App::Instance().IsConnect())
            {
                App::Instance().Stop();
            }
        }
    );
}

/// <summary>
/// 微信消息处理器，负责处理接收的微信消息
/// </summary>
/// <param name="type"></param>
/// <param name="from"></param>
/// <param name="msg"></param>
/// <param name="from_wxid"></param>
/// <param name="final_from_wxid"></param>
/// <param name="msgid"></param>
void HandleWxMsg(
    unsigned int type,
    unsigned int from,
    const wchar_t* msg,
    const wchar_t* from_wxid,
    const wchar_t* final_from_wxid,
    const wchar_t* msgid)
{
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();

    writer.Key("dataType");
    writer.Uint((UINT)DataType::kRobotMsg);

    writer.Key("msgType");
    writer.Uint(type);

    writer.Key("msgFrom");
    writer.Uint(from);

    writer.Key("robotWxid");
    writer.String(WeChat::Instance().wxid());

    writer.Key("fromWxid");
    //writerUTF16.String(from_wxid);
    writer.String(helper::W2A(from_wxid).c_str());

    writer.Key("fromName");
    writer.String("");

    writer.Key("msg");
    writer.String(helper::W2A(msg).c_str());
    //writerUTF16.String(msg);


    if (final_from_wxid != nullptr)
    {
        writer.Key("finalFromWxid");
        writer.String(helper::W2A(final_from_wxid).c_str());
    }
    writer.Key("msgid");
    writer.String(helper::W2A(msgid).c_str());

    writer.EndObject();
    App::Instance().Send(s.GetString(), s.GetSize());
}

}

/// <summary>
/// 启动程序
/// </summary>
/// <param name="hModule"></param>
void App::Run(HMODULE hModule)
{
    //加载dll目录
    std::string path;
    path.resize(MAX_PATH);
    GetModuleFileNameA(hModule, &path[0], path.size());
    path.resize(path.rfind('\\'));
    //SetDllDirectoryA(path.c_str());
    path += "\\HPSocket4C.dll";
    //隐藏模块自身
    helper::HideModule(hModule);

    //加载HP
    HPSocket4C_ = LoadLibraryA(path.c_str());
    if (HPSocket4C_ == NULL) {
        helper::ErrorMsg("加载HP模块失败");
        exit(1);
    }

    //加载Wechat模块
    std::string ret;
    if (WeChat::Instance().Init(ret) == false)
    {
        helper::ErrorMsg("加载Wechat模块失败");
        exit(1);
    }

    pListener_ = Create_HP_TcpPackClientListener();
    pClient_ = Create_HP_TcpPackClient(pListener_);

    HP_Set_FN_Client_OnConnect(pListener_, OnConnect);
    HP_Set_FN_Client_OnReceive(pListener_, OnRecevice);
    //HP_Set_FN_Client_OnClose(pListener_, OnClose);
    //helper::Error("初始化HPSOCKET完毕！");

    //修改内存跳过互斥使微信多开
    if (!WeChat::Instance().CloseMutex() ||
        !WeChat::Instance().CloseRevokeMsg() ||
        !WeChat::Instance().UpdateVersion(0x6302018F))
    {
        helper::ErrorMsg("内存写入失败");
        exit(1);
    }
    WeChat::Instance().BindMsgHandler(HandleWxMsg);
    SetTimerTask();
}

/// <summary>
/// 发送消息到服务端
/// </summary>
/// <param name="data"></param>
void App::Send(const char* data, size_t len)
{
    if (pClient_ != NULL && HP_Client_IsConnected(pClient_))
        HP_Client_Send(pClient_, (const BYTE*)data, len > 0 ? len : strlen(data));
}

bool App::IsConnect()
{
    if (!HP_Client_IsConnected(pClient_) && HP_Client_GetState(pClient_) == SS_STOPPED)
    {
        return false;
    }
    return true;
}

void App::Stop()
{
    HP_Client_Stop(pClient_);
}

void App::Start()
{
    auto hMap = OpenFileMappingA(FILE_MAP_READ, FALSE, "TAPER_PORT");
    if (hMap == NULL) { return; };

    LPVOID pData = MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 0);
    if (pData == NULL) { return; };

    USHORT port = *(USHORT*)pData;
    UnmapViewOfFile(pData);
    CloseHandle(hMap);

    HP_Client_Start(pClient_, "127.0.0.1", port, TRUE);
}

void App::GetInfo()
{
    info_.wxid = WeChat::Instance().wxid();
    info_.avatar = WeChat::Instance().avatar();
    info_.platform = WeChat::Instance().platform();
    info_.nickname = WeChat::Instance().nickname();
    info_.phone = WeChat::Instance().phone();
}

void App::Login()
{
    StringBuffer s;
    Writer<StringBuffer> writer(s);
    writer.StartObject();
    writer.Key("dataType");
    writer.Uint((UINT)DataType::kRobotLogin); //登录

    writer.Key("wxid");
    writer.String(info_.wxid.c_str());

    writer.Key("avatar");
    writer.String(info_.avatar.c_str());

    writer.Key("platform");
    writer.String(info_.platform.c_str());

    writer.Key("nickname");
    writer.String(info_.nickname.c_str());

    writer.Key("phone");
    writer.String(info_.phone.c_str());

    writer.EndObject();
    Send(s.GetString());
}
