#include <distorm.h>
#include <mnemonics.h>
#include "analysis.h"
#include <iostream>
#include <map>
#define LEN 100
using namespace std;
std::map<uint64_t, const tentropy::CodeBlock *> codeblocks;
void print_disas(const tentropy::CodeBlock *block){
  const bool use64bit = true;
  _DInst dec[256];
  
  unsigned int dec_count = 0;
  _DecodeType dt = use64bit ? Decode64Bits : Decode32Bits;
  
    _CodeInfo ci;
    ci.code = (const uint8_t *)block->code().data();
    ci.codeLen = block->code().size();
    ci.codeOffset = 0x100000;
    ci.dt = dt;
    ci.features = DF_NONE;
    distorm_decompose(&ci, dec, 256, &dec_count);
    for (int i = dec_count - 1; i >= 0; i--) {
      _DecodedInst inst; 
        if (dec[i].flags == FLAG_NOT_DECODABLE) {
          cout << "Instruction not decodable " <<  dec[i].addr;
          break;
        }
        distorm_format(&ci, &dec[i], &inst);

        cout << inst.mnemonic.p << "\t" << inst.operands.p << endl;
    }
}
int main(){
  std::vector<tentropy::Trace *> out;
  parsefd(stdin, out);
  for(auto p: out){
    if(p->has_code()){
      codeblocks[p->code().identifier()] =  &p->code();
    }
  }
  int i =0;
  for(auto p:out){
    if(p->has_exectrace()){
      cout << endl << endl << endl;
      cout << "trace "<<i++<<"\n";
      cout << p->exectrace().start() << endl;
      for(auto &exec: p->exectrace().exec()){
        assert(codeblocks.count(exec));
        print_disas(codeblocks[exec]);
        cout << endl;
      }
      cout << p->exectrace().end() << endl ;
    }
  }
}
