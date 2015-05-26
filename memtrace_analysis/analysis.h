#include <stdlib.h>
#include <stdio.h>
#include "trace.pb.h"
#include <vector>
static int parsefd(FILE *fd, std::vector<tentropy::Trace *> &out){
  uint64_t siz,allocsiz= 0;
  char *data  =NULL;
  int err =0;
  while(!feof(fd)){
    err = fread(&siz,1,sizeof siz, fd);
    if(err != sizeof siz)
      return 0;
    if(siz >= allocsiz){
      allocsiz = 2 *siz;
      free(data);
      data  = (char *)malloc(allocsiz);
      assert(data!=NULL);
    }
    err = fread(data,1 , siz, fd);
    assert(err == siz);
    auto obj = new tentropy::Trace();
    assert(obj->ParseFromArray(data,siz));
    out.push_back(obj);  
  }
  return 0;
}
template<typename F>  analysis(FILE *fd, const F& functor){
  uint64_t siz,allocsiz= 0;
  char *data  =NULL;
  int err =0;
  while(!feof(fd)){
    err = fread(&siz,1,sizeof siz, fd);
    if(err != sizeof siz)
      return 0;
    if(siz >= allocsiz){
      allocsiz = 2 *siz;
      free(data);
      data  = (char *)malloc(allocsiz);
      assert(data!=NULL);
    }
    err = fread(data,1 , siz, fd);
    assert(err == siz);
    auto obj = new tentropy::Trace();
    assert(obj->ParseFromArray(data,siz));
    out.push_back(obj);  
  }
  return 0;
}
