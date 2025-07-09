#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
#include "stool/include/light_stool.hpp"
#include "../../include/b_tree_plus_alpha.hpp"
#include "../modules/bit_vector/bit_vector/bv.hpp"
//#include "../../test/permutation_test.hpp"

void bptree_dynamic_bit_test(uint64_t item_num, uint64_t query_num, uint64_t seed)
{
    stool::bptree::DynamicBitSequence dbs;

    std::mt19937_64 mt64(seed);
    std::uniform_int_distribution<uint64_t> get_rand_value(0, 100);
    std::uniform_int_distribution<uint64_t> get_rand_item_num(0, item_num-1);

    uint64_t hash = 0;

    std::chrono::system_clock::time_point st1, st2;

    std::cout << "Construction..." << std::endl;

    st1 = std::chrono::system_clock::now();
    for (uint64_t i = 0; i < item_num; i++)
    {        
        bool b = get_rand_value(mt64) % 2 == 1;
        dbs.push_back(b);
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_construction = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::cout << "Random Insertion..." << std::endl;
    st1 = std::chrono::system_clock::now();
    for (uint64_t i = 0; i < query_num; i++)
    {
        uint64_t pos = get_rand_item_num(mt64);
        bool value = (get_rand_value(mt64) % 2) == 1;
        dbs.insert(pos, value);
        hash += value + pos;
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_insertion = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::cout << "Random Deletion..." << std::endl;
    st1 = std::chrono::system_clock::now();
    for (uint64_t i = 0; i < query_num; i++)
    {
        uint64_t pos = get_rand_item_num(mt64);
        dbs.remove(pos);
        hash += pos;
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_deletion = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    uint64_t count1 = dbs.count_c(true);
    std::uniform_int_distribution<uint64_t> get_rand_bits(0, count1-1);

    std::cout << "rank1..." << std::endl;
    st1 = std::chrono::system_clock::now();
    for (uint64_t i = 0; i < query_num; i++)
    {
        uint64_t m = get_rand_item_num(mt64);
        uint64_t value = dbs.rank1(m);
        hash += value;
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_psum = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::cout << "select1..." << std::endl;

    st1 = std::chrono::system_clock::now();
    for (uint64_t i = 0; i < query_num; i++)
    {
        uint64_t m = get_rand_bits(mt64);
        uint64_t value = dbs.select1(m);
        hash += value;
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_search = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    /*
    if(dbs.size() < 100){
        std::vector<uint64_t> vec = dbs.to_vector();
        stool::DebugPrinter::print_integers(vec, "Items");
    }
    */


    std::cout << "\033[36m";
    std::cout << "=============RESULT===============" << std::endl;
    std::cout << "Test: stool::bptree::DynamicPrefixSum<>" << std::endl;
    std::cout << "item_num = " << item_num  << ", query_num = " << query_num << ", seed = " << seed << std::endl;
    std::cout << "Checksum            : " << hash << std::endl;
    std::cout << "Construction Time   : " << (time_construction / (1000 * 1000)) << "[ms] (Avg: " << (time_construction / item_num) << "[ns])" << std::endl;
    std::cout << "Rank Time           : " << (time_psum / (1000 * 1000)) << "[ms] (Avg: " << (time_psum/ query_num) << "[ns])" << std::endl;
    std::cout << "Select Time         : " << (time_search / (1000 * 1000)) << "[ms] (Avg: " << (time_search/ query_num) << "[ns])" << std::endl;
    std::cout << "Insertion Time      : " << (time_insertion / (1000 * 1000)) << "[ms] (Avg: " << (time_insertion/ query_num) << "[ns])" << std::endl;
    std::cout << "Deletion Time       : " << (time_deletion / (1000 * 1000)) << "[ms] (Avg: " << (time_deletion/ query_num) << "[ns])" << std::endl;


    /*
    std::cout << "Update Time         : " << (time2 / (1000 * 1000)) << "[ms] (Avg: " << (time2 / (2 * len)) << "[ns])" << std::endl;
    std::cout << "Total Running Time : " << (time1 + time2 + time3 + time4) / (1000 * 1000) << "[ms] (Avg: " << ((time1 + time2 + time3 + time4) / len) << "[ns])" << std::endl;
    std::cout << "Size: " << spsi.size() << std::endl;
    spsi.print_memory_usage();
    */

    stool::print_memory_usage();
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

    bptree_dynamic_bit_test(item_num, query_num, seed);

}
