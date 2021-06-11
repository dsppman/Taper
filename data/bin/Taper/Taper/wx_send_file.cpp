#include "pch.h"
#include "wechat.h"

void WeChat::SendFile(const wchar_t* to_wxid, const wchar_t* path)
{
    DWORD call1_arg = Offset(0x17DAB00);
    DWORD call_addr1 = Offset(0x58CBC0);
    DWORD call_addr2 = Offset(0x58CC00);
    DWORD call_addr3 = Offset(0x68A40);
    DWORD call_addr4 = Offset(0x2C0BE0);

    String to_wxid_stu{ to_wxid };

    String path_stu{ path };

    char call1_buf[0x30];
    char call3_buf[0x230];

    /*
    70B2CCAD    83EC 14         sub esp,0x14
    70B2CCB0    884D 94         mov byte ptr ss:[ebp-0x6C],cl
    70B2CCB3    8D47 E0         lea eax,dword ptr ds:[edi-0x20]
    70B2CCB6    8BCC            mov ecx,esp
    70B2CCB8    8965 BC         mov dword ptr ss:[ebp-0x44],esp
    70B2CCBB    50              push eax
    70B2CCBC    E8 3FFF4700     call WeChatWi.70FACC00
    6334CCC1    FF75 94         push dword ptr ss:[ebp-0x6C]             ; 发送文件call开始 压入0
    6334CCC4    83EC 14         sub esp,0x14
    6334CCC7    8BCC            mov ecx,esp
    6334CCC9    8965 90         mov dword ptr ss:[ebp-0x70],esp
    6334CCCC    6A FF           push -0x1
    6334CCCE    68 00ABA164     push WeChatWi.64A1AB00                   ; 基址参数
    6334CCD3    E8 E8FE4700     call WeChatWi.637CCBC0                   ; call1
    6334CCD8    83EC 14         sub esp,0x14
    6334CCDB    8BCC            mov ecx,esp
    6334CCDD    8965 8C         mov dword ptr ss:[ebp-0x74],esp
    6334CCE0    53              push ebx                                 ; 文件路径参数
    6334CCE1    E8 1AFF4700     call WeChatWi.637CCC00                   ; call2
    6334CCE6    83EC 14         sub esp,0x14
    6334CCE9    8D85 78FFFFFF   lea eax,dword ptr ss:[ebp-0x88]          ; wxid
    6334CCEF    8BCC            mov ecx,esp
    6334CCF1    8965 B0         mov dword ptr ss:[ebp-0x50],esp
    6334CCF4    50              push eax                                 ; wxid参数
    6334CCF5    E8 06FF4700     call WeChatWi.637CCC00                   ; call2
    6334CCFA    8D85 B0FCFFFF   lea eax,dword ptr ss:[ebp-0x350]         ; 接收缓存区
    6334CD00    C645 FC 10      mov byte ptr ss:[ebp-0x4],0x10
    6334CD04    50              push eax                                 ; 缓存区参数
    6334CD05    E8 36BDF5FF     call WeChatWi.632A8A40                   ; call3
    6334CD0A    8BC8            mov ecx,eax                              ; call3返回值
    6334CD0C    C645 FC 0C      mov byte ptr ss:[ebp-0x4],0xC
    6334CD10    E8 CB3E1B00     call WeChatWi.63500BE0                   ; 发送文件call4 结束


    */
    __asm {

        sub esp, 0x14;
        lea eax, call1_buf;
        mov ecx, esp;
        push eax;
        call call_addr2;

        push 0;
        sub esp, 0x14;
        mov ecx, esp;
        push - 0x1;
        push call1_arg;
        call call_addr1;

        sub esp, 0x14;
        mov ecx, esp;
        lea ebx, path_stu;
        push ebx;
        call call_addr2;

        sub esp, 0x14;
        lea eax, to_wxid_stu;
        mov ecx, esp;
        push eax;
        call call_addr2;

        lea eax, call3_buf;
        push eax;
        call call_addr3;

        mov ecx, eax;
        call call_addr4;
    }
}