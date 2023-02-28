#include "../../utils/test_runner.h"

#include <iostream>
#include <functional>
#include <string>
using namespace std;

template <typename T>
class LazyValue {
public:
  explicit LazyValue(std::function<T()> init) :
  init_(move(init)) {}

  bool HasValue() const {
    return object_.has_value();
  }
  
  const T& Get() const {
    if (lock_guard g(m); !HasValue()) {
      object_ = init_();
    }

    return *object_;
  }

private:
  std::function<T()> init_;
  mutable std::optional<T> object_;
  mutable mutex m;
};

void UseExample() {
  const string big_string = "Giant amounts of memory";

  LazyValue<string> lazy_string([&big_string] { return big_string; });

  ASSERT(!lazy_string.HasValue());
  ASSERT_EQUAL(lazy_string.Get(), big_string);
  ASSERT_EQUAL(lazy_string.Get(), big_string);
}

void TestInitializerIsntCalled() {
  bool called = false;

  {
    LazyValue<int> lazy_int([&called] {
      called = true;
      return 0;
    });
  }
  ASSERT(!called);
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, UseExample);
  RUN_TEST(tr, TestInitializerIsntCalled);
  return 0;
}
