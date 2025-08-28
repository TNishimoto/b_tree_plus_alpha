#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cstdio>
#include "../include/b_tree_plus_alpha.hpp"


std::vector<uint64_t> create_random_rank_array(uint64_t size, uint64_t seed)
{
    std::mt19937_64 mt64(seed);
    std::vector<uint64_t> r;
    for (uint64_t i = 0; i < size; i++)
    {
        r.push_back(i);
    }
    std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, size - 1);

    for (uint64_t i = 0; i < size; i++)
    {
        uint64_t pos1 = get_rand_uni_int(mt64);
        uint64_t pos2 = get_rand_uni_int(mt64);
        std::swap(r[pos1], r[pos2]);
    }
    return r;
}


void build_test(uint64_t size, uint64_t seed)
{
    std::vector<uint64_t> rank_array = create_random_rank_array(size, seed);
    stool::bptree::DynamicWaveletTreeForRangeSearch ds;
    ds.build(rank_array);

    std::vector<uint64_t> test_rank_array = ds.to_rank_elements();
    try{
        stool::EqualChecker::equal_check(test_rank_array, rank_array);
    }catch(const std::exception& e){
        std::cout << "Error: " << e.what() << std::endl;
        ds.print_tree();


        std::cout << "test_rank_array: ";
        stool::DebugPrinter::print_integers(test_rank_array);
        std::cout << "rank_array: ";
        stool::DebugPrinter::print_integers(rank_array);
        throw e;
    }
}
void build_test(uint64_t size, uint64_t number_of_trials, uint64_t seed)
{
    std::cout << "BUILD TEST" << std::endl;
    for(uint64_t len = 1; len <= size; len*=2){
        std::cout << "len: " << len << std::endl;
        for(uint64_t i = 0; i < number_of_trials; i++){
            std::cout << "+" << std::flush;

            build_test(len, seed++);
        }    
    }
    std::cout << "[END]" << std::endl;

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
    p.add<uint64_t>("seed", 's', "seed", false, 0);

    p.parse_check(argc, argv);
    uint64_t seed = p.get<uint64_t>("seed");
    //uint64_t mode = p.get<uint>("mode");

    build_test(10000, 100, seed);



}