#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
#include "stool/include/light_stool.hpp"
#include "../../include/b_tree_plus_alpha.hpp"
// #include "../../test/permutation_test.hpp"
#include "../include/dyn_packed_spsi_wrapper.hpp"

template <typename T>
void bptree_prefix_sum_test(T &dynamic_prefix_sum, std::string name, std::string test_type, uint64_t item_num, uint64_t max_value, uint64_t query_num, uint64_t seed)
{
    // stool::bptree::DynamicPrefixSum<> dps;

    std::mt19937_64 mt64(seed);
    std::uniform_int_distribution<uint64_t> get_rand_value(0, max_value);
    std::uniform_int_distribution<uint64_t> get_rand_item_num(0, item_num - 1);
    uint64_t hash = 0;
    double density_when_build_is_complete = 0;

    std::chrono::system_clock::time_point st1, st2;

    std::cout << "Construction..." << std::endl;

    st1 = std::chrono::system_clock::now();

    if constexpr (std::is_same<T, stool::bptree::DynamicBitSequence>::value) {
        std::vector<uint64_t> buffer;
        uint64_t buffer_size = 10000;

        for (uint64_t i = 0; i < item_num; i++)
        {
            uint64_t m = get_rand_value(mt64);
            hash += m;

            buffer.push_back(m);
            if(buffer.size() >= buffer_size){
                dynamic_prefix_sum.push_many(buffer);
                buffer.clear();
            }
        }
        if(buffer.size() > 0){
            dynamic_prefix_sum.push_many(buffer);
            buffer.clear();
        }
    }else{
        for (uint64_t i = 0; i < item_num; i++)
        {
            uint64_t m = get_rand_value(mt64);
            dynamic_prefix_sum.push_back(m);
            hash += m;
        }    
    }


    st2 = std::chrono::system_clock::now();
    uint64_t time_construction = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    if constexpr (std::is_same<T, stool::bptree::DynamicPrefixSum<>>::value) {
        density_when_build_is_complete = dynamic_prefix_sum.density();
    }


    st1 = std::chrono::system_clock::now();
    if (test_type == "all" || test_type == "insertion")
    {
        std::cout << "Random Insertion..." << std::endl;
        for (uint64_t i = 0; i < query_num; i++)
        {

            uint64_t pos = get_rand_item_num(mt64);
            uint64_t value = get_rand_value(mt64);
            dynamic_prefix_sum.insert(pos, value);
            hash += value + pos;
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_insertion = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    st1 = std::chrono::system_clock::now();
    if (test_type == "all" || test_type == "deletion")
    {
        std::cout << "Random Deletion..." << std::endl;

        int64_t n_delete = dynamic_prefix_sum.size() - query_num;
        if(n_delete < 0){
            throw std::runtime_error("n_delete < 0");
        }
        std::uniform_int_distribution<uint64_t> get_rand_for_delete(0, n_delete);


        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t pos = get_rand_for_delete(mt64);
            dynamic_prefix_sum.remove(pos);
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
            hash += dynamic_prefix_sum.at(m);
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_access = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    st1 = std::chrono::system_clock::now();
    if (test_type == "all" || test_type == "psum")
    {
        std::cout << "PSUM..." << std::endl;
        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t m = get_rand_item_num(mt64);
            uint64_t value = dynamic_prefix_sum.psum(m);
            hash += value;
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_psum = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    st1 = std::chrono::system_clock::now();
    if (test_type == "all" || test_type == "search")
    {
        std::cout << "Search..." << std::endl;
        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t m = get_rand_value(mt64);
            uint64_t value = dynamic_prefix_sum.search(m);
            hash += value;
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_search = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    /*
    if(dps.size() < 100){
        std::vector<uint64_t> vec = dps.to_vector();
        stool::DebugPrinter::print_integers(vec, "Items");
    }
    */

    std::cout << "\033[36m";
    std::cout << "=============RESULT===============" << std::endl;
    std::cout << "Test: " << name << std::endl;
    std::cout << "item_num = " << item_num << ", max_value = " << max_value << ", query_num = " << query_num << ", seed = " << seed << std::endl;
    std::cout << "Checksum            : " << hash << std::endl;
    std::cout << "Construction Time   : " << (time_construction / (1000 * 1000)) << "[ms] (Avg: " << (time_construction / item_num) << "[ns])" << std::endl;
    std::cout << "Access Time         : " << (time_access / (1000 * 1000)) << "[ms] (Avg: " << (time_access / query_num) << "[ns])" << std::endl;
    std::cout << "PSUM Time           : " << (time_psum / (1000 * 1000)) << "[ms] (Avg: " << (time_psum / query_num) << "[ns])" << std::endl;
    std::cout << "Search Time         : " << (time_search / (1000 * 1000)) << "[ms] (Avg: " << (time_search / query_num) << "[ns])" << std::endl;
    std::cout << "Insertion Time      : " << (time_insertion / (1000 * 1000)) << "[ms] (Avg: " << (time_insertion / query_num) << "[ns])" << std::endl;
    std::cout << "Deletion Time       : " << (time_deletion / (1000 * 1000)) << "[ms] (Avg: " << (time_deletion / query_num) << "[ns])" << std::endl;

    if constexpr (std::is_same<T, stool::bptree::DynamicPrefixSum<>>::value) {
        std::cout << "Density of the B-tree when the build is complete: " << density_when_build_is_complete << std::endl;
        dynamic_prefix_sum.print_information_about_performance();
    }
    /*
    std::cout << "Update Time         : " << (time2 / (1000 * 1000)) << "[ms] (Avg: " << (time2 / (2 * len)) << "[ns])" << std::endl;
    std::cout << "Total Running Time : " << (time1 + time2 + time3 + time4) / (1000 * 1000) << "[ms] (Avg: " << ((time1 + time2 + time3 + time4) / len) << "[ns])" << std::endl;
    std::cout << "Size: " << spsi.size() << std::endl;
    spsi.print_memory_usage();
    */

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
    p.add<uint64_t>("max_value", 'v', "max_value", false, 100);
    p.add<uint64_t>("query_num", 'q', "query_num", false, 100000);
    p.add<uint64_t>("seed", 's', "seed", false, 0);

    p.parse_check(argc, argv);
    std::string index_name = p.get<std::string>("index_name");
    std::string query_type = p.get<std::string>("test");
    uint64_t item_num = p.get<uint64_t>("item_num");
    uint64_t max_value = p.get<uint64_t>("max_value");
    uint64_t query_num = p.get<uint64_t>("query_num");
    uint64_t seed = p.get<uint64_t>("seed");

    if (index_name == "BTreePlusAlpha")
    {
        stool::bptree::DynamicPrefixSum<> dps;
        bptree_prefix_sum_test(dps, "stool::bptree::DynamicPrefixSum<>", query_type, item_num, max_value, query_num, seed);
    }
    else if(index_name == "DYNAMIC")
    {
        DynPackedSPSIWrapper dps;
        bptree_prefix_sum_test(dps, "DynPackedSPSIWrapper", query_type, item_num, max_value, query_num, seed);
    }
}
