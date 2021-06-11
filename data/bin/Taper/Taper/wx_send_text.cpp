#include "pch.h"
#include "wechat.h"

void WeChat::SendText(const wchar_t* to_wxid, const wchar_t* text, const wchar_t* notify_wxid)
{
    DWORD call_addr1 = Offset(0x3B56A0);

    String to_wxid_stu{ to_wxid };

    String text_stu{ text };

    DWORD notify_call_buf[3] = { 0 };
    String notify_call_stu;

    if (notify_wxid != nullptr)
    {
        notify_call_stu.data = notify_wxid;
        notify_call_stu.size = wcslen(notify_wxid);
        notify_call_stu.capacity = notify_call_stu.size;

        notify_call_buf[0] = (DWORD)&notify_call_stu; //字符串内存首地址
        notify_call_buf[1] = notify_call_buf[0] + sizeof(notify_call_stu); //字符串内存尾地址
        notify_call_buf[2] = notify_call_buf[1];
    }

    char call1_buf[0x254];
    /*
    6334CA5B    6A 01           push 0x1
    6334CA5D    57              push edi                                 ; 传入@结构体
    6334CA5E    53              push ebx                                 ; 传入内容结构体
    6334CA5F    8D95 78FFFFFF   lea edx,dword ptr ss:[ebp-0x88]          ; 传入wxid
    6334CA65    8D8D 58FAFFFF   lea ecx,dword ptr ss:[ebp-0x5A8]         ; 传入接收缓冲区
    6334CA6B    E8 308C2A00     call WeChatWi.635F56A0                   ; 发送文本消息call
    */
    __asm {
        push 0x1
        lea edi, notify_call_buf
        push edi
        lea ebx, text_stu //
        push ebx
        lea edx, to_wxid_stu //
        lea ecx, call1_buf
        call call_addr1
        add esp, 0xC
    }
}