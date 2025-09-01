#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cstdio>
#include "../include/b_tree_plus_alpha.hpp"
#include "stool/test/sources/template/dynamic_integer_test.hpp"

using SEQ = stool::bptree::DynamicSequence64<stool::NaiveFLCVector<>, 62, 256>;


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
    p.add<uint>("mode", 'm', "mode", false, 1);

    p.parse_check(argc, argv);
    [[maybe_unused]]uint64_t mode = p.get<uint>("mode");

    uint64_t seed = 0;
    uint64_t seq_len = 10000;
    uint64_t number_of_trials = 100;
    uint64_t max_value = 1000000;

    using SEQ = stool::bptree::SimpleDynamicSequence64;

    stool::DynamicIntegerTest::build_test<SEQ>(seq_len, max_value, number_of_trials, seed);
    //stool::DynamicIntegerTest::psum_test<SEQ>(seq_len, max_value, number_of_trials, seed);
    //stool::DynamicIntegerTest::search_test<SEQ>(seq_len, max_value, number_of_trials, seed);
    stool::DynamicIntegerTest::load_write_file_test<SEQ>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::load_write_bits_test<SEQ>(seq_len, max_value, number_of_trials, seed, false);

    stool::DynamicIntegerTest::push_and_pop_test<SEQ>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::insert_test<SEQ>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::remove_test<SEQ>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::replace_test<SEQ>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::random_test<SEQ, false>(seq_len, max_value, number_of_trials, 100, seed, false);


}
