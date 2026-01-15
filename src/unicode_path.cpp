#include "unicode_path.h"
#include <string>
#include <vector>
#include <cstring>
#include <errno.h>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <fcntl.h>
#include <io.h>

wchar_t* utf8_to_wchar(const char* src){
    if (!src) return NULL;
    int len = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
    if(len == 0) return NULL;
    wchar_t* out = (wchar_t*)malloc(len * sizeof(wchar_t));
    if(!out) return NULL;
    MultiByteToWideChar(CP_UTF8, 0, src, -1, out, len);
    return out;
}

char* wchar_to_utf8(const wchar_t* src){
    if (!src) return NULL;
    int len = WideCharToMultiByte(CP_UTF8, 0, src, -1, NULL, 0, NULL, NULL);
    if(len == 0) return NULL;
    char* out = (char*)malloc(len);
    if(!out) return NULL;
    WideCharToMultiByte(CP_UTF8, 0, src, -1, out, len, NULL, NULL);
    return out;
}

FILE* fopen_utf8(const char* filename, const char* mode){
    wchar_t* wfile = utf8_to_wchar(filename);
    if(!wfile) return NULL;
    int mlen = MultiByteToWideChar(CP_UTF8, 0, mode, -1, NULL, 0);
    wchar_t* wmode = (wchar_t*)malloc(mlen * sizeof(wchar_t));
    if(!wmode){ free(wfile); return NULL; }
    MultiByteToWideChar(CP_UTF8, 0, mode, -1, wmode, mlen);
    FILE* f = _wfopen(wfile, wmode);
    free(wfile);
    free(wmode);
    return f;
}

int stat_utf8(const char* path, struct stat* buf){
    wchar_t* w = utf8_to_wchar(path);
    if(!w) return -1;
#ifdef _MSC_VER
    struct _stat64 st64;
    int res = _wstat64(w, &st64);
    if (res == 0 && buf) {
        // copy commonly used fields
        buf->st_mode = (decltype(buf->st_mode))st64.st_mode;
        buf->st_size = (decltype(buf->st_size))st64.st_size;
        buf->st_mtime = st64.st_mtime;
        buf->st_atime = st64.st_atime;
        buf->st_ctime = st64.st_ctime;
    }
#else
    int res = _wstat64(w, buf);
#endif
    free(w);
    return res;
}

int unlink_utf8(const char* path){
    wchar_t* w = utf8_to_wchar(path);
    if(!w) return -1;
    int res = _wunlink(w);
    free(w);
    return res;
}

int remove_utf8(const char* path){
    return unlink_utf8(path);
}

int rename_utf8(const char* oldpath, const char* newpath){
    wchar_t* wold = utf8_to_wchar(oldpath);
    wchar_t* wnew = utf8_to_wchar(newpath);
    if(!wold || !wnew){ free(wold); free(wnew); return -1; }
    BOOL r = MoveFileExW(wold, wnew, MOVEFILE_REPLACE_EXISTING);
    free(wold); free(wnew);
    return r ? 0 : -1;
}

int access_utf8(const char* path, int amode){
    wchar_t* w = utf8_to_wchar(path);
    if(!w) return -1;
    int res = _waccess(w, amode);
    free(w);
    return res;
}

int mkdir_utf8(const char* path, int mode){
    wchar_t* w = utf8_to_wchar(path);
    if(!w) return -1;
    int res = _wmkdir(w);
    free(w);
    return res;
}

int GetFullPathUtf8(const char* path, char* outBuf, int outBufSize){
    wchar_t* w = utf8_to_wchar(path);
    if(!w) return 0;
    wchar_t wout[MAX_PATH];
    DWORD n = GetFullPathNameW(w, MAX_PATH, wout, NULL);
    free(w);
    if(n == 0) return 0;
    char* tmp = wchar_to_utf8(wout);
    if(!tmp) return 0;
    strncpy(outBuf, tmp, outBufSize - 1);
    outBuf[outBufSize - 1] = '\0';
    free(tmp);
    return (int)strlen(outBuf);
}

#else // POSIX

FILE* fopen_utf8(const char* filename, const char* mode){
    return fopen(filename, mode);
}

int stat_utf8(const char* path, struct stat* buf){
    return stat(path, buf);
}

int unlink_utf8(const char* path){
    return unlink(path);
}

int remove_utf8(const char* path){
    return remove(path);
}

int rename_utf8(const char* oldpath, const char* newpath){
    return rename(oldpath, newpath);
}

int access_utf8(const char* path, int amode){
    return access(path, amode);
}

int mkdir_utf8(const char* path, int mode){
    return mkdir(path, mode);
}

int GetFullPathUtf8(const char* path, char* outBuf, int outBufSize){
    if(!realpath(path, outBuf)) return 0;
    return (int)strlen(outBuf);
}

#endif

long long filesize_utf8(const char* path){
    struct stat s;
    if(stat_utf8(path, &s) != 0) return -1;
    return s.st_size;
}

bool exists_utf8(const char* path){
    struct stat s;
    return stat_utf8(path, &s) == 0;
}

bool isDirectory_utf8(const char* path){
    struct stat sb;
    if (stat_utf8(path, &sb) != 0) return false;
    return (sb.st_mode & S_IFDIR) != 0;
}

#ifdef _WIN32
#include "zlib.h"

// gzopen_utf8: fallback to gzopen(filename,mode). We avoid gzdopen since it's not
// available in this zlib build. On Windows this may fail for non-ANSI paths, but
// the code paths that required gzopen have been replaced to use fopen_utf8+inflate.
gzFile gzopen_utf8(const char* filename, const char* mode){
    (void)filename; (void)mode;
    return gzopen(filename, mode);
}
#else
gzFile gzopen_utf8(const char* filename, const char* mode){
    return gzopen(filename, mode);
}
#endif
