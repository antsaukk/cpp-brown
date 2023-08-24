#include "Common.h"

#include <unordered_map>

using namespace std;

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

    if (book_keeper_.books_.count(book_name))
    {
      book_keeper_.books_ratings_[book_name] = book_keeper_.increaseAndAssign();
      return book_keeper_.books_[book_name];
    }
    else
    {
      // book is not in cache fetch from Unpacker
      auto requestedBook = books_unpacker_->UnpackBook(book_name);

      // perform LRU selection first
      while(books_unpacker_->GetMemoryUsedByBooks() > settings_.max_memory ||
        !book_keeper_.books_.empty())
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
     
        // Remove the book_keeper_.books_ratings_ page
        book_keeper_.books_.erase(candidateToDelete);
        book_keeper_.books_ratings_.erase(candidateToDelete);
      }

      if (books_unpacker_->GetMemoryUsedByBooks() < settings_.max_memory)
      {
        book_keeper_.books_[book_name] = requestedBook;
        book_keeper_.books_ratings_[book_name] = book_keeper_.increaseAndAssign();
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

  // cache structure that stores books
  struct BookKeeper {
    unordered_map<string, BookPtr> books_;
    unordered_map<string, int> books_ratings_;
    size_t rating_ = 0;

    size_t increaseAndAssign()
    {
      return ++rating_;
    }
  };

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
