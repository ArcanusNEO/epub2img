#ifndef _EPUB2IMG_H
#define _EPUB2IMG_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <dirent.h>

#define _MAX_PATH_LENGTH 4096
#define _MAX_FILE_NAME 255
#define _MAX_CMD_LENGTH 9000

void read_from_html(const char* __path, const char* __out);
void read_from_opf(const char* __path, const char* __out);
void convert_epub(const char* __path, const char* __out, int __volMoe);

#endif