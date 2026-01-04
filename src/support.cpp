//
//  support.cpp
//  Efficient Compression Tool
//
//  Created by Felix Hanau on 09.04.15.
//  Copyright (c) 2015 Felix Hanau.
//

#include "support.h"
#include "unicode_path.h"
#include <sys/stat.h>
#ifdef _WIN32
#include <Windows.h>
#endif
#ifdef _MSC_VER
#include <sys/utime.h>
#include <io.h>
#define access _access
#define W_OK 2
#else
#include <utime.h>
#endif
#include <time.h>
#include <stdio.h>

long long filesize (const char * Infile) {
    return filesize_utf8(Infile);
}

bool exists(const char * Infile) {
    return exists_utf8(Infile);
}

bool writepermission (const char * Infile) {
    return access_utf8(Infile, W_OK) == 0;
}

void RenameAndReplace(const char * Infile, const char * Outfile) {
    // rename_utf8 uses MoveFileExW on Windows and standard rename() on POSIX
    if (rename_utf8(Infile, Outfile) != 0){
        // best-effort fallback
        rename(Infile, Outfile);
    }
}

bool isDirectory(const char *path) {
  return isDirectory_utf8(path);
}

time_t get_file_time(const char* Infile){
  struct stat stats;
  if(stat_utf8(Infile, &stats)){
    printf("%s: Could not get time\n", Infile);
    return -1;
  }
  return stats.st_mtime;
}

void set_file_time(const char* Infile, time_t otime){
  #ifdef _WIN32
  struct _utimbuf wtime;
  wtime.actime = time(0);
  wtime.modtime = otime;
  wchar_t* w = utf8_to_wchar(Infile);
  if(!w || _wutime(w, &wtime)){
    printf("%s: Could not set time\n", Infile);
  }
  if(w) free(w);
  #else
  struct utimbuf oldtime;
  oldtime.actime = time(0);
  oldtime.modtime = otime;
  if (utime(Infile, &oldtime)){
    printf("%s: Could not set time\n", Infile);
  }
  #endif
}
