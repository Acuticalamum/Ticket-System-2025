#ifndef TOKENSCANNER_HPP
#define TOKENSCANNER_HPP
#include<string>
#include<cstring>
#include"../STLite/vector/vector.hpp"
#include"../STLite/map/map.hpp"

using sjtu::vector;
using std::string;
using sjtu::map;

vector<string> splitString(string &s) {
  vector<string> ret{};
  s = s + " ";
  int last = -1;
  for(size_t i = 0; i < s.size(); i++) {
    if(s[i] == ' ') {
      string t = s.substr(last + 1 , i - last - 1);
      last = i;
      if(t == "") continue;
      if(t == " ") continue;
      ret.push_back(t);
    }
  }
  return ret;
}

long long stringToInt(string &s) {
  bool flag = false;
  long long ret = 0;
  if(s[0] == '-') {
    s = s.substr(1, s.size() - 1);
    flag = true;
  }
  for(size_t i = 0; i < s.size(); i++) {
    ret = ret * 10 + s[i] - '0';
  }
  return flag ? -ret : ret;
}

vector<string> processKeyword(string s) {//将keyword分成一个个关键词
  vector<string> ret{};
  s = s + "|";
  int last = -1;
  for(size_t i = 0; i < s.size(); i++) {
    if(s[i] == '|') {
      string t = s.substr(last + 1 , i - last - 1);
      last = i;
      ret.push_back(t);
    }
  }
  return ret;
}

vector<int> processKeywordToInt(string s) {//将keyword分成一个个关键词
  vector<int> ret{};
  s = s + "|";
  int last = -1;
  for(size_t i = 0; i < s.size(); i++) {
    if(s[i] == '|') {
      string t = s.substr(last + 1 , i - last - 1);
      last = i;
      ret.push_back(stringToInt(t));
    }
  }
  return ret;
}

void scan(map<char, string> &para, vector<string> &tokens) {
  para.clear();
  char key = '\0';
  for(size_t i = 2; i < tokens.size(); i++) {
    if(tokens[i][0] == '-') {
      if(i == tokens.size() - 1) return;
      key = tokens[i][1];
      para[key] = tokens[++i];
    }
  }
}

char* to_cstr(string &str) {
  return const_cast<char*>(str.c_str());
}

#endif
