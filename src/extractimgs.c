#include "epub2img.h"

const char _CONTAINERSTR[] = "rootfilefull-path=\"";
#define _CONTAINERN 20

const char _ITEMSTR[] = "itemid=\"";
#define _ITEMN 9

const char _HREFSTR[] = "href=\"";
#define _HREFN 7

const char _TYPESTR[] = "media-type=\"";
#define _TYPEN 13

const char _JPEGSTR[] = "image/jpeg";
#define _JPEGN 11

const char _PNGSTR[] = "image/png";
#define _PNGN 10

const char _IMGSTR[] = "imgsrc=\"";
#define _IMGN 9

void convert_epub(const char* __path, const char* __out, int __volMoe) {
  int   i, j;
  char* path = malloc(sizeof(char) * _MAX_FILE_NAME);
  char* cmd  = malloc(sizeof(char) * _MAX_CMD_LENGTH);

  for (i = 0; "/tmp/"[i]; ++i) { path[i] = "/tmp/"[i]; }
  for (j = 0; j < 32; ++i, ++j) {
    if (rand() & 1) {
      path[i] = rand() % 26 + 'a';
    } else {
      path[i] = rand() % 10 + '0';
    }
  }
  path[i] = '\0';
  strcpy(cmd, "mkdir ");
  strcat(cmd, path);
  system(cmd);
  strcpy(cmd, "unzip -d ");
  strcat(cmd, path);
  strcat(cmd, " \"");
  strcat(cmd, __path);
  strcat(cmd, "\"");
  system(cmd);

  strcpy(cmd, "mkdir -p \"");
  strcat(cmd, __out);
  strcat(cmd, "\"");
  system(cmd);

  if (__volMoe) {
    read_from_html(path, __out);
  } else {
    read_from_opf(path, __out);
  }
  strcpy(cmd, "rm -rf ");
  strcat(cmd, path);
  system(cmd);

  free(path);
  free(cmd);
}

void read_from_opf(const char* __path, const char* __out) {
  FILE* fp;
  int   ch, i, j, k;
  char* path   = malloc(sizeof(char) * _MAX_PATH_LENGTH);
  char* buffer = malloc(sizeof(char) * _MAX_PATH_LENGTH);
  char* cmd    = malloc(sizeof(char) * _MAX_CMD_LENGTH);
  char* item   = malloc(sizeof(char) * _MAX_FILE_NAME);

  strcpy(path, __path);
  strcat(path, "/META-INF/container.xml");
  if (!(fp = fopen(path, "r"))) {
    free(path);
    free(buffer);
    free(item);
    free(cmd);
    return;
  }
  for (i = -1, ch = fgetc(fp); ~ch; ch = fgetc(fp)) {
    if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') { continue; }
    if (ch == _CONTAINERSTR[++i]) {
      if (i == _CONTAINERN - 2) {
        for (buffer[0] = '/', i = 1, ch = fgetc(fp); ~ch && ch != '\"';
             ch = fgetc(fp), ++i) {
          buffer[i] = ch;
        }
        buffer[i] = '\0';
        break;
      }
    } else {
      i = -1;
    }
  }
  fclose(fp);
  strcpy(path, __path);
  strcat(path, buffer);

  if (!(fp = fopen(path, "r"))) {
    free(path);
    free(buffer);
    free(item);
    free(cmd);
    return;
  }
  for (j = strlen(path), i = j - 1; path[i] != '/'; --i) { path[i] = '\0'; }
  for (i = -1, j = -1, k = -1, ch = fgetc(fp); ~ch; ch = fgetc(fp)) {
    if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') { continue; }
    if (ch == _ITEMSTR[++i]) {
      if (i == _ITEMN - 2) {
        for (i = 0, ch = fgetc(fp); ~ch && ch != '\"'; ch = fgetc(fp), ++i) {
          item[i] = ch;
        }
        item[i] = '\0';
        i = j = k = -1;
      }
    } else {
      i = -1;
    }
    if (ch == _HREFSTR[++j]) {
      if (j == _HREFN - 2) {
        for (buffer[0] = '/', i = 1, ch = fgetc(fp); ~ch && ch != '\"';
             ch = fgetc(fp), ++i) {
          buffer[i] = ch;
        }
        buffer[i] = '\0';
        i = j = k = -1;
      }
    } else {
      j = -1;
    }
    if (ch == _TYPESTR[++k]) {
      if (k == _TYPEN - 2) {
        for (i = 0, ch = fgetc(fp); (i <= _JPEGN - 2 && ch == _JPEGSTR[i])
                                    || (i <= _PNGN - 2 && ch == _PNGSTR[i]);
             ch = fgetc(fp), ++i) {
          NULL;
        }
        if (ch == '\"' && (i == _JPEGN - 1 || i == _PNGN - 1)) {
          strcpy(cmd, "mv \"");
          for (i = 4, j = i; path[i - j]; ++i) { cmd[i] = path[i - j]; }
          for (j = i; buffer[i - j]; ++i) { cmd[i] = buffer[i - j]; }
          cmd[i++] = '\"';
          cmd[i++] = ' ';
          cmd[i++] = '\"';
          for (j = i; __out[i - j]; ++i) { cmd[i] = __out[i - j]; }
          cmd[i++] = '/';
          for (j = i; item[i - j]; ++i) { cmd[i] = item[i - j]; }
          for (j = strlen(buffer) - 1; buffer[j] != '.'; --j) { NULL; }
          for (; buffer[j]; ++i, ++j) { cmd[i] = buffer[j]; }
          cmd[i++] = '\"';
          cmd[i]   = '\0';
          system(cmd);
        }
        i = j = k = -1;
      }
    } else {
      k = -1;
    }
  }

  fclose(fp);
  free(item);
  free(path);
  free(buffer);
  free(cmd);
}

void read_from_html(const char* __path, const char* __out) {
  DIR*           dp;
  struct dirent* dirp;
  FILE*          fp;
  char*          path = malloc(sizeof(char) * _MAX_PATH_LENGTH);
  char*          ext  = malloc(sizeof(char) * _MAX_FILE_NAME);
  char*          cmd  = malloc(sizeof(char) * _MAX_CMD_LENGTH);
  int            ch, i, j, k;

  strcpy(path, __path);
  strcat(path, "/html/");
  if (!(dp = opendir(path))) {
    free(path);
    free(ext);
    free(cmd);
    return;
  }
  while (dirp = readdir(dp)) {
    if (dirp->d_type != 8) { continue; }
    strcpy(path, __path);
    strcat(path, "/html/");
    strcat(path, dirp->d_name);
    if (!(fp = fopen(path, "r"))) {
      closedir(dp);
      free(path);
      free(ext);
      free(cmd);
      return;
    }
    for (i = -1, ch = fgetc(fp); ~ch; ch = fgetc(fp)) {
      if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') { continue; }
      if (ch == _IMGSTR[++i]) {
        if (i == _IMGN - 2) {
          for (j = i = 0; __path[i - j]; ++i) { path[i] = __path[i - j]; }
          for (j = i; "/html/"[i - j]; ++i) { path[i] = "/html/"[i - j]; }
          for (ch = fgetc(fp); ~ch && ch != '\"'; ch = fgetc(fp), ++i) {
            if (ch == '{') { goto LFinishMv; }
            path[i] = ch;
          }
          path[i] = '\0';
          for (j = i = 0; "mv \""[i - j]; ++i) { cmd[i] = "mv \""[i - j]; }
          for (j = i; path[i - j]; ++i) { cmd[i] = path[i - j]; }
          for (k = _MAX_FILE_NAME - 2, ext[_MAX_FILE_NAME - 1] = '\0';
               cmd[i - _MAX_FILE_NAME + 1 + k] != '.'; --k) {
            ext[k] = cmd[i - _MAX_FILE_NAME + 1 + k];
          }
          ext[k] = '.';
          for (j = i; "\" \""[i - j]; ++i) { cmd[i] = "\" \""[i - j]; }
          for (j = i; __out[i - j]; ++i) { cmd[i] = __out[i - j]; }
          cmd[i++] = '/';
          for (j = 0; dirp->d_name[j] != '.'; ++i, ++j) {
            cmd[i] = dirp->d_name[j];
          }
          for (; ext[k]; ++i, ++k) { cmd[i] = ext[k]; }
          cmd[i++] = '\"';
          cmd[i]   = '\0';
          // puts(cmd);
          system(cmd);

LFinishMv:
          i = j = k = -1;
        }
      } else {
        i = -1;
      }
    }
    fclose(fp);
  }

  closedir(dp);
  free(path);
  free(ext);
  free(cmd);
}
