/**
 * implement a container like std::map
 */
#ifndef SJTU_MAP_HPP
#define SJTU_MAP_HPP

// only for std::less<T>
#include <functional>
#include <cstddef>
#include <iostream>
#include "utility.hpp"
#include "exceptions.hpp"

namespace sjtu {

template<
    class Key,
    class T,
    class Compare = std::less <Key>
> class map {
 public:
  /**
   * the internal type of data.
   * it should have a default constructor, a copy constructor.
   * You can use sjtu::map as value_type by typedef.
   */
  typedef pair<const Key, T> value_type;
  class node {
   public:
    mutable value_type data;
    node* parent;
    node* lchild;
    node* rchild;
    int height_ = 1;
    node() = default;
    node(value_type data_) : data(data_) {}
    node(value_type data_, node* parent_, node* lchild_, node* rchild_, int height__) : data(data_), 
    parent(parent_), lchild(lchild_), rchild(rchild_), height_(height__) {}
    node(const node &other) {
      data = pair(other.data.first, other.data.second);
      parent = other.parent;
      lchild = other.lchild;
      rchild = other.rchild;
      height_ = other.height_;
    }
  };
  /**
   * see BidirectionalIterator at CppReference for help.
   *
   * if there is anything wrong throw invalid_iterator.
   *     like it = map.begin(); --it;
   *       or it = map.end(); ++end();
   */
  int height(node* u) {
    if(u) return u -> height_;
    return 0;
  }

  int bf(node* u) {
    if(u) return height(u -> lchild) - height(u -> rchild);
    return 0;
  }
  
  void update(node* u) {
    u -> height_ = std::max(height(u -> lchild), height(u -> rchild)) + 1;
  }

  node* LL(node* u) {
    node* v = u -> lchild;
    u -> lchild = v -> rchild;
    if(v -> rchild) v -> rchild -> parent = u;
    v -> rchild = u;
    v -> parent = u -> parent;
    if(u -> parent) {
      if(u == u -> parent -> lchild) u -> parent -> lchild = v;
      else u -> parent -> rchild = v;
    }
    u -> parent = v;
    update(u);
    update(v);
    return v;
  }

  node* RR(node* u) {
    node* v = u -> rchild;
    u -> rchild = v -> lchild;
    if(v -> lchild) v -> lchild -> parent = u;
    v -> lchild = u;
    v -> parent = u -> parent;
    if(u -> parent) {
      if(u == u -> parent -> lchild) u -> parent -> lchild = v;
      else u -> parent -> rchild = v;
    }
    u -> parent = v;
    update(u);
    update(v);
    return v;
  }

  node* LR(node* u) {
    u -> lchild = RR(u -> lchild);
    return u = LL(u);
  }

  node* RL(node* u) {
    u -> rchild = LL(u -> rchild);
    return u = RR(u);
  }

  node* insert(const value_type& val, node* u, node* pa) {
    if(u == nullptr) {
      u = new node(val);
      u -> parent = pa;
      u -> lchild = nullptr;
      u -> rchild = nullptr;
      return u;
    }
    if(cmp(val.first, (u -> data).first)) {
      u -> lchild = insert(val, u -> lchild, u);
      if(bf(u) == 2) {
        if(cmp(val.first, (u -> lchild -> data).first)) u = LL(u);
        else u = LR(u);
      }
    }
    else {
      u -> rchild = insert(val, u -> rchild, u);
      if(bf(u) == -2) {
        if(cmp(val.first, (u -> rchild -> data).first)) u = RL(u);
        else u = RR(u);
      }
    }
    update(u);
    return u;
  }
  
  node* adjust(node* u) {
    if(bf(u) > 1) {
      if(bf(u -> lchild) >= 0) return LL(u);
      else return LR(u);
    }
    if(bf(u) < -1) {
      if(bf(u -> rchild) <= 0) return RR(u);
      else return RL(u);
    }
    return u;
  }

  node* erase(const Key& key, node* u) {
    if(u == nullptr) return nullptr;
    const Key& now_key = (u -> data).first;
    if((cmp(now_key, key) ^ cmp(key, now_key)) == false) {
      if(u -> lchild == nullptr || u -> rchild == nullptr) {
        node* tmp = u;
        u = u -> lchild ? u -> lchild : u -> rchild;
        if(tmp -> parent) {
          if(tmp == tmp -> parent -> lchild) tmp -> parent -> lchild = u;
          if(tmp == tmp -> parent -> rchild) tmp -> parent -> rchild = u;
        }
        delete tmp;
        return u;
      }

      else {
        node* next = u -> rchild;
        while(next -> lchild) next = next -> lchild;
        if(next -> parent != u) {
          if(next -> rchild) next -> rchild -> parent = next -> parent;
          next -> parent -> lchild = next -> rchild;
          next -> rchild = u -> rchild;
          if(next -> rchild) next -> rchild -> parent = next;
        }
        next -> lchild = u -> lchild;
        if(next -> lchild) next -> lchild -> parent = next;
        next -> parent = u -> parent;
        if(u -> parent) {
          if(u == u -> parent -> lchild) u -> parent -> lchild = next;
          else u -> parent -> rchild = next;
        }
        delete u;
        u = next;
      }

    }
    else if(cmp(key, now_key)) {
      u -> lchild = erase(key, u -> lchild);
      if(u -> lchild) u -> lchild -> parent = u;
    }
    else {
      u -> rchild = erase(key, u -> rchild);
      if(u -> rchild) u -> rchild -> parent = u;
    }
    update(u);
    u = adjust(u);
    return u;
  }

  class const_iterator;
  class iterator {
   friend class map<Key, T, Compare>;
   private:
    /**
     * TODO add data members
     *   just add whatever you want.
     */
    node* ptr;
    map* id;

   public:
    iterator() {
      // TODO
      ptr = nullptr;
      id = nullptr;
    }

    iterator(const iterator &other) {
      // TODO
      ptr = other.ptr;
      id = other.id;
    }

    iterator(node* ptr_, map* id_) {
      ptr = ptr_;
      id = id_;
    }

    /**
     * TODO iter++
     */
    iterator operator++(int) {
      if(ptr == nullptr) throw invalid_iterator();
      iterator ret = *this;
      ++(*this);
      return ret;
    }

    /**
     * TODO ++iter
     */
    iterator &operator++() {
      if(ptr == nullptr) throw invalid_iterator();
      if(ptr -> rchild) {
        ptr = ptr -> rchild;
        while(ptr -> lchild) ptr = ptr -> lchild;
      } else {
        node* pa = ptr -> parent;
        while(pa != nullptr && ptr == pa -> rchild) {
          ptr = pa;
          pa = pa -> parent;
        }
        ptr = pa;
      }
      return *this;
    }

    /**
     * TODO iter--
     */
    iterator operator--(int) {
      if(ptr == nullptr) throw invalid_iterator();
      iterator ret = *this;
      --(*this);
      return ret;
    }

    /**
     * TODO --iter
     */
    iterator &operator--() {
      if(ptr == nullptr) {
        ptr = id -> root;
        if(ptr == nullptr) throw invalid_iterator();
        while(ptr -> rchild) ptr = ptr -> rchild;
        return *this;
      }
      if(ptr -> lchild) {
        ptr = ptr -> lchild;
        while(ptr -> rchild) ptr = ptr -> rchild;
      } else {
        node* pa = ptr -> parent;
        while(pa != nullptr && ptr == pa -> lchild) {
          ptr = pa;
          pa = pa -> parent;
        }
        ptr = pa;
      }
      if(ptr == nullptr) throw invalid_iterator();
      return *this;
    }

    /**
     * a operator to check whether two iterators are same (pointing to the same memory).
     */
    value_type &operator*() const {
      if(ptr == nullptr) throw invalid_iterator();
      return ptr -> data;
    }

    bool operator==(const iterator &rhs) const {
      return ptr == rhs.ptr && id == rhs.id;
    }

    bool operator==(const const_iterator &rhs) const {
      return ptr == rhs.ptr && id == rhs.id;
    }

    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {
      return !(*this == rhs);
    }

    bool operator!=(const const_iterator &rhs) const {
      return !(*this == rhs);
    }

    /**
     * for the support of it->first.
     * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
     */
    value_type *operator->() const
    noexcept {
      return &(ptr -> data);
    }
  };
  class const_iterator {
    // it should has similar member method as iterator.
    //  and it should be able to construct from an iterator.
   friend class map<Key, T, Compare>;
   private:
    node* ptr;
    map* id;
    // data members.
   public:
    const_iterator() {
      ptr = nullptr;
      id = nullptr;
      // TODO
    }

    const_iterator(const const_iterator &other) {
      ptr = other.ptr;
      id = other.id;
      // TODO
    }

    const_iterator(const iterator &other) {
      ptr = other.ptr;
      id = other.id;
      // TODO
    }

    const_iterator(node* ptr_, map* id_) {
      ptr = ptr_;
      id = id_;
    }
    // And other methods in iterator.
    // And other methods in iterator.
    // And other methods in iterator.
    const_iterator operator++(int) {
      if(ptr == nullptr) throw invalid_iterator();
      const_iterator ret = *this;
      ++(*this);
      return ret;
    }

    /**
     * TODO ++iter
     */
    const_iterator &operator++() {
      if(ptr == nullptr) throw invalid_iterator();
      if(ptr -> rchild) {
        ptr = ptr -> rchild;
        while(ptr -> lchild) ptr = ptr -> lchild;
      } else {
        node* pa = ptr -> parent;
        while(pa != nullptr && ptr == pa -> rchild) {
          ptr = pa;
          pa = pa -> parent;
        }
        ptr = pa;
      }
      return *this;
    }

    /**
     * TODO iter--
     */
    const_iterator operator--(int) {
      if(ptr == nullptr) throw invalid_iterator();
      const_iterator ret = *this;
      --(*this);
      return ret;
    }

    /**
     * TODO --iter
     */
    const_iterator &operator--() {
      if(ptr == nullptr) {
        ptr = id -> root;
        if(ptr == nullptr) throw invalid_iterator();
        while(ptr -> rchild) ptr = ptr -> rchild;
        return *this;
      }
      if(ptr -> lchild) {
        ptr = ptr -> lchild;
        while(ptr -> rchild) ptr = ptr -> rchild;
      } else {
        node* pa = ptr -> parent;
        while(pa != nullptr && ptr == pa -> lchild) {
          ptr = pa;
          pa = pa -> parent;
        }
        ptr = pa;
      }
      if(ptr == nullptr) throw invalid_iterator();
      return *this;
    }

    /**
     * a operator to check whether two iterators are same (pointing to the same memory).
     */
    const value_type &operator*() const {
      if(ptr == nullptr) throw invalid_iterator();
      return ptr -> data;
    }

    bool operator==(const iterator &rhs) const {
      return ptr == rhs.ptr && id == rhs.id;
    }

    bool operator==(const const_iterator &rhs) const {
      return ptr == rhs.ptr && id == rhs.id;
    }

    /**
     * some other operator for iterator.
     */
    bool operator!=(const iterator &rhs) const {
      return !(*this == rhs);
    }

    bool operator!=(const const_iterator &rhs) const {
      return !(*this == rhs);
    }
    /**
     * for the support of it->first.
     * See <http://kelvinh.github.io/blog/2013/11/20/overloading-of-member-access-operator-dash-greater-than-symbol-in-cpp/> for help.
     */
    const value_type *operator->() const
    noexcept {
      return &(ptr -> data);
    }
  };
 public:
  node* root;
  size_t mapsize;
  Compare cmp;
  node* find_ptr(const Key &key) const {
    node* now = root;
    while(now != nullptr) {
      const Key& now_key = (now -> data).first;
      if((cmp(now_key, key) ^ cmp(key, now_key)) == false) return now;
      else {
        if(cmp(key, now_key)) now = now -> lchild;
        else now = now -> rchild;
      }
    }
    return nullptr;
  }
 public:
  node* copymap(node* root_, node* parent_) {
    if(root_ == nullptr) return nullptr;
    node* tmp = new node(root_ -> data);
    tmp -> parent = parent_;
    tmp -> height_ = root_ -> height_;
    tmp -> lchild = copymap(root_ -> lchild, tmp);
    tmp -> rchild = copymap(root_ -> rchild, tmp);
    return tmp;
  }
  void deletemap(node* root_) {
    if(root_ == nullptr) return;
    deletemap(root_ -> lchild);
    deletemap(root_ -> rchild);
    delete root_;
  }
  /**
   * TODO two constructors
   */
  map() {
    root = nullptr;
    mapsize = 0;
  }

  map(const map &other) {
    root = copymap(other.root, nullptr);
    mapsize = other.mapsize;
  }

  /**
   * TODO assignment operator
   */
  map &operator=(const map &other) {
    if(this == &other) return *this;
    deletemap(root);
    root = copymap(other.root, nullptr);
    mapsize = other.mapsize;
    return *this;
  }

  /**
   * TODO Destructors
   */
  ~map() {
    deletemap(root);
    root = nullptr;
  }

  /**
   * TODO
   * access specified element with bounds checking
   * Returns a reference to the mapped value of the element with key equivalent to key.
   * If no such element exists, an exception of type index_out_of_bound'
   */
  T &at(const Key &key) {
    node* pos = find_ptr(key);
    if(pos == nullptr) throw index_out_of_bound();
    return (pos -> data).second;
  }

  const T &at(const Key &key) const {
    node* pos = find_ptr(key);
    if(pos == nullptr) throw index_out_of_bound();
    return (pos -> data).second;
  }

  /**
   * TODO
   * access specified element
   * Returns a reference to the value that is mapped to a key equivalent to key,
   *   performing an insertion if such key does not already exist.
   */
  T &operator[](const Key &key) {
    node* pos = find_ptr(key);
    if(pos == nullptr) {
      value_type data_ = pair(key, T());
      pair<iterator, bool> insert_pos = insert(data_);
      return (*(insert_pos.first)).second;
    }
    return (pos -> data).second;
  }

  /**
   * behave like at() throw index_out_of_bound if such key does not exist.
   */
  const T &operator[](const Key &key) const {
    node* pos = find_ptr(key);
    if(pos == nullptr) throw index_out_of_bound();
    return (pos -> data).second;
  }

  /**
   * return a iterator to the beginning
   */
  iterator begin() {
    node* ret = root;
    if(ret == nullptr) return iterator(ret, this);
    while(ret -> lchild) ret = ret -> lchild;
    return iterator(ret, this);
  }

  const_iterator cbegin() const {
    node* ret = root;
    if(ret == nullptr) return const_iterator(ret, const_cast<map*>(this));
    while(ret -> lchild) ret = ret -> lchild;
    return const_iterator(ret, const_cast<map*>(this));
  }

  /**
   * return a iterator to the end
   * in fact, it returns past-the-end.
   */
  iterator end() {
    return iterator(nullptr, this);
  }

  const_iterator cend() const {
    return const_iterator(nullptr, const_cast<map*>(this));
  }

  /**
   * checks whether the container is empty
   * return true if empty, otherwise false.
   */
  bool empty() const {
    return mapsize == 0;
  }

  /**
   * returns the number of elements.
   */
  size_t size() const {
    return mapsize;
  }

  /**
   * clears the contents
   */
  void clear() {
    deletemap(root);
    root = nullptr;
    mapsize = 0;
  }

  /**
   * insert an element.
   * return a pair, the first of the pair is
   *   the iterator to the new element (or the element that prevented the insertion),
   *   the second one is true if insert successfully, or false.
   */
  pair<iterator, bool> insert(const value_type &value) {
    iterator pos = find(value.first);
    if(pos.ptr != nullptr) return pair(pos, false);
    root = insert(value, root, nullptr);
    if(root) root -> parent = nullptr;
    iterator newpos = find(value.first);
    ++mapsize;
    return pair(newpos, true);
  }

  /**
   * erase the element at pos.
   *
   * throw if pos pointed to a bad element (pos == this->end() || pos points an element out of this)
   */
  void erase(iterator pos) {
    if(pos.ptr == nullptr) throw invalid_iterator();
    if(pos.id != this) throw invalid_iterator();
    root = erase(pos -> first, root);
    --mapsize;
    if(mapsize == 0) root = nullptr;
  }

  /**
   * Returns the number of elements with key
   *   that compares equivalent to the specified argument,
   *   which is either 1 or 0
   *     since this container does not allow duplicates.
   * The default method of check the equivalence is !(a < b || b > a)
   */
  size_t count(const Key &key) const {
    node* pos = find_ptr(key);
    if(pos) return 1;
    return 0;
  }

  /**
   * Finds an element with key equivalent to key.
   * key value of the element to search for.
   * Iterator to an element with key equivalent to key.
   *   If no such element is found, past-the-end (see end()) iterator is returned.
   */
  iterator find(const Key &key) {
    node* pos = find_ptr(key);
    if(pos == nullptr) return iterator(nullptr, this);
    return iterator(pos, this);
  }

  const_iterator find(const Key &key) const {
    node* pos = find_ptr(key);
    if(pos == nullptr) return const_iterator(nullptr, const_cast<map*>(this));
    return const_iterator(pos, const_cast<map*>(this));
  }
};

}

#endif
