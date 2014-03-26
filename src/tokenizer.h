#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>

using namespace std;

class tokenizer{
 private:
  char * buf;
  long start,end;
  bool EOC,EOT;
  int chars_in_buf;
  char nextChar();
 public:
  bool eot();
  void  init(char*buf,long start,long end);
  char peekNext();
  string  nextToken();
};


