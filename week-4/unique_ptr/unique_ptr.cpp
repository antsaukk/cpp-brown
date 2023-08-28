#include "../../utils/test_runner.h"

#include <iostream>
#include <cstddef>

using namespace std;

template <typename T>
class UniquePtr {
public:
  UniquePtr() : ptr_(nullptr) {}

  UniquePtr(T * ptr) : ptr_(ptr)
  {
    ptr = nullptr;
  }

  UniquePtr(const UniquePtr&) = delete;

  UniquePtr(UniquePtr&& other)
  {
    if (this != &other)
    {
      ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }
  }

  UniquePtr& operator = (const UniquePtr&) = delete;

  UniquePtr& operator = (UniquePtr&& other)
  {
    if (this != &other)
    {
      delete ptr_;
      ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }
    return *this;
  }

  UniquePtr& operator = (nullptr_t)
  {
    delete ptr_;
    ptr_ = nullptr;
    return *this;
  }

  ~UniquePtr()
  {
    delete ptr_;
  }

  T& operator * () const
  {
    return *ptr_;
  }

  T * operator -> () const
  {
    return Get();
  }

  T * Release()
  {
    T *temp = ptr_;
    ptr_ = nullptr;
    return temp;
  }

  void Reset(T * ptr)
  {
    if (ptr != ptr_)
    {
      delete ptr_;
      ptr_ = ptr;
      ptr = nullptr;
    } 
  }

  void Swap(UniquePtr& other)
  {
    if (other.Get() != ptr_)
    {
      delete ptr_;
      ptr_ = other.ptr_;
      other.ptr_ = nullptr;
    }
  }

  T * Get() const
  {
    return ptr_;
  }

private:
  T *ptr_ = nullptr;
};


struct Item {
  static int counter;
  int value;
  Item(int v = 0): value(v) {
    ++counter;
  }
  Item(const Item& other): value(other.value) {
    ++counter;
  }
  ~Item() {
    --counter;
  }
};

int Item::counter = 0;


void TestLifetime() {
  Item::counter = 0;
  {
    UniquePtr<Item> ptr(new Item);
    ASSERT_EQUAL(Item::counter, 1);

    ptr.Reset(new Item);
    ASSERT_EQUAL(Item::counter, 1);
  }
  ASSERT_EQUAL(Item::counter, 0);

  {
    UniquePtr<Item> ptr(new Item);
    ASSERT_EQUAL(Item::counter, 1);

    auto rawPtr = ptr.Release();
    ASSERT_EQUAL(Item::counter, 1);

    delete rawPtr;
    ASSERT_EQUAL(Item::counter, 0);
  }
  ASSERT_EQUAL(Item::counter, 0);
}

void TestGetters() {
  UniquePtr<Item> ptr(new Item(42));
  ASSERT_EQUAL(ptr.Get()->value, 42);
  ASSERT_EQUAL((*ptr).value, 42);
  ASSERT_EQUAL(ptr->value, 42);
}

void TestSwap() {
  UniquePtr<Item> ptr1(new Item(42));
  UniquePtr<Item> ptr2(new Item(43));
  ptr1.Swap(ptr2);
  ASSERT_EQUAL(ptr1->value, 43);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestLifetime);
  RUN_TEST(tr, TestGetters);
  RUN_TEST(tr, TestSwap);
}
