#pragma GCC optimize("O3")
#include "BooklistManager.h"

const char ROOT_PATH[] = "/books";
const char BOOKLIST_LIB_NAME[] = "/booklist.lib";
const char BOOK_EXT[] = ".txt";

bool BooklistManager::updateBookDirs(std::function<void()> on_finish)
{
    String db_path = ROOT_PATH;
    db_path += BOOKLIST_LIB_NAME;
    return indexDirs(ROOT_PATH, db_path.c_str(), on_finish);
}

bool BooklistManager::updateBooklist(const char *book_dir_name, std::function<void()> on_finish)
{
    String dir_path = ROOT_PATH;
    dir_path += "/";
    dir_path += book_dir_name;

    String db_path = ROOT_PATH;
    db_path += "/";
    db_path += book_dir_name;
    db_path += BOOKLIST_LIB_NAME;

    return indexFilesExt(dir_path.c_str(), BOOK_EXT, db_path.c_str(), on_finish);
}

bool BooklistManager::removeBook(const char *book_dir_name, const char *book_name, std::function<void()> on_finish)
{
    String db_path = ROOT_PATH;
    db_path += "/";
    db_path += book_dir_name;
    db_path += "/";
    db_path += book_name;

    return startRemoving(db_path.c_str(), on_finish);
}

uint16_t BooklistManager::getDirSize(const char *book_dir_name)
{
    String db_path = ROOT_PATH;
    db_path += "/";
    db_path += book_dir_name;
    db_path += BOOKLIST_LIB_NAME;

    return getDBSize(db_path.c_str());
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

    return path;
}

size_t BooklistManager::getBookSize(const char *book_dir_name, const char *book_name)
{
    String path = getBookPath(book_dir_name, book_name);
    return getFileSize(path.c_str());
}

bool BooklistManager::containCyrillic(const char *book_dir_name, const char *book_name)
{
    String path = getBookPath(book_dir_name, book_name);

    const uint8_t ARR_SIZE = 160;
    char ch_arr[ARR_SIZE];

    size_t read_bytes = readFile(ch_arr, path.c_str(), ARR_SIZE);

    for (size_t i{0}; i < read_bytes; ++i)
    {
        if (isCyrillic(ch_arr[i]))
            return true;
    }

    return false;
}

bool BooklistManager::isCyrillic(char ch)
{
    unsigned char uc = static_cast<unsigned char>(ch);
    return (uc >= 0xC0 && uc <= 0xFF) || (uc >= 0x80 && uc <= 0xBF);
}

String BooklistManager::readText(bool &out_is_eof, const char *book_dir_name, const char *book_name, uint32_t pos, uint32_t size)
{
    String book_path = getBookPath(book_dir_name, book_name);

    String ret_str;

    char *buffer = (char *)malloc(size + 1);

    if (!buffer)
    {
        log_e("malloc error: %lu b", size + 1);
        return ret_str;
    }

    uint32_t bytes_read = readFile(buffer, book_path.c_str(), size, pos);

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
            char *temp_buff = (char *)malloc(correct_char_pos + 2);
            if (temp_buff)
            {
                memcpy(temp_buff, buffer, correct_char_pos + 1);
                temp_buff[correct_char_pos + 1] = '\0';

                ret_str = String(temp_buff);
                free(temp_buff);
            }
            else
                log_e("Помилка створення temp буферу на %lu байт", size);
        }
    }
    else
        ret_str = String(buffer);

    free(buffer);
    return ret_str;
}