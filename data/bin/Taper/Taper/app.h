#pragma once
#include "pch.h"

class App
{
public:
    static App& Instance();

    void Run(HMODULE hModule);
    void Send(const char* data, size_t len = 0);
    bool IsConnect();
    void Start();
    void Stop();

    void GetInfo();
    void Login();

private:
    App(const App& other) = delete;
    App& operator=(const App&) = delete;
    App() = default;
    ~App();
    HP_TcpClientListener pListener_ = nullptr;
    HP_TcpPackClient pClient_ = nullptr;
    HMODULE HPSocket4C_ = nullptr;

    struct
    {
        std::string wxid;
        std::string avatar;
        std::string nickname;
        std::string platform;
        std::string phone;
    } info_;
};