#include "../panda/qemu/panda_plugins/cryptofind/trace.pb.h"
#include <stdint.h>
#include <map>
#include <vector>
struct trace_block{
  uint64_t ic_start, ic_end;
  std::map<uint64_t, std::vector<uint8_t>> reads;
  std::map<uint64_t, std::vector<uint8_t>> writes;
};
//extern std::ostream & operator<<(std::ostream &os,std::map<uint64_t, std::vector<uint8_t> >&m);
//extern std::ostream &operator<<(std::ostream &os, trace_block &tb);
//void vectorize_set(std::map<uint64_t, uint8_t> &m, std::map<uint64_t, std::vector<uint8_t> > &out);
