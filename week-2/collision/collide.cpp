//#include "geo2d.h"
#include "geo2d.cpp"
#include "game_object.h"

#include "../../utils/test_runner.h"

#include <vector>
#include <memory>

using namespace std;

class Unit : public GameObject {
public:
  explicit Unit(geo2d::Point position) :
  position_(position) {}

  bool Collide(const GameObject& that) const override;
  bool CollideWith(const Unit& that) const override;
  bool CollideWith(const Building& that) const override;
  bool CollideWith(const Tower& that) const override;
  bool CollideWith(const Fence& that) const override;

  geo2d::Point getPosition() const { return position_; }

private:
  geo2d::Point position_;
};

class Building : public GameObject {
public:
  explicit Building(geo2d::Rectangle geometry) :
  position_(geometry) {}

  bool Collide(const GameObject& that) const override;
  bool CollideWith(const Unit& that) const override;
  bool CollideWith(const Building& that) const override;
  bool CollideWith(const Tower& that) const override;
  bool CollideWith(const Fence& that) const override;

  geo2d::Rectangle getPosition() const { return position_; }

private:
  geo2d::Rectangle position_;
};

class Tower : public GameObject {
public:
  explicit Tower(geo2d::Circle geometry) : 
  position_(geometry) {}

  bool Collide(const GameObject& that) const override;
  bool CollideWith(const Unit& that) const override;
  bool CollideWith(const Building& that) const override;
  bool CollideWith(const Tower& that) const override;
  bool CollideWith(const Fence& that) const override;

  geo2d::Circle getPosition() const { return position_; }

private:
  geo2d::Circle position_;
};

class Fence : public GameObject {
public:
  explicit Fence(geo2d::Segment geometry) :
  position_(geometry) {}

  bool Collide(const GameObject& that) const override;
  bool CollideWith(const Unit& that) const override;
  bool CollideWith(const Building& that) const override;
  bool CollideWith(const Tower& that) const override;
  bool CollideWith(const Fence& that) const override;

  geo2d::Segment getPosition() const { return position_; }

private:
  geo2d::Segment position_;
};

bool Unit::Collide(const GameObject& that) const
{
  return that.CollideWith(*this);
}

bool Unit::CollideWith(const Unit& that) const
{
  return geo2d::Collide(that.getPosition(), position_);
}

bool Unit::CollideWith(const Building& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Unit::CollideWith(const Tower& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Unit::CollideWith(const Fence& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Building::Collide(const GameObject& that) const
{
  return that.CollideWith(*this);
}

bool Building::CollideWith(const Unit& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Building::CollideWith(const Building& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Building::CollideWith(const Tower& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Building::CollideWith(const Fence& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Tower::Collide(const GameObject& that) const
{
  return that.CollideWith(*this);
}

bool Tower::CollideWith(const Unit& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Tower::CollideWith(const Building& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Tower::CollideWith(const Tower& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Tower::CollideWith(const Fence& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Fence::Collide(const GameObject& that) const
{
  return that.CollideWith(*this);
}

bool Fence::CollideWith(const Unit& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Fence::CollideWith(const Building& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Fence::CollideWith(const Tower& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Fence::CollideWith(const Fence& that) const
{
  return geo2d::Collide(position_, that.getPosition());
}

bool Collide(const GameObject& first, const GameObject& second) {
  return first.Collide(second);
}

void TestAddingNewObjectOnMap() {
  using namespace geo2d;

  const vector<shared_ptr<GameObject>> game_map = {
    make_shared<Unit>(Point{3, 3}),
    make_shared<Unit>(Point{5, 5}),
    make_shared<Unit>(Point{3, 7}),
    make_shared<Fence>(Segment{{7, 3}, {9, 8}}),
    make_shared<Tower>(Circle{Point{9, 4}, 1}),
    make_shared<Tower>(Circle{Point{10, 7}, 1}),
    make_shared<Building>(Rectangle{{11, 4}, {14, 6}})
  };

  for (size_t i = 0; i < game_map.size(); ++i) {
    Assert(
      Collide(*game_map[i], *game_map[i]),
      "An object doesn't collide with itself: " + to_string(i)
    );

    for (size_t j = 0; j < i; ++j) {
      Assert(
        !Collide(*game_map[i], *game_map[j]),
        "Unexpected collision found " + to_string(i) + ' ' + to_string(j)
      );
    }
  }

  auto new_warehouse = make_shared<Building>(Rectangle{{4, 3}, {9, 6}});
  ASSERT(!Collide(*new_warehouse, *game_map[0]));
  ASSERT( Collide(*new_warehouse, *game_map[1]));
  ASSERT(!Collide(*new_warehouse, *game_map[2]));
  ASSERT( Collide(*new_warehouse, *game_map[3]));
  ASSERT( Collide(*new_warehouse, *game_map[4]));
  ASSERT(!Collide(*new_warehouse, *game_map[5]));
  ASSERT(!Collide(*new_warehouse, *game_map[6]));

  auto new_defense_tower = make_shared<Tower>(Circle{{8, 2}, 2});
  ASSERT(!Collide(*new_defense_tower, *game_map[0]));
  ASSERT(!Collide(*new_defense_tower, *game_map[1]));
  ASSERT(!Collide(*new_defense_tower, *game_map[2]));
  ASSERT( Collide(*new_defense_tower, *game_map[3]));
  ASSERT( Collide(*new_defense_tower, *game_map[4]));
  ASSERT(!Collide(*new_defense_tower, *game_map[5]));
  ASSERT(!Collide(*new_defense_tower, *game_map[6]));
}

int main() {
  TestRunner tr;
  RUN_TEST(tr, TestAddingNewObjectOnMap);
  return 0;
}