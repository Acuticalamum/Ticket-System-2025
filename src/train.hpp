#ifndef TRAIN_HPP
#define TRAIN_HPP
#include"time.hpp"
#include"Storage.hpp"
#include"tokenscanner.hpp"
#include"hash.hpp"
#include"../STLite/vector/vector.hpp"
#include"../STLite/map/utility.hpp"
using sjtu::vector;
using sjtu::pair;
using Seat = int[100];
using station_pair = sjtu::pair<String_Hash, Hash_pair>;

class Train {
public:
  char trainID[21]{};
  int stationNum, seatNum;
  char stations[101][21]{};
  int prices[101]{};
  Time arrive[101]{}, leave[101]{};
  Date begin_date, end_date;
  char type;
  bool released;
  Train() = default;
  Train(char* id_, int stationNum_, int seatNum_, vector<string> &stations_, vector<int> &prices_, Time &start_time_,
       vector<int> &travel_times_, vector<int> &stopover_times_, Date &begin_date_, Date &end_date_, char type_) : 
       stationNum(stationNum_), seatNum(seatNum_), begin_date(begin_date_), end_date(end_date_), type(type_), released(false) {
          strcpy(trainID, id_);
          prices[0] = 0;
          for(int i = 1; i < stationNum; i++) prices[i] = prices[i - 1] + prices_[i - 1];
          for(int i = 0; i < stationNum; i++) strcpy(stations[i], to_cstr(stations_[i]));
          leave[0] = start_time_;
          for(int i = 1; i < stationNum - 1; i++) {
            arrive[i] = leave[i - 1] + travel_times_[i - 1];
            leave[i] = arrive[i] + stopover_times_[i - 1];
          }
          arrive[stationNum - 1] = leave[stationNum - 2] + travel_times_[stationNum - 2];
       }
  Train(char* id_) {strcpy(trainID, id_);}
  bool operator==(const Train &other) const {
    return strcmp(trainID, other.trainID) == 0;
  }
  bool operator<(const Train &other) const {
    return strcmp(trainID, other.trainID) < 0;
  }
  bool operator>(const Train &other) const {
    return strcmp(trainID, other.trainID) > 0;
  }
  bool operator!=(const Train &other) const {
    return strcmp(trainID, other.trainID) != 0;
  }
  
};
class Ticket {
public:
};

Database<Train>StorageofTrain("Train");
FileStorage<Hash_pair>MapofTrain("Train");
Database<Seat>StorageofSeat("Seat");
FileStorage<Hash_pair>MapofSeat("Seat");
FileStorage<station_pair>MapofStationPair("Station_pair");

bool istrainexist(char* i_) {
  Hash_pair tmp(i_, 0);
  return MapofTrain.findToken(tmp).hash == tmp.hash;
}

int trainpos(char* i_) {
  Hash_pair tmp(i_, 0);
  return MapofTrain.findToken(tmp).pos;
}

int seatpos(char* i_) {
  Hash_pair tmp(i_, 0);
  return MapofSeat.findToken(tmp).pos;
}

bool istrainreleased(char* i_) {
  Train cur;
  StorageofTrain.read(cur, trainpos(i_));
  return cur.released;
}

int add_train(char* i_, int n_, int m_, vector<string> &s_, vector<int> &p_, Time &x_,
       vector<int> &t_, vector<int> &o_, Date &d1_, Date &d2_, char y_) {
  // -i -n -m -s -p -x -t -o -d -y
  if(istrainexist(i_)) return -1;
  Train newtrain(i_, n_, m_, s_, p_, x_, t_, o_, d1_, d2_, y_);
  int pos = StorageofTrain.write(newtrain);
  Hash_pair newhash(String_Hash(i_), pos);
  MapofTrain.insertToken(newhash);
  return 0;
}

int delete_train(char* i_) {
  if(!istrainexist(i_) || istrainreleased(i_)) return -1;
  Hash_pair tmp(i_, 0);
  MapofTrain.deleteToken(tmp);
  return 0;
}

int release_train(char* i_) {
  if(!istrainexist(i_) || istrainreleased(i_)) return -1;
  Hash_pair hash(i_, trainpos(i_));
  Train cur;
  StorageofTrain.read(cur, hash.pos);
  cur.released = true;
  StorageofTrain.update(cur, hash.pos);
  Seat seats[93];
  int datenum = cur.end_date - cur.begin_date + 1;
  for(int i = 0; i < datenum; i++) {
    for(int j = 0; i < cur.stationNum; j++) {
      seats[i][j] = cur.seatNum;
    }
  }
  int pos = 0;
  for(int i = datenum - 1; i >= 0; i--) {
    pos = StorageofSeat.write(seats[i]);
  }
  hash = Hash_pair(i_, pos);
  MapofSeat.insertToken(hash);
  for(int i = 0; i < cur.stationNum; i++) {
    station_pair pair_ = {String_Hash(cur.stations[i]), Hash_pair(i_, i)};
    MapofStationPair.insertToken(pair_);
  }
  return 0;
}

int query_train(char* i_, Date &d_) {
  Train cur;
  Seat seat;
  Hash_pair hash(i_, trainpos(i_));
  StorageofTrain.read(cur, hash.pos);
  if(cur.end_date - d_ < 0 || d_ - cur.begin_date < 0) return -1;
  int offset = d_ - cur.begin_date;
  if(cur.released) StorageofSeat.read(seat, seatpos(i_) + sizeof(Seat) * offset);
  cout << cur.trainID << " " << cur.type << "\n";
  for(int i = 0; i < cur.stationNum; i++) {
    cout << cur.stations[i] << " ";

    if(i == 0) cout << "xx-xx xx:xx ";
    else cout << d_ + cur.arrive[i] << " ";

    cout << "-> ";

    if(i == cur.stationNum - 1) cout << "xx-xx xx:xx ";
    else cout << d_ + cur.leave[i] << " ";

    cout << cur.prices[i] << " ";

    if(!cur.released) cout << cur.seatNum << "\n";
    else if(i != cur.stationNum - 1) cout << seat[i] << "\n";
    else cout << "x\n";
  }
}

void query_ticket(char* s_, char* t_, Date &d_, int p_) {
  
}

int query_transfer(char* s_, char* t_, Date &d_, int p_) {
  
}

int buy_ticket(char* u_, char* i_, Date &d_, int n_, char* f_, char* t_, bool q_) {
  
}

int query_order(char* u_) {
  
}

int refund_ticket(char* u_, int n_) {
  
}

void clean_train() {
  StorageofTrain.clear();
  MapofTrain.clear();
  StorageofSeat.clear();
  MapofSeat.clear();
  MapofStationPair.clear();
}

#endif
