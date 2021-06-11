#include "pch.h"
#include "wechat.h"

void WeChat::CollectMoney(const wchar_t* from_wxid, const wchar_t* transferid)
{
    String call_stu[2] = {
        {
            transferid
        },
        {
            from_wxid
        }
    };

    DWORD call_addr1 = Offset(0x9B48B0);
    DWORD call_addr2 = Offset(0x9B4930);


    /*

    63BF483F    83EC 30         sub esp,0x30                             ; 收款call开始
    63BF4842    8945 D0         mov dword ptr ss:[ebp-0x30],eax
    63BF4845    8BCC            mov ecx,esp
    63BF4847    8D45 A8         lea eax,dword ptr ss:[ebp-0x58]          ; wxid+转帐id
    63BF484A    C745 D4 0000000>mov dword ptr ss:[ebp-0x2C],0x0
    63BF4851    50              push eax                                 ; wxid+转帐id参数
    63BF4852    E8 59000000     call WeChatWi.63BF48B0
    63BF4857    E8 D4000000     call WeChatWi.63BF4930
    63BF485C    83C4 30         add esp,0x30                             ; 收款call结束
    */
    _asm
    {
        sub esp, 0x30;
        mov ecx, esp;
        lea eax, call_stu;
        push eax;
        call call_addr1;
        call call_addr2;
        add esp, 0x30;
    }
}