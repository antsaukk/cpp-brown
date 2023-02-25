#include <limits>
#include <random>
#include <unordered_set>
#include <functional>

#include "../../utils/test_runner.h"

using namespace std;

using CoordType = int;

struct Point3D {
  CoordType x;
  CoordType y;
  CoordType z;

  bool operator==(const Point3D& other) const {
    return x == other.x && y == other.y && z == other.z;
  }
};

struct Hasher {
  size_t operator() (const Point3D& point) const
  {
    size_t A = CoordHash(point.x);
    size_t B = CoordHash(point.y);
    size_t C = CoordHash(point.z);

    size_t X = 1019;

    return A*X*X + B*X + C;
  }

  hash<CoordType> CoordHash;
};