#include "PrefUtil.h"

namespace meow
{
    const char ROOT[] = "/.data";
    const char PREF_DIR[] = "/preferences";

    bool PrefUtil::set(const char *pref_name, const char *value)
    {
        SdUtil sd;
        if (!sd.hasConnection())
            return false;

        File *file = loadPrefFile(pref_name, "w");

        if (file == nullptr)
            return false;

        file->print(value);
        file->close();

        delete file;
        return true;
    }

    String PrefUtil::get(const char *pref_name)
    {
        SdUtil sd;
        if (!sd.hasConnection())
            return "";

        File *file = loadPrefFile(pref_name, "r");

        if (!file)
            return "";

        String temp = file->readString();

        file->close();
        delete file;

        return temp;
    }

    File *PrefUtil::loadPrefFile(const char *file_name, const char *mode)
    {
        String path = ROOT;

        if (!SD.exists(path))
            if (!SD.mkdir(path))
            {
                log_e("Помилка створення ROOT каталогу");
                return nullptr;
            }

        path += PREF_DIR;

        if (!SD.exists(path))
            if (!SD.mkdir(path))
            {
                log_e("Помилка створення PREF_DIR каталогу");
                return nullptr;
            }

        path += "/";
        path += file_name;

        File file;

        if (mode == "w")
            file = SD.open(path, mode, true);
        else
            file = SD.open(path, mode);

        if (!file)
        {
            log_e("Помилка читання файлу налаштувань: %s", path);
            return nullptr;
        }

        if (file.isDirectory())
        {
            log_e("Помилка. Файл налаштувань не може бути каталогом: %s", path);
            file.close();
            return nullptr;
        }

        return new File(file);
    }
}
