// Yi Zong Kuang
// Double Probing Class

#ifndef DOUBLE_PROBING_H
#define DOUBLE_PROBING_H

#include <vector>
#include <algorithm>
#include <functional>
#include <iostream>
#include "PrimeCalculate.h"

// Quadratic probing implementation.
template <typename HashedObj>
class HashTableDouble {
 public:
  enum EntryType {ACTIVE, EMPTY, DELETED};

  explicit HashTableDouble(size_t size = 101) : array_(NextPrime(size)), total_collision_(0)
    { MakeEmpty(); }

  bool Contains(const HashedObj & x) const {
    return IsActive(FindPos(x));
  }

  bool Contains(const HashedObj & x, unsigned int & probes) const {
    return IsActive(FindPos(x, probes));
  }
  
  void MakeEmpty() {
    current_size_ = 0;
    total_collision_ = 0;
    for (auto &entry : array_)
      entry.info_ = EMPTY;
  }

  bool Insert(const HashedObj & x) {
    // Insert x as active
    size_t current_pos = FindPos(x, total_collision_);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos].element_ = x;
    array_[current_pos].info_ = ACTIVE;
    
    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();    
    return true;
  }
    
  bool Insert(HashedObj && x) {
    // Insert x as active
    size_t current_pos = FindPos(x, total_collision_);
    if (IsActive(current_pos))
      return false;
    
    array_[current_pos] = std::move(x);
    array_[current_pos].info_ = ACTIVE;

    // Rehash; see Section 5.5
    if (++current_size_ > array_.size() / 2)
      Rehash();

    return true;
  }

  bool Remove(const HashedObj & x) {
    size_t current_pos = FindPos(x);
    if (!IsActive(current_pos))
      return false;

    array_[current_pos].info_ = DELETED;
    return true;
  }

   // My own fct.
  const size_t & size() {
    return current_size_;
  }
 
  const size_t arraySize() {
    return array_.capacity();
  }

  const unsigned int & collision() {
    return total_collision_;
  }

 private:        
  struct HashEntry {
    HashedObj element_;
    EntryType info_;
    
    HashEntry(const HashedObj& e = HashedObj{}, EntryType i = EMPTY)
    :element_{e}, info_{i} { }
    
    HashEntry(HashedObj && e, EntryType i = EMPTY)
    :element_{std::move(e)}, info_{ i } {}
  };
    

  std::vector<HashEntry> array_;
  size_t current_size_;
   // My own private variable.
  unsigned int total_collision_;
  size_t second_hash_prime_r_ = 83;
   // End of My own private variable.

  bool IsActive(size_t current_pos) const
  { return array_[current_pos].info_ == ACTIVE; }

  size_t FindPos(const HashedObj & x) const {
    //size_t offset = 1;
    size_t current_pos = InternalHash(x);
    size_t second_hash_offset = secondHash( x );
      
    while (array_[current_pos].info_ != EMPTY &&
	   array_[current_pos].element_ != x) {
      //current_pos += offset;  // Compute ith probe.
      //offset += 2;
      current_pos += second_hash_offset;
      if (current_pos >= array_.size())
	current_pos -= array_.size();
    }
    return current_pos;
  }

   // Only used by insert(), keeps track of number of collision.
  size_t FindPos(const HashedObj & x, unsigned int & total_collision_) const {
    //size_t offset = 1;
    size_t current_pos = InternalHash(x);
    size_t second_hash_offset = secondHash( x );
      
    while (array_[current_pos].info_ != EMPTY &&
	   array_[current_pos].element_ != x) {
      //current_pos += offset;  // Compute ith probe.
      //offset += 2;
      ++total_collision_;
      current_pos += second_hash_offset;
      if (current_pos >= array_.size())
	current_pos -= array_.size();
    }
    return current_pos;
  }

  void Rehash() {
    std::vector<HashEntry> old_array = array_;

    // Create new double-sized, empty table.
    array_.resize(NextPrime(2 * old_array.size()));
    for (auto & entry : array_)
      entry.info_ = EMPTY;
    
    // Copy table over.
    current_size_ = 0;
    //total_collision_ = 0;
    for (auto & entry :old_array)
      if (entry.info_ == ACTIVE)
	Insert(std::move(entry.element_));
  }
  
  size_t InternalHash(const HashedObj & x) const {
    static std::hash<HashedObj> hf;
    return hf(x) % array_.size( );
  }
  
  // Second Hash fct.
  size_t secondHash( HashedObj x ) const {
    static std::hash<HashedObj> hf;
    size_t first_hash = hf( x );
    
    return ( second_hash_prime_r_ - ( first_hash % second_hash_prime_r_ ) );
  }

};

#endif  // QUADRATIC_PROBING_H
