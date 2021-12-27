/**

Author D.Nikiforov, 12.2021

*/

#ifndef _CONCURRENT_MAP_
#define _CONCURRENT_MAP_

#include <map>
#include <mutex>
#include <unordered_map>

namespace util {

	template <typename _Key, typename _Tp,
		typename _Compare = std::less<_Key>,
		typename _Allocator = std::allocator<std::pair<const _Key, _Tp>>> class map_node {
		std::mutex mutex_;
		std::map<_Key, _Tp, _Compare, _Allocator> _storage;
		public:
			map_node();
			map_node(const map_node&) = delete;
			map_node& operator=(const map_node&) = delete;
			_Tp& put(const _Key, const _Tp);
			const _Tp* get(const _Key&);
	};

	template <typename _Key, typename _Tp,
		typename _Compare,
		typename _Allocator>
		map_node<_Key, _Tp, _Compare, _Allocator>::map_node() {
	}

	template <typename _Key, typename _Tp,
		typename _Compare,
		typename _Allocator>
		_Tp& map_node<_Key, _Tp, _Compare, _Allocator>::put(const _Key key, const _Tp val) {
		const std::lock_guard<std::mutex> lock(mutex_);
		_storage[key] = val;
		return _storage[key];
	}

	template <typename _Key, typename _Tp,
		typename _Compare,
		typename _Allocator>
		const _Tp* map_node<_Key, _Tp, _Compare, _Allocator>::get(const _Key& key) {
		const std::lock_guard<std::mutex> lock(this->mutex_);
		const _Tp* ptr = nullptr;
		if (auto found = _storage.find(key); found != _storage.end())
			ptr = &(found->second);
		return ptr;
	}

	template <typename _Key, typename _Tp> class concurrent_map {
		int concurrency_level;
		std::unordered_map<int, map_node<_Key, _Tp>> _storege;
	public:
		concurrent_map();
		explicit concurrent_map(int);
	};

	template <typename _Key, typename _Tp>
	concurrent_map<_Key, _Tp>::concurrent_map()
		: concurrency_level{ 4 } {
	}

	template <typename _Key, typename _Tp>
	concurrent_map<_Key, _Tp>::concurrent_map(int level)
		: concurrency_level{ level } {
	}



}


#endif