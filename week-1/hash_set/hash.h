#include <forward_list>
#include <vector>
#include <iterator>
#include <algorithm>

using namespace std;

template <typename Type, typename Hasher = hash<Type>>
class HashSet {
public:
  using BucketList = forward_list<Type>;

public:
  explicit HashSet(size_t num_buckets, const Hasher& hasher = {}) : 
  hash_set_(num_buckets),
  hasher_(hasher)
  {}

  void Add(const Type& value)
  {
    const auto& bucket = hash_set_[getIndex(value)];
    auto checkPresence = find(begin(bucket), end(bucket), value);

    if (checkPresence == bucket.end())
    {
      hash_set_[getIndex(value)].push_front(value);
    }
  }

  bool Has(const Type& value) const
  {
    const auto& bucket = hash_set_[getIndex(value)];
    return find(begin(bucket), end(bucket), value) != bucket.end();
  }

  void Erase(const Type& value)
  {
    hash_set_[getIndex(value)].remove(value);
  }

  const BucketList& GetBucket(const Type& value) const
  {
    return hash_set_[getIndex(value)];
  }

private:

  size_t getIndex(const Type& value) const
  {
    return hasher_(value) % hash_set_.size();
  }

  vector<BucketList> hash_set_;
  Hasher hasher_;
};
