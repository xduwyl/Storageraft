//utf-8 
//CREATE 2025-2-1
//AUTHOR xdu_wyl
#ifndef _SKPLIST_H
#define _SKPLIST_H

#include <random>
#include <cstring>
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
    SkiplistNode<K,V>** forward;

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
    this->forward = new SkiplistNode<K, V> *[level + 1];
    memset(this->forward, 0, sizeof(SkiplistNode<K, V> *) * (level + 1));
}
template <typename K, typename V>
SkiplistNode<K, V>::~SkiplistNode(){
    delete[] forward;
}
template <typename K, typename V>
K SkiplistNode<K, V>::get_key() const {
  return key;
};
template <typename K, typename V>
V SkiplistNode<K, V>::get_value() const {
  return value;
};
template <typename K, typename V>
void SkiplistNode<K, V>::set_value(V val){
    this->value = val;
}
template <typename K, typename V>
class SkipList{
public:
    SkipList(int);
    ~SkipList();
    int get_random_level();
    SkiplistNode<K, V>*create_node(K, V, int);
    int insert_element(K, V);
    void display_list();
    bool search_element(K, V &value);
    void delete_element(K);
    void insert_set_element(K &, V &);

    //递归删除节点
    void clear(SkiplistNode<K, V> *);
    int size();
private:
    //跳表头指针
    SkiplistNode<K, V> *_header;
    //最大等级
    int _max_level;
    //当前等级；
    int _current_level;
    //当前元素数量
    int _element_count;
};
template <typename K, typename V>
SkipList<K, V>::SkipList(int max_level) {
  this->_max_level = max_level;
  this->_current_level = 0;
  this->_element_count = 0;

  //最大level 作为头节点
  K k;
  V v;
  this->_header = new SkiplistNode<K, V>(k, v, _max_level);
};
template <typename K, typename V>
SkiplistNode<K, V>* SkipList<K,V>::create_node(const K k, const V v, int level){
    SkiplistNode<K, V> *node = new SkiplistNode<K, V>(k, v, level);
    return node;
}
template <typename K, typename V>
int SkipList<K, V>::size() {
  return _element_count;
}
//增
template <typename K, typename V>
int SkipList<K, V>::insert_element(const K key, const V value) {
}
//删
template <typename K, typename V>
void SkipList<K, V>::delete_element(K key) {
}
//查
template <typename K, typename V>
bool SkipList<K, V>::search_element(K key, V &value) {
}
//改
template <typename K, typename V>
void SkipList<K, V>::insert_set_element(K &key, V &value) {
  V oldValue;
  if (search_element(key, oldValue)) {
    delete_element(key);
  }
  insert_element(key, value);
}
template <typename K, typename V>
void SkipList<K, V>::display_list() {
  std::cout << "\n*****Skip List*****"
            << "\n";
  for (int i = _current_level; i >= 0; i--) {
    SkiplistNode<K, V> *node = this->_header->forward[i];
    std::cout << "Level " << i << ": ";
    while (node != NULL) {
      std::cout << node->get_key() << ":" << node->get_value() << ";";
      node = node->forward[i];
    }
    std::cout << std::endl;
  }
}
template <typename K, typename V>
int SkipList<K, V>::get_random_level() {
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::bernoulli_distribution d(0.5); 
    int k = 1;
    while (d(gen)) {
        k++;
    }
    k = (k < _max_level) ? k : _max_level;
    return k;
};

#endif