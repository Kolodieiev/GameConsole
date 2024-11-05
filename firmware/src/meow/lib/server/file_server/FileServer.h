#pragma once
#pragma GCC optimize("O3")

#include <Arduino.h>
#include <WebServer.h>

namespace meow
{
    class FileServer
    {

    public:
        enum ServerMode : uint8_t
        {
            SERVER_MODE_RECEIVE = 0,
            SERVER_MODE_SEND
        };

        ~FileServer();

        bool begin(String server_dir, ServerMode mode);
        void stop();
        bool isWorking() const { return _is_working; }

        void setSSID(String ssid) { _ssid = ssid; }
        void setPWD(String pwd) { _pwd = pwd; }
        void setDomainName(String domain_name);

        String getAddress() const { return _server_addr; }

    private:
        static bool _is_working;

        String _server_addr;
        String _server_dir;
        String _domain_name;
        String _ssid;
        String _pwd;

        ServerMode _mode;
        WebServer _server{80};
        bool _must_work = false;

        static void startWebServer(void *params);
        void fileServerTask(void *params);

        void handleReceive();
        void handleSend();
        void handleFile();
        void handle404();
    };
}