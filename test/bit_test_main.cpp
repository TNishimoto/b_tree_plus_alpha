#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
// #include "qgram_tree.h"
// #include "include/debug/debug.hpp"
#include "stool/include/light_stool.hpp"
#include "include/bit_sequence_test.hpp"

template <typename DBV>
void test(DBV &dbv, uint64_t seed, uint64_t insert_num, int message_paragraph = stool::Message::SHOW_MESSAGE)
{
    if(message_paragraph != stool::Message::NO_MESSAGE){
        std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Testing..." << std::endl;
    }

    if constexpr (std::is_same<DBV, stool::bptree::SimpleDynamicBitSequence>::value || std::is_same<DBV, stool::bptree::SimpleDynamicBitDequeSequence>::value) {
        stool::BitSequenceTest::build_test(dbv, insert_num, seed, message_paragraph+1);

        stool::BitSequenceTest::load_write_test(dbv, message_paragraph+1);
        stool::BitSequenceTest::load_write_test2(dbv, message_paragraph+1);
        
        stool::BitSequenceTest::test_iterator(dbv, message_paragraph+1);    
    }


    stool::BitSequenceTest::test_psum(dbv, message_paragraph+1);
    stool::BitSequenceTest::test_search(dbv, message_paragraph+1);
    stool::BitSequenceTest::test_rank(dbv, 0, message_paragraph+1);
    stool::BitSequenceTest::test_rank(dbv, 1, message_paragraph+1);
    stool::BitSequenceTest::test_select(dbv, 0, message_paragraph+1);
    stool::BitSequenceTest::test_select(dbv, 1, message_paragraph+1);




    dbv.clear();
    stool::BitSequenceTest::insert_and_delete_test(dbv, insert_num, seed, message_paragraph+1);

    dbv.clear();
    stool::BitSequenceTest::insert_and_delete_test2(dbv, insert_num, seed, message_paragraph+1);


}
void _test(uint64_t mode, uint64_t seed, int message_paragraph = stool::Message::SHOW_MESSAGE)
{
    if (mode == 1)
    {
        if(message_paragraph != stool::Message::NO_MESSAGE){
            std::cout << stool::Message::get_paragraph_string(message_paragraph) << "TEST: PlainSPSIContainer" << std::endl; 
        }
        std::mt19937_64 mt64(seed);
        std::vector<bool> items = stool::BitSequenceTest::create_sequence2(60, mt64);
        stool::bptree::PlainSPSIContainer plain_container;
        for(uint64_t i = 0; i < items.size();i++){
            plain_container.push_back(items[i] ? 1 : 0);
        }
        test(plain_container, seed, 62, message_paragraph+1);
    }
    else if (mode == 2)
    {
        if(message_paragraph != stool::Message::NO_MESSAGE){
            std::cout << stool::Message::get_paragraph_string(message_paragraph) << "TEST: BitContainer" << std::endl; 
        }

        std::mt19937_64 mt64(seed);
        std::vector<bool> items = stool::BitSequenceTest::create_sequence2(60, mt64);
        stool::bptree::BitContainer bit_container(items);
        test(bit_container, seed, 62, message_paragraph+1);
    }
    else if (mode == 3)
    {
        if(message_paragraph != stool::Message::NO_MESSAGE){
            std::cout << stool::Message::get_paragraph_string(message_paragraph) << "TEST: SimpleDynamicBitSequence" << std::endl; 
        }

        //std::mt19937_64 mt64(seed);
        //std::vector<bool> items = stool::BitTest::create_sequence2(1000, mt64);
        stool::bptree::SimpleDynamicBitSequence bit_seq;

        test(bit_seq, seed, 1000, message_paragraph+1);

        if(message_paragraph != stool::Message::NO_MESSAGE){
            std::cout << "OK!" << std::endl;
        }
    }
    else if (mode == 4){
        if(message_paragraph != stool::Message::NO_MESSAGE){
            std::cout << stool::Message::get_paragraph_string(message_paragraph) << "TEST: SimpleDynamicBitDequeSequence" << std::endl; 
        }

        stool::bptree::SimpleDynamicBitDequeSequence bit_seq;

        test(bit_seq, seed, 200, message_paragraph+1);

        if(message_paragraph != stool::Message::NO_MESSAGE){
            std::cout << "OK!" << std::endl;
        }

    }
    else{
        if(message_paragraph != stool::Message::NO_MESSAGE){
            std::cout << stool::Message::get_paragraph_string(message_paragraph) << "TEST ALL" << std::endl;
        }
        _test(1, seed, message_paragraph+1);
        _test(2, seed, message_paragraph+1);
        _test(3, seed, message_paragraph+1);
        _test(4, seed, message_paragraph+1);
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
