//utf-8 
//CREATE 2025-2-1
//AUTHOR xdu_wyl
#ifndef _SKPLIST_H
#define _SKPLIST_H

#include <random>
#include <cstring>
#include <iostream>
#include <vector>
#include <mutex>


#include <boost/serialization/access.hpp>
// #include <boost/archive/text_oarchive.hpp>  // 文本输出归档
// #include <boost/archive/text_iarchive.hpp>  // 文本输入归档
#include <boost/serialization/vector.hpp>   // 支持 std::vector 序列化
#include <sstream>    

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
    bool search_element(K);
    void delete_element(K);
    void insert_set_element(K , V);
    //存储文件
    std::string dump_file();
    //加载文件
    void load_file(const std::string &dumpStr);

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
    //锁
    std::mutex _mtx;
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
void SkipList<K,V>:: clear(SkiplistNode<K, V> *cur){
    if(cur->forward[0]!=nullptr){
        //高层为引用
        clear(cur->forward[0]);
    }
    delete (cur);
}
template <typename K, typename V>
SkipList<K,V>::~SkipList(){
    if (_header->forward[0] != nullptr) {
        clear(_header->forward[0]);
    }
    delete (_header);
}
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
    _mtx.lock();
    SkiplistNode<K, V> *cur = _header;
    //前一层节点
    SkiplistNode<K, V> *update[_max_level + 1];
    memset(update, 0, sizeof(SkiplistNode<K, V>*) *(_max_level + 1));
    for (int i = _current_level; i >= 0;--i){
        while(cur->forward[i]&&cur->forward[i]->get_key()<key){
            cur = cur->forward[i];
        }
        update[i] = cur;
    }
    cur = cur->forward[0];
    if (cur != NULL && cur->get_key() == key) {
        //可在此添加替换
        std::cout << "key: " << key << ", exists" << std::endl;
        _mtx.unlock();
        return 1;
    }
    if(cur==nullptr||cur->get_key()!=key){
        int rand_level = get_random_level();
        if(rand_level>_current_level){
            for (int i = _current_level + 1; i <= rand_level;++i){
                update[i] = _header;
            }
            _current_level = rand_level;
        }
        SkiplistNode<K, V> *new_node = create_node(key, value, _current_level);
        for (int i = 0; i <= _current_level;++i){
            new_node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = new_node;
        }
        std::cout << "Successfully inserted key:" << key << ", value:" << value << std::endl;
        _element_count++;
    }
    _mtx.unlock();
    return 0;  
}

//                            +------------+
//                            |  delete 50 |
//                            +------------+
// level 4     +-->1+                                                      100
//                  |
//                  |                      delete +----+
// level 3         1+-------->10+---------------> | 50 |          70       100
//                                                |    |
//                                                |    |
// level 2         1          10         30       | 50 |          70       100
//                                                |    |
//                                                |    |
// level 1         1    4     10         30       | 50 |          70       100
//                                                |    |
//                                                |    |
// level 0         1    4   9 10         30   40  | 50 |  60      70       100
//                                                +----+

//删
template <typename K, typename V>
void SkipList<K, V>::delete_element(K key) {
    _mtx.lock();
    SkiplistNode<K, V> *cur = _header;
    //前一层节点
    SkiplistNode<K, V> *update[_max_level + 1];
    memset(update, 0, sizeof(SkiplistNode<K, V>*) *(_max_level + 1));
    for (int i = _current_level; i >= 0;--i){
        while(cur->forward[i]&&cur->forward[i]->get_key()<key){
            cur = cur->forward[i];
        }
        update[i] = cur;
    }
    cur = cur->forward[0];
    if (cur != NULL && cur->get_key() == key) {
        //找到即可删除
        for (int i = 0; i <= _current_level;++i){
            if(update[i]->forward[i]!=cur){
                break;
            }
            update[i]->forward[i] = cur->forward[i];
        }
        //若删除最高 则当前level需要更新
        while(_current_level>0&&_header->forward[_current_level]==nullptr){
            _current_level--;
        }
        std::cout << "Successfully deleted key " << key << std::endl;
        delete cur;
        _element_count--;
    }
    _mtx.unlock();
    return;
}
//查
template <typename K, typename V>
bool SkipList<K, V>::search_element(K key) {
    std::cout << "search_element-----------------" << std::endl;
    SkiplistNode<K, V> *cur = _header;
    for (int i = _current_level; i >= 0;--i){
        while(cur->forward[i]&&cur->forward[i]->get_key()<key){
            cur = cur->forward[i];
        }
    }
    cur = cur->forward[0];
    if(cur&&cur->get_key()==key){
        V value = cur->get_value();
        std::cout << "Found key: " << key << ", value: " << value << std::endl;
        return true;
    }
    std::cout << "Not Found Key:" << key << std::endl;
    return false;
}
//改(删了加)
template <typename K, typename V>
void SkipList<K, V>::insert_set_element(const K key, const V value) {
  V oldValue;
  if (search_element(key)) {
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
//序列化类
template <typename K, typename V>
class SkipListDump {
 public:
  friend class boost::serialization::access;

  template <class Archive>
  void serialize(Archive &ar, const unsigned int version) {
    ar &keyDumpVt_;
    ar &valDumpVt_;
  }
  std::vector<K> keyDumpVt_;
  std::vector<V> valDumpVt_;

 public:
  void insert(const SkiplistNode<K, V> &node);
};
template <typename K, typename V>
void SkipListDump<K, V>::insert(const  SkiplistNode<K, V> &node) {
  keyDumpVt_.emplace_back(node.get_key());
  valDumpVt_.emplace_back(node.get_value());
}

template <typename K, typename V>
std::string SkipList<K, V>::dump_file(){
    SkiplistNode<K, V> *cur = this->_header->forward[0];
    SkipListDump<K, V> dumper;
    while(cur){
        dumper.insert(*cur);
        cur = cur->forward[0];
    }
    std::stringstream ss;
    // boost::archive::text_oarchive oa(ss);
    // oa << dumper;
    return ss.str();
}
template <typename K, typename V>
void SkipList<K, V>::load_file(const std::string &dumpStr){
    if (dumpStr.empty()) {
        return;
    }
    SkipListDump<K, V> dumper;
    std::stringstream iss(dumpStr);
    // boost::archive::text_iarchive ia(iss);
    // ia >> dumper;
    for (int i = 0; i < dumper.keyDumpVt_.size(); ++i)
    {
        insert_element(dumper.keyDumpVt_[i], dumper.valDumpVt_[i]);
    }
}

#endif