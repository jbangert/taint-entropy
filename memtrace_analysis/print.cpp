#include "analysis.h"
#include <pb2json.h>
#include <iostream>
#define LEN 100
using namespace std;
int main(){
  std::vector<tentropy::Trace *> out;
  parsefd(stdin, out);
  cout << "[";
  char delim = ' ';
  for(auto p: out){
    
        char *json = pb2json(*p);
	cout<<delim << json<<endl;
        delim = ',';
	free(json);
  }
  cout << "]" << std::endl;
}
