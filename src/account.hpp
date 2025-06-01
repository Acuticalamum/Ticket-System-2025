#ifndef ACCOUNT_HPP
#define ACCOUNT_HPP
#include<cstring>
#include<iostream>
#include"../STLite/map/map.hpp"
#include"Storage.hpp"
#include"hash.hpp"

using sjtu::map;
using std::string;
using std::cout;
using std::endl;

class Account {
public:
  char username[21]{};
  char password[31]{};
  char name[21]{};
  char mailaddr[31]{};
  int privilege = 0;
  Account() = default;
  Account(char* _username, char* _password, char* _name, char* _mailaddr, int _pri) : privilege(_pri) {
    strcpy(username, _username);
    strcpy(password, _password);
    strcpy(name, _name);
    strcpy(mailaddr, _mailaddr);
  }
  Account(char* _username) {
    strcpy(username, _username);
  }
  bool operator<(const Account &other) {
    return strcmp(username, other.username) < 0;
  }
  bool operator==(const Account &other) {
    return strcmp(username, other.username) == 0;
  }
  bool operator>(const Account &other) {
    return strcmp(username, other.username) > 0;
  }
  bool operator!=(const Account &other) {
    return strcmp(username, other.username) != 0;
  }
  string Account_to_string() {
    string ret{};
    ret = string(username) + " " + string(name) + " " + string(mailaddr) + " " + std::to_string(privilege) + "\n";
    return ret;
  }
};

FileStorage<Account> StroageofAccount("Account");
map<String_Hash, int>login_pool;

bool islogged(char* name) {
  return login_pool.find(convert_to_hash(name)) != login_pool.end();
}

bool isexist(char* name) {
  Account tmp(name);
  return StroageofAccount.findToken(tmp).username == name;
}

int add_user(char* c_, char* u_, char* p_, char* n_, char* m_, int g_) {//改一个首次创建直接创建g=10
  if(!islogged(c_) || isexist(u_)) return -1;
  Account cur(c_);
  cur = StroageofAccount.findToken(cur);
  if(cur.privilege <= g_) return -1;
  Account newuser(u_, p_, n_, m_, g_);
  StroageofAccount.insertToken(newuser);
  return 0;
}

int login(char* u_, char* p_) {
  if(islogged(u_) || !isexist(u_)) return -1;
  Account cur(u_);
  cur = StroageofAccount.findToken(cur);
  if(strcmp(cur.password, p_)) return -1;
  login_pool.insert({convert_to_hash(u_), cur.privilege});
  return 0;
}

int logout(char* u_) {
  if(!islogged(u_) || !isexist(u_)) return -1;
  login_pool.erase(login_pool.find(convert_to_hash(u_)));
  return 0;
}

int query_profile(char* c_, char* u_) {
  if(!islogged(c_) || !isexist(u_)) return -1;
  Account cur(u_);
  cur = StroageofAccount.findToken(cur);
  int c_pri = login_pool.find(convert_to_hash(c_)) -> second;
  if(c_pri <= cur.privilege && strcmp(u_, c_)) return -1;
  cout << cur.Account_to_string();
  return 0;
}

void modify_profile(char* c_, char* u_, char* p_, char* n_, char* m_, int g_) {
  
}

#endif
