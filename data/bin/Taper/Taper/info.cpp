#include "pch.h"
#include "app.h"


const char* App::wxid()
{
    return *(const char**)(WeChatWin_ + 0x1AA6038);
}

const char* App::avatar()
{
    return *(const char**)(WeChatWin_ + 0x1AA5EFC);
}

const char* App::nickname()
{
    return (const char*)(WeChatWin_ + 0x1AA6020);
}

const char* App::platform()
{
    return (const char*)(WeChatWin_ + 0x1AA6070);
}

const char* App::phone()
{
    return (const char*)(WeChatWin_ + 0x1AA5C50);
}