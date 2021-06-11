#include "pch.h"
#include "wechat.h"

/// <summary>
/// 单例模式
/// </summary>
/// <returns></returns>
WeChat& WeChat::Instance()
{
    static WeChat vx;
    return vx;
}

DWORD WeChat::Offset(DWORD offset)
{
    return (DWORD)WeChatWin_ + offset;
}


bool WeChat::Init(std::string& ret)
{
    //加载Wechat模块
    WeChatWin_ = LoadLibraryA("WeChatWin");
    if (WeChatWin_ == nullptr)
    {
        ret = std::to_string(GetLastError());
        return false;
    }
    return true;
}
