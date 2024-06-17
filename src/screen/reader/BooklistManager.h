#pragma once
#include <Arduino.h>

#include "meow/util/files/FileManager.h"

using namespace meow;

class BooklistManager : public FileManager
{
public:
    bool updateCategories();
    bool updateBooklist(const char *cat_name);
    uint16_t getCatSize(const char *cat_name);
    bool removeBook(const char *cat_name, const char *book_name);

    std::vector<String> getCategories();
    std::vector<String> getBooks(const char *cat_name, uint8_t start_pos, uint8_t size);

    String getBookPath(const char *playlist_name, const char *track_name);
};
