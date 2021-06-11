#include "pch.h"
#include "wechat.h"

void WeChat::AgreeFriendReq(const wchar_t* encryptusername, const wchar_t* ticket)
{

    String encryptusername_stu{ encryptusername };
    String ticket_stu{ ticket };

    DWORD call_addr1 = Offset(0x58CC00);// 0x1EFE60;
    DWORD* call2_arg = (DWORD*)Offset(0x1A87118);
    DWORD call_addr2 = Offset(0x10FD10);
    DWORD call_addr3 = Offset(0x1D6C80);

    char call3_buf[0x48] = { 0 };

    /*
    6342FE7F    50              push eax                                 ; v4
    6342FE80    E8 7BCD3900     call WeChatWi.637CCC00                   ; 压入v4 call1
    */

    /*
    63429F75    83EC 14         sub esp,0x14
    63429F78    54              push esp
    63429F79    E8 E25E0000     call WeChatWi.6342FE60                   ; 同意好友call 这里进去取v4
    63429F7E    8B8F 600B0000   mov ecx,dword ptr ds:[edi+0xB60]
    63429F84    8D45 DC         lea eax,dword ptr ss:[ebp-0x24]
    63429F87    50              push eax
    63429F88    E8 0336E7FF     call WeChatWi.6329D590                   ; 同意好友call1 取v3 这里不需要 把v3赋值到eax即可
    63429F8D    8BF0            mov esi,eax                              ; esi v3
    63429F8F    83EC 08         sub esp,0x8
    63429F92    C745 FC 0000000>mov dword ptr ss:[ebp-0x4],0x0
    63429F99    8B0D 1871CC64   mov ecx,dword ptr ds:[0x64CC7118]        ; ecx 一个固定基址参数
    63429F9F    E8 6C5DF2FF     call WeChatWi.6334FD10                   ; 这里得到eax
    63429FA4    8BD7            mov edx,edi
    63429FA6    8D8F 2C0B0000   lea ecx,dword ptr ds:[edi+0xB2C]
    63429FAC    F7DA            neg edx
    63429FAE    50              push eax
    63429FAF    1BD2            sbb edx,edx
    63429FB1    23D1            and edx,ecx
    63429FB3    8D8F 640B0000   lea ecx,dword ptr ds:[edi+0xB64]         ; 706badec的指针
    63429FB9    52              push edx                                 ; 压入 706bd468
    63429FBA    56              push esi                                 ; 压入 v3
    63429FBB    E8 C0CCFEFF     call WeChatWi.63416C80                   ; 好友请求call3 结束
    */
    __asm
    {
        push 0x3;
        sub esp, 0x14;
        mov ecx, esp; //缓冲区
        lea eax, ticket_stu; //赋值v4
        push eax; //压入v4
        call call_addr1;

        lea eax, encryptusername_stu; //赋值v3
        mov esi, eax; //压入v3
        sub esp, 0x8;
        mov ecx, call2_arg;
        call call_addr2;

        lea ecx, call3_buf;
        mov edx, ecx;
        push eax;
        push edx;
        push esi;
        call call_addr3;
    }
}