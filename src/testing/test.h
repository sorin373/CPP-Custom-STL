#ifndef __TEST_H__
#define __TEST_H__

#include <iostream>
#include "../STL/unordered_map.h"

#define N 3

class Test {
public:

    class my_hash
    {
    public:
        size_t operator()(const std::string key, const unsigned int table_size) const noexcept
        {
            size_t result = 0;
            for (size_t i = 0; i < key.length(); ++i)
                result = result + toupper(key[i]);
            return result % table_size;
        }
    };

    static void test_1()
    {
        stl::unordered_map<std::string, std::string, my_hash> map;

        map.insert("apple", "fruit");
        map.insert("carrot", "vegetable");
        map.insert("banana", "fruit");
        map.insert("car", "vehicle");
        map.insert("bike", "vehicle");

        for (stl::unordered_map<std::string, std::string, my_hash>::iterator it = map.begin(); it != map.end(); it++)
            std::cout << it->get_key() << ": " << it->get_value() << std::endl;
    }

    static void test_2()
    {
        stl::unordered_map<std::string, std::string, my_hash> map;

        map.insert("abcd", "first");
        map.insert("bcda", "second");

        for (stl::unordered_map<std::string, std::string, my_hash>::iterator it = map.begin(); it != map.end(); it++)
            std::cout << it->get_key() << ": " << it->get_value() << std::endl;
    }

    static void test_3()
    {
        stl::unordered_map<std::string, std::string, my_hash> map;

        map.insert("dog", "animal");
        map.insert("cat", "animal");

        map.insert("cat", "pet");

        for (stl::unordered_map<std::string, std::string, my_hash>::iterator it = map.begin(); it != map.end(); it++)
            std::cout << it->get_key() << ": " << it->get_value() << std::endl;
    }























    static void run(const int key)
    {
        switch (key)
        {
        case 1:
            test_1();
            break;
        case 2:
            test_2();
            break;
        case 3:
            test_3();
            break;
        
        default:
            break;
        }
    }

    static void init()
    {
        for (unsigned int test = 1; test <= N; ++test)
        {
            std::cout << "Test " << test << ":\n\n";

            run(test);

            std::cout << "\n-----------------------------------\n\n";
        }
    }
};

#endif