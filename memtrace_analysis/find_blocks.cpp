#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <map>
#include <vector>
#include <iostream>
#include <stdint.h>
#include "memtrace.h"
#define LEN 100
struct trace_block{
  uint64_t ic_start, ic_end;
  std::map<uint64_t, std::vector<uint8_t>> reads;
  std::map<uint64_t, std::vector<uint8_t>> writes;
  
};
std::ostream & operator<<(std::ostream &os,std::map<uint64_t, std::vector<uint8_t> >&m){
  os << "["<< std::endl;
  bool firstblock = true;
  for(auto &c : m){
    if(!firstblock)
      os << ",";
    
    os << "{\n"
       << "\"address\":"<< c.first <<  ",\n";
    os << "\"size\": "<<c.second.size() << ",\n";
    os << "\"data\": [";
    bool first =true;
    for (auto  &i : c.second){
      if(!first)
        os << ",";
      os << (uint16_t)i;
      first = false;
    }
    os << "]\n }" << std::endl;
  }
  os << "]";
  return os;
}
std::ostream &operator<<(std::ostream &os, trace_block &tb){
  os<< "{\n"
    << "\"begin\":" << tb.ic_start << ",\n"
    << "\"end\":" << tb.ic_end << ",\n"
    << "\"reads\":"<< tb.reads << ",\n"
    << "\"writes\":"<< tb.writes<< "\n"
    << "}";
  return os;
}
void vectorize_set(std::map<uint64_t, uint8_t> &m, std::map<uint64_t, std::vector<uint8_t> > &out){
  uint64_t lastaddr = -1;
  int size=-1;
  std::vector<uint8_t> * v;
  for (auto &c: m){
    if(lastaddr +1!= c.first || size  == -1){
      
      size = 0;
      v = &out[c.first];
    }
    v->push_back(c.second);
    lastaddr = c.first;
    size++;
  }
}
void read_tb(trace_block &out){
  trace_message m;
  int len = 0;
  do {
    fread(&m,sizeof m,1,stdin);
    assert(!feof(stdin));
  } while(m.type!= CRYPTO_BEGIN);
  std::cout << "block beginning\t " << m.addr << std::endl;
  out.ic_start = m.addr;
  std::map<uint64_t, uint8_t> reads;
  std::map<uint64_t, uint8_t> writes;
  do {
    fread(&m,sizeof m,1,stdin);
    if(feof(stdin))
      assert(m.type == CRYPTO_END);
    if(m.type == CRYPTO_READ)
      reads[m.addr] = m.data;
    else if (m.type == CRYPTO_WRITE)
      writes[m.addr] = m.data;
  } while(m.type!= CRYPTO_END);
  vectorize_set(reads, out.reads);
  vectorize_set(writes,out.writes);
  out.ic_end = m.addr;
}
int main(){
  int i=0;
  for(;!feof(stdin);i++){
    trace_block tb;
    read_tb(tb);
    std::cout << tb << std::endl;
  }
}
