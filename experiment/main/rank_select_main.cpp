#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
#include "stool/include/light_stool.hpp"
#include "../../include/b_tree_plus_alpha.hpp"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include "../modules/DYNAMIC/include/dynamic/dynamic.hpp"
#pragma GCC diagnostic pop

template <typename T>
void dynamic_operation_test(T &dynamic_sequence, std::string name, uint64_t item_num, uint64_t query_num, uint64_t alphabet_size, uint64_t seed, bool use_access_test, bool use_rank_test, bool use_select_test, bool use_insertion_test, bool use_deletion_test)
{
    std::cout << "Test: " << name << std::endl;

    std::mt19937_64 mt64(seed);
    std::uniform_int_distribution<uint64_t> get_rand_char(0, alphabet_size - 1);
    std::uniform_int_distribution<uint64_t> get_rand_item_num(0, item_num - 1);

    uint64_t hash = 0;

    std::chrono::system_clock::time_point st1, st2;

    std::cout << "Checksum: " << hash << std::endl;
    std::cout << "Construction..." << std::endl;

    st1 = std::chrono::system_clock::now();
    for (uint64_t i = 0; i < item_num; i++)
    {
        uint64_t c = get_rand_char(mt64) + 1;
        hash += c;
        dynamic_sequence.push_back(c);
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_construction = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::cout << "Checksum: " << hash << std::endl;
    std::cout << "Random Insertion..." << std::endl;
    st1 = std::chrono::system_clock::now();
    if (use_insertion_test)
    {
        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t pos = get_rand_item_num(mt64);
            uint64_t c = get_rand_char(mt64) + 1;
            dynamic_sequence.insert(pos, c);
            hash += c + pos;
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_insertion = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::cout << "Checksum: " << hash << std::endl;
    std::cout << "Random Deletion..." << std::endl;
    st1 = std::chrono::system_clock::now();
    if (use_deletion_test)
    {
        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t pos = get_rand_item_num(mt64);
            dynamic_sequence.remove(pos);
            hash += pos;
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_deletion = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::vector<uint64_t> count_c_array;
    for (uint64_t c = 0; c <= alphabet_size; c++)
    {
        if constexpr (std::is_same<T, stool::bptree::DynamicWaveletTree>::value){
            count_c_array.push_back(dynamic_sequence.one_based_rank(dynamic_sequence.size()-1, c));
        }else{
            count_c_array.push_back(dynamic_sequence.rank(dynamic_sequence.size()-1, c));
        }
    }

    std::cout << "Checksum: " << hash << std::endl;
    std::cout << "access1..." << std::endl;

    st1 = std::chrono::system_clock::now();
    if (use_access_test)
    {
        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t m = get_rand_item_num(mt64);
            hash += dynamic_sequence.at(m);
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_access = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::cout << "Checksum: " << hash << std::endl;
    std::cout << "rank..." << std::endl;

    st1 = std::chrono::system_clock::now();
    if (use_rank_test)
    {
        for (uint64_t x = 0; x < query_num; x++)
        {
            uint64_t c = get_rand_char(mt64)+1;
            uint64_t i = get_rand_item_num(mt64);
            if constexpr (std::is_same<T, stool::bptree::DynamicWaveletTree>::value){
                uint64_t value = dynamic_sequence.one_based_rank(i, c);
                hash += value;
            }else{
                uint64_t value = dynamic_sequence.rank(i, c);
                hash += value;
            }
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_psum = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();


    std::cout << "Checksum: " << hash << std::endl;
    std::cout << "select..." << std::endl;

    st1 = std::chrono::system_clock::now();
    if (use_select_test)
    {
        for (uint64_t x = 0; x < query_num; x++)
        {
            uint64_t c = get_rand_char(mt64)+1;
            uint64_t i = get_rand_item_num(mt64) % count_c_array[c];
            uint64_t value = dynamic_sequence.select(i, c);
            hash += value;
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_search = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::cout << "\033[36m";
    std::cout << "=============RESULT===============" << std::endl;
    std::cout << "Test: " << name << std::endl;
    std::cout << "item_num = " << item_num << ", query_num = " << query_num << ", alphabet_size = " << alphabet_size << ", seed = " << seed << std::endl;
    std::cout << "Checksum            : " << hash << std::endl;
    std::cout << "Construction Time   : " << (time_construction / (1000 * 1000)) << "[ms] (Avg: " << (time_construction / item_num) << "[ns])" << std::endl;
    std::cout << "Access Time         : " << (time_access / (1000 * 1000)) << "[ms] (Avg: " << (time_access / query_num) << "[ns])" << std::endl;
    std::cout << "Rank Time           : " << (time_psum / (1000 * 1000)) << "[ms] (Avg: " << (time_psum / query_num) << "[ns])" << std::endl;
    std::cout << "Select Time         : " << (time_search / (1000 * 1000)) << "[ms] (Avg: " << (time_search / query_num) << "[ns])" << std::endl;
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
    p.add<std::string>("index_name", 'x', "index_name", true);
    p.add<uint64_t>("item_num", 'n', "item_num", false, 1000000);
    p.add<uint64_t>("query_num", 'q', "query_num", false, 1000000);
    p.add<uint64_t>("alphabet_size", 'a', "alphabet_size", false, 16);
    p.add<uint64_t>("seed", 's', "seed", false, 0);

    p.parse_check(argc, argv);
    std::string index_name = p.get<std::string>("index_name");
    uint64_t item_num = p.get<uint64_t>("item_num");
    uint64_t query_num = p.get<uint64_t>("query_num");
    uint64_t seed = p.get<uint64_t>("seed");
    uint64_t alphabet_size = p.get<uint64_t>("alphabet_size");

    if (index_name == "BTreePlusAlpha")
    {
        std::vector<uint8_t> _alphabet;
        for (uint64_t i = 0; i <= alphabet_size; i++)
        {
            _alphabet.push_back(i);
        }

        stool::bptree::DynamicWaveletTree dbs(_alphabet);

        dynamic_operation_test(dbs, "stool::bptree::DynamicWaveletTree", item_num, query_num, alphabet_size, seed, true, true, true, true, true);
    }
    else if (index_name == "DYNAMIC")
    {
        dyn::wt_string<dyn::suc_bv> wb;
        dynamic_operation_test(wb, "dyn::wt_string<dyn::suc_bv>", item_num, query_num, alphabet_size, seed, true, true, true, true, true);
    }
}
