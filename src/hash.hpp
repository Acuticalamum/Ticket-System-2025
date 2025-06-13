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
  bool operator==(const String_Hash &other) const {
    return h1 == other.h1 && h2 == other.h2;
  }
  bool operator!=(const String_Hash &other) const {
    return h1 != other.h1 || h2 != other.h2;
  }
  bool operator<(const String_Hash &other) const {
    if(h1 == other.h1) return h2 < other.h2;
    return h1 < other.h1;
  }
  bool operator>(const String_Hash &other) const {
    if(h1 == other.h1) return h2 > other.h2;
    return h1 > other.h1;
  }
  String_Hash() = default;
  String_Hash(long long a, long long b) : h1(a), h2(b) {}
  String_Hash(char* str) {
    h1 = h2 = 0;
    for(size_t i = 0; i < strlen(str); i++) {
      h1 = (h1 * p1 + str[i]) % mod1;
      h2 = (h2 * p2 + str[i]) % mod2;
    }
  }
};
String_Hash convert_to_hash(char* str) {
  long long h1 = 0, h2 = 0;
  for(size_t i = 0; i < strlen(str); i++) {
    h1 = (h1 * p1 + str[i]) % mod1;
    h2 = (h2 * p2 + str[i]) % mod2;
  }
  return String_Hash(h1, h2);
}
class Hash_pair {
public:
  String_Hash hash;
  int pos;
  bool operator==(const Hash_pair &other) const {
    return hash == other.hash;
  }
  bool operator<(const Hash_pair &other) const {
    return hash < other.hash;
  }
  bool operator>(const Hash_pair &other) const {
    return hash > other.hash;
  }
  bool operator!=(const Hash_pair &other) const {
    return hash != other.hash;
  }
  Hash_pair() = default;
  Hash_pair(String_Hash hash_, int pos_) : hash(hash_), pos(pos_) {};
};
//using station_pair = pair<String_Hash, Hash_pair>;

#endif
