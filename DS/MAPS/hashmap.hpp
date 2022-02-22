#ifndef _MY_HASHMAP
#define _MY_HASHMAP

#include <iostream>
#include <vector>
#include "map.hpp"

template<typename KEY, typename VALUE, typename HashFunc=std::hash<KEY>, size_t Capacity=101>
class HashMap{
	public:

		class HashMapIterator: public std::iterator<std::bidirectional_iterator_tag, TYPE<KEY,VALUE>>{
			public:
				explicit HashMapIterator(HashMap *h, typename std::vector<Map<KEY,VALUE>>::iterator viter);
				HashMapIterator& operator++();
				HashMapIterator& operator--();
				HashMapIterator operator++(int){
					HashMapIterator retval(this->hmap, this->bucket_iter);
					++(*this);
					return retval;
				}
				HashMapIterator operator--(int){
					HashMapIterator retval(this->hmap, this->bucket_iter);
					--(*this);
					return retval;
				}
				bool operator==(HashMapIterator other);
				bool operator!=(HashMapIterator other);
				auto& operator*();
			private:
				HashMap *hmap;
				typename std::vector<Map<KEY,VALUE>>::iterator bucket_iter;
				Iterator<TYPE<KEY,VALUE>> entry_iter;
		};


		HashMap(): n{0}, bucket(Capacity) {}
		~HashMap()=default;

		size_t size() const { return n; }
		bool is_empty() const { return size()==0; }
		HashMapIterator find(const KEY&);
		void erase(const KEY&);
		void erase(HashMapIterator);
		VALUE& operator[](const KEY&);
		HashMapIterator begin() {
			HashMapIterator iter(this, bucket.begin());
			return iter;
		}
		HashMapIterator end() {
			HashMapIterator iter(this, bucket.end());
			return iter;
		}
	private:
		HashFunc hfunc{};
		size_t n;	// numbers of entries
		std::vector<Map<KEY,VALUE>> bucket;
		size_t compress(typename HashFunc::result_type hashvalue) {
			return (hashvalue % Capacity);
		}

};

template<typename KEY, typename VALUE, typename HashFunc, size_t Capacity>
typename HashMap<KEY,VALUE,HashFunc,Capacity>::HashMapIterator HashMap<KEY,VALUE,HashFunc,Capacity>::find(const KEY& k)
{
	for(auto it=begin(); it!=end(); ++it) {
		if((*it).first == k)
			return it;
	}
	return end();
}

template<typename KEY, typename VALUE, typename HashFunc, size_t Capacity>
void HashMap<KEY,VALUE,HashFunc,Capacity>::erase(const KEY& k)
{
	auto hashvalue = hfunc(k);
	auto index = compress(hashvalue);
	if(bucket[index].is_empty())
		throw std::runtime_error("erase: key not in storage!!!");
	bucket[index].erase(k);
}

template<typename KEY, typename VALUE, typename HashFunc, size_t Capacity>
void HashMap<KEY,VALUE,HashFunc,Capacity>::erase(typename HashMap<KEY,VALUE,HashFunc,Capacity>::HashMapIterator p)
{
	auto iter = find((*p).first);
	if(iter == end()) throw std::runtime_error("Iterator not referencing any member!!!");
	erase((*p).first);
}

template<typename KEY, typename VALUE, typename HashFunc, size_t Capacity>
VALUE& HashMap<KEY,VALUE,HashFunc,Capacity>::operator[](const KEY& k)
{
	VALUE v{};
	auto hashvalue = hfunc(k);
	auto index = compress(hashvalue);
	if(bucket[index].is_empty()){
		bucket[index].put(k,v);
		++n;
	}
	else{
		if (bucket[index].find(k) == bucket[index].end()){
			bucket[index].put(k,v);
			++n;
		}
	}
	return (*(bucket[index].find(k))).second;
}

template<typename KEY, typename VALUE, typename HashFunc, size_t Capacity>
HashMap<KEY,VALUE,HashFunc,Capacity>::HashMapIterator::HashMapIterator(HashMap<KEY,VALUE,HashFunc,Capacity> *hm, typename std::vector<Map<KEY,VALUE>>::iterator viter)
	: hmap{hm}, bucket_iter{viter}, entry_iter{(*bucket_iter).begin()}
{
	// finds the first non-empty entry and returns an iterator to the
	// map in that entry.
	if(bucket_iter == (hmap->bucket).begin()){
		while(bucket_iter != (hmap->bucket).end()){
			if(!(*bucket_iter).is_empty()){
				entry_iter = (*bucket_iter).begin();
				break;
			}
			++bucket_iter;
		}
		if(bucket_iter == (hmap->bucket).end()){
			--bucket_iter;
			entry_iter = (*bucket_iter).end();
		}
	}
	else if (bucket_iter == (hmap->bucket).end()) {
		--bucket_iter;
		entry_iter = (*bucket_iter).end();
	}
}

template<typename KEY, typename VALUE, typename HashFunc, size_t Capacity>
typename HashMap<KEY,VALUE,HashFunc,Capacity>::HashMapIterator& HashMap<KEY,VALUE,HashFunc,Capacity>::HashMapIterator::operator++()
{
	// raise an exception once the end of bucket is reached without
	// finding a non-empty entry.
	if(bucket_iter == (hmap->bucket).end()){
		throw std::runtime_error("Iterator: operation on end iterator is undefined!!!");
	}

	// checks if the entry iterator has reach its end, if true, the
	// program then searches for the next non-empty entry and then
	// make the entry iterator point to the beginning of the map at
	// that entry.
	if(++entry_iter == (*bucket_iter).end()){
		if ((bucket_iter+1) == (hmap->bucket).end()) return *this;
		while(++bucket_iter != (hmap->bucket).end()){
			if(!(*bucket_iter).is_empty()){
				break;
			}
		}
		entry_iter = (*bucket_iter).begin();
		return *this;
	}
	return *this;
}

template<typename KEY, typename VALUE, typename HashFunc, size_t Capacity>
typename HashMap<KEY,VALUE,HashFunc,Capacity>::HashMapIterator& HashMap<KEY,VALUE,HashFunc,Capacity>::HashMapIterator::operator--()
{
	// raise an exception once the begining of bucket is reached without
	// finding a non-empty entry.
	if(bucket_iter == (hmap->bucket).begin()){
		throw std::runtime_error("Iterator: operation before iterator is undefined!!!");
	}

	// checks if the entry iterator has reach its beginning, if true, the
	// program then searches for the previous non-empty entry and then
	// make the entry iterator point to the beginning of the map at
	// that entry.
	if(--entry_iter == --((*bucket_iter).begin())){
		while(--bucket_iter != (hmap->bucket).begin()){
			if(!(*bucket_iter).is_empty()){
				break;
			}
		}
		entry_iter = --(*bucket_iter).end();
		return *this;
	}
	return *this;
}

template<typename KEY, typename VALUE, typename HashFunc, size_t Capacity>
bool HashMap<KEY,VALUE,HashFunc,Capacity>::HashMapIterator::operator==(typename HashMap<KEY,VALUE,HashFunc,Capacity>::HashMapIterator other)
{
	return ((this->bucket_iter == other.bucket_iter) && (this->entry_iter == other.entry_iter));
}

template<typename KEY, typename VALUE, typename HashFunc, size_t Capacity>
bool HashMap<KEY,VALUE,HashFunc,Capacity>::HashMapIterator::operator!=(typename HashMap<KEY,VALUE,HashFunc,Capacity>::HashMapIterator other)
{
	return (!(*this == other));
}

template<typename KEY, typename VALUE, typename HashFunc, size_t Capacity>
auto& HashMap<KEY,VALUE,HashFunc,Capacity>::HashMapIterator::operator*()
{
	return *entry_iter;
}
#endif //_MY_HASHMAP
