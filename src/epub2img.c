#include <getopt.h>
#include <stdio.h>

#include "epub2img.h"

int main(int argc, char* argv[]) {
  int   ch;
  char* in           = malloc(sizeof(char) * _MAX_PATH_LENGTH);
  char* out          = malloc(sizeof(char) * _MAX_PATH_LENGTH);
  int   isVolMoeType = 0;

  srand(time(NULL)), rand(), rand(), rand(), rand();
  strcpy(in, ".");
  strcpy(out, "./out");
  while (~(ch = getopt(argc, argv, "f:o:V"))) {
    switch (ch) {
      case 'f': strcpy(in, optarg); break;
      case 'o': strcpy(out, optarg); break;
      case 'V': isVolMoeType = 1; break;
      default: break;
    }
  }

  convert_epub(in, out, isVolMoeType);
  free(in);
  free(out);
  return 0;
}