//utf-8 
//CREATE 2025-2-1
//AUTHOR xdu_wyl
#ifndef _SKPLIST_H
#define _SKPLIST_H

#include <iostream>
#include <vector>

template<typename K,typename V>
class SkiplistNode{
public:
    SkiplistNode(const K&k, const V&v,int level);
    ~SkiplistNode();
    K get_key() const;
    V get_value() const;
    void set_value(V);
    //节点等级
    int level;
    //下一节点指针
    std::vector<SkiplistNode<K,V>*> forward;

private:
    //键
    K key;
    //值
    V value;
};
template <typename K, typename V>
SkiplistNode<K, V>::SkiplistNode(const K &k, const V &v, int level){
    this->key = k;
    this->value = v;
    this->level = level;
    this->forward.push_back(new SkiplistNode<K, V>(k,v,0));
}
template <typename K, typename V>
class SkipList{
public:
    SkipList(int max_level);
    ~SkipList();
private:
    //跳表头指针
    SkiplistNode<K, V> *_header;
    //最大等级
    int _max_level;
    //当前等级；
    int _now_level;

};
#endif