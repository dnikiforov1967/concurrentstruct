/**

Author D.Nikiforov, 12.2021

*/

#ifndef _CONCURRENT_MAP_
#define _CONCURRENT_MAP_

#include <set>
#include <vector>
#include <shared_mutex>

namespace util {

	template <typename _Key, typename _Compare = std::less<_Key>> class map_node {
		std::shared_mutex mutex_;
		std::set<_Key, _Compare> storage_;
		public:
			map_node();
			map_node(const map_node&) = delete;
			map_node& operator=(const map_node&) = delete;
			const _Key* put(const _Key&);
			const _Key* get(const _Key&);
	};

	template <typename _Key, typename _Compare> map_node<_Key, _Compare>::map_node() {
	}


	template <typename _Key, typename _Compare> const _Key* map_node< _Key, _Compare>::put(const _Key& key) {
		const std::lock_guard<std::shared_mutex> lock(mutex_);
		storage_.emplace(key);
		auto found = storage_.find(key);
		return &(*found);
	}

	
	template <typename _Key, typename _Compare> const _Key* map_node< _Key, _Compare>::get(const _Key& key) {
		const std::shared_lock<std::shared_mutex> lock(this->mutex_);
		const _Key* ptr = nullptr;
		if (auto found = storage_.find(key); found != storage_.end())
			ptr = &(*found);
		return ptr;
	}

	template <
		typename _Key, 
		typename _Compare = std::less<_Key>,
		typename _Hash = std::hash<_Key>
	>
	class concurrent_hash_map {
		std::size_t hash_buckets;
		std::vector<std::unique_ptr<map_node<_Key, _Compare>>> vector_;
		void initialize();
	public:
		concurrent_hash_map();
		concurrent_hash_map(std::size_t hash_buckets);
		concurrent_hash_map(const concurrent_hash_map&) = delete;
		concurrent_hash_map& operator=(const concurrent_hash_map&) = delete;
		const _Key* put(const _Key&);
		const _Key* get(const _Key&);
	};

	template <
		typename _Key,
		typename _Compare,
		typename _Hash
	>
	void concurrent_hash_map<_Key, _Compare, _Hash>::initialize() {
		map_node<_Key, _Compare> *node_ = nullptr;
		for (auto i = 0; i < hash_buckets; i++) {
			node_ = new map_node<_Key, _Compare>();
			auto uptr_ = std::unique_ptr<map_node<_Key, _Compare>>(node_);
			vector_.push_back(std::move(uptr_));
		}
	}

	template <
		typename _Key,
		typename _Compare,
		typename _Hash
	>
	concurrent_hash_map<_Key, _Compare, _Hash>::concurrent_hash_map() : hash_buckets{ 4 } {
		initialize();
	}

	template <
		typename _Key,
		typename _Compare,
		typename _Hash
	>
	concurrent_hash_map<_Key, _Compare, _Hash>::concurrent_hash_map(std::size_t hash_buckets) : hash_buckets{ hash_buckets } {
		initialize();
	}

	template <
		typename _Key,
		typename _Compare,
		typename _Hash
	>
	const _Key* concurrent_hash_map<_Key, _Compare, _Hash>::put(const _Key& key) {
		int backet = _Hash{}(key) % hash_buckets;
		std::unique_ptr<map_node<_Key, _Compare>> &ptr = vector_[std::abs(backet)];
		return ptr.get()->put(key);
	}

	template <
		typename _Key,
		typename _Compare,
		typename _Hash
	>
	const _Key* concurrent_hash_map<_Key, _Compare, _Hash>::get(const _Key& key) {
		int backet = _Hash{}(key) % hash_buckets;
		std::unique_ptr<map_node<_Key, _Compare>>& ptr = vector_[std::abs(backet)];
		return ptr.get()->get(key);
	}

}


#endif