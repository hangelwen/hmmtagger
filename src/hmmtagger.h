#include <map>
#include <string>
#include <vector>

using namespace std;
#define BUFSIZE 4096
#define TAGSETSIZE 45
typedef string word;
typedef string tag;

class hmmtagger{
 private:
  //total tags and words in traning data
  int total_tag;
  int total_word;
  int total_sen_in_traning;
  map <tag,pair<int,double> > hash_tags;
  map <word,pair<int,double> > hash_words;
  map <pair<word,tag>, pair<int, pair<double,double> > > hash_wordtags;
  map <pair<tag,tag>, pair<int, vector<double> > > hash_tagtag;
  vector <tag> sortedtags;
  void viterbi(vector<tag> & result, vector<word> sentence);
 public:
  //use the tagged file to init the hash_tags and hash_words hash tables.
  void init(string taggedfile);
  //
  void estimate_Eprob();
  //
  void estimate_Tprob_ML();
  //
  void estimate_Tprob_Simple();
  //
  void estimate_Tprob_Katz();
  //
  void tag_File(string file);
  void tag_sentence(char* buf, long start, long end);

};
