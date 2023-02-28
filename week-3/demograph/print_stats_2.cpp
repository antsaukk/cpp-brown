#include <algorithm>
#include <iostream>
#include <vector>
#include <string>
#include <numeric>
#include <unordered_map>

using namespace std;

template <typename Iterator>
class IteratorRange {
public:
  IteratorRange(Iterator begin, Iterator end)
    : first(begin)
    , last(end)
  {
  }

  Iterator begin() const {
    return first;
  }

  Iterator end() const {
    return last;
  }

private:
  Iterator first, last;
};

template <typename Collection>
auto Head(Collection& v, size_t top) {
  return IteratorRange{v.begin(), next(v.begin(), min(top, v.size()))};
}

struct Person {
  string name;
  int age, income;
  bool is_male;
};

void DisplayState(const vector<Person>& people) {
  for (const auto& p : people) {
    cout << p.name << " " << p.age << " " << p.income << " " << p.is_male << "\n"; 
  }
}

vector<Person> ReadPeople(
  istream& input) {
  int count;
  input >> count;

  vector<Person> result(count);
  for (Person& p : result) {
    char gender;
    input >> p.name >> p.age >> p.income >> gender;
    p.is_male = gender == 'M';
  }

  return result;
}

void ProcessAgeCommand(const vector<Person>& people) {
  int adult_age;
  cin >> adult_age;

  auto adult_begin = lower_bound(
    begin(people), end(people), adult_age, [](const Person& lhs, int age) {
      return lhs.age < age;
    }
  );

  cout << "There are " << std::distance(adult_begin, end(people))
    << " adult people for maturity age " << adult_age << '\n';
}

void ProcessWealthyCommand(vector<int>& earnings) {
  int count;
  cin >> count;

  cout << "Top-" << count << " people have total income " << earnings[count - 1] << '\n';
}

string ComputePopularName(
  vector<Person>& people, string gender) {

  IteratorRange range{begin(people),
    partition(begin(people), end(people), [gender](Person& p) {
      return p.is_male == (gender == "M");
    })
  };

  if (range.begin() == range.end()) {
    return "No people of gender " + gender + "\n";
  } else {
    sort(range.begin(), range.end(), [](const Person& lhs, const Person& rhs) {
      return lhs.name < rhs.name;
    });
    const string* most_popular_name = &range.begin()->name;
    int count = 1;
    
    for (auto i = range.begin(); i != range.end(); ) {
      auto same_name_end = find_if_not(i, range.end(), [i](const Person& p) {
        return p.name == i->name;
      });
      auto cur_name_count = std::distance(i, same_name_end);
      
      if (cur_name_count > count) {
        count = cur_name_count;
        most_popular_name = &i->name;
      }

      i = same_name_end;
    }
        
    return "Most popular name among people of gender " + gender + " is " + *most_popular_name + "\n";
  }

}

void ComputeEarnings(
  vector<Person>& people,
  vector<int>& earnings) {
  sort(begin(people), end(people), [](const Person& lhs, const Person& rhs) {
    return lhs.income > rhs.income;
  });

  earnings[0] = people[0].income;
  for (size_t i = 1; i < earnings.size(); i++) {
    earnings[i] = earnings[i-1] + people[i].income;
  }
}

int main() {
  vector<Person> people = ReadPeople(cin);

  const string maleNameResponse = ComputePopularName(people, "M");
  const string femaleNameResponse = ComputePopularName(people, "W");

  vector<int> earnings(people.size());

  ComputeEarnings(people, earnings);

  sort(begin(people), end(people), [](const Person& lhs, const Person& rhs) {
    return lhs.age < rhs.age;
  });

  string gender;
  for (string command; cin >> command; ) {

    if (command == "AGE") {
      ProcessAgeCommand(people);
    } else if (command == "WEALTHY") {
      ProcessWealthyCommand(earnings);
    } else if (command == "POPULAR_NAME") {
      cin >> gender;
      if (gender == "M")
        cout << maleNameResponse;
      else
        cout << femaleNameResponse;
    }

  }
}
