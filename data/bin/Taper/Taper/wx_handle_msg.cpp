#include "pch.h"
#include "wechat.h"

namespace {

WeChat::MsgHandler khandler;
DWORD old_call_addr;
DWORD ret_addr;
DWORD msg_addr;

void __declspec(naked) RecvMsgCall()
{
    _asm
    {
        call old_call_addr; // 被覆盖的原指令
        mov msg_addr, esi;
    }
    [] {
        if (::IsBadReadPtr(&msg_addr, 0x258) != 0)
        {
            return;
        }
        // 判断是否来消息
        if (*(LPCWSTR*)(msg_addr + 0x68) == nullptr)
        {
            return;
        }

        DWORD type = *(DWORD*)(msg_addr + 0x30);
        DWORD from = *(DWORD*)(msg_addr + 0x34);

        //消息类型 0x01文字 0x03图片 0x22语音 0x25好友确认 0x28其它 0x2A名片 0x2B视频 0x2F表情 0x30位置 0x2710系统消息
        //        0x31共享实时位置 文件 转账 链接 收款
        DWORD type2 = *(DWORD*)(msg_addr + 0xEC);
        if (type == 0x31 && type2 != 0x0)
        {
            type = type2;
        }
        const wchar_t* from_wxid = *(const wchar_t**)(msg_addr + 0x40);
        const wchar_t* msg = *(const wchar_t**)(msg_addr + 0x68);
        const wchar_t* final_from_wxid = *(const wchar_t**)(msg_addr + 0x164);
        const wchar_t* msgid = *(const wchar_t**)(msg_addr + 0x178);
        khandler(type, from, msg, from_wxid, final_from_wxid, msgid);
    }();
    __asm jmp ret_addr;
}

} //namespace


void WeChat::BindMsgHandler(MsgHandler handler)
{
    khandler = handler;

    old_call_addr = Offset(0x87800); //原指令的call

    //msg_ptr = WeChatWin_ + 0x1AA872C; //消息结构体指针的存放地址

    DWORD jmp_addr = Offset(0x2F24E7); //需要跳转的地址
    ret_addr = jmp_addr + 5; //跳转返回的地址


    BYTE jmp_code[5] = { 0xE9 }; //新指令 call
    *(DWORD*)&jmp_code[1] = (DWORD)RecvMsgCall - jmp_addr - 5; //欲跳转到的函数

    /*
    70D124DC    8D4D B8         lea ecx,dword ptr ss:[ebp-0x48]
    70D124DF    84C0            test al,al
    70D124E1    75 03           jnz short WeChatWi.70D124E6
    70D124E3    8D4D D0         lea ecx,dword ptr ss:[ebp-0x30]
    70D124E6    56              push esi                                 ; 消息结构体
    70D124E7    E8 1453D9FF     call WeChatWi.70AA7800                   ; 不漏消息的接受消息hook
    70D124EC    81C6 58020000   add esi,0x258
    70D124F2    81C7 58020000   add edi,0x258
    70D124F8    3B73 04         cmp esi,dword ptr ds:[ebx+0x4]
    */
    WriteProcessMemory(
        GetCurrentProcess(),
        (void*)jmp_addr,
        &jmp_code,
        sizeof(jmp_code),
        NULL
    );
}