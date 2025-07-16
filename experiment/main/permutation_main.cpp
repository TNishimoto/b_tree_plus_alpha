#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
#include "stool/include/light_stool.hpp"
#include "../../include/b_tree_plus_alpha.hpp"
#include "../../test/include/permutation_test.hpp"

void bptree_permutation_test(stool::bptree::DynamicPermutation &dynamic_permutation, std::string name, std::string test_type, uint64_t item_num, uint64_t query_num, uint64_t seed)
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

    double density_for_pi_tree_when_build_is_complete = dynamic_permutation.get_pi_tree().get_value_density();
    double density_for_inverse_pi_tree_when_build_is_complete = dynamic_permutation.get_inverse_pi_tree().get_value_density();

    st1 = std::chrono::system_clock::now();
    if (test_type == "all" || test_type == "insertion")
    {
        std::cout << "Random Insertion..." << std::endl;
        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t pi_index = get_rand_item_num(mt64);
            uint64_t inverse_pi_index = get_rand_item_num(mt64);
            dynamic_permutation.insert(pi_index, inverse_pi_index);
            hash += pi_index + inverse_pi_index;
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_insertion = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    st1 = std::chrono::system_clock::now();

    if (test_type == "all" || test_type == "deletion")
    {
        int64_t n_delete = dynamic_permutation.size() - query_num;
        if (n_delete < 0)
        {
            throw std::runtime_error("n_delete < 0");
        }
        std::uniform_int_distribution<uint64_t> get_rand_for_delete(0, n_delete);

        std::cout << "Random Deletion..." << std::endl;
        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t pos = get_rand_for_delete(mt64);
            dynamic_permutation.erase(pos);
            hash += pos;
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_deletion = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    st1 = std::chrono::system_clock::now();
    if (test_type == "all" || test_type == "access")
    {
        std::cout << "Access..." << std::endl;
        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t m = get_rand_item_num(mt64);
            hash += dynamic_permutation.access(m);
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_access = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    st1 = std::chrono::system_clock::now();

    if (test_type == "all" || test_type == "inverse")
    {
        std::cout << "Inverse Access..." << std::endl;
        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t m = get_rand_item_num(mt64);
            hash += dynamic_permutation.inverse(m);
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_inverse_access = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::cout << "\033[36m";
    std::cout << "=============RESULT===============" << std::endl;
    std::cout << "Test: " << name << std::endl;
    std::cout << "item_num = " << item_num << ", query_num = " << query_num << ", seed = " << seed << std::endl;
    std::cout << "Checksum \t\t : " << hash << std::endl;
    std::cout << "Construction Time \t : " << (time_construction / (1000 * 1000)) << "[ms] (Avg: " << (time_construction / item_num) << "[ns])" << std::endl;
    std::cout << "Access Time \t\t : " << (time_access / (1000 * 1000)) << "[ms] (Avg: " << (time_access / query_num) << "[ns])" << std::endl;
    std::cout << "Inverse Access Time \t : " << (time_inverse_access / (1000 * 1000)) << "[ms] (Avg: " << (time_inverse_access / query_num) << "[ns])" << std::endl;
    std::cout << "Insertion Time \t\t : " << (time_insertion / (1000 * 1000)) << "[ms] (Avg: " << (time_insertion / query_num) << "[ns])" << std::endl;
    std::cout << "Deletion Time \t\t : " << (time_deletion / (1000 * 1000)) << "[ms] (Avg: " << (time_deletion / query_num) << "[ns])" << std::endl;

    std::cout << "== B-trees information in Dynamic Permutation (pi-tree) == " << std::endl;   
    std::cout << "Density of the B-tree when the build is complete: " << density_for_pi_tree_when_build_is_complete << std::endl;
    dynamic_permutation.get_pi_tree().print_information_about_performance(1);
    std::cout << "== B-trees information in Dynamic Permutation (inverse pi-tree) == " << std::endl;   
    std::cout << "Density of the B-tree when the build is complete: " << density_for_inverse_pi_tree_when_build_is_complete << std::endl;
    dynamic_permutation.get_inverse_pi_tree().print_information_about_performance(1);

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
    p.add<std::string>("index_name", 'x', "index_name", true);
    p.add<std::string>("test", 't', "test", false, "all");
    p.add<uint64_t>("item_num", 'n', "item_num", false, 1000000);
    p.add<uint64_t>("query_num", 'q', "query_num", false, 1000000);
    p.add<uint64_t>("seed", 's', "seed", false, 0);

    p.parse_check(argc, argv);
    std::string index_name = p.get<std::string>("index_name");
    std::string test_type = p.get<std::string>("test");
    uint64_t item_num = p.get<uint64_t>("item_num");
    uint64_t query_num = p.get<uint64_t>("query_num");
    uint64_t seed = p.get<uint64_t>("seed");

    if (index_name == "BTreePlusAlpha")
    {
        stool::bptree::DynamicPermutation dp;
        bptree_permutation_test(dp, "stool::bptree::DynamicPermutation", test_type, item_num, query_num, seed);
    }
    else
    {
        /*
        DynPackedSPSIWrapper dps;
        bptree_prefix_sum_test(dps, "DynPackedSPSIWrapper", item_num, max_value, query_num, seed);
        */
    }
}
