#ifndef TIME_HPP
#define TIME_HPP
#include<string>
#include<iostream>

using std::string;
using std::ostream;

static int DaysofMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
           SumsofMonth[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
class Time {
public:
  int day, hour, min;
  Time() = default;
  Time(int d_, int h_, int m_) : day(d_), hour(h_), min(m_) {}
  Time(int minute) : day(minute / 1440), hour(minute % 1440 / 60), min(minute % 60){}
  Time(string &str) : hour(10 * (str[0] - '0') + str[1] - '0'), min(10 * (str[3] - '0') + str[4] - '0'){}
  int getminute() {
    return day * 1440 + hour * 60 + min;
  }
  Time operator+(Time &other) {
    return Time(getminute() + other.getminute());
  }
  Time operator+(int &other) {
    return Time(getminute() + other);
  }
  Time operator-(Time &other) {
    return Time(getminute() + other.getminute());
  }
  Time operator-(int &other) {
    return Time(getminute() - other);
  }
};

class Date {
public:
  int month, day, hour, min;
  Date() = default;
  Date(int m_, int d_) : month(m_), day(d_) {}
  Date(int m_, int d_, Time t_) : month(m_), day(d_), hour(t_.hour), min(t_.min) {}
  Date(string &str) : month(10 * (str[0] - '0') + str[1] - '0'), day(10 * (str[3] - '0') + str[4] - '0') {
    if(str.size() == 11) {
      hour = 10 * (str[6] - '0') + str[7] - '0';
      min = 10 * (str[9] - '0') + str[10] - '0';
    } else {
      hour = min = 0;
    }
  }
  int getday() const{
    return SumsofMonth[month] + day;
  }
  int operator-(Date &other) const{
    return getday() - other.getday();
  }
  Date operator+(Time &other) const {
    Date ret(month, day + other.day, Time(0, hour + other.hour, min + other.min));
    while(ret.min >= 60) {
      ++ret.hour;
      ret.min -= 60;
    }
    while(ret.hour >= 24) {
      ++ret.day;
      ret.hour -= 24;
    }
    while(ret.day > DaysofMonth[ret.month]) {
      ret.day -= DaysofMonth[ret.month];
      ++ret.month;
    }
    return ret;
  }
};


ostream &operator<<(ostream &os, Date d) {
  os << '0' << d.month << '-';
  os << (d.day < 10 ? "0" : "") << d.day << ' ';
  os << (d.hour < 10 ? "0" : "") << d.hour << ':';
  os << (d.min < 10 ? "0" : "") << d.min;
  return os;
}

#endif
