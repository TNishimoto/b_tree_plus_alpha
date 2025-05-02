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

    cmdline::parser p;

    // p.add<std::string>("input_file", 'i', "input file name", true);
    p.add<uint>("mode", 'm', "mode", false, 0);

    p.parse_check(argc, argv);
   // uint64_t mode = p.get<uint>("mode");

    uint64_t num = 16;
    uint64_t seed = 0;
    for (uint64_t i = 0; i < 10; i++)
    {
        uint64_t degree = 4;
        if (num > 10000)
        {
            break;
        }
        std::cout << "Num: " << num << " \t " << std::flush;

        for (uint64_t j = 0; j < 4; j++)
        {

            for (uint64_t x = 0; x < 10; x++)
            {
                stool::bptree::DynamicPermutation dp;
                stool::NaivePermutation npom;
                
                dp.set_degree(degree);
                
                stool::PermutationTest::random_build_test(dp, num, seed++);
                std::cout << "A" << std::flush;
                dp.clear();
                stool::PermutationTest::random_insert_and_remove_test(dp, npom, num, seed++);
                std::cout << "B" << std::flush;
                
                
                dp.clear();
                npom.clear();
                stool::PermutationTest::random_insert_test(dp, npom, num, seed++);
                std::cout << "C" << std::flush;

                
                stool::PermutationTest::random_remove_test(dp, npom, seed++);
                std::cout << "D" << std::flush;

                stool::PermutationTest::permutation_builder_test(dp, num, seed);
                std::cout << "E" << std::flush;

                stool::PermutationTest::load_write_bits_test(dp);
                std::cout << "F" << std::flush;
                stool::PermutationTest::load_write_file_test(dp);
                std::cout << "G" << std::flush;



                ++seed;
            }
            std::cout << "/" << std::flush;
            degree *= 2;
        }
        std::cout << std::endl;

        num *= 4;
    }
}
