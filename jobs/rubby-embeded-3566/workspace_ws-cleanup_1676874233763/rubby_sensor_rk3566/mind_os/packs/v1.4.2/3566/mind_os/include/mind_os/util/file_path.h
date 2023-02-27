#pragma once

#include <string>
#include <vector>

namespace mind_os {

namespace util {

class FilePath {
public:
    // [brief] gets the full path of the current process.
    // [note] Returns an empty string on failure.
    // e.g. "/home/ubuntu/sample"
    static std::string getCurExeWholePath();

    // [brief] gets the path where the current process file is located.
    // [note] Returns an empty string on failure.
    // e.g. "/home/ubuntu/sample" => "/home/ubuntu/"
    static std::string getCurExePath();

    // [brief] get current process name.
    // e.g. "/home/ubuntu/sample" => "sample"
    static std::string getCurExeName();

    static bool isFileExisted(const char* filePath);

    static bool isDirExisted(const char* dirPath);

    static bool isFileExisted(std::string& filePath);

    static bool isDirExisted(std::string& dirPath);

    // no support recursive creation
    static bool createDir(const char* dirPath);
    // support recursive creation
    static bool createDirR(const char* dirPath);

    // no support recursive creation
    static bool createFile(const char* filePath);

    // support recursive creation
    static bool createFileR(const char* filePath);

    // no support recursive remove
    static bool deleteFile(const char* filePath);

    // no support recursive remove
    static bool deleteDir(const char* dirPath);

    // support recursive remove
    static bool deleteDirR(const char* dirPath);

    // get file size
    // return size on success, return -1 failure
    static int getFileSize(const char* filePath);

    // move fileOld to fileNew
    static bool moveFile(const char* fileOld, const char* fileNew);
    // move dirOld to dirNew
    static bool moveDir(const char* dirOld, const char* dirNew);

    // rename fileOld to fileNew
    static bool renameFile(const char* fileOld, const char* fileNew);

    // rename dirOld to dirNew
    static bool renameDir(const char* dirOld, const char* dirNew);

    // copy file to other dir
    static bool copyFile(const char* source, const char* dest);

    // copy dir to other dir
    static bool copyDir(const char* source, const char* dest);

    // get file list by dir.  'depth': Recursive lookup depth.
    static std::vector<std::string> getFileList(const std::string& dirPath, size_t depth);

    static std::vector<std::string> getAllDirs(std::string path, bool recursive = false);

    static std::string getFileNameFromPath(std::string path);

    static size_t getDirectorySize(std::string dir);
};

}  // namespace util
}  // namespace mind_os
