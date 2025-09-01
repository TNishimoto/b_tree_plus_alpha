#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
#include "stool/include/light_stool.hpp"
#include "include/spsi_test.hpp"
#include "stool/test/sources/template/dynamic_integer_test.hpp"

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

    uint64_t seed = 0;
    cmdline::parser p;

    // p.add<std::string>("input_file", 'i', "input file name", true);
    p.add<uint>("mode", 'm', "mode", false, 0);

    p.parse_check(argc, argv);
    //uint64_t mode = p.get<uint>("mode");

    uint64_t seq_len = 10000;
    uint64_t number_of_trials = 100;
    uint64_t max_value = 1000000;

    stool::DynamicIntegerTest::build_test<stool::bptree::SimpleDynamicPrefixSum>(seq_len, max_value, number_of_trials, seed);
    stool::DynamicIntegerTest::psum_test<stool::bptree::SimpleDynamicPrefixSum>(seq_len, max_value, number_of_trials, seed);
    stool::DynamicIntegerTest::search_test<stool::bptree::SimpleDynamicPrefixSum>(seq_len, max_value, number_of_trials, seed);
    stool::DynamicIntegerTest::load_write_file_test<stool::bptree::SimpleDynamicPrefixSum>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::load_write_bits_test<stool::bptree::SimpleDynamicPrefixSum>(seq_len, max_value, number_of_trials, seed, false);

    stool::DynamicIntegerTest::push_and_pop_test<stool::bptree::SimpleDynamicPrefixSum>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::insert_test<stool::bptree::SimpleDynamicPrefixSum>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::remove_test<stool::bptree::SimpleDynamicPrefixSum>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::replace_test<stool::bptree::SimpleDynamicPrefixSum>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::random_test<stool::bptree::SimpleDynamicPrefixSum>(seq_len, max_value, number_of_trials, 100, seed, false);

}
