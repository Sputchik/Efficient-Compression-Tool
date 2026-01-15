#pragma once

#include <stdio.h>
#include <sys/stat.h>

// UTF-8 aware wrappers for file system operations. On Windows these convert
// UTF-8 input to UTF-16 and call wide APIs; on other platforms they forward
// to the standard C APIs.

#ifdef __cplusplus
extern "C" {
#endif

FILE* fopen_utf8(const char* filename, const char* mode);
int stat_utf8(const char* path, struct stat* buf);
int unlink_utf8(const char* path);
int rename_utf8(const char* oldpath, const char* newpath);
int remove_utf8(const char* path);
int access_utf8(const char* path, int amode);
int mkdir_utf8(const char* path, int mode);
int GetFullPathUtf8(const char* path, char* outBuf, int outBufSize);

// zlib gzip open accepting UTF-8 filenames on Windows
#ifdef _WIN32
#include "zlib/zlib.h"
#include "zlib/gzguts.h"
#else
#include <zlib.h>
#endif

gzFile gzopen_utf8(const char* filename, const char* mode);

// Convenience helpers used in this project
long long filesize_utf8(const char* path);
bool exists_utf8(const char* path);
bool isDirectory_utf8(const char* path);

#ifdef _WIN32
// Convert UTF-8 -> UTF-16
wchar_t* utf8_to_wchar(const char* src);
char* wchar_to_utf8(const wchar_t* src);
#endif

#ifdef __cplusplus
}
#endif
