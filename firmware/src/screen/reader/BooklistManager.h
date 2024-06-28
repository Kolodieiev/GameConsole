#pragma once
#include <Arduino.h>

#include "meow/util/files/FileManager.h"

using namespace meow;

class BooklistManager : public FileManager
{
public:
    bool updateBookDirs();
    bool updateBooklist(const char *book_dir_name);
    uint16_t getDirSize(const char *book_dir_name);
    bool removeBook(const char *book_dir_name, const char *book_name);

    std::vector<String> getBookDirs();
    std::vector<String> getBooks(const char *book_dir_name, uint8_t start_pos, uint8_t size);

    String getBookPath(const char *book_dir_name, const char *book_name);

    size_t getBookSize(const char *book_dir_name, const char *book_name);
    String readText(bool &out_is_eof, const char *book_dir_name, const char *book_name, uint64_t pos, uint64_t size);

    bool containCyrillic(const char *book_dir_name, const char *book_name);

private:
    bool isCyrillic(char ch);
};
