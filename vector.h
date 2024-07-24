#ifndef VECTOR_H
#define VECTOR_H

#include <algorithm>
#include <exception>
#include <iostream>
#include <iterator>
#include <type_traits>

template <class T>
class Vector {

 public:
  using ValueType = T;
  using Pointer = T*;
  using ConstPointer = const T*;
  using Reference = T&;
  using ConstReference = const T&;
  using SizeType = std::size_t;
  using Iterator = T*;
  using ConstIterator = const T*;
  using ReverseIterator = std::reverse_iterator<Iterator>;
  using ConstReverseIterator = std::reverse_iterator<ConstIterator>;

private:
  SizeType size_{};
  SizeType capacity_{};
  Pointer data_{};

public:
  Vector() = default;
  explicit Vector(size_t count) : size_(count), capacity_(count) {
    try {
      data_ = (count > 0 ? new T[count] : nullptr);
    } catch (...) {
      delete[] data_;
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
  }

  Vector(size_t count, const T& value)
    : size_(count), capacity_(count) {
    try {
      data_ = (count > 0 ? new T[count] : nullptr);
    } catch (...) {
      delete[] data_;
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
    try {
      std::fill(data_, data_ + count, value);
    } catch (...) {
      delete[] data_;
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
  }

  Vector(const Vector& other)
    : size_(other.size_), capacity_(other.capacity_) {
    try {
      data_ = (other.capacity_ > 0 ? new T[other.capacity_] : nullptr);
    } catch (...) {
      delete[] data_;
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }

    try {
      std::copy(other.data_, other.data_ + other.size_, data_);
    } catch (...) {
      delete[] data_;
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
  }

  Vector(Vector&& other) noexcept
    : size_(other.size_), capacity_(other.capacity_), data_(other.data_) {
    other.data_ = nullptr;
    other.capacity_ = 0;
    other.size_ = 0;
  }

  Vector(std::initializer_list<T> list)
    : size_(list.size()), capacity_(list.size()) {
    try {
      data_ = (list.size() > 0 ? new T[list.size()] : nullptr);
    } catch (...) {
      delete[] data_;
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }

    try {
      std::copy(list.begin(), list.end(), data_);
    } catch (...) {
      delete[] data_;
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
  }

  template <class Iterator,class = std::enable_if_t<std::is_base_of_v<std::forward_iterator_tag, typename std::iterator_traits<Iterator>::iterator_category>>>
  Vector(Iterator first, Iterator last) {
    try {
      for (auto it = first; it != last; ++it) {
        PushBack(*it);
      }
    } catch (...) {
      delete[] data_;
      data_ = nullptr;
      size_ = 0;
      capacity_ = 0;
      throw;
    }
  }

  size_t Capacity() const {
    return capacity_;
  };

  size_t Size() const {
    return size_;
  };

  bool Empty() const {
    return size_ == 0;
  };

  Vector<T>& operator=(const Vector<T>& other) {
    if (this != &other) {
      Vector<T> temp(other);
      std::swap(*this, temp);
    }

    return *this;
  }
  Vector<T>& operator=(Vector&& other) noexcept {
    if (this != &other) {
      delete[] data_;
      data_ = other.data_;
      size_ = other.size_;
      capacity_ = other.capacity_;
      other.data_ = nullptr;
      other.size_ = 0;
      other.capacity_ = 0;
    }

    return *this;
  }

  T& At(size_t index) {
    if (index >= size_) {
      throw std::out_of_range("Out of range");
    }

    return data_[index];
  };
  const T& At(size_t index) const{
    if (index >= size_) {
      throw std::out_of_range("Out of range");
    }

    return data_[index];
  };

  T& operator[](size_t index) {return data_[index];};

  const T& operator[](size_t index) const {return data_[index];};



  T& Front() {
    return data_[0];
  };
  const T& Front() const {
    return data_[0];
  };
  T& Back() {return data_[size_ - 1];};
  const T& Back() const {
    return data_[size_ - 1];
  };

  T* Data() {
    return data_;
  }
  const T* Data() const {
    return data_;
  }

  void Swap(Vector& other) {
    std::swap(*this,other);
  }

  void Reserve(size_t capacity){
    if (capacity_ >= capacity) {
      return;
    }

    T* new_ptr{};
    try {
      new_ptr = new T[capacity];
      std::copy(std::make_move_iterator(data_),
                std::make_move_iterator(data_ + size_), new_ptr);
      delete[] data_;
      capacity_ = capacity;
      data_ = new_ptr;
    } catch (...) {
      delete[] new_ptr;
      throw;
    }
  }

  void Resize(size_t new_size){
    if (new_size <= capacity_) {
      size_ = new_size;
    } else {
      T* new_ptr{};
      try {
        new_ptr = new T[new_size * 2];
        std::copy(std::make_move_iterator(data_),
                  std::make_move_iterator(data_ + size_), new_ptr);
        size_ = new_size;
        delete[] data_;
        data_ = new_ptr;
        capacity_ = new_size * 2;
      } catch (...) {
        delete[] new_ptr;
        throw;
      }
    }
  }
  void Resize(size_t new_size, const T& value) {
    if (new_size <= size_) {
      size_ = new_size;
    } else {
      if (new_size <= capacity_) {
        std::fill(data_ + size_, data_ + new_size, value);
        size_ = new_size;
      } else {
        T* new_ptr{};
        try {
          new_ptr = new T[new_size * 2];
          std::copy(std::make_move_iterator(data_),
                    std::make_move_iterator(data_ + size_), new_ptr);
          std::fill(new_ptr + size_, new_ptr + new_size, value);
          size_ = new_size;
          delete[] data_;
          data_ = new_ptr;
          capacity_ = new_size * 2;
        } catch (...) {
          delete[] new_ptr;
          throw;
        }
      }
    }
  }
  void PushBack(const T& value){
    if (size_ == capacity_) {
      Reserve(size_ == 0 ? 1 : (size_ * 2) + 1);
    }
    data_[size_++] = value;
  }

  void PushBack(T&& value){
    if (size_ == capacity_) {
      Reserve(size_ == 0 ? 1 : (size_ * 2) + 1);
    }
    data_[size_++] = std::move(value);
  }
  void PopBack(){
    if (size_ > 0) {
      --size_;
    }
  }

  void ShrinkToFit(){
    if (size_ == 0) {
      delete[] data_;
      data_ = nullptr;
      capacity_ = 0;
    } else if (capacity_ > size_) {
      T* new_ptr{};
      try {
        new_ptr = new T[size_];
        std::copy(std::make_move_iterator(data_),
                  std::make_move_iterator(data_ + size_), new_ptr);
        delete[] data_;
        data_ = new_ptr;
        capacity_ = size_;
      } catch (...) {
        delete[] new_ptr;
        throw;
      }
    }
  }

  void Clear() {
    size_ = 0;
  }

bool operator<(const Vector<T>& obj) const {
    return std::lexicographical_compare(this->data_, this->data_ + this->size_, obj.data_, obj.data_ + obj.size_);
  }
bool operator>(const Vector<T>& obj) const {
    return obj < *this;
  }

bool operator==(const Vector<T>& obj) const {
    if (this->size_ != obj.size_) {
      return false;
    }
    return std::equal(this->data_, this->data_ + this->size_, obj.data_);
  }

bool operator!=(const Vector<T>& obj) const {
    return !(*this == obj);
  }

bool operator<=(const Vector<T>& obj) const {
    return !(*this > obj);
  }
bool operator>=(const Vector<T>& obj) const {
    return !(*this < obj);
  }

  Iterator begin() { return data_; };            // NOLINT
  ConstPointer begin() const { return data_; };  // NOLINT

  Iterator end() { return data_ + size_; };             // NOLINT
  ConstIterator end() const { return data_ + size_; };  // NOLINT

  ReverseIterator rbegin() { // NOLINT
    return std::reverse_iterator<Pointer>(end());
  };
  ConstReverseIterator rbegin() const { // NOLINT
    return std::reverse_iterator<ConstIterator>(end());
  };


  ReverseIterator rend() {// NOLINT
    return std::reverse_iterator<Pointer>(begin());
  };
  ConstReverseIterator rend() const {// NOLINT
    return std::reverse_iterator<ConstIterator>(begin());
  };

  ConstIterator cbegin() { return data_; };        // NOLINT
  ConstIterator cend() { return data_ + size_; };  // NOLINT



  ConstReverseIterator crbegin() { // NOLINT
    return std::reverse_iterator<ConstIterator>(cend());
  };

  ConstReverseIterator crend() {// NOLINT
    return std::reverse_iterator<ConstIterator>(cbegin());
  };

  ~Vector() { delete[] data_; }


};








#endif
