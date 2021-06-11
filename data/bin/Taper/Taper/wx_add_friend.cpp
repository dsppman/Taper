#include "pch.h"
#include "wechat.h"

void WeChat::AddFriend(const wchar_t* wxid, const wchar_t* req_msg)
{
    DWORD call_addr1 = Offset(0x83500);
    DWORD call_addr2 = Offset(0x58CBC0);
    DWORD call_addr3 = Offset(0x58CC00);
    DWORD call_addr4 = Offset(0x63660);
    DWORD call_addr5 = Offset(0x321D80);

    String wxid_stu{ wxid };

    /*
    7150015C    83EC 18         sub esp,0x18                             ; 添加好友call 开始
    7150015F    C645 FC 03      mov byte ptr ss:[ebp-0x4],0x3
    71500163    8D86 3C060000   lea eax,dword ptr ds:[esi+0x63C]
    71500169    89A5 1CFFFFFF   mov dword ptr ss:[ebp-0xE4],esp
    7150016F    8BCC            mov ecx,esp
    71500171    50              push eax                                 ; 0
    71500172    E8 8933ECFF     call WeChatWi.713C3500                   ; 添加好友call 必要
    71500177    FFB6 38060000   push dword ptr ds:[esi+0x638]
    7150017D    85FF            test edi,edi
    7150017F    74 06           je short WeChatWi.71500187
    71500181    66:833F 00      cmp word ptr ds:[edi],0x0
    71500185    75 05           jnz short WeChatWi.7150018C
    71500187    BF 00ABB172     mov edi,WeChatWi.72B1AB00
    7150018C    83EC 14         sub esp,0x14
    7150018F    8BCC            mov ecx,esp
    71500191    89A5 18FFFFFF   mov dword ptr ss:[ebp-0xE8],esp
    71500197    6A FF           push -0x1
    71500199    57              push edi
    7150019A    E8 21CA3C00     call WeChatWi.718CCBC0                   ; 得到添加信息eax 自行组装非必要call
    7150019F    FFB6 30060000   push dword ptr ds:[esi+0x630]            ; -1
    715001A5    83EC 14         sub esp,0x14                             ; 很有可能是加人call
    715001A8    8BCC            mov ecx,esp
    715001AA    89A5 20FFFFFF   mov dword ptr ss:[ebp-0xE0],esp
    715001B0    53              push ebx                                 ; 添加好友wxid参数
    715001B1    E8 4ACA3C00     call WeChatWi.718CCC00                   ; 添加好友call 必要
    715001B6    C645 FC 06      mov byte ptr ss:[ebp-0x4],0x6
    715001BA    E8 A134EAFF     call WeChatWi.713A3660                   ; 添加好友call 必要
    715001BF    8BC8            mov ecx,eax
    715001C1    C645 FC 01      mov byte ptr ss:[ebp-0x4],0x1
    715001C5    E8 B61B1600     call WeChatWi.71661D80                   ; 加人call 结束
    */
    _asm
    {
        sub esp, 0x18;
        mov eax, 0;
        mov ecx, esp;
        push eax;
        call call_addr1;

        push 6;
        sub esp, 0x14;
        mov ecx, esp;
        push - 0x1;
        mov edi, req_msg;
        push edi;
        call call_addr2;

        push 2;
        sub esp, 0x14;
        mov ecx, esp;
        lea ebx, wxid_stu;
        push ebx;
        call call_addr3;
        call call_addr4;

        mov ecx, eax;
        call call_addr5;
    }
}