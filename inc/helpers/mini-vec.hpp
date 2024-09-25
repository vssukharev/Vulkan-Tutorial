
#include <cstddef>
#include <cstdlib>
#include <initializer_list>
#include <utility>
#include <iostream>

namespace App {
  template <typename T>
  class mini_vec
  {
    private:
      T* elem;
      std::size_t sz;

    public:
      using iterator = T*;
      using const_iterator = const T*;

    private:
      void copy_elem();

    public:
      mini_vec() = default;
      
      ~mini_vec()
      {
        delete[] elem;
      }

      mini_vec(const mini_vec& o)
      {
        T* new_elem = new T[o.size()];

        try {
          for (std::size_t i = 0; i != o.size(); ++i)
            new_elem[i] = o.elem[i];
        } catch (...) {
          delete[] new_elem;
          throw;
        }

        elem = new_elem;
        sz = o.size();
      }

      mini_vec(mini_vec&& o) : elem{o.elem}, sz{o.sz}
      {
        o.elem = nullptr; 
        o.sz = 0;
      }

      mini_vec& operator=(const mini_vec& o)
      {
        T* new_elem = new T[o.size()];

        try {
          for (std::size_t i = 0; i != o.size(); ++i)
            new_elem[i] = o.elem[i];
        } catch (...) {
          delete[] new_elem;
          throw;
        }

        elem = new_elem;
        sz = o.size();

        return *this;
      }

      mini_vec& operator=(mini_vec&& o)
      {
        std::swap(*this, o);
        return *this;
      }

      mini_vec(std::size_t vec_sz)
      {
        reallocate(vec_sz);
      }

      mini_vec(std::initializer_list<T> il)
      {
        T* new_elem = new T[il.size()];

        try {
          std::size_t i = 0;
          for (const T& list_elem : il) {
            new_elem[i] = list_elem;
            ++i;
          }
        } catch (...) {
          delete[] new_elem;
          throw;
        }

        elem = new_elem;
        sz = il.size();
      }

    public:
      T& operator[](std::size_t i) { return elem[i]; }
      const T& operator[](std::size_t i) const { return elem[i]; }

    public:
      iterator begin() { return elem; }
      iterator end() { return elem + sz; }
      const_iterator begin() const { return elem; }
      const_iterator end() const { return elem + sz; }

    public:
      std::size_t size() const { return sz; }
      T* data() { return elem; }
      const T* data() const { return elem; }

      void reallocate(std::size_t newSize)
      {
        T* new_elem = new T[newSize];

        for (std::size_t i = 0; i != sz; ++i)
          new_elem[i] = std::move(elem[i]);

        delete[] elem;
        elem = new_elem;
        sz = newSize;
      }

      void expand(std::size_t count)
      {
        std::size_t new_size = sz + count;
        reallocate(new_size);
      }
  };
}



