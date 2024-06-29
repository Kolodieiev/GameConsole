#include "BooklistManager.h"

const char ROOT_PATH[] = "/books";
const char BOOKLIST_LIB_NAME[] = "/booklist.lib";
const char BOOK_EXT[] = ".txt";

bool BooklistManager::updateBookDirs()
{
    String db_path = ROOT_PATH;
    db_path += BOOKLIST_LIB_NAME;
    return indexDirs(ROOT_PATH, db_path.c_str());
}

bool BooklistManager::updateBooklist(const char *book_dir_name)
{
    String dir_path = ROOT_PATH;
    dir_path += "/";
    dir_path += book_dir_name;

    String db_path = ROOT_PATH;
    db_path += "/";
    db_path += book_dir_name;
    db_path += BOOKLIST_LIB_NAME;

    return indexFilesExt(dir_path.c_str(), BOOK_EXT, db_path.c_str());
}

uint16_t BooklistManager::getDirSize(const char *book_dir_name)
{
    String db_path = ROOT_PATH;
    db_path += "/";
    db_path += book_dir_name;
    db_path += BOOKLIST_LIB_NAME;

    return getDBSize(db_path.c_str());
}

bool BooklistManager::removeBook(const char *book_dir_name, const char *book_name)
{
    String db_path = ROOT_PATH;
    db_path += "/";
    db_path += book_dir_name;
    db_path += "/";
    db_path += book_name;

    return SD.remove(db_path.c_str());
}

std::vector<String> BooklistManager::getBookDirs()
{
    String db_path = ROOT_PATH;
    db_path += BOOKLIST_LIB_NAME;

    return readFilesFromDB(db_path.c_str(), 0, 100);
}

std::vector<String> BooklistManager::getBooks(const char *book_dir_name, uint8_t start_pos, uint8_t size)
{
    String db_path = ROOT_PATH;
    db_path += "/";
    db_path += book_dir_name;
    db_path += BOOKLIST_LIB_NAME;

    return readFilesFromDB(db_path.c_str(), start_pos, size);
}

String BooklistManager::getBookPath(const char *book_dir_name, const char *book_name)
{
    String path = ROOT_PATH;
    path += "/";
    path += book_dir_name;
    path += "/";
    path += book_name;

    if (!SD.exists(path.c_str()))
        return "";

    return path;
}

size_t BooklistManager::getBookSize(const char *book_dir_name, const char *book_name)
{
    String path = getBookPath(book_dir_name, book_name);

    if (path.isEmpty())
    {
        log_e("Файл не знайдено: %s %s", book_dir_name, book_name);
        return 0;
    }

    File f = SD.open(path.c_str(), FILE_READ);

    if (!f)
        return 0;

    size_t f_size = f.size();

    f.close();

    return f_size;
}

bool BooklistManager::containCyrillic(const char *book_dir_name, const char *book_name)
{
    String path = getBookPath(book_dir_name, book_name);

    if (path.isEmpty())
    {
        log_e("Файл не знайдено: %s %s", book_dir_name, book_name);
        return false;
    }

    File f = SD.open(path.c_str(), FILE_READ);

    if (!f)
        return false;

    String str = f.readString();

    for (size_t i{0}; i < str.length(); ++i)
    {
        if (isCyrillic(str.c_str()[i]))
            return true;
    }

    return false;
}

bool BooklistManager::isCyrillic(char ch)
{
    unsigned char uc = static_cast<unsigned char>(ch);
    return (uc >= 0xC0 && uc <= 0xFF) || (uc >= 0x80 && uc <= 0xBF);
}

String BooklistManager::readText(bool &out_is_eof, const char *book_dir_name, const char *book_name, uint64_t pos, uint64_t size)
{
    String book_path = getBookPath(book_dir_name, book_name);

    if (book_path.isEmpty())
    {
        log_e("Файл не знайдено: %s", book_path.c_str());
        return "";
    }

    File file = SD.open(book_path.c_str(), FILE_READ);

    if (!file)
    {
        log_e("Помилка читання файлу: %s", book_path.c_str());
        return "";
    }

    String ret_str;

    char *buffer = new char[size + 1];
    if (!buffer)
    {
        log_e("Помилка створення буферу на %lu байт", size);
        goto exit;
    }

    if (!file.seek(pos))
    {
        log_e("Помилка встановлення позиції: %lu", pos);
        goto exit;
    }

    {
        size_t bytes_read = file.readBytes(buffer, size);

        if (bytes_read == size)
            out_is_eof = false;
        else
            out_is_eof = true;

        buffer[bytes_read] = '\0';

        if (bytes_read > 2 && (buffer[bytes_read - 1] & 0x80) != 0)
        {
            size_t correct_char_pos{0};

            for (size_t i{bytes_read - 2}; i > 0; --i)
            {
                if ((buffer[i] & 0x80) == 0)
                {
                    correct_char_pos = i;
                    break;
                }
            }

            if (correct_char_pos > 0)
            {
                char *temp = new char[correct_char_pos + 2];
                if (temp)
                {
                    memcpy(temp, buffer, correct_char_pos + 1);
                    temp[correct_char_pos + 1] = '\0';

                    ret_str = String(temp);
                    delete[] temp;
                }
                else
                    log_e("Помилка створення temp буферу на %lu байт", size);
            }
        }
        else
            ret_str = String(buffer);
    }

exit:
    file.close();
    delete[] buffer;
    return ret_str;
}