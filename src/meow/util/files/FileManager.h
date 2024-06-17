#pragma once
#include <Arduino.h>
#include <SD.h>
#include <vector>
#include "../sd/SdUtil.h"

namespace meow
{
    class FileManager
    {
    public:
        static const char STR_DIR_PREFIX[];

        ~FileManager();

        bool fileExist(const char *path);
        bool dirExist(const char *path);

        bool indexFilesExt(const char *dir_path, const char *file_ext, const char *db_path);
        bool indexFiles(const char *dir_path, const char *db_path);
        bool indexDirs(const char *dir_path, const char *db_path);
        bool indexAll(const char *dir_path, const char *db_path);
        void stopTasks();

        bool remove(const char *path);
        bool rename(const char *old_name, const char *new_name);

        bool createDir(const char *path);

        void end();

        uint16_t getDBSize(const char *db_path);
        std::vector<String> readFilesFromDB(const char *db_path, uint16_t start_pos, uint16_t size);
        std::vector<String> readFilesFromSD(const char *dir_path, uint16_t start_pos, uint16_t size);

        bool copyFile(const char *from, const char *to);

        inline bool isTaskDone() const { return _task_params.is_done; }
        inline bool isTaskSucceeded() const { return _task_params.is_succeeded; }
        inline uint8_t getCopyProgress() const { return _copy_progress; }
        bool hasConnection();

    protected:
        enum IndexMode : uint8_t
        {
            INDX_MODE_DIR = 0,
            INDX_MODE_FILES,
            INDX_MODE_FILES_EXT,
            INDX_MODE_ALL
        };

        struct TaskParams
        {
            bool is_canceled;
            bool is_done;
            bool is_succeeded;
        } static _task_params;

        static String _dir_path;
        static String _file_ext;
        static String _db_path;
        static String _old_name;
        static String _new_name;
        static uint8_t _copy_progress;
        static IndexMode _index_mode;

    private:
        SdUtil _sd;

        bool startIndexTask(IndexMode mode, const char *dir_path, const char *db_path);

        static void indexTask(void *params);
        //
        static void rmTask(void *params);
        static bool rm(const char *path);
        //
        static void copyFileTask(void *params);
        static bool strEndsWith(const char *str, const char *suffix);

        static void freeRes();
    };
}