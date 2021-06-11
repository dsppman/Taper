#include "pch.h"
#include "wechat.h"


/// <summary>
/// 关闭多开限制
/// </summary>
/// <returns></returns>
/// _WeChat_App_Instance_Identity_Mutex_Name
BOOL WeChat::CloseMutex()
{
    DWORD mutex_addr = Offset(0x9FF3C0);
    char code = 0xC3;  // ret

    BOOL ret = WriteProcessMemory(GetCurrentProcess(), (LPVOID)mutex_addr, &code,
        sizeof(code), NULL);

    return ret;
}
/// <summary>
/// 防撤回
/// </summary>
/// <returns></returns>
BOOL WeChat::CloseRevokeMsg()
{
    DWORD revokemsg_addr = Offset(0x2F0159);
    char code = 0xEB;  // jmp

    BOOL ret = WriteProcessMemory(GetCurrentProcess(), (LPVOID)revokemsg_addr, &code,
        sizeof(code), NULL);

    return ret;
}

BOOL WeChat::UpdateVersion(DWORD version)
{
    DWORD version_addr1 = Offset(0x1A87CA4);
    DWORD version_addr2 = Offset(0x1A93FA8);
    DWORD version_addr3 = Offset(0x1A94190);
    DWORD version_addr4 = Offset(0x1AA6914);
    DWORD version_addr5 = Offset(0x1AA88AC);

    //DWORD version = 0x6302018F;  // 3.2.1.143

    BOOL ret = WriteProcessMemory(GetCurrentProcess(), (LPVOID)version_addr1, &version,
        sizeof(version), NULL);
    ret = WriteProcessMemory(GetCurrentProcess(), (LPVOID)version_addr2, &version,
        sizeof(version), NULL);
    ret = WriteProcessMemory(GetCurrentProcess(), (LPVOID)version_addr3, &version,
        sizeof(version), NULL);
    ret = WriteProcessMemory(GetCurrentProcess(), (LPVOID)version_addr4, &version,
        sizeof(version), NULL);
    ret = WriteProcessMemory(GetCurrentProcess(), (LPVOID)version_addr5, &version,
        sizeof(version), NULL);
    return ret;
}

