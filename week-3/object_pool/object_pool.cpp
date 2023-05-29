#include "../../utils/test_runner.h"

#include <algorithm>
#include <iostream>
#include <string>
#include <queue>
#include <stdexcept>
#include <set>
using namespace std;

// this is erroneous implementation
// and it is done with intention
template <class T>
class ObjectPool {
public:
  T* Allocate() {
    if (freed.empty()) {
        freed.push(new T);
    }
    auto ret = freed.front();
    freed.pop();
    allocated.insert(ret);
    return ret;
  }

  T* TryAllocate() {
    return !freed.empty() ? Allocate() : nullptr;
  }

  void Deallocate(T* object){
    if (allocated.count(object)) {
      freed.push(object);
      allocated.erase(object);
    } else {
      throw invalid_argument("Object does not exist");
    }
  }

  ~ObjectPool() {
    while(!freed.empty()) {
      T* obj = freed.front();
      freed.pop(); 
      delete obj;
    }
    for(auto obj : allocated) {
      delete obj;
    }
  }

  void DisplayAllocated() {
    for(auto obj : allocated) {
      cout << *obj << " ";
    } cout << endl;
  }

private:
  set<T*> allocated; 
  queue<T*> freed;
};

void TestObjectPool() {
  ObjectPool<string> pool;

  auto p1 = pool.Allocate();
  auto p2 = pool.Allocate();
  auto p3 = pool.Allocate();

  *p1 = "first";
  *p2 = "second";
  *p3 = "third";

  pool.Deallocate(p2);

  ASSERT_EQUAL(*pool.Allocate(), "second");

  pool.Deallocate(p3);

  pool.Deallocate(p1);

  ASSERT_EQUAL(*pool.Allocate(), "third");

  ASSERT_EQUAL(*pool.Allocate(), "first");

  pool.Deallocate(p1);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestObjectPool);
  return 0;
}
