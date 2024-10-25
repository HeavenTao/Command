#include <stdio.h>

typedef struct LineInfo {
  long size;
  char power[9];
  unsigned char type;
  char *name;
} LineInfo;

typedef struct LineInfo2 {
  long long size;
  unsigned char type;
  char *name;
  char power[10];
} LineInfo2;

int main() {
  printf("%lu %lu",sizeof(LineInfo),sizeof(LineInfo2));
  return 0;
}
