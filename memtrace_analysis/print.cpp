#include <stdlib.h>
#include <stdio.h>
#include "memtrace.h"
#define LEN 100
int main(){
  trace_message m;
  do {
    fread(&m,sizeof m,1,stdin);
    printf("%16llX\t" "%u\t" "%hhu\n", m.addr, m.type, (unsigned char) m.data );
  } while(!feof(stdin));
}
