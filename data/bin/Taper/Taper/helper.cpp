#include "pch.h"
#include "helper.h"

namespace helper {

void ErrorMsg(const char* msg)
{
    char errmsg[100];
    wsprintfA(errmsg, "错误原因：%s\r\n错误代码：%d\r\n请联系管理员", msg, GetLastError());
    MessageBoxA(NULL, errmsg, "TAPER ROBOT ERROR", MB_ICONERROR);
}

std::wstring A2W(std::string str)
{
    std::wstring wstr;

    int wlen = MultiByteToWideChar(CP_ACP, 0, str.data(), str.size(), NULL, 0);
    if (wlen > 0)
    {
        wstr.resize(wlen);
        MultiByteToWideChar(CP_ACP, 0, str.data(), str.size(), &wstr[0], wstr.size());
    }
    return wstr;
}


std::string W2A(std::wstring wstr)
{
    std::string str;

    int len = WideCharToMultiByte(CP_ACP, 0, wstr.data(), wstr.size(), NULL, 0, NULL, FALSE);
    if (len > 0)
    {
        str.resize(len);
        WideCharToMultiByte(CP_ACP, 0, wstr.data(), wstr.size(), &str[0], str.size(), NULL, FALSE);
    }
    return str;
}

bool GetSubStr(const char* str, const char* first, const char* end, std::string& ret)
{
    const char* pos1 = strstr(str, first);
    if (pos1 == nullptr)
    {
        return false;
    }
    pos1 += strlen(first);
    const char* pos2 = strstr(pos1, end);
    if (pos2 == nullptr)
    {
        return false;
    }
    size_t len = strlen(pos1) - strlen(pos2);
    ret.resize(len);
    memcpy_s(&ret[0], len, pos1, len);
    return true;
}

bool HideModule(HMODULE hModule)
{
    typedef struct _LSA_UNICODE_STRING {
        USHORT Length;
        USHORT MaximumLength;
        PWSTR  Buffer;
    }
    UNICODE_STRING, * PUNICODE_STRING;

    typedef struct _PEB_LDR_DATA
    {
        DWORD Length; // +0x00
        bool Initialized; // +0x04
        PVOID SsHandle; // +0x08
        LIST_ENTRY InLoadOrderModuleList; // +0x0c
        LIST_ENTRY InMemoryOrderModuleList; // +0x14
        LIST_ENTRY InInitializationOrderModuleList;// +0x1c
    } PEB_LDR_DATA, * PPEB_LDR_DATA; // +0x24

    typedef struct _LDR_MODULE
    {
        LIST_ENTRY          InLoadOrderModuleList;
        LIST_ENTRY          InMemoryOrderModuleList;
        LIST_ENTRY          InInitializationOrderModuleList;
        void* BaseAddress;
        void* EntryPoint;
        ULONG               SizeOfImage;
        UNICODE_STRING   FullDllName;
        UNICODE_STRING      BaseDllName;
        ULONG               Flags;
        SHORT               LoadCount;
        SHORT               TlsIndex;
        HANDLE              SectionHandle;
        ULONG               CheckSum;
        ULONG               TimeDateStamp;
    } LDR_MODULE, * PLDR_MODULE;

    PLIST_ENTRY Head, Cur;
    PPEB_LDR_DATA ldr;
    PLDR_MODULE ldm;
    __asm
    {
        mov eax, fs: [0x30]
        mov ecx, [eax + 0x0c] //Ldr
        mov ldr, ecx
    }
    Head = &(ldr->InLoadOrderModuleList);
    Cur = Head->Flink;
    do
    {
        ldm = CONTAINING_RECORD(Cur, LDR_MODULE, InLoadOrderModuleList);
        if (hModule == ldm->BaseAddress)
        {
            // 三个链表同时给断掉
            ldm->InLoadOrderModuleList.Blink->Flink =
                ldm->InLoadOrderModuleList.Flink;
            ldm->InLoadOrderModuleList.Flink->Blink =
                ldm->InLoadOrderModuleList.Blink;

            //
            ldm->InInitializationOrderModuleList.Blink->Flink =
                ldm->InInitializationOrderModuleList.Flink;
            ldm->InInitializationOrderModuleList.Flink->Blink =
                ldm->InInitializationOrderModuleList.Blink;

            //
            ldm->InMemoryOrderModuleList.Blink->Flink =
                ldm->InMemoryOrderModuleList.Flink;
            ldm->InMemoryOrderModuleList.Flink->Blink =
                ldm->InMemoryOrderModuleList.Blink;
            break;
        }
        Cur = Cur->Flink;
    } while (Head != Cur);
}

}