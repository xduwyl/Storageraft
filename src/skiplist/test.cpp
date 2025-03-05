#include <iostream>
#include "skiplist.h"
#define FILE_PATH "../store/dumpFile.txt"

int main() {

    // 键值中的key用int型，如果用其他类型，需要自定义比较函数
    // 而且如果修改key的类型，同时需要修改skipList.load_file函数
    SkipList<int, std::string> skipList(6);
	skipList.insert_element(1, "我"); 
	skipList.insert_element(3, "是"); 
	skipList.insert_element(7, "超级"); 
	skipList.insert_element(8, "无敌"); 
	skipList.insert_element(9, "大"); 
	skipList.insert_element(19, "帅"); 
	skipList.insert_element(19, "哥！！！");
    skipList.insert_set_element(20, "hhhhhhh");
    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.dump_file(FILE_PATH);

    skipList.load_file(FILE_PATH);

    skipList.search_element(9);
    skipList.search_element(18);


    skipList.display_list();

    skipList.delete_element(3);
    skipList.delete_element(7);

    std::cout << "skipList size:" << skipList.size() << std::endl;

    skipList.display_list();
}