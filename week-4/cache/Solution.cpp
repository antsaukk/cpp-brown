#include "Common.h"

#include <unordered_set>
#include <unordered_map>
#include <map>
#include <climits>
#include <memory>
#include <mutex>

#include <iostream>

using namespace std;

// cache structure that stores books
struct BookKeeper
{
  using BookPtr = std::shared_ptr<const IBook>;

  void IncreaseRating(const std::string& book_name)
  {
    books_ordered_.erase(books_ratings_[book_name]);
    books_ratings_[book_name] = ++rating_;
    books_ordered_[books_ratings_[book_name]] = book_name;
  }

  void AddBook(const std::string& book_name, std::unique_ptr<IBook>& bookToAdd)
  {
    books_[book_name] = move(bookToAdd);
    books_ratings_[book_name] = ++rating_;
    books_ordered_[books_ratings_[book_name]] = book_name;

    memory_used_by_books_ += books_[book_name]->GetContent().size();
  }

  void DeleteBook(const std::string& book_name)
  {
    memory_used_by_books_ -= books_[book_name]->GetContent().size();

    books_.erase(book_name);
    books_ordered_.erase(books_ratings_[book_name]);
    books_ratings_.erase(book_name);
  }

  void DeleteBook()
  {
    const auto book_name = books_ordered_.begin()->second;
    memory_used_by_books_ -= books_[book_name]->GetContent().size();
    books_.erase(book_name);
    books_ordered_.erase(books_ratings_[book_name]);
    books_ratings_.erase(book_name);
  }

  const bool IsKeeperEmpty() const
  {
    return books_.empty() || books_ordered_.empty() || books_ratings_.empty();
  }

  const bool IsBookInCache(const std::string& book_name)
  {
    return books_.count(book_name);
  }

  const size_t GetMemoryUsedByBooks() const
  {
    return memory_used_by_books_;
  }

  BookPtr GetBook(const string& book_name)
  {
    return books_[book_name];
  }

private:
  unordered_map<string, BookPtr> books_;
  unordered_map<string, int> books_ratings_;
  map<int, string> books_ordered_;
  size_t rating_ = 0;
  size_t memory_used_by_books_ = 0;
};

class LruCache : public ICache {
public:
  LruCache(
      shared_ptr<IBooksUnpacker> books_unpacker,
      const Settings& settings
  ) :
    books_unpacker_(move(books_unpacker)),
    settings_(settings) {}

  BookPtr GetBook(const string& book_name) override
  {
    // if book with requested name is in cache, return it
    // otherwise fetch book from books_unpacker
    
    lock_guard<mutex> lock(mut_);

    if (book_keeper_.IsBookInCache(book_name))
    {
      book_keeper_.IncreaseRating(book_name);
    }
    else
    {
      // book is not in cache fetch from Unpacker
      std::unique_ptr<IBook> requestedBook = books_unpacker_->UnpackBook(book_name);

      // perform LRU selection first
      while(book_keeper_.GetMemoryUsedByBooks() + requestedBook->GetContent().size() > settings_.max_memory &&
        !book_keeper_.IsKeeperEmpty())
      {
        book_keeper_.DeleteBook();
      }

      if (book_keeper_.GetMemoryUsedByBooks() + requestedBook->GetContent().size() <= settings_.max_memory)
      {
        book_keeper_.AddBook(
        	book_name,
        	requestedBook
        );
      }
    }
    
    return book_keeper_.GetBook(book_name);
  }

private:
  // to be used to track memory used by the books
  // unpack the book that is not in cache
  shared_ptr<IBooksUnpacker> books_unpacker_;

  // to be used to track max available memory
  Settings settings_;

  // cache structure that holds books
  BookKeeper book_keeper_;
  
  mutable mutex mut_;
};


unique_ptr<ICache> MakeCache(
    shared_ptr<IBooksUnpacker> books_unpacker,
    const ICache::Settings& settings
) {
  return std::make_unique<LruCache>(books_unpacker, settings);;
}
