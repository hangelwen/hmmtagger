#include "tokenizer.h"
#include <string>
#include <fstream>
#include <iostream>
#include <cctype>

using namespace std;

void tokenizer::init(char* inbuf, long startpos, long endpos)
{
  if(startpos>endpos)
    {
      cerr<<"Wrong index in buffer, start:"<<startpos<<" end:"<<endpos<<endl;
      exit(-1);
    }
  buf = inbuf;
  start = startpos;
  end = endpos;
  EOC = false;
  EOT = false;
  chars_in_buf = end - start + 1;
}

char tokenizer::nextChar()
{
  if(start<end)
    {
      start++;
      return tolower(buf[start-1]);
    }
  else if(start==end)
    {
      char ret = buf[start];
      EOC = true;
      return tolower(ret);
    }
}
char tokenizer::peekNext()
{
  return buf[start];
}
string tokenizer::nextToken()
{
  string token;
  token.reserve(20);
  token="";
  while(isspace(peekNext()))
    {
      nextChar();
      if(EOC == true)
	{
	  EOT = true;
	  return " ";
	}
    };
  
  char cur;
  while(!isspace(cur=nextChar()))
    {
      token.push_back(cur);
      if(EOC == true)
	{
	  EOT = true;
	  return token;
	}
    }
  return token;
}

bool tokenizer::eot()
{
  return EOT;
}
