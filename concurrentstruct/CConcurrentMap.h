/**

Author D.Nikiforov, 12.2021

*/

#ifndef _CONCURRENT_MAP_
#define _CONCURRENT_MAP_

#include <set>
#include <mutex>
#include <unordered_map>

namespace util {

	template <typename _Key, typename _Compare = std::less<_Key>> class map_node {
		std::mutex mutex_;
		std::set<_Key, _Compare> _storage;
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
		const std::lock_guard<std::mutex> lock(mutex_);
		_storage.emplace(key);
		auto found = _storage.find(key);
		return &(*found);
	}

	
	template <typename _Key, typename _Compare> const _Key* map_node< _Key, _Compare>::get(const _Key& key) {
		const std::lock_guard<std::mutex> lock(this->mutex_);
		const _Key* ptr = nullptr;
		if (auto found = _storage.find(key); found != _storage.end())
			ptr = &(*found);
		return ptr;
	}

}


#endif