#pragma GCC optimize("O3")

#include "FileServer.h"
#include <ESPmDNS.h>
#include "./tmpl_html.cpp"
#include "../../../util/sd/SdUtil.h"

namespace meow
{
    bool FileServer::_is_working = false;

    FileServer::~FileServer()
    {
        stop();
    }

    bool FileServer::begin(String server_dir, ServerMode mode)
    {
        if (_is_working)
            return false;

        _server_dir = server_dir;

        if (!_server_dir.equals("/") && _server_dir.endsWith("/"))
            _server_dir.remove(1, -1);

        SdUtil sd;

        if (!sd.begin())
        {
            log_e("Помилка ініціалізації SD");
            return false;
        }
        else
        {
            File f_dir = SD.open(_server_dir.c_str());
            if (!f_dir)
            {
                log_e("Директорія %s не існує", _server_dir.c_str());
                return false;
            }
            else
            {
                bool is_dir = f_dir.isDirectory();
                f_dir.close();

                if (!is_dir)
                {
                    log_e("Файл %s не є директорією", _server_dir.c_str());
                    return false;
                }
            }
        }

        WiFi.softAP(_ssid, _pwd, 1, 0, 1);
        delay(10);

        if (_domain_name.isEmpty() || !MDNS.begin(_domain_name))
        {
            log_e("Помилка запуску mDNS");
            IPAddress IP = WiFi.softAPIP();
            _server_addr = "http://";
            _server_addr += IP.toString();
        }
        else
        {
            _server_addr = "http://";
            _server_addr += _domain_name;
            _server_addr += ".local";
        }

        log_i("AP addr: %s", _server_addr.c_str());

        _mode = mode;

        BaseType_t result = xTaskCreatePinnedToCore(startWebServer, "fileServerTask", (1024 / 2) * 20, this, 10, NULL, 1);

        if (result == pdPASS)
        {
            log_i("File server is working now");
            return true;
        }
        else
        {
            log_e("fileServerTask was not running");
            return false;
        }
    }

    void FileServer::stop()
    {
        if (!_is_working)
            return;

        _must_work = false;

        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF);
    }

    void FileServer::setDomainName(String domain_name)
    {
        if (domain_name.indexOf(" ") > -1 || domain_name.indexOf("/") > -1) // Очікується адекватне використання
            return;

        _domain_name = domain_name;
    }

    void FileServer::fileServerTask(void *params)
    {
        if (_mode == SERVER_MODE_RECEIVE)
        {
            _server.on("/", HTTP_GET, [this]()
                       { this->handleReceive(); });
            _server.on("/upload", HTTP_POST, []() {}, [this]()
                       { this->handleFile(); });
        }
        else
        {
            _server.on("/", HTTP_GET, [this]()
                       { this->handleSend(); });
        }

        _server.onNotFound([this]()
                           { this->handle404(); });

        _is_working = true;
        _must_work = true;

        _server.begin();

        while (_must_work)
        {
            _server.handleClient();
            vTaskDelay(1);
        }

        _is_working = false;
        vTaskDelete(NULL);
    }

    void FileServer::handleReceive()
    {
        String html = HEAD_HTML;
        html += RECEIVE_TITLE_STR;
        html += RECEIVE_FILE_HTML;
        _server.send(200, "text/html", html);
    }

    void FileServer::handleSend()
    {
        // Відкрити директорію з файлами на SD
        File root = SD.open(_server_dir.c_str());

        if (!root)
        {
            log_e("Помилка відкриття директорії %s", _server_dir.c_str());
            _server.send(500, "text/html", "");
            return;
        }
        else if (_server.args() > 0)
        {
            String path = _server_dir;
            path += "/";
            path += _server.arg(0);

            File file = SD.open(path);

            if (!file)
                handle404();
            else
            {
                _server.sendHeader("Content-Type", "application/force-download");
                _server.sendHeader("Content-Disposition", "attachment; filename=\"" + _server.arg(0) + "\"");
                _server.sendHeader("Content-Transfer-Encoding", "binary");
                _server.sendHeader("Cache-Control", "no-cache");
                _server.streamFile(file, "application/octet-stream");
                file.close();
            }
        }
        // Якщо відсутні параметри, відобразити список файлів в директорії
        else
        {
            String html = HEAD_HTML;
            html += SEND_TITLE_STR; // Заголовок
            html += MID_HTML;

            File file;
            while ((file = root.openNextFile()))
            {
                if (!file.isDirectory())
                {
                    html += HREF_HTML;
                    html += file.name();
                    html += "\">";
                    html += file.name();
                    html += "</a>";
                }
                file.close();
                vTaskDelay(1);
            }
            html += FOOT_HTML;
            _server.send(200, "text/html", html);
        }
    }

    void FileServer::handleFile()
    {
        static File input_file;

        HTTPUpload &uploadfile = _server.upload();
        _server.setContentLength(CONTENT_LENGTH_UNKNOWN);

        if (uploadfile.status == UPLOAD_FILE_START)
        {
            String file_name = _server_dir;
            file_name += "/";
            file_name += uploadfile.filename;

            log_i("Запит на створення файлу %s", file_name.c_str());

            input_file = SD.open(file_name.c_str(), FILE_READ);

            bool file_open_fail = false;

            if (input_file)
            {
                if (input_file.isDirectory())
                {
                    input_file.close();
                    file_open_fail = true;
                }
                else
                    SD.remove(file_name.c_str());
            }

            if (!file_open_fail)
            {
                input_file = SD.open(file_name.c_str(), FILE_APPEND, true);

                if (!input_file)
                    file_open_fail = true;
                else
                    log_i("Файл створено");
            }

            if (file_open_fail)
            {
                log_e("Не можу відкрити файл %s на запис", file_name.c_str());
                _server.send(500, "text/html", "");
            }
        }
        else if (uploadfile.status == UPLOAD_FILE_WRITE)
        {
            if (input_file)
            {
                input_file.write(uploadfile.buf, uploadfile.currentSize);
                vTaskDelay(1);
            }
        }
        else if (uploadfile.status == UPLOAD_FILE_END || uploadfile.status == UPLOAD_FILE_ABORTED)
        {
            if (input_file)
            {
                input_file.close();

                handleReceive();

                if (uploadfile.status == UPLOAD_FILE_END)
                    log_i("Файл отримано");
                else
                    log_i("Завантаження перервано");
            }
            else
            {
                log_e("Необроблений файл");
                _server.send(500, "text/html", "");
            }
        }
    }

    void FileServer::handle404()
    {
        String html = HEAD_HTML;
        html += NOT_FOUND_TITLE_STR;
        html += MID_HTML;
        html += NOT_FOUND_BODY_START;
        html += _server_addr;
        html += NOT_FOUND_BODY_END;
        html += FOOT_HTML;
        _server.send(404, "text/html", html);
    }

    void FileServer::startWebServer(void *params)
    {
        FileServer *instance = static_cast<FileServer *>(params);
        instance->fileServerTask(params);
    }
}