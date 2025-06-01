#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <iostream>

namespace sjtu
{
  template<typename T>
  class vector
  {
    private:
      T* data;
      int nowsize;
      int maxsize;
    public:
      class const_iterator;
      class iterator
      {
        public:
          using difference_type = std::ptrdiff_t;
          using value_type = T;
          using pointer = T*;
          using reference = T&;
          using iterator_category = std::output_iterator_tag;
        private:
          pointer ptr;
        public:
          iterator(const pointer _ptr) {
            ptr = _ptr;
          }
          iterator operator+(const int &n) const
          {
            return iterator(ptr + n);
          }
          iterator operator-(const int &n) const
          {
            return iterator(ptr - n);
          }
          int operator-(const iterator &rhs) const
          {
            return ptr - rhs.ptr;
          }
          iterator& operator+=(const int &n)
          {
            ptr += n;
            return *this;
          }
          iterator& operator-=(const int &n)
          {
            ptr -= n;
            return *this;
          }
          iterator operator++(int) {
            iterator ret = *this;
            ++ptr;
            return ret;
          }
          iterator& operator++() {
            ++ptr;
            return *this;
          }
          iterator operator--(int) {
            iterator ret = *this;
            --ptr;
            return ret;
          }
          iterator& operator--() {
            --ptr;
            return *this;
          }
          T& operator*() const{
            return *ptr;
          }
          bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
          }
          bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
          }
          bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
          }
          bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
          }
      };
      class const_iterator
      {
        public:
          using difference_type = std::ptrdiff_t;
          using value_type = T;
          using pointer = T*;
          using reference = T&;
          using iterator_category = std::output_iterator_tag;

        private:
          pointer ptr;
        public:
          const_iterator(const pointer _ptr) {
            ptr = _ptr;
          }
          const_iterator operator+(const int &n) const
          {
            return const_iterator(ptr + n);
          }
          const_iterator operator-(const int &n) const
          {
            return const_iterator(ptr - n);
          }
          int operator-(const const_iterator &rhs) const
          {
            return ptr - rhs.ptr;
          }
          const_iterator& operator+=(const int &n)
          {
            ptr += n;
            return *this;
          }
          const_iterator& operator-=(const int &n)
          {
            ptr -= n;
            return *this;
          }
          const_iterator operator++(int) {
            const_iterator ret = *this;
            ++ptr;
            return ret;
          }
          const_iterator& operator++() {
            ++ptr;
            return *this;
          }
          const_iterator operator--(int) {
            const_iterator ret = *this;
            --ptr;
            return ret;
          }
          const_iterator& operator--() {
            --ptr;
            return *this;
          }
          const T& operator*() const{
            return *ptr;
          }
          bool operator==(const iterator &rhs) const {
            return ptr == rhs.ptr;
          }
          bool operator==(const const_iterator &rhs) const {
            return ptr == rhs.ptr;
          }
          bool operator!=(const iterator &rhs) const {
            return ptr != rhs.ptr;
          }
          bool operator!=(const const_iterator &rhs) const {
            return ptr != rhs.ptr;
          }

      };
      vector() {
        data = nullptr;
        nowsize = 0;
        maxsize = 0;
      }
      vector(const vector &other) {
        nowsize = other.nowsize;
        maxsize = other.maxsize;
        data = static_cast<T*>(operator new[](maxsize * sizeof(T)));
        for(int i = 0; i < nowsize; i++) {
          new (data + i) T(other.data[i]);
        }
      }
      ~vector() {
        for(int i = 0; i < nowsize; i++) {
          data[i].~T();
        }
        operator delete[](data);
      }
      vector &operator=(const vector &other) {
        if(this == &other) return *this;
        for(int i = 0; i < nowsize; i++) {
          data[i].~T();
        }
        operator delete[](data);
        nowsize = other.nowsize;
        maxsize = other.maxsize;
        data = static_cast<T*>(operator new[](maxsize * sizeof(T)));
        for(int i = 0; i < nowsize; i++) {
          new (data + i) T(other.data[i]);
        }
        return *this;
      }
      T & at(const size_t &pos) {
        if(pos < 0 || pos >= nowsize) throw index_out_of_bound();
        return data[pos];
      }
      const T & at(const size_t &pos) const {
        if(pos < 0 || pos >= nowsize) throw index_out_of_bound();
        return data[pos];
      }
      T & operator[](const size_t &pos) {
        if(pos < 0 || pos >= nowsize) throw index_out_of_bound();
        return data[pos];
      }
      const T & operator[](const size_t &pos) const {
        if(pos < 0 || pos >= nowsize) throw index_out_of_bound();
        return data[pos];
      }
      const T & front() const {
        if(nowsize == 0) {
          throw container_is_empty();
        }
        return data[0];
      }
      const T & back() const {
        if(nowsize == 0) {
          throw container_is_empty();
        }
        return data[nowsize - 1];
      }
      iterator begin() {
        return iterator(data);
      }
      const_iterator begin() const {
        return const_iterator(data);
      }
      const_iterator cbegin() const {
        return const_iterator(data);
      }
      iterator end() {
        return iterator(data + nowsize);
      }
      const_iterator end() const {
        return const_iterator(data + nowsize);
      }
      const_iterator cend() const {
        return const_iterator(data + nowsize);
      }
      bool empty() const {
        return (nowsize == 0);
      }
      size_t size() const {
        return nowsize;
      }
      void clear() {
        for(int i = 0; i < nowsize; i++) {
          data[i].~T();
        }
        nowsize = 0;
        maxsize = 0;
        operator delete[](data);
        data = nullptr;
      }
      iterator insert(iterator pos, const T &value) {
        int ind = pos - begin();
        if(nowsize == maxsize) {
          if(maxsize == 0) {
            maxsize = 1;
          }
          else {
            maxsize *= 2;
          }
          T* tmp = static_cast<T*>(operator new[](maxsize * sizeof(T)));
          for(int i = 0; i < ind; i++) {
            new (tmp + i) T(data[i]);
          }
          for(int i = ind; i < nowsize; i++) {
            new (tmp + i + 1) T(data[i]);
          }
          for(int i = 0; i < nowsize; i++) {
            data[i].~T();
          }
          operator delete[](data);
          data = tmp;
        }
        else {
          for(int i = nowsize - 1; i >= ind; i--) {
            new (data + i + 1)  T(data[i]);
            data[i].~T();
          }
        }
        new (data + ind) T(value);
        nowsize++;
        return iterator(data + ind);
      }
      iterator insert(const size_t &ind, const T &value) {
        if(ind > nowsize) {
          throw index_out_of_bound();
        }
        if(nowsize == maxsize) {
          if(maxsize == 0) {
            maxsize = 1;
          }
          else {
            maxsize *= 2;
          }
          T* tmp = static_cast<T*>(operator new[](maxsize * sizeof(T)));
          for(int i = 0; i < ind; i++) {
            new (tmp + i) T(data[i]);
          }
          for(int i = ind; i < nowsize; i++) {
            new (tmp + i + 1) T(data[i]);
          }
          for(int i = 0; i < nowsize; i++) {
            data[i].~T();
          }
          operator delete[](data);
          data = tmp;
        }
        else {
          for(int i = nowsize - 1; i >= ind; i--) {
            new (data + i + 1)  T(data[i]);
            data[i].~T();
          }
        }
        new (data + ind) T(value);
        nowsize++;
        return iterator(data + ind);
      }
      iterator erase(iterator pos) {
        int ind = pos - begin();
        for(int i = ind; i < nowsize - 1; i++) data[i] = data[i + 1];
        --nowsize;
        data[nowsize].~T();
        return iterator(data + ind);
      }
      iterator erase(const size_t &ind) {
        if(ind >= nowsize) {
          throw index_out_of_bound();
        }
        for(int i = ind; i < nowsize - 1; i++) data[i] = data[i + 1];
        --nowsize;
        data[nowsize].~T();
        return iterator(data + ind);
      }
      void push_back(const T &value) {
        if(nowsize == maxsize) {
          if(maxsize == 0) {
            maxsize = 1;
          }
          else {
            maxsize *= 2;
          }
          T* tmp = static_cast<T*>(operator new[](maxsize * sizeof(T)));
          for(int i = 0; i < nowsize; i++) {
            new (tmp + i) T(data[i]);
          }
          for(int i = 0; i < nowsize; i++) {
            data[i].~T();
          }
          operator delete[](data);
          data = tmp;
        }
        new (data + nowsize) T(value);
        ++nowsize;
      }
      void pop_back() {
        if(nowsize == 0) {
          throw container_is_empty();
        }
        else {
          data[nowsize - 1].~T();
          nowsize--;
        }
      }
  };
}

#endif
