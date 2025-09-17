#include <errno.h>
#include <stdlib.h>

void * memset(void *, int, size_t);
int puts(const char *);

int main(int argc, char ** argv) {
  if (argc <= 1) {
    puts("There is nothing here!");
    return 1;
  }
  long long val = strtoll(argv[1], NULL, 10);
  if (errno == ERANGE) {
    puts("Input is out of range!");
    return 2;
  }
  char res[65];
  { // sets res buffer
    res[64] = 0;
    int index = 63;
    for (; index >= 0 && val; index--, val >>= 1U) {
      res[index] = (val & 1U) ? '1' : '0';
    }
    memset(res, '0', (size_t)index + 1);
  }
  if (val >= 0 && argv[2] != NULL && argv[2][0] == '-' && argv[2][1] == 'u') {
    const char * new_start = res;
    while (*new_start != 0 && *new_start != '1') {
      new_start++;
    }
    puts(*new_start == 0 ? "0" : new_start);
  } else {
    puts(res);
  }
  return 0;
}
