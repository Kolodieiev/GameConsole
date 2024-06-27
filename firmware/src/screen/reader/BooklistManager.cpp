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

    return readFilesFromDB(db_path.c_str(), 0, 0);
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

String BooklistManager::readText(const char *book_dir_name, const char *book_name, uint64_t pos, uint64_t size)
{
    char *buffer = new char[size + 1];

    if (!buffer)
    {
        log_e("Помилка створення буферу на %lu байт", size);
        return "";
    }

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

    if (!file.seek(pos))
    {
        log_e("Помилка встановлення позиції: %lu", pos);
    }

    size_t bytes_read = file.readBytes(buffer, size);
    buffer[bytes_read] = '\0';

    String str = String(buffer);

    file.close();
    delete[] buffer;

    return str;
}
