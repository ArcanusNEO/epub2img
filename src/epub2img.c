#include <getopt.h>
#include <stdio.h>

#include "epub2img.h"

signed main(int argc, char* argv[]) {
  int   ch;
  char* in              = malloc(_MAX_PATH_LENGTH);
  char* out             = malloc(_MAX_PATH_LENGTH);
  int   is_vol_moe_type = 0;

  srand(time(NULL)), rand(), rand(), rand(), rand();
  strcpy(in, ".");
  strcpy(out, "./out");
  while (~(ch = getopt(argc, argv, "i:o:V"))) switch (ch) {
      case 'i': strcpy(in, optarg); break;
      case 'o': strcpy(out, optarg); break;
      case 'V': is_vol_moe_type = 1; break;
      default: break;
    }

  convert_epub(in, out, is_vol_moe_type);
  free(in);
  free(out);
}