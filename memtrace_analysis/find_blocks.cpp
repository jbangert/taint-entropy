#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <iostream>
#include <stdint.h>
#include "memtrace.h"
#define LEN 100
struct trace_blocks{
  uint64_t ic_start, ic_end;
  std::map<uint64_t, std::vector<uint8_t>> reads;
  std::map<uint64_t, std::vector<uint8_t>> writes;
  
};
void print_set(std::map<uint64_t, uint8_t> &m, std::map<uint64_t, std::vector<uint8_t>> &out){
  uint64_t lastaddr = -1;
  int size=-1;
  std::vector<uint8_t>&v;
  for (auto &c: m){
    if(lastaddr +1!= c.first || size  == -1){
      size = 0;
      v = out[c.first];
    }
    lastaddr = c.first;
    size++;
  }
  printf("\t%d\n",size);
}
int main(){
  trace_message m;
  int i=0;
  for(;!feof(stdin);i++){
    int len = 0;
    do {
      fread(&m,sizeof m,1,stdin);
    } while(!feof(stdin) && m.type!= CRYPTO_BEGIN);
    std::cout << "block beginning\t " << m.addr << std::endl;
    std::map<uint64_t, uint8_t> reads;
    std::map<uint64_t, uint8_t> writes;
    do {
      fread(&m,sizeof m,1,stdin);  
      if(m.type == CRYPTO_READ)
        reads[m.addr] = m.data;
      else if (m.type == CRYPTO_WRITE)
        writes[m.addr] = m.data;
    } while(!feof(stdin) && m.type!= CRYPTO_END);
    uint64_t lastaddr = -1;
        print_set("read",reads);0
    print_set("write",writes);
  }
}
