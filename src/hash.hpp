#ifndef HASH_HPP
#define HASH_HPP
#include<cstring>
const static long long mod1 = 998244353,
                   mod2 = 1e9 + 7,
                   p1 = 137,
                   p2 = 139;
class String_Hash {
public:
  long long h1, h2;
  bool operator==(const String_Hash &other) {
    return h1 == other.h1 && h2 == other.h2;
  }
  bool operator!=(const String_Hash &other) {
    return h1 != other.h1 || h2 != other.h2;
  }
  bool operator<(const String_Hash &other) {
    if(h1 == other.h1) return h2 < other.h2;
    return h1 < other.h1;
  }
  bool operator>(const String_Hash &other) {
    if(h1 == other.h1) return h2 > other.h2;
    return h1 > other.h1;
  }
  String_Hash(long long a, long long b) : h1(a), h2(b) {}
};
String_Hash convert_to_hash(char* str) {
  long long h1 = 0, h2 = 0;
  for(size_t i = 0; i < strlen(str); i++) {
    h1 = (h1 * p1 + str[i]) % mod1;
    h2 = (h2 * p2 + str[i]) % mod2;
  }
  return String_Hash(h1, h2);
}
#endif
