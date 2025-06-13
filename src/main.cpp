#include<iostream> 
#include<string>
#include"account.hpp"
#include"time.hpp"
#include"train.hpp"
#include"tokenscanner.hpp"
#include"../STLite/map/map.hpp"

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::exit;
using sjtu::vector;
using sjtu::map;

map<char, string> para;

void execute(int stamp, string& command, map<char, string> &para) {
  cout << '[' << stamp << "] ";
  if(command == "add_user") {
    int pri = para.find('g') != para.end() ? stringToInt(para['g']) : -1;
    cout << add_user(to_cstr(para['c']), to_cstr(para['u']), to_cstr(para['p']), to_cstr(para['n']), to_cstr(para['m']), pri) << endl;
    //add_user(c_, u_, p_, n_, m_, g_);
  }
  else if(command == "login") {
    cout << login(to_cstr(para['u']), to_cstr(para['p'])) << endl;
    //login(u_, p_);
  }
  else if(command == "logout") {
    cout << logout(to_cstr(para['u'])) << endl;
    //logout(u_);
  }
  else if(command == "query_profile") {
    if(query_profile(to_cstr(para['c']), to_cstr(para['u']))) {
      cout << "-1\n";
    }
    //query_profile(c_, u_);
  }
  else if(command == "modify_profile") {
    int pri = para.find('g') != para.end() ? stringToInt(para['g']) : -1;
    if(modify_profile(to_cstr(para['c']), to_cstr(para['u']), to_cstr(para['p']), to_cstr(para['n']), to_cstr(para['m']), pri)) {
      cout << "-1\n";
    }
    //modify_profile(c_, u_, p_, n_, m_, g_);
  }
  else if(command == "add_train") {
    vector<string> s_ = processKeyword(para['s']);
    vector<int> p_ = processKeywordToInt(para['p']);
    Time x_(para['x']);
    vector<int> t_ = processKeywordToInt(para['t']);
    vector<int> o_;
    if(para['o'] != "_") o_ = processKeywordToInt(para['o']);
    vector<string> dates = processKeyword(para['d']);
    Date d1_(dates[0]), d2_(dates[1]); 
    cout << add_train(to_cstr(para['i']), stringToInt(para['n']), stringToInt(para['m']), s_, p_, x_, t_, o_, d1_, d2_, para['y'][0])
         << endl;
  }
  else if(command == "delete_train") {
    cout << delete_train(to_cstr(para['i'])) << endl;
  }
  else if(command == "release_train") {
    cout << release_train(to_cstr(para['i'])) << endl;
  }
  else if(command == "query_train") {
    Date d_(para['d']);
    if(query_train(to_cstr(para['i']), d_)) {
      cout << "-1\n";
    }
  }
  else if(command == "query_ticket") {
    Date d_(para['d']);
    int p_ = (para['p'] == "time" ? 1 : 2);
    query_ticket(to_cstr(para['s']), to_cstr(para['t']), d_, p_);
  }
  else if(command == "query_transfer") {
    Date d_(para['d']);
    int p_ = (para['p'] == "time" ? 1 : 2);
    if(query_transfer(to_cstr(para['s']), to_cstr(para['t']), d_, p_)) {
      cout << "0\n";
    }
  }
  else if(command == "buy_ticket") {
    Date d_(para['d']);
    bool q_ = (para['q'] == "true" ? true : false);
    int flag = buy_ticket(to_cstr(para['u']), to_cstr(para['i']), d_, stringToInt(para['n']), 
                         to_cstr(para['f']), to_cstr(para['t']), q_);
    if(flag) cout << flag << "\n";
    else cout << "queue\n";
  }
  else if(command == "query_order") {
    if(query_order(to_cstr(para['u']))) {
      cout << "-1\n";
    }
  }
  else if(command == "refund_ticket") {
    if(refund_ticket(to_cstr(para['u']), stringToInt(para['n']))) {
      cout << "-1\n";
    }
  }
  else if(command == "clean") {
    clean_account();
  }
  else if(command == "exit") {
    cout << "bye" << endl;
    logout_all();
    exit(0);
  }
}

int main() {
  string str;
  while(getline(cin, str)) {
    vector<string> tokens = splitString(str);
    string stamp_string = tokens[0].substr(1, tokens[0].size() - 2);
    int stamp = stringToInt(stamp_string);
    string command = tokens[1];
    scan(para, tokens);
    execute(stamp, command, para);
  }
  return 0;
}
