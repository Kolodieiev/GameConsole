#include "./FileManager.h"
#include "FileManager.h"
#include "esp_system.h"

namespace meow
{
    const char FileManager::STR_DIR_PREFIX[] = "_D ";
    const char STR_TEMP_EXT[] = "_tmp";

    FileManager::TaskParams FileManager::_task_params;
    String FileManager::_dir_path;
    String FileManager::_file_ext;
    String FileManager::_db_path;
    //
    FileManager::IndexMode FileManager::_index_mode{INDX_MODE_DIR};
    //
    String FileManager::_old_name;
    String FileManager::_new_name;
    uint8_t FileManager::_copy_progress{0};

    FileManager::~FileManager()
    {
    }

    bool FileManager::fileExist(const char *path)
    {
        if (!hasConnection())
            return false;

        File f = SD.open(path);

        if (!f)
            return false;

        if (f.isDirectory())
        {
            f.close();
            return false;
        }

        return true;
    }

    bool FileManager::dirExist(const char *path)
    {
        if (!hasConnection())
            return false;

        File f = SD.open(path);

        if (!f)
            return false;

        if (!f.isDirectory())
        {
            f.close();
            return false;
        }

        return true;
    }

    bool FileManager::createDir(const char *path)
    {
        if (!hasConnection())
            return false;

        if (!SD.mkdir(path))
        {
            log_e("Помилка створення директорії: %s", path);
            return false;
        }

        return true;
    }

    void FileManager::end()
    {
        _sd.end();
    }

    bool FileManager::rm(const char *path)
    {
        File root_f = SD.open(path);
        File temp_f;
        String f_path;

        if (!root_f.isDirectory())
        {
            root_f.close();

            disableCore0WDT();
            bool result = SD.remove(path);
            enableCore0WDT();

            return result;
        }
        else
        {
            while (!_task_params.is_canceled && (temp_f = root_f.openNextFile()))
            {
                f_path = temp_f.path();
                bool is_dir = temp_f.isDirectory();
                temp_f.close();

                if (is_dir && !rm(f_path.c_str()))
                {
                    log_e("Помилка видалення каталогу: %s", f_path);
                    root_f.close();
                    return false;
                }
                else
                {
                    disableCore0WDT();
                    bool result = SD.remove(f_path);
                    enableCore0WDT();

                    if (!result)
                    {
                        log_e("Помилка видалення файлу: %s", f_path);
                        root_f.close();
                        return false;
                    }
                }

                vTaskDelay(1 / portTICK_PERIOD_MS);
            }

            root_f.close();

            if (_task_params.is_canceled)
                return false;

            return SD.rmdir(path);
        }
    }

    void FileManager::rmTask(void *params)
    {
        if (rm(_dir_path.c_str()))
            _task_params.is_succeeded = true;

        freeRes();
    }

    bool FileManager::remove(const char *path)
    {
        if (!hasConnection())
            return false;

        File fl = SD.open(path);

        if (!fl)
        {
            log_e("Помилка відкриття файлу %s", path);
            return false;
        }

        fl.close();

        _task_params.is_succeeded = false;
        _task_params.is_canceled = false;
        _task_params.is_done = false;
        _dir_path = path;

        uint8_t task_res = xTaskCreatePinnedToCore(rmTask, "rmTask", (1024 / 2) * 30, NULL, 10, NULL, 0);

        if (!task_res)
            log_e("Помилка запуску rmTask");

        return task_res;
    }

    bool FileManager::rename(const char *old_name, const char *new_name)
    {
        if (!hasConnection())
            return false;

        File f = SD.open(old_name);

        if (!f)
        {
            f.close();
            return false;
        }

        f.close();

        return SD.rename(old_name, new_name);
    }

    std::vector<String> FileManager::readFilesFromDB(const char *db_path, uint16_t start_pos, uint16_t size)
    {
        std::vector<String> result;

        if (!hasConnection())
            return result;

        if (size == 0)
            size = 100;

        if (size > result.max_size())
        {
            log_e("Некоректний розмір вектора: %i", size);
            esp_restart();
        }

        result.reserve(size);

        File db = SD.open(db_path, FILE_READ);

        if (!db)
        {
            log_e("Помилка відкриття файлу: %s", db_path);
            return result;
        }

        if (db.isDirectory())
        {
            log_e("Помилка. Не DB: %s", db_path);
            return result;
        }

        if (db.available())
            db.readStringUntil('|');

        uint16_t pos{0};
        String temp;
        while (pos != start_pos && db.available())
        {
            temp = db.readStringUntil('|');
            ++pos;
        }

        uint16_t i{0};
        while (db.available() && i < size)
        {
            result.push_back(db.readStringUntil('|'));
            ++i;
        }

        db.close();

        return result;
    }

    std::vector<String> FileManager::readFilesFromSD(const char *dir_path, uint16_t start_pos, uint16_t size)
    {
        std::vector<String> result;

        if (size > result.max_size())
        {
            log_e("Некоректний розмір вектора: %i", size);
            esp_restart();
        }

        if (!hasConnection())
            return result;

        result.reserve(size);

        File dir = SD.open(dir_path);

        if (!dir)
        {
            log_e("Помилка відкриття каталогу %s", dir_path);
            return result;
        }

        if (!dir.isDirectory())
        {
            dir.close();
            log_e("Помилка. Не DIR: %s", dir_path);
            return result;
        }

        File file;
        uint16_t pos{0};
        String temp;
        while (pos != start_pos && (file = dir.openNextFile()))
        {
            file.close();
            ++pos;
        }

        uint16_t i{0};
        while ((file = dir.openNextFile()) && i < size)
        {
            result[i] = file.name();
            file.close();
            ++i;
        }

        dir.close();

        result.shrink_to_fit();
        return result;
    }

    bool FileManager::strEndsWith(const char *str, const char *suffix)
    {
        size_t len = strlen(str);
        size_t suffix_len = strlen(suffix);
        if (suffix_len > len)
            return false;

        str += (len - suffix_len);
        return strcmp(str, suffix) == 0;
    }

    void FileManager::copyFileTask(void *params)
    {
        File old_file = SD.open(_db_path, FILE_READ);

        if (!old_file)
        {
            log_e("Помилка відкриття файлу %s", _db_path);
            freeRes();
        }

        if (SD.exists(_new_name))
            SD.remove(_new_name);

        File new_file = SD.open(_new_name, FILE_APPEND, true);

        if (!new_file)
        {
            log_e("Помилка відкриття файлу %s", _new_name);
            old_file.close();
            freeRes();
        }

        const size_t buf_size{1024 * 100};
        uint8_t *buffer = (uint8_t *)ps_malloc(buf_size);

        if (!buffer)
        {
            old_file.close();
            new_file.close();
            log_e("Помилка виділення пам'яті");
            esp_restart();
        }

        size_t file_size = old_file.size();
        size_t writed_bytes_counter{0};
        size_t bytes_readed;
        while (!_task_params.is_canceled && old_file.available())
        {
            bytes_readed = old_file.read(buffer, buf_size);
            new_file.write(buffer, bytes_readed);
            writed_bytes_counter += bytes_readed;
            _copy_progress = ((float)writed_bytes_counter / file_size) * 100;
            vTaskDelay(1);
        }

        free(buffer);

        old_file.close();
        new_file.close();

        if (!_task_params.is_canceled)
            _task_params.is_succeeded = true;
        else
            SD.remove(_new_name);

        freeRes();
    }

    bool FileManager::copyFile(const char *from, const char *to)
    {
        if (!hasConnection())
            return false;

        if (!SD.exists(from))
            return false;

        _task_params.is_succeeded = false;
        _task_params.is_canceled = false;
        _task_params.is_done = false;
        _copy_progress = 0;
        _db_path = from;
        _new_name = to;

        uint8_t task_res = xTaskCreatePinnedToCore(copyFileTask, "copyFileTask", (1024 / 2) * 70, NULL, 10, NULL, 0);

        if (!task_res)
            log_e("Помилка запуску copyFileTask");

        return task_res;
    }

    bool FileManager::hasConnection()
    {
        return _sd.hasConnection();
    }

    uint16_t FileManager::getDBSize(const char *db_path)
    {
        if (!SD.exists(db_path))
            return 0;

        File db = SD.open(db_path, FILE_READ);

        if (!db || db.isDirectory())
            return 0;

        String temp = db.readStringUntil('|');

        if (temp.isEmpty())
            return 0;

        return atoi(temp.c_str());
    }

    void FileManager::stopTasks()
    {
        _task_params.is_canceled = true;
    }

    void FileManager::indexTask(void *params)
    {
        File dir = SD.open(_dir_path);

        if (!dir)
        {
            log_e("Помилка відкриття каталогу %s", _dir_path);
            _task_params.is_done = true;
            freeRes();
        }

        if (!dir.isDirectory())
        {
            dir.close();
            log_e("Помилка. Не каталог %s", _dir_path);
            freeRes();
        }

        String t_db_path = _db_path;
        t_db_path += STR_TEMP_EXT;

        if (SD.exists(t_db_path))
            SD.remove(t_db_path);

        File temp_db = SD.open(t_db_path, FILE_APPEND, true);

        if (!temp_db)
        {
            log_e("Помилка відкриття файлу %s", _db_path);
            dir.close();
            freeRes();
        }

        if (temp_db.isDirectory())
        {
            log_e("Помилка. Не файл %s", _dir_path);
            temp_db.close();
            dir.close();
            freeRes();
        }

        File file;
        uint16_t counter{0};
        String file_name;
        while (!_task_params.is_canceled && (file = dir.openNextFile()))
        {
            switch (_index_mode)
            {
            case INDX_MODE_DIR:
                if (file.isDirectory())
                {
                    file_name = file.name();
                    file_name += "|";
                    temp_db.print(file_name);
                    ++counter;
                }
                break;
            case INDX_MODE_FILES:
                if (!file.isDirectory() && !strEndsWith(file.name(), STR_TEMP_EXT))
                {
                    file_name = file.name();
                    file_name += "|";
                    temp_db.print(file_name);
                    ++counter;
                }
                break;
            case INDX_MODE_FILES_EXT:
                if (!file.isDirectory())
                {
                    file_name = file.name();
                    if (file_name.endsWith(_file_ext))
                    {
                        file_name += "|";
                        temp_db.print(file_name);
                        ++counter;
                    }
                }
                break;
            case INDX_MODE_ALL:
                file_name = "";

                if (file.isDirectory())
                    file_name = STR_DIR_PREFIX;
                else if (strEndsWith(file.name(), STR_TEMP_EXT))
                    goto cont;

                file_name += file.name();
                file_name += "|";
                temp_db.print(file_name);
                ++counter;

            cont:
                break;
            }

            file.close();
            vTaskDelay(1 / portTICK_PERIOD_MS);
        }

        dir.close();
        temp_db.close();

        if (!_task_params.is_canceled)
        {
            File temp_db = SD.open(t_db_path, FILE_READ);

            if (temp_db)
            {
                if (SD.exists(_db_path))
                    SD.remove(_db_path);

                File db = SD.open(_db_path, FILE_APPEND, true);
                if (db)
                {
                    String num_str = "";
                    num_str += counter;
                    num_str += "|";
                    db.print(num_str);

                    const uint16_t buf_size{1024 * 3};
                    uint8_t buffer[buf_size];

                    while (!_task_params.is_canceled && temp_db.available())
                    {
                        size_t bytes_readed = temp_db.read(buffer, buf_size);
                        db.write(buffer, bytes_readed);
                        vTaskDelay(1);
                    }

                    db.close();

                    if (!_task_params.is_canceled)
                        _task_params.is_succeeded = true;
                }
                else
                    log_e("Помилка відкриття db %s", _db_path);

                temp_db.close();
            }
            else
                log_e("Помилка читання temp_db %s", t_db_path);
        }

        SD.remove(t_db_path.c_str());

        freeRes();
    }

    bool FileManager::indexFilesExt(const char *dir_path, const char *file_ext, const char *db_path)
    {
        _file_ext = file_ext;
        return startIndexTask(INDX_MODE_FILES_EXT, dir_path, db_path);
    }

    bool FileManager::indexFiles(const char *dir_path, const char *db_path)
    {
        return startIndexTask(INDX_MODE_FILES, dir_path, db_path);
    }

    bool FileManager::indexDirs(const char *dir_path, const char *db_path)
    {
        return startIndexTask(INDX_MODE_DIR, dir_path, db_path);
    }

    bool FileManager::indexAll(const char *dir_path, const char *db_path)
    {
        return startIndexTask(INDX_MODE_ALL, dir_path, db_path);
    }

    bool FileManager::startIndexTask(IndexMode mode, const char *dir_path, const char *db_path)
    {
        if (!hasConnection())
            return false;

        _index_mode = mode;
        _dir_path = dir_path;
        _db_path = db_path;

        _task_params.is_succeeded = false;
        _task_params.is_canceled = false;
        _task_params.is_done = false;

        uint8_t task_res = xTaskCreatePinnedToCore(indexTask, "indexTask", (1024 / 2) * 70, NULL, 10, NULL, 0);

        if (!task_res)
            log_e("Помилка запуску indexTask");

        return task_res;
    }

    void FileManager::freeRes()
    {
        _db_path = "";
        _dir_path = "";
        _file_ext = "";
        _old_name = "";
        _new_name = "";
        _task_params.is_done = true;
        vTaskDelete(NULL);
    }
}