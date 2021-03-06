#include <stdlib.h>
#include <stdio.h>
#include "memtrace.h"
#define LEN 100
int main(){
  trace_message m;
  int total_read;
  int total_write;
  int i=0;
  for(;i<LEN && !feof(stdin);i++){
    int len = 0;
    do {
      fread(&m,sizeof m,1,stdin);
    } while(!feof(stdin) && m.type!= CRYPTO_BEGIN);
    do {
      fread(&m,sizeof m,1,stdin);
      len++;
      if(m.type == CRYPTO_WRITE)
        total_write++;
      else if (m.type==CRYPTO_READ)
        total_read++;
    } while(!feof(stdin) && m.type!= CRYPTO_END);
    //    printf("%d %ul\n",i,len);
  }
  printf("Reads %f\n", (double) total_read/ (double)i);
  printf("Writes %f\n", (double) total_write/ (double)i);
}
