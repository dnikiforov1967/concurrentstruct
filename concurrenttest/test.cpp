#include "pch.h"
#include "../../concurrentstruct/concurrentstruct/CConcurrentMap.h"
#include "CProbeObject.h"

using namespace util;

TEST(MapNodeTestCase, PutAndGetTest) {
	map_node<int> node;
	auto i = node.put(2);
	auto r = node.get(2);
	EXPECT_EQ(*r, 2);
}

TEST(MapNodeTestCase, NoDataFoundTest) {
	map_node<int> node;
	auto r = node.get(1);
	EXPECT_TRUE(r==nullptr);
}

TEST(MapNodeTestCase, PutAndGetComplexObjectTest) {
	CProbeObject key{ 3, "ABC" };
	CProbeObject key_probe{ 3, "???" };
	map_node<CProbeObject, CProbeObject::less> node;
	auto i = node.put(key);
	auto r = node.get(key_probe);
	EXPECT_EQ(r->name, key.name);
}

TEST(ConcurrentHashMapTestCase, PutTest) {
	concurrent_hash_map<int> chm;
	for(auto i=1; i<5; i++)
		chm.put(i);
	const int *r = chm.get(3);
	EXPECT_EQ(*r, 3);
}

TEST(ConcurrentHashMapTestCase, PutAndGetComplexObjectTest) {
	CProbeObject key1{ 3, "ABC" };
	CProbeObject key2{ 4, "DEF" };
	CProbeObject key_probe{ 4, "???" };
	concurrent_hash_map<CProbeObject, CProbeObject::less, CProbeObject::hash> chm;
	chm.put(key1);
	chm.put(key2);
	auto r = chm.get(key_probe);
	EXPECT_EQ(r->name, key2.name);
}