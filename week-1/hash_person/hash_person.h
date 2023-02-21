#include "test_runner.h"
#include <limits>
#include <random>
#include <unordered_set>

using namespace std;

struct Address {
  string city, street;
  int building;

  bool operator==(const Address& other) const {
    return city == other.city && street == other.street && building == other.building;
  }
};

struct Person {
  string name;
  int height;
  double weight;
  Address address;

  bool operator==(const Person& other) const {
    return name == other.name &&
        height == other.height &&
        weight == other.weight &&
        address == other.address;
  }
};

struct AddressHasher {
  size_t operator() (const Address& address) const
  {
    size_t A = shash(address.city);
    size_t B = shash(address.street);
    size_t C = ihash(address.building);

    size_t X = 1019;

    return A*X*X + B*X + C;
  }

  hash<string> shash;
  hash<int> ihash;
};

struct PersonHasher {
  size_t operator() (const Person& person) const
  {
    size_t A = shash(person.name);
    size_t B = ihash(person.height);
    size_t C = dhash(person.weight);
    size_t D = shash(person.address.city);
    size_t E = shash(person.address.street);
    size_t F = ihash(person.address.building);

    size_t X = 2137;

    return A*X*X*X*X*X +
            B*X*X*X*X +
            C*X*X*X +
            D*X*X +
            E*X +
            F;
  }

  hash<string> shash;
  hash<int> ihash;
  hash<double> dhash;
};