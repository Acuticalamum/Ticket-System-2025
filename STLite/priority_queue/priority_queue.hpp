#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {
  /**
   * @brief a container like std::priority_queue which is a heap internal.
   * **Exception Safety**: The `Compare` operation might throw exceptions for certain data.
   * In such cases, any ongoing operation should be terminated, and the priority queue should be restored to its original state before the operation began.
   */
  template<typename T, class Compare = std::less<T>>
  class priority_queue {
    private:

      struct heapnode {
        heapnode(T val_) : val(val_){
          lc = nullptr;
          rc = nullptr;
          dis = 0;
        }
        T val;
        heapnode* lc;
        heapnode* rc;
        int dis;
      };

      heapnode* root;
      size_t size_;
      Compare cmp;

      heapnode* mergeheap(heapnode* root1, heapnode* root2) {
        if(root1 == nullptr) return root2;
        if(root2 == nullptr) return root1;
        if(cmp(root1 -> val, root2 -> val)) {
          heapnode* tmp = root1;
          root1 = root2;
          root2 = tmp;
        }
        root1 -> rc = mergeheap(root1 -> rc, root2);
        if(root1 -> lc == nullptr || root1 -> lc -> dis < root1 -> rc -> dis) {
          heapnode* tmp = root1 -> lc;
          root1 -> lc = root1 -> rc;
          root1 -> rc = tmp;
        }
        if(root1 -> rc == nullptr) root1 -> dis = 0;
        else root1 -> dis = (root1 -> rc -> dis) + 1;
        return root1;
      }

      void deleteheap(heapnode* root1) {
        if(root1 == nullptr) return;
        deleteheap(root1 -> lc);
        deleteheap(root1 -> rc);
        delete root1;
      }

      heapnode* copyheap(heapnode* root1) {
        if(root1 == nullptr) return nullptr;
        heapnode* tmp = new heapnode(root1 -> val);
        tmp -> lc = copyheap(root1 -> lc);
        tmp -> rc = copyheap(root1 -> rc);
        tmp -> dis = root1 -> dis;
        return tmp;
      }

    public:

      /**
       * @brief default constructor
       */
      priority_queue() {
        root = nullptr;
        size_ = 0;
      }

      /**
       * @brief copy constructor
       * @param other the priority_queue to be copied
       */
      priority_queue(const priority_queue &other) {
        root = copyheap(other.root);
        size_ = other.size_;
      }

      /**
       * @brief deconstructor
       */
      ~priority_queue() {
        deleteheap(root);
      }

      /**
       * @brief Assignment operator
       * @param other the priority_queue to be assigned from
       * @return a reference to this priority_queue after assignment
       */
      priority_queue &operator=(const priority_queue &other) {
        if(this == &other) return *this;
        deleteheap(root);
        root = copyheap(other.root);
        size_ = other.size_;
        return *this;
      }

      /**
       * @brief get the top element of the priority queue.
       * @return a reference of the top element.
       * @throws container_is_empty if empty() returns true
       */
      const T & top() const {
        if(size_ == 0) throw container_is_empty();
        return root -> val;
      }

      /**
       * @brief push new element to the priority queue.
       * @param e the element to be pushed
       */
      void push(const T &e) {
        heapnode* other = new heapnode(e);
        try {
          root = mergeheap(root, other);
          ++size_;
        } catch(...) {
          delete other;
          throw runtime_error();
        }
      }

      /**
       * @brief delete the top element from the priority queue.
       * @throws container_is_empty if empty() returns true
       */
      void pop() {
        try {
          if(size_ == 0) throw container_is_empty();
          heapnode* tmp = root;
          root = mergeheap(root -> lc, root -> rc);
          delete tmp;
          --size_;
        } catch(...) {
          throw runtime_error();
        }
      }

      /**
       * @brief return the number of elements in the priority queue.
       * @return the number of elements.
       */
      size_t size() const {
        return size_;
      }

      /**
       * @brief check if the container is empty.
       * @return true if it is empty, false otherwise.
       */
      bool empty() const {
        return size_ == 0;
      }

      /**
       * @brief merge another priority_queue into this one.
       * The other priority_queue will be cleared after merging.
       * The complexity is at most O(logn).
       * @param other the priority_queue to be merged.
       */
      void merge(priority_queue &other) {
        try {
          root = mergeheap(root, other.root);
          size_ += other.size_;
          other.root = nullptr;
          other.size_ = 0;
        } catch(...) {
          throw runtime_error();
        }
      }
  };
}

#endif
