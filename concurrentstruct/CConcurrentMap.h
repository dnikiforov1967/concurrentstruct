/**

Author D.Nikiforov, 12.2021

*/

#ifndef _CONCURRENT_MAP_
#define _CONCURRENT_MAP_

#include <map>
#include <mutex>

template <typename _Key, typename _Tp> class CMapNode {
	std::mutex mutex_;
	std::map<_Key, _Tp> _storage;
public:
	CMapNode();
	CMapNode(const  CMapNode&) = delete;
	CMapNode& operator=(const CMapNode&)=delete;
	_Tp& put(const _Key, const _Tp);
	const _Tp* get(const _Key&);
};

template <typename _Key, typename _Tp> CMapNode<_Key, _Tp>::CMapNode() {
}

template <typename _Key, typename _Tp> 
_Tp& CMapNode<_Key, _Tp>::put(const _Key key, const _Tp val) {
	const std::lock_guard<std::mutex> lock(mutex_);
	_storage[key] = val;
	return _storage[key];
}

template <typename _Key, typename _Tp> 
const _Tp* CMapNode<_Key, _Tp>::get(const _Key& key) {
	const std::lock_guard<std::mutex> lock(this->mutex_);
	const _Tp* ptr;
	if (auto found = _storage.find(key); found != _storage.end())
		ptr = &(found->second);
	return ptr;
}


#endif