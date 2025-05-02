#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
// #include "qgram_tree.h"
// #include "include/debug/debug.hpp"
#include "stool/include/light_stool.hpp"
#include "sub/bit_test.hpp"

template <typename DBV>
void test(DBV &dbv, uint64_t seed, uint64_t insert_num)
{
    stool::BitTest::test_psum(dbv);
    stool::BitTest::test_search(dbv);
    stool::BitTest::test_rank(dbv, 0);
    stool::BitTest::test_rank(dbv, 1);
    stool::BitTest::test_select(dbv, 0);
    stool::BitTest::test_select(dbv, 1);

    dbv.clear();
    stool::BitTest::insert_and_delete_test(dbv, insert_num, seed);

}
void _test(uint64_t mode, uint64_t seed)
{
    if (mode == 1)
    {
        std::cout << "TEST: PlainSPSIContainer" << std::endl; 
        std::mt19937_64 mt64(seed);
        std::vector<bool> items = stool::BitTest::create_sequence2(60, mt64);
        stool::bptree::PlainSPSIContainer plain_container;
        for(uint64_t i = 0; i < items.size();i++){
            plain_container.push_back(items[i] ? 1 : 0);
        }
        test(plain_container, seed, 62);
    }
    else if (mode == 2)
    {
        std::cout << "TEST: BitContainer" << std::endl; 

        std::mt19937_64 mt64(seed);
        std::vector<bool> items = stool::BitTest::create_sequence2(60, mt64);
        stool::bptree::BitContainer bit_container(items);
        test(bit_container, seed, 62);
    }
    else if (mode == 3)
    {
        std::cout << "TEST: DynamicBitSequence" << std::endl; 

        //std::mt19937_64 mt64(seed);
        //std::vector<bool> items = stool::BitTest::create_sequence2(1000, mt64);
        stool::bptree::DynamicBitSequence bit_seq;
        stool::BitTest::build_test(bit_seq, 1000, seed);

        stool::BitTest::load_write_test(bit_seq);
        stool::BitTest::load_write_test2(bit_seq);

        /*
        for (uint64_t v : items)
        {
            bit_seq.push_back(v);
        }
        */
        
        stool::BitTest::test_iterator(bit_seq);

        test(bit_seq, seed, 1000);

        bit_seq.clear();
        stool::BitTest::insert_and_delete_test2(bit_seq, 10000, seed);
        std::cout << "OK!" << std::endl;


    }else{
        _test(1, seed);
        _test(2, seed);
        _test(3, seed);

    }
}

int main(int argc, char *argv[])
{
#ifdef DEBUG
    std::cout << "\033[41m";
    std::cout << "DEBUG MODE!" << std::endl;
    std::cout << "\e[m" << std::endl;
    // std::cout << "\033[30m" << std::endl;
#endif
#ifdef SLOWDEBUG
    std::cout << "\033[41m";
    std::cout << "SLOWDEBUG MODE!" << std::endl;
    std::cout << "\e[m" << std::endl;
    // std::cout << "\033[30m" << std::endl;
#endif

    cmdline::parser p;

    // p.add<std::string>("input_file", 'i', "input file name", true);
    p.add<uint>("mode", 'm', "mode", false, 0);

    p.parse_check(argc, argv);
    uint64_t mode = p.get<uint>("mode");
    uint64_t seed = 0;

    _test(mode, seed);

}
