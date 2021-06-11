#include "pch.h"
#include "wechat.h"

void WeChat::SendGif(const wchar_t* to_wxid, const wchar_t* path)
{
    DWORD call1_arg = Offset(0x17DAB00);
    DWORD call_addr1 = Offset(0x58CBC0);
    DWORD call_addr2 = Offset(0x58CC00);
    DWORD call_addr3 = Offset(0x97AD0);
    DWORD call_addr4 = Offset(0x32CEE0);

    String to_wxid_stu{ to_wxid };
    String path_stu{ path };


    /*
    7144CDED    6A 00           push 0x0                                 ; 发送gif表情包call 开始
    7144CDEF    83EC 14         sub esp,0x14
    7144CDF2    8BCC            mov ecx,esp
    7144CDF4    8965 B0         mov dword ptr ss:[ebp-0x50],esp
    7144CDF7    6A FF           push -0x1
    7144CDF9    68 00ABB172     push WeChatWi.72B1AB00
    7144CDFE    E8 BDFD4700     call WeChatWi.718CCBC0
    7144CE03    6A 02           push 0x2
    7144CE05    83EC 14         sub esp,0x14
    7144CE08    8D85 78FFFFFF   lea eax,dword ptr ss:[ebp-0x88]          ; wxid
    7144CE0E    8BCC            mov ecx,esp
    7144CE10    8965 8C         mov dword ptr ss:[ebp-0x74],esp
    7144CE13    50              push eax                                 ; wxid参数
    7144CE14    E8 E7FD4700     call WeChatWi.718CCC00
    7144CE19    83EC 14         sub esp,0x14
    7144CE1C    8BCC            mov ecx,esp
    7144CE1E    8965 90         mov dword ptr ss:[ebp-0x70],esp
    7144CE21    6A FF           push -0x1
    7144CE23    68 00ABB172     push WeChatWi.72B1AB00
    7144CE28    E8 93FD4700     call WeChatWi.718CCBC0
    7144CE2D    83EC 14         sub esp,0x14
    7144CE30    8BCC            mov ecx,esp
    7144CE32    8965 BC         mov dword ptr ss:[ebp-0x44],esp
    7144CE35    53              push ebx                                 ; gif文件参数
    7144CE36    E8 C5FD4700     call WeChatWi.718CCC00
    7144CE3B    C645 FC 15      mov byte ptr ss:[ebp-0x4],0x15
    7144CE3F    E8 8CACF8FF     call WeChatWi.713D7AD0
    7144CE44    8BC8            mov ecx,eax
    7144CE46    C645 FC 01      mov byte ptr ss:[ebp-0x4],0x1
    7144CE4A    E8 91002200     call WeChatWi.7166CEE0                   ; 发送gif表情包call结束
    */
    _asm
    {
        push 0x0;
        sub esp, 0x14;
        mov ecx, esp;
        push - 0x1;
        push call1_arg;
        call call_addr1;

        push 0x2;
        sub esp, 0x14;
        lea eax, to_wxid_stu;
        mov ecx, esp;
        push eax;
        call call_addr2;

        sub esp, 0x14;
        mov ecx, esp;
        push - 0x1;
        push call1_arg;
        call call_addr1;

        sub esp, 0x14;
        mov ecx, esp;
        push ebx;
        call call_addr2;
        call call_addr3;
        mov ecx, eax;
        call call_addr4;
    }
}