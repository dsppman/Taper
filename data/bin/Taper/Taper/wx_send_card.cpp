#include "pch.h"
#include "wechat.h"

void WeChat::SendCard(const wchar_t* to_wxid, const wchar_t* wxid, const wchar_t* nickname)
{
    DWORD call_addr1 = Offset(0x3B56A0);

    String to_wxid_stu{ to_wxid };
    const wchar_t* card_xml_tpl = L"<?xml version=\"1.0\"?><msg username=\"%s\" nickname=\"%s\" imagestatus=\"3\" scene=\"17\" certflag=\"0\" brandFlags=\"0\" />";
    std::wstring card_xml;
    card_xml.resize(wcslen(card_xml_tpl) + wcslen(wxid) + wcslen(nickname));
    wsprintfW(&card_xml[0], card_xml_tpl, wxid, nickname);

    String card_xml_stu{ card_xml.c_str() };

    char call1_buf[0x254];

    /*
    70BF6152    83C4 04         add esp,0x4                              ; 发送xml名片call开始
    70BF6155    C645 FC 04      mov byte ptr ss:[ebp-0x4],0x4
    70BF6159    6A 2A           push 0x2A
    70BF615B    8D45 D8         lea eax,dword ptr ss:[ebp-0x28]          ; 名片xml
    70BF615E    8BD6            mov edx,esi                              ; 接收wxid参数
    70BF6160    6A 00           push 0x0
    70BF6162    50              push eax                                 ; 名片xml参数
    70BF6163    8D8D 7CFDFFFF   lea ecx,dword ptr ss:[ebp-0x284]         ; 缓冲区参数
    70BF6169    E8 32F51D00     call WeChatWi.70DD56A0                   ; 发送xml名片call
    70BF616E    83C4 0C         add esp,0xC                              ; 发送xml名片call 结束
    */
    _asm
    {
        add esp, 0x4;
        push 0x2A;
        lea eax, card_xml_stu;
        lea edx, to_wxid_stu;
        push 0x0;
        push eax;
        lea ecx, call1_buf;
        call call_addr1;
        add esp, 0xC;
    }
}