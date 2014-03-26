#include <iostream>
#include <string>
#include <string.h>
#include "hmmtagger.h"
#include "tokenizer.h"

using namespace std;

int produce_prob;
int main(int argc, char *argv[])
{

  if(argc!=4)
  	{
	  cerr<<"Usage: program -m0|-m1|-m2|-m|-s|-k trainning-file test-file"<<endl;
	  cerr<<"       -m0 produce model file of  ML estimation for transition probability"<<endl;
	  cerr<<"       -m1 produce model file of simple smoothing for transition probability"<<endl;
	  cerr<<"       -m2 produce model file of katz backoff for transition probability"<<endl;
	  cerr<<"       -m using ML estimation for transition probability"<<endl;
	  cerr<<"       -s using simple smoothing for transition probability"<<endl;
	  cerr<<"       -k using Katz backoff algorithm for transition probability"<<endl;
	  exit(-1);
  	}
  const char* flag = argv[1];
  int which = 0;
  produce_prob = 0;
  if(strcmp(flag,"-m0")==0)
    {
      produce_prob = 1;
    }
  else if(strcmp(flag,"-m1")==0)
    {
      produce_prob = 2;
    }
  else if(strcmp(flag,"-m2")==0)
    {
      produce_prob = 3;
    }
  else if(strcmp(flag,"-m")==0)
    {
      which = 1;
    }
   else if(strcmp(flag,"-s")==0)
    {
      which =2;
    }
   else if(strcmp(flag,"-k")==0)
    {
      which =3;
    }
   else
    {
	  cerr<<"Usage: program -m0|-m1|-m2|-m|-s|-k trainning-file test-file"<<endl;
	  cerr<<"       -m0 produce model file of  ML estimation for transition probability"<<endl;
	  cerr<<"       -m1 produce model file of simple smoothing for transition probability"<<endl;
	  cerr<<"       -m2 produce model file of katz backoff for transition probability"<<endl;
	  cerr<<"       -m using ML estimation for transition probability"<<endl;
	  cerr<<"       -s using simple smoothing for transition probability"<<endl;
	  cerr<<"       -k using Katz backoff algorithm for transition probability"<<endl;
	  exit(-1);
    }
  const char* trainingfile = argv[2];
  const char* testfile = argv[3];
  hmmtagger tagger;
  tagger.init(trainingfile);
  tagger.estimate_Eprob();
  tagger.estimate_Tprob_ML();
  if(which ==2)
    tagger.estimate_Tprob_Simple();
  else if(which ==3)
    tagger.estimate_Tprob_Katz();
  if(!produce_prob)
    {
      char buf[4096];
      ifstream inFile(testfile);
      if(!inFile)
	{
	  cerr<<"can not open file: "<<endl;
	  exit(-1);
	}
      while(inFile.getline(buf,4096))
	{
	  int count = inFile.gcount();
	  if(!isprint(buf[count-1]))
	    count = count -1;
	  tagger.tag_sentence(buf,0,count-1);
	}
    }
  
  return 0;
}
