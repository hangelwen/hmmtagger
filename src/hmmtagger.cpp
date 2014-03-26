#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <math.h>
#include "hmmtagger.h"
#include "tokenizer.h"

using namespace std;
extern int produce_prob;
#define MINUMINF -10000;
tag START = "START";
tag END = "END";
word UNKA = "unka";
tag alltag[] =
  {
    "#",
    "$",
    "''",
    ",",
    "-lrb-",
    "-rrb-",
    ".",
    ":",
    "``",
    "cc",
    "cd",
    "dt",
    "ex",
    "fw",
    "in",
    "jj",
    "jjr",
    "jjs",
    "ls",
    "md",
    "nn",
    "nnp",
    "nnps",
    "nns",
    "pdt",
    "pos",
    "prp",
    "prp$",
    "rb",
    "rbr",
    "rbs",
    "rp",
    "sym",
    "to",
    "uh",
    "vb",
    "vbd",
    "vbg",
    "vbn",
    "vbp",
    "vbz",
    "wdt",
    "wp",
    "wp$",
    "wrb"
  };

void hmmtagger::init(string taggedfile)
{
  char buf[BUFSIZE];
  ifstream inFile(taggedfile.c_str());
  if(!inFile)
    {
      cerr<<"can not open file: "<<taggedfile<<endl;
      exit(-1);
    }
  tokenizer mytokenizer;
  pair<word, tag> wordtag;
  pair<tag,tag> tagtag;
  pair<int,int> wt;
  pair<int,int> tt;
  //initialize all the tag-tag pairs' count as 0.
  for(int i=0;i<45;i++)
    {    
      for(int j=0;j<45;j++)
	{
	  tagtag.first = alltag[i];
	  tagtag.second = alltag[j];
	  hash_tagtag.insert(pair < pair <tag, tag>, pair<int,vector<double> > >(tagtag,pair<int, vector<double> >(0,vector<double>(4,0.0))));
	}
    }
  for(int i=0;i<45;i++)
    {
      tagtag.first = START;
      tagtag.second = alltag[i];
      hash_tagtag.insert(pair < pair <tag, tag>, pair<int,vector<double> > >(tagtag,pair<int, vector<double> >(0,vector<double>(4,0.0))));
      tagtag.first = alltag[i];
      tagtag.second = END;
      hash_tagtag.insert(pair < pair <tag, tag>, pair<int,vector<double> > >(tagtag,pair<int, vector<double> >(0,vector<double>(4,0.0))));
    }

  //pair<map <pair<word,tag>, pair<int, double> > ::iterator,bool> ret_wordtag;
  //pair<map <pair<tag,tag>, pair<int, double> > ::iterator,bool> ret_tagtag;
  //loop for every line in the file
  //TODO: there is risk that some lines are longer than the bufsize
  hash_tags.insert(pair<tag,pair<int,double> >(START,pair<int, double>(0,0.0)));
  hash_tags.insert(pair<tag,pair<int,double> >(END,pair<int, double>(0,0.0)));
  while(inFile.getline(buf,BUFSIZE))
    {
      long count = inFile.gcount();
      if(count==1)//blank line
	continue;
      if(!isprint(buf[count-1]))
	count = count -1;
      mytokenizer.init(buf,0,count-1);
      word w = mytokenizer.nextToken();
      wordtag.first = w;
      tag t= mytokenizer.nextToken();
      wordtag.second = t;
      tagtag.first = START;
      tagtag.second = t;
      hash_tags[START].first++;
      if(hash_words.insert(pair<word, pair<int,double> >(w, pair<int, double>(1,0.0))).second == true)
	{
	}
      else
	{
	  hash_words[w].first++;
	}
      if(hash_tags.insert(pair<tag, pair<int,double> >(t, pair<int, double>(1,0.0))).second == true)
	{
	}
      else
	{
	  hash_tags[t].first++;
	}
      if(hash_wordtags.insert(pair<pair <word, tag>, pair<int,pair<double,double> > >(wordtag,pair<int, pair<double,double> >(1,pair<double,double>(0.0,0.0)))).second == true)
	{
	  //element inserted
	}
      else//element aready exist, update value
	{
	  hash_wordtags[wordtag].first++;
	}
      
      hash_tagtag[tagtag].first++;

      while(!mytokenizer.eot())
	{
	  word w = mytokenizer.nextToken();
	  wordtag.first =w;
	  
	  if(hash_words.insert(pair<word, pair<int,double> >(w, pair<int, double> (1,0.0))).second == true)
	    {
	    }
	  else
	    {
	      hash_words[w].first = hash_words[w].first + 1;
	    }
	  if(!mytokenizer.eot())
	    {
	      tag t = mytokenizer.nextToken();
	      wordtag.second = t;
	      if(hash_tags.insert(pair<tag, pair<int,double> >(t, pair<int, double>(1,0.0))).second == true)
		{
		}
	      else
		{
		  hash_tags[t].first++;
		}
	      if(hash_wordtags.insert(pair<pair <word, tag>, pair<int,pair<double,double> > >(wordtag,pair<int, pair<double,double> >(1,pair<double,double>(0.0,0.0)))).second == true)
		{
		}
	      else
		hash_wordtags[wordtag].first++;
	      tagtag.first = tagtag.second;
	      tagtag.second = t;
	      hash_tagtag[tagtag].first++;
	    }
	  else
	    break;
	}
      tagtag.first = tagtag.second;
      tagtag.second = END;
      hash_tags[END].first++;
      hash_tagtag[tagtag].first++;
}

int count_remove = hash_words[" "].first;
 hash_words.erase(" ");
 total_word -= count_remove;
map <pair<word,tag>, pair<int, pair<double,double> > > ::iterator it1;

  for(it1=hash_wordtags.begin();it1!=hash_wordtags.end();it1++)
    {
      word w = (*it1).first.first;
      if(w.compare(UNKA)==0)
	continue;
      if(hash_words[w].first<5)
	{
	  pair<word,tag> temp;
	  temp.first = UNKA;
	  temp.second = (*it1).first.second;
	  if(hash_wordtags.insert(pair<pair <word, tag>, pair<int,pair<double,double> > >(temp, pair<int, pair<double,double> >((*it1).second.first,pair<double,double> (0.0,0.0)))).second == true)
	    {
	    }
	  else
	    {
	      hash_wordtags[temp].first += (*it1).second.first;
	    }
	  (*it1).second.first=-10;//mark that this entry should be deleted
	}
      //cout<<"word:"<<(*it1).first.first<<" tag:"<<(*it1).first.second<<" value1:"<<(*it1).second.first<<" value2:"<<(*it1).second.second<<endl; 
    }


  for(it1=hash_wordtags.begin();it1!=hash_wordtags.end();)
    {
      if((*it1).second.first==-10)
	hash_wordtags.erase(it1++);
      else
	it1++;
    }
  // for(it1=hash_wordtags.begin();it1!=hash_wordtags.end();it1++)
  //   {
  //     cout<<"word:"<<(*it1).first.first<<" tag:"<<(*it1).first.second<<" value1:"<<(*it1).second.first<<endl;
  //   }

  // map <pair<tag,tag>, pair<int, double> > ::iterator it2;
  // for(it2=hash_tagtag.begin();it2!=hash_tagtag.end();it2++)
  //   {
  //     cout<<"tag1:"<<(*it2).first.first<<" tag2:"<<(*it2).first.second<<" value1:"<<(*it2).second.first<<" value2:"<<(*it2).second.second<<endl;
  //   }

  map <tag, pair<int, double> > ::iterator it31;
  map <tag, pair<int, double> > ::iterator it32;
  map <tag, pair<int, double> > ::iterator it3;
  total_tag = 0;
  tag curtag;
  it31 = hash_tags.find(START);
  total_sen_in_traning = (*it31).second.first;
  it32 = hash_tags.find(END);
  //Question?? do we need to include the START and END state when we computing the transition prob using simple smoothing??
  for(it3=hash_tags.begin();it3!=hash_tags.end();it3++)
    {
      //do not include START and END
      if((it3==it31)||(it3==it32))
	continue;
      total_tag += (*it3).second.first;
      curtag = (*it3).first;
      for(int i=0;i<curtag.length();i++)
	{
	  if(!isalpha(curtag[i]))
	    continue;
	  curtag.replace(i,1,1,curtag[i]-32);
	}
      sortedtags.push_back(curtag);
     // cout<<"tag["<<(*it3).first.length()<<"]:"<<(*it3).first<<" value1:"<<(*it3).second.first<<" value2:"<<(*it3).second.second<<endl;
    }
  map <word, pair<int, double> > ::iterator it4;
  total_word = 0;
  int cur_count = 0;
  hash_words.insert(pair <word, pair < int,double> > (UNKA, pair<int, double> (5,0.0)));
  for(it4=hash_words.begin();it4!=hash_words.end();)
    {
      cur_count = (*it4).second.first;
      total_word += cur_count;
      if(cur_count<5)
	{
	  hash_words[UNKA].first += cur_count;
	  hash_words.erase(it4++);
	}
      else
	it4++;
    }
  hash_words[UNKA].first -= 5;
   // for(it4=hash_words.begin();it4!=hash_words.end();it4++)
   //   {
   //     cout<<(*it4).first<<" value1:"<<(*it4).second.first<<" value2:"<<(*it4).second.second<<endl;
   //   }
}

void hmmtagger::estimate_Eprob()
{
  map <pair<word,tag>, pair<int, pair<double,double> > > ::iterator it1;
  for(it1=hash_wordtags.begin();it1!=hash_wordtags.end();it1++)
    {
      (*it1).second.second.second =(double)(*it1).second.first/(double)hash_tags[(*it1).first.second].first;
      // cout<<"count(w,t):"<<(*it1).second.first<<" count(t):"<<hash_tags[(*it1).first.second].first<<endl;
      //log prob
      (*it1).second.second.first = log((*it1).second.second.second);
    }
    if(produce_prob)
      {
	cout<<"WORD"<<"     TAG"<<"    PROB[logged Prob]"<<endl;
	  for(it1=hash_wordtags.begin();it1!=hash_wordtags.end();it1++)
	    {
	      cout<<(*it1).first.first<<"  "<<(*it1).first.second<<"  "<<(*it1).second.second.second<<"["<<(*it1).second.second.first<<"]"<<endl;
	    }
      }
}

void hmmtagger::estimate_Tprob_ML()
{
  hash_tags[START] = hash_tags[END] = pair<int,double>(total_sen_in_traning,0.0);
  map <pair<tag,tag>, pair<int, vector<double> > > ::iterator it2;
  for(it2=hash_tagtag.begin();it2!=hash_tagtag.end();it2++)
    {
      if((*it2).second.first == 0)//count is 0, logged prob set to MINUMINF 
	{
	  (*it2).second.second[0] = MINUMINF;
	}
      else//not zero, compute prob and logged prob
	{
	  (*it2).second.second[1] = (double)(*it2).second.first / (double)hash_tags[(*it2).first.first].first;
	  //log prob
	  (*it2).second.second[0] = log((*it2).second.second[1]);
	}
    }
    if(produce_prob==1)
      {
	cout<<endl<<"Tansition prob for ML"<<endl;
	cout<<"TAG[From]"<<"     TAG[TO]"<<"    PROB[logged Prob]"<<endl;
	for(it2=hash_tagtag.begin();it2!=hash_tagtag.end();it2++)
	    {
	      cout<<(*it2).first.first<<"  "<<(*it2).first.second<<"  "<<(*it2).second.second[1]<<"["<<(*it2).second.second[0]<<"]"<<endl;
	    }
      }
  hash_tags.erase(START);
  hash_tags.erase(END);
}

//This requires call estimate_Tprob_ML before each call of estimate_Tprob_Simple.
void hmmtagger::estimate_Tprob_Simple()
{
  hash_tags[START] = hash_tags[END] = pair<int,double>(total_sen_in_traning,0.0);
  map <pair<tag,tag>, pair<int, vector<double> > > ::iterator it2;
  tag t1,t2;
  for(it2=hash_tagtag.begin();it2!=hash_tagtag.end();it2++)
    {
      t1 = (*it2).first.first;
      t2 = (*it2).first.second;
      if(hash_tags[t1].first>0)
	{
	  (*it2).second.second[2] = (*it2).second.second[1]/2; 
	  (*it2).second.second[2] += (double)hash_tags[t2].first / (double)(2*total_tag);
	}
      (*it2).second.second[0] = log((*it2).second.second[2]);
     // cout<<"[Simple]transition FROM TAG "<<(*it2).first.first<<" TO TAG "<<(*it2).first.second<<":"<<(*it2).second.second[2]<<"[log:"<<(*it2).second.second[0]<<"]"<<endl;
    }
    if(produce_prob==2)
      {
	cout<<endl<<"Tansition prob for Simple Smoothing"<<endl;
	cout<<"TAG[From]"<<"     TAG[TO]"<<"    PROB[logged Prob]"<<endl;
	for(it2=hash_tagtag.begin();it2!=hash_tagtag.end();it2++)
	    {
	      cout<<(*it2).first.first<<"  "<<(*it2).first.second<<"  "<<(*it2).second.second[2]<<"["<<(*it2).second.second[0]<<"]"<<endl;
	    }
      }
  hash_tags.erase(START);
  hash_tags.erase(END);
}

void hmmtagger::estimate_Tprob_Katz()
{

  hash_tags[START] = hash_tags[END] = pair<int,double>(total_sen_in_traning,0.0);
  
  map <pair<tag,tag>, pair<int, vector<double> > > ::iterator it2;
  tag t1,t2;
  double d;

  map <tag,double > sumexist;
  map <tag,double > sumnonexist;
  map <tag, double > ::iterator itexist;
  double sum1=0;

  for(int i=0;i<45;i++)
  {
   sumexist.insert(pair<tag,double>(alltag[i],0.0));
   
   int count = hash_tags[alltag[i]].first;
   double value;
   if(count==1)
     value = count - 0.5;
   else if(count>1)
     value = count - 0.75;
   value = (double)value/(double)(total_tag + 2*total_sen_in_traning);
   sumnonexist.insert(pair<tag,double>(alltag[i],value));
  }
  sumexist.insert(pair<tag,double>(START,(double)(total_sen_in_traning-1)/(double)(total_tag+2*total_sen_in_traning)));
  sumnonexist.insert(pair<tag,double>(END,(double)(total_sen_in_traning-1)/(double)(total_tag+2*total_sen_in_traning)));

  for(it2=hash_tagtag.begin();it2!=hash_tagtag.end();it2++)
    {
      int count = (*it2).second.first;
      t1 = (*it2).first.first;
      t2 = (*it2).first.second;
      if(count>0)
	{
	  if(count==1)
	    d = 0.5;
	  else 
	    d = 0.75;
	  (*it2).second.second[3] = (double)(count - d)/ (double)hash_tags[t1].first;
	  (*it2).second.second[0] = log((*it2).second.second[3]);
	  sumexist[t1] += (*it2).second.second[3];
	}
      else
	{
	  continue;
	}
     // cout<<"[Simple]transition FROM TAG "<<(*it2).first.first<<" TO TAG "<<(*it2).first.second<<":"<<(*it2).second.second[2]<<"[log:"<<(*it2).second.second[0]<<"]"<<endl;
    }
  
  for(it2=hash_tagtag.begin();it2!=hash_tagtag.end();it2++)
    {
      int count = (*it2).second.first;
      t1 = (*it2).first.first;
      t2 = (*it2).first.second;
      if(count>0)
      {
	continue;
      }
      else if(hash_tags[t1].first>0)
	{
	  double alpha = double(1 - sumexist[t1]);
	  map <pair<tag,tag>, pair<int, vector<double> > > ::iterator it3;
	  double nominator= 0.0;
	  for(it3=hash_tagtag.begin();it3!=hash_tagtag.end();it3++)
	    {
	      int count = (*it3).second.first;
	      if(count==0)
		{
		  if((*it3).first.first==t1)
		    nominator += sumnonexist[(*it3).first.second];
		}
	    }
	  alpha = alpha / nominator;
	  (*it2).second.second[3] = sumnonexist[t2] * alpha;
	  (*it2).second.second[0] = log((*it2).second.second[3]);
	}
      else
	{
	  cerr<<"Wrong, should never be here!!"<<endl;
	}
     // cout<<"[Simple]transition FROM TAG "<<(*it2).first.first<<" TO TAG "<<(*it2).first.second<<":"<<(*it2).second.second[2]<<"[log:"<<(*it2).second.second[0]<<"]"<<endl;
     //cout<<(*it2).first.first<<" "<<(*it2).first.second<<" "<<(*it2).second.second[0]<<endl;
    }
    if(produce_prob==3)
      {
	cout<<endl<<"Tansition prob for Katz Backoff"<<endl;
	cout<<"TAG[From]"<<"     TAG[TO]"<<"    PROB[logged Prob]"<<endl;
	for(it2=hash_tagtag.begin();it2!=hash_tagtag.end();it2++)
	    {
	      cout<<(*it2).first.first<<"  "<<(*it2).first.second<<"  "<<(*it2).second.second[3]<<"["<<(*it2).second.second[0]<<"]"<<endl;
	    }
      }
  hash_tags.erase(START);
  hash_tags.erase(END);
}

void hmmtagger::tag_File(string file)
{
  char buf[BUFSIZE];
  ifstream inFile(file.c_str());
  if(!inFile)
    {
      cerr<<"can not open file: "<<file<<endl;
      exit(-1);
    }
  tokenizer tagtokenizer;
}

void hmmtagger::tag_sentence(char* buf, long start1, long end1)
{
  tokenizer sentence_tokenizer;
  sentence_tokenizer.init(buf,start1,end1);
  vector<word> sentence;
  while(!sentence_tokenizer.eot())
    {
      string s = sentence_tokenizer.nextToken();
      sentence.push_back(s);
    }
  vector<tag> tags;
  viterbi(tags,sentence);
}
void hmmtagger::viterbi(vector<tag> & result, vector<word> sentence)
{
  vector< vector<pair<float,int> > >vmatrix;
  vector<pair<float,int> >temp;
  for(int i=0; i< sentence.size();i++)
    {
      temp.push_back(pair<float,int>(0.0,0));
    }
  for(int i = 0; i < TAGSETSIZE; i++)
    {
      vmatrix.push_back(temp);
    }
  map <tag, pair<int, double> > ::iterator it3;
  map <tag, pair<int, double> > ::iterator it4;
  int ind = 0;
  pair<word,tag> wt;
  pair<tag,tag> tt;
  word word0  =  sentence[0];
  map <word, pair<int, double> > ::iterator itword;
  itword = hash_words.find(word0);
  if(itword == hash_words.end())//the word do not exist,replace with unka
    word0 = UNKA;
  wt.first = word0;
  tt.first = START;
  for(it3 = hash_tags.begin(); it3!=hash_tags.end();it3++,ind++)
    {
      wt.second = (*it3).first;
      tt.second = wt.second;
      map <pair<word,tag>, pair<int, pair<double,double> > > ::iterator itwt;
      double pwt = MINUMINF;
      itwt = hash_wordtags.find(wt);
      if(itwt!=hash_wordtags.end())
	pwt = hash_wordtags[wt].second.first;
      // else
      //  	{
      //  	  wt.first = UNKA;
      //  	  itwt = hash_wordtags.find(wt);
      //  	  if(itwt!=hash_wordtags.end())
      //  	    pwt = hash_wordtags[wt].second.first;
      //  	  else
      //  	    pwt = MINUMINF;
      //  	}
      map <pair<word,tag>, pair<int,vector<double> > > ::iterator ittt;
      double ptt = MINUMINF;
      ittt = hash_tagtag.find(tt);
      if(ittt!=hash_tagtag.end())
	ptt = hash_tagtag[tt].second[0];
      vmatrix[ind][0].first = pwt + ptt;
      vmatrix[ind][0].second = 0;
    }
  vector<float> scores;
  scores.reserve(hash_tags.size());
  for(int i=0;i< hash_tags.size();i++)
    {
      scores.push_back(0.0);
    }
  for(int j=1; j<sentence.size(); j++)
    {
      int i = 0;
      word w = sentence[j];
      map <word, pair<int, double> > ::iterator itword;
      itword = hash_words.find(w);
      if(itword == hash_words.end())//the word do not exist,replace with unka
	w = UNKA;
      for(it3 = hash_tags.begin(); it3!=hash_tags.end(); it3++)
	{
	  pair<tag,tag> tt;
	  pair<word,tag> wt;
	  tt.second = (*it3).first; 
	  wt.first = w;
	  wt.second = tt.second;
	  map <pair<word,tag>, pair<int, pair<double,double> > > ::iterator itwt;
	  map <pair<tag,tag>, pair<int,vector<double> > > ::iterator ittt;
	  double pwt = MINUMINF;;
	  itwt = hash_wordtags.find(wt);
	  if(itwt!=hash_wordtags.end())
	    pwt = hash_wordtags[wt].second.first;
	  // else//emission prob do not exist, use UNKA
	  //   {
	  //     wt.first = UNKA;
	  //     itwt = hash_wordtags.find(wt);
	  //     if(itwt!=hash_wordtags.end())
	  // 	pwt = hash_wordtags[wt].second.first;
	  //     else 
	  // 	pwt = MINUMINF;
	  //   }
	  int k = 0;
	  int index = 0;
	  double max = -1000000.00;
	  for(it4 = hash_tags.begin(); it4!=hash_tags.end(); it4++)
	    {
	      tt.first = (*it4).first;
	      double ptt = MINUMINF;;
	      ittt = hash_tagtag.find(tt);
	      if(ittt!=hash_tagtag.end())
		ptt = hash_tagtag[tt].second[0];
	      scores[k] = vmatrix[k][j-1].first + ptt + pwt;
	      if(scores[k]>max)
		{
		  max = scores[k];
		  index = k;
		}
	      k++;
	    }
	  vmatrix[i][j].first = max;
	  vmatrix[i][j].second = index;
	  i++;
	}
    }
  vector<int> C;
  C.reserve(sentence.size());
  for(int i=0;i<sentence.size();i++)
    {
      C.push_back(-1);
    }
  pair<tag,tag> tagend;
  double toend = MINUMINF;
  tagend.first = sortedtags[0];
  tagend.second = END;
  
  map <pair<tag,tag>, pair<int,vector<double> > > ::iterator ittt;
  ittt = hash_tagtag.find(tagend);
 
  if(ittt!=hash_tagtag.end())
    {
      toend = hash_tagtag[tagend].second[0];
    }
  int send = sentence.size()-1;
  double max = vmatrix[0][send].first + toend; 
  int index = 0;
  for(int s=1; s<hash_tags.size();s++)
    {
      toend = MINUMINF;
      tagend.first = sortedtags[s];
      ittt = hash_tagtag.find(tagend);
      if(ittt!=hash_tagtag.end())
	{
	  toend = hash_tagtag[tagend].second[0];
	}
      
      if(vmatrix[s][send].first + toend> max)
	{
	  max = vmatrix[s][send].first + toend;
	  index = s;
	}
    }
  C[send] = index;
  for(int j = send-1;j>=0; j--)
    {
      C[j] = vmatrix[C[j+1]][j+1].second;
    }
  cout<<sentence[0]<<" "<<sortedtags[C[0]]<<" ";
  for(int i=1;i<C.size()-1;i++)
    {
      cout<<sentence[i]<<" "<<sortedtags[C[i]]<<" ";
    }
  cout<<sentence[C.size()-1]<<" "<<sortedtags[C[C.size()-1]];
  cout<<" "<<max<<endl;
}
