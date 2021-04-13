#include <stdio.h>
int
main(void)
{
  FILE *fi, *fo;
  int b;
  fi = fopen("in", "r");
  fo = fopen("out", "w");
  b = fgetc(fi);
  while (EOF != b) {
    fputc(b, fo);
    b = fgetc(fi);
  }
  fclose(fi);
  fclose(fo);

  printf("double close\n");
  fclose(fi);
  printf("close(NULL)\n");
  fclose(NULL);  // null fclose
  printf("Done\n");
}
