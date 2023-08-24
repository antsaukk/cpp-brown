#include "Common.h"

#include <unordered_set>
#include <unordered_map>

using namespace std;

// cache structure that stores books
struct BookKeeper
{
  unordered_set<string> books_;
  unordered_map<string, int> books_ratings_;
  size_t rating_ = 0;
  size_t memory_used_by_books_ = 0;

  void IncreaseAndAssign(const std::string& book_name)
  {
    books_ratings_[book_name] = ++rating_;
  }

  const size_t GetMemoryUsedByBooks() const
  {
    return memory_used_by_books_;
  }

  void AddBook(const std::string& book_name, BookPtr bookToAdd)
  {
    books_.insert(book_name);
    IncreaseAndAssign(book_name);
    memory_used_by_books_ += books_[book_name].size();
  }

  void DeleteBook(const std::string& book_name)
  {
    memory_used_by_books_ -= books_[book_name].size();
    books_.erase(book_name);
    books_ratings_.erase(book_name);
  }

  const bool IsKeeperEmpty() const
  {
    return books_.empty();
  }
};

class LruCache : public ICache {
public:
  LruCache(
      shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  ) {
    books_unpacker_ = books_unpacker;
    settings_ = settings;
  }

  BookPtr GetBook(const string& book_name) override
  {
    // if book with requested name is in cache, return it
    // otherwise fetch book from books_unpacker

    if (book_keeper_.books_.find(book_name) != book_keeper_.books_.end())
    {
      book_keeper_.IncreaseAndAssign(book_name);
      //return book_keeper_.books_[book_name];
    }
    else
    {
      // book is not in cache fetch from Unpacker
      auto requestedBook = books_unpacker_->UnpackBook(book_name);

      // perform LRU selection first
      while(book_keeper_.GetMemoryUsedByBooks() > settings_.max_memory ||
        !book_keeper_.IsKeeperEmpty())
      {
        int lru = INT_MAX;
        string candidateToDelete = "";

        for (auto it=book_keeper_.books_ratings_.begin();
          it!=book_keeper_.books_ratings_.end(); it++)
        {
          if (it->second < lru)
          {
            lru = it->second;
            candidateToDelete = it->first;
          }
        }
     
        // Remove the book_ 
        book_keeper_.DeleteBook(book_name);
      }

      if (book_keeper_.GetMemoryUsedByBooks() < settings_.max_memory)
      { 
        // typecast pointer
        book_keeper_.AddBook(book_name, /*second argument typecasted ppointer*/);
      }

      return requestedBook;
    }
  }

private:
  // to be used to track memory used by the books
  // unpack the book that is not in cache
  shared_ptr<IBooksUnpacker> books_unpacker_;

  // to be used to track max available memory
  Settings settings_;

  // cache structure that holds books
  BookKeeper book_keeper_;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  // реализуйте функцию

  std::unique_ptr<ICache> cache = std::make_unique<LruCache>(books_unpacker, settings);
  return cache;
}
