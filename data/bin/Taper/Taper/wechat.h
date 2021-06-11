#pragma once
#include "pch.h"

class WeChat
{
public:
    struct Info
    {

    };

    typedef void(* MsgHandler)(
        unsigned int type, 
        unsigned int from, 
        const wchar_t* msg,
        const wchar_t* from_wxid,
        const wchar_t* final_from_wxid,
        const wchar_t* msgid
        );

    static WeChat& Instance();
    DWORD Offset(DWORD offset);

    const char* wxid();
    const char* avatar();
    const char* nickname();
    const char* platform();
    const char* phone();

    bool Init(std::string& ret);
    bool IsLogin();
    Info GetInfo(const wchar_t* wxid);

    void BindMsgHandler(MsgHandler handler);
    void SendText(const wchar_t* to_wxid, const wchar_t* text, const wchar_t* at_wxid = nullptr);
    void SendPic(const wchar_t* to_wxid, const wchar_t* path);
    void SendFile(const wchar_t* to_wxid, const wchar_t* path);
    void SendGif(const wchar_t* to_wxid, const wchar_t* path);
    void SendXml(const wchar_t* to_wxid, const wchar_t* msg);
    void SendCard(const wchar_t* to_wxid, const wchar_t* wxid, const wchar_t* nickname);

    void AgreeFriendReq(const wchar_t* encryptusername, const wchar_t* ticket);
    void AddFriend(const wchar_t* wxid, const wchar_t* req_msg);

    void CollectMoney(const wchar_t* from_wxid, const wchar_t* transferid);

    BOOL CloseMutex();
    BOOL CloseRevokeMsg();
    BOOL UpdateVersion(DWORD version);
private:
    WeChat() = default;
    ~WeChat() = default;
    WeChat(const WeChat& other) = delete;
    WeChat& operator=(const WeChat&) = delete;

    HMODULE WeChatWin_;

    struct String {
        const wchar_t* data;
        size_t size = wcslen(data);
        size_t capacity = size;
        DWORD fill[2] = { 0 };
    };
};