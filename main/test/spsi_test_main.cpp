#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
// #include "qgram_tree.h"
// #include "include/debug/debug.hpp"
#include "stool/include/light_stool.hpp"
#include "sub/spsi_test.hpp"
#include "sub/permutation_test.hpp"

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

    stool::VLCDequeSPSI spsi;
    stool::SPSITest::random_build(spsi, 1000, 100, 32, seed++);
    stool::SPSITest::load_write_bits_test(spsi);
    stool::SPSITest::load_write_file_test(spsi);

    stool::SPSITest::random_remove(spsi, seed++);


}
