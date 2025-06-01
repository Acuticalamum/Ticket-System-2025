#ifndef TRAIN_HPP
#define TRAIN_HPP
#include"time.hpp"
#include"../STLite/vector/vector.hpp"
using sjtu::vector;

class Train {
public:
  char trainID[21]{};
  int stationNum, seatNum;
  char stations[101][21];
  int prices[101];
  Time arrive[101], leave[101];
  Date begin_date, end_date;
  char type;
  bool released;
  Train() = default;
  Train(char* id_, int stationNum_, int seatNum_, vector<char[21]> &stations_, vector<int> &prices_, Time &start_time_,
       vector<int> &travel_times_, vector<int> &stopover_times_, Date &begin_date_, Date &end_date_, char type_) : 
       stationNum(stationNum_), seatNum(seatNum_), begin_date(begin_date_), end_date(end_date_), type(type_), released(false) {
          strcpy(trainID, id_);
          prices[0] = 0;
          for(int i = 1; i < stationNum; i++) prices[i] = prices[i - 1] + prices_[i - 1];
          for(int i = 0; i < stationNum; i++) strcpy(stations[i], stations_[i]);
          leave[0] = start_time_;
          for(int i = 1; i < stationNum - 1; i++) {
            //arrive[i] = leave[i - 1] + travel_times_[i - 1];
            //leave[i] = arrive[i] + stopover_times_[i - 1];
          }
          //arrive[stationNum - 1] = leave[stationNum - 2] + travel_times_[stationNum - 2];
       }
};
class Ticket {
public:
};
#endif
