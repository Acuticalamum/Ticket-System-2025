#ifndef TIME_HPP
#define TIME_HPP
static int DaysofMonth[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
           SumsofMonth[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334, 365};
class Time {
public:
  int day, hour, min;
  Time() = default;
  Time(int d_, int h_, int m_) : day(d_), hour(h_), min(m_) {}
};
class Date {
  int month, day;
  Date() = default;
  Date(int m_, int d_) : month(m_), day(d_) {}
};
#endif
