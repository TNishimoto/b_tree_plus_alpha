#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
#include "stool/include/light_stool.hpp"
#include "../../include/b_tree_plus_alpha.hpp"
#include "../../test/include/permutation_test.hpp"

void bptree_prefix_sum_test(stool::bptree::DynamicPermutation &dynamic_permutation, std::string name, uint64_t item_num, uint64_t query_num, uint64_t seed)
{
    // stool::bptree::DynamicPrefixSum<> dps;

    std::mt19937_64 mt64(seed);
    std::uniform_int_distribution<uint64_t> get_rand_item_num(0, item_num - 1);
    uint64_t hash = 0;

    std::chrono::system_clock::time_point st1, st2;

    std::vector<uint64_t> permutation = stool::PermutationTest::create_random_permutation(item_num, mt64);


    std::cout << "Construction..." << std::endl;
    st1 = std::chrono::system_clock::now();
    dynamic_permutation.build(permutation.begin(), permutation.end(), permutation.size());
    st2 = std::chrono::system_clock::now();
    uint64_t time_construction = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::cout << "Random Insertion..." << std::endl;
    st1 = std::chrono::system_clock::now();
    for (uint64_t i = 0; i < query_num; i++)
    {
        uint64_t pi_index = get_rand_item_num(mt64);
        uint64_t inverse_pi_index = get_rand_item_num(mt64);
        dynamic_permutation.insert(pi_index, inverse_pi_index);
        hash += pi_index + inverse_pi_index;
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_insertion = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::cout << "Random Deletion..." << std::endl;
    st1 = std::chrono::system_clock::now();
    for (uint64_t i = 0; i < query_num; i++)
    {
        uint64_t pos = get_rand_item_num(mt64);
        dynamic_permutation.erase(pos);
        hash += pos;
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_deletion = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::cout << "Access..." << std::endl;
    st1 = std::chrono::system_clock::now();
    for (uint64_t i = 0; i < query_num; i++)
    {
        uint64_t m = get_rand_item_num(mt64);
        hash += dynamic_permutation.access(m);
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_access = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::cout << "Inverse Access..." << std::endl;
    st1 = std::chrono::system_clock::now();
    for (uint64_t i = 0; i < query_num; i++)
    {
        uint64_t m = get_rand_item_num(mt64);
        hash += dynamic_permutation.inverse(m);
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_inverse_access = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();


    std::cout << "\033[36m";
    std::cout << "=============RESULT===============" << std::endl;
    std::cout << "Test: " << name << std::endl;
    std::cout << "item_num = " << item_num  << ", query_num = " << query_num << ", seed = " << seed << std::endl;
    std::cout << "Checksum            : " << hash << std::endl;
    std::cout << "Construction Time   : " << (time_construction / (1000 * 1000)) << "[ms] (Avg: " << (time_construction / item_num) << "[ns])" << std::endl;
    std::cout << "Access Time         : " << (time_access / (1000 * 1000)) << "[ms] (Avg: " << (time_access / query_num) << "[ns])" << std::endl;
    std::cout << "Inverse Access Time           : " << (time_inverse_access / (1000 * 1000)) << "[ms] (Avg: " << (time_inverse_access / query_num) << "[ns])" << std::endl;
    std::cout << "Insertion Time      : " << (time_insertion / (1000 * 1000)) << "[ms] (Avg: " << (time_insertion / query_num) << "[ns])" << std::endl;
    std::cout << "Deletion Time       : " << (time_deletion / (1000 * 1000)) << "[ms] (Avg: " << (time_deletion / query_num) << "[ns])" << std::endl;


    stool::Memory::print_memory_usage();
    std::cout << "==================================" << std::endl;
    std::cout << "\033[39m" << std::endl;
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
    p.add<uint>("mode", 'm', "mode", true);
    p.add<uint64_t>("item_num", 'n', "item_num", false, 1000000);
    p.add<uint64_t>("query_num", 'q', "query_num", false, 100000);
    p.add<uint64_t>("seed", 's', "seed", false, 0);

    p.parse_check(argc, argv);
    uint64_t mode = p.get<uint>("mode");
    uint64_t item_num = p.get<uint64_t>("item_num");
    uint64_t query_num = p.get<uint64_t>("query_num");
    uint64_t seed = p.get<uint64_t>("seed");

    if(mode == 0){
        stool::bptree::DynamicPermutation dp;
        bptree_prefix_sum_test(dp, "stool::bptree::DynamicPermutation", item_num, query_num, seed);
    
    }else{
        /*
        DynPackedSPSIWrapper dps;
        bptree_prefix_sum_test(dps, "DynPackedSPSIWrapper", item_num, max_value, query_num, seed);    
        */
    }

}
