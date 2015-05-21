#include <stdlib.h>
#include <stdio.h>
#include <cassert>
#include <map>
#include <vector>
#include <iostream>
#include <stdint.h>
#include "memtrace.h"
#define LEN 100
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
  } while(m.type!= CRYPTO_BEGIN);
  
  if(feof(stdin)){
    std::cout << "block not found\n" <<std::endl;
  }
  //  std::cout << "block beginning\t " << m.addr << std::endl;
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
uint64_t find_result(const uint8_t *data, size_t siz, const trace_block &tb){
  std::string cs((const char *)data,siz);
  for(auto &block: tb.writes){
    std::string blk((char *)block.second.data(), block.second.size());
    size_t s = blk.find(cs);
    if(s!=std::string::npos){
      return block.first + s;
    }
  }
  return 0;
}
#include <openssl/aes.h>
/*
#define SWEEP_PARAMETER(var,blocks,size)
template <class f, class container> sweep_parameter(container &c, int size, f& func){
  for(auto &block: c){
    for(int off= 0; off+size <= block.second.size(); off++){
      f((const uint8_t *) block.second.data() + off);
    }
  }
}

for(auto &var ## _blk : blocks) for(int var ## _off= 0, const uint8_t *var; var = var##_blk.second.data() + var##_offset, var ##_off + size <= var ##_blk.second.size(); var ##_off ++)

void find_AES(const trace_block &t){
  for(int keysize : {16,24, 32}){
    sweep_parameter(t.reads,keysize, [](const uint8_t *key){
      AES_KEY wctx;
      AES_set_encrypt_key(key, keysize * 8, &wctx);
      sweep_parameter(t.reads,16, [](const uint8_t *plain){
          uint64_t addr;
          uint8_t cypher[16];
        AES_encrypt(plain, cypher,  &wctx);
        addr = find_result(cypher, 16, t);
        if(addr!=0){
          printf("FOUND AES\n");
        }
        });
      });
    SWEEP_PARAMETER(key, t.reads, keysize){
      SWEEP_PARAMETER(plain, t.reads, 16){
      }
    }
    }*/
static int count = 0;
void find_AES(const trace_block &t){
  for(int keysize : {16,24, 32}){
    for(auto &keyblock: t.reads){
      for(int keyoff=0;keyoff + keysize <= keyblock.second.size();keyoff++){
        const uint8_t *key = keyblock.second.data() + keyoff;
        AES_KEY wctx;
        //        AES_set_encrypt_key(key, keysize * 8, &wctx);
        const int plainsize = 16;
        for(auto &plainblock: t.reads){
          for(int plainoff=0; plainoff + plainsize <= plainblock.second.size();plainoff++){
            const uint8_t *plain = plainblock.second.data() + plainoff;
            uint64_t addr = 0;
            uint8_t cypher[16];
            //AES_encrypt(plain, cypher, &wctx);
            //addr = find_result(cypher, 16, t);
            count++;
            if(addr!=0){
              printf("FOUND AES\n");
            }
          }
        }
      }
    }
  }
}

void analyze(const trace_block &t){
  find_AES(t);
}
int main(){
  int i=0;
  for(;!feof(stdin);i++){
    trace_block tb;
    read_tb(tb);
    analyze(tb);
    std::cout << tb << std::endl;
  }
    std::cout << count;
}
