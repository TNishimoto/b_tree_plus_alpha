#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
#include "stool/include/light_stool.hpp"
#include "../../include/b_tree_plus_alpha.hpp"
#if defined(__x86_64__)
#include "../include/bv_bv_wrapper.hpp"
#endif
#include "../include/dyn_suc_bv_wrapper.hpp"
// #include "../../test/permutation_test.hpp"

template <typename T>
void dynamic_bit_operation_test(T &dynamic_bit_sequence, std::string name, std::string test_type, uint64_t item_num, uint64_t query_num, uint64_t seed)
{

    std::cout << "Test: " << name << std::endl;

    std::mt19937_64 mt64(seed);
    std::uniform_int_distribution<uint64_t> get_rand_value(0, 100);
    std::uniform_int_distribution<uint64_t> get_rand_item_num(0, item_num - 1);

    uint64_t hash = 0;
    double density_when_build_is_complete = 0;

    std::chrono::system_clock::time_point st1, st2;

    std::cout << "Checksum: " << hash << std::endl;
    std::cout << "Construction..." << std::endl;

    st1 = std::chrono::system_clock::now();

    if constexpr (std::is_same<T, stool::bptree::SimpleDynamicBitSequence>::value ||
                  std::is_same<T, stool::bptree::DynamicBitDequeSequence>::value ||
                  std::is_same<T, stool::bptree::DynamicBitDequeSequence1>::value ||
                  std::is_same<T, stool::bptree::DynamicBitDequeSequence2>::value ||
                  std::is_same<T, stool::bptree::DynamicBitDequeSequence3>::value ||
                  std::is_same<T, stool::bptree::DynamicBitDequeSequence4>::value ||
                  std::is_same<T, stool::bptree::DynamicBitDequeSequenceA>::value ||
                  std::is_same<T, stool::bptree::DynamicBitDequeSequenceB>::value ||
                  std::is_same<T, stool::bptree::DynamicBitDequeSequenceC>::value ||
                  std::is_same<T, stool::bptree::DynamicBitDequeSequenceD>::value)
    {
        std::vector<bool> buffer;
        uint64_t buffer_size = 10000;

        for (uint64_t i = 0; i < item_num; i++)
        {
            bool b = get_rand_value(mt64) % 2 == 1;
            buffer.push_back(b);
            hash += (int)b;

            if (buffer.size() >= buffer_size)
            {
                dynamic_bit_sequence.push_many(buffer);
                buffer.clear();
            }
        }
        if (buffer.size() > 0)
        {
            dynamic_bit_sequence.push_many(buffer);
            buffer.clear();
        }
    }
    else
    {
        for (uint64_t i = 0; i < item_num; i++)
        {
            bool b = get_rand_value(mt64) % 2 == 1;
            hash += (int)b;
            dynamic_bit_sequence.push_back(b);
        }
    }

    st2 = std::chrono::system_clock::now();
    uint64_t time_construction = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    // if constexpr (std::is_same<T, stool::bptree::SimpleDynamicBitSequence>::value || std::is_same<T, stool::bptree::DynamicBitDequeSequence>::value || std::is_same<T, stool::bptree::DynamicBitDequeSequence2>::value) {
    //     density_when_build_is_complete = dynamic_bit_sequence.density();
    // }

    std::cout << "Checksum: " << hash << std::endl;
    st1 = std::chrono::system_clock::now();
    if (test_type == "all" || test_type == "insertion")
    {
        std::cout << "Random Insertion..." << std::endl;
        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t pos = get_rand_item_num(mt64);
            bool value = (get_rand_value(mt64) % 2) == 1;
            dynamic_bit_sequence.insert(pos, value);
            hash += value + pos;
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_insertion = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::cout << "Checksum: " << hash << std::endl;
    st1 = std::chrono::system_clock::now();
    if (test_type == "all" || test_type == "deletion")
    {
        std::cout << "Random Deletion..." << std::endl;
        int64_t n_delete = dynamic_bit_sequence.size() - query_num;
        if (n_delete < 0)
        {
            throw std::runtime_error("n_delete < 0");
        }
        std::uniform_int_distribution<uint64_t> get_rand_for_delete(0, n_delete);

        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t pos = get_rand_for_delete(mt64);
            dynamic_bit_sequence.remove(pos);
            hash += pos;
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_deletion = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    uint64_t count1 = dynamic_bit_sequence.count_c(true);
    std::uniform_int_distribution<uint64_t> get_rand_bits(0, count1 - 1);

    std::cout << "Checksum: " << hash << std::endl;

    /*
    dyn::__time_count = 0;
    dyn::__time_count_counter = 0;
    dyn::__size_count = 0;
    stool::bptree::time_count2 = 0;
    */

    st1 = std::chrono::system_clock::now();
    if (test_type == "all" || test_type == "access")
    {
        std::cout << "access1..." << std::endl;

        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t m = get_rand_item_num(mt64);
            hash += dynamic_bit_sequence.at(m);
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_access = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    if constexpr (std::is_same<T, stool::bptree::SimpleDynamicBitSequence>::value ||
                  std::is_same<T, stool::bptree::DynamicBitDequeSequence>::value || std::is_same<T, stool::bptree::DynamicBitDequeSequence2>::value || std::is_same<T, stool::bptree::DynamicBitDequeSequenceA>::value ||
                  std::is_same<T, stool::bptree::DynamicBitDequeSequenceB>::value || std::is_same<T, stool::bptree::DynamicBitDequeSequenceC>::value || std::is_same<T, stool::bptree::DynamicBitDequeSequenceD>::value)
    {
        dynamic_bit_sequence.print_debug_info();
    }
    /*
            if( dyn::__time_count_counter > 0){
                std::cout << "dynl::__time_count: " << (double)dyn::__time_count / 1000000.0 << " ms" << std::endl;
                std::cout << "dyn::__time_count_counter: " << dyn::__time_count_counter << std::endl;
                std::cout << "dyn::__size_count: " << dyn::__size_count << std::endl;

                std::cout << "average: " << (double)dyn::__time_count / dyn::__time_count_counter << " ns" << std::endl;
                std::cout << "average size: " << (double)dyn::__size_count / dyn::__time_count_counter << " elements" << std::endl;

            }
            */

    std::cout << "Checksum: " << hash << std::endl;

    st1 = std::chrono::system_clock::now();
    if (test_type == "all" || test_type == "rank")
    {
        std::cout << "rank1..." << std::endl;
        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t m = get_rand_item_num(mt64);
            uint64_t value = dynamic_bit_sequence.rank1(m);
            hash += value;
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_psum = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    // std::cout << count1 << std::endl;

    std::cout << "Checksum: " << hash << std::endl;

    st1 = std::chrono::system_clock::now();
    if (test_type == "all" || test_type == "select")
    {
        std::cout << "select1..." << std::endl;
        for (uint64_t i = 0; i < query_num; i++)
        {
            uint64_t m = get_rand_bits(mt64);
            uint64_t value = dynamic_bit_sequence.select1(m);
            hash += value;
        }
    }
    st2 = std::chrono::system_clock::now();
    uint64_t time_search = std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

    std::cout << "\033[36m";
    std::cout << "=============RESULT===============" << std::endl;
    std::cout << "Test: " << name << std::endl;
    std::cout << "item_num = " << item_num << ", query_num = " << query_num << ", seed = " << seed << std::endl;
    std::cout << "Checksum            : " << hash << std::endl;
    std::cout << "Construction Time   : " << (time_construction / (1000 * 1000)) << "[ms] (Avg: " << (time_construction / item_num) << "[ns])" << std::endl;
    std::cout << "Access Time         : " << (time_access / (1000 * 1000)) << "[ms] (Avg: " << (time_access / query_num) << "[ns])" << std::endl;
    std::cout << "Rank Time           : " << (time_psum / (1000 * 1000)) << "[ms] (Avg: " << (time_psum / query_num) << "[ns])" << std::endl;
    std::cout << "Select Time         : " << (time_search / (1000 * 1000)) << "[ms] (Avg: " << (time_search / query_num) << "[ns])" << std::endl;
    std::cout << "Insertion Time      : " << (time_insertion / (1000 * 1000)) << "[ms] (Avg: " << (time_insertion / query_num) << "[ns])" << std::endl;
    std::cout << "Deletion Time       : " << (time_deletion / (1000 * 1000)) << "[ms] (Avg: " << (time_deletion / query_num) << "[ns])" << std::endl;

    /*
    if constexpr (std::is_same<T, stool::bptree::SimpleDynamicBitSequence>::value || std::is_same<T, stool::bptree::DynamicBitDequeSequence>::value || std::is_same<T, stool::bptree::DynamicBitDequeSequence2>::value) {
        std::cout << "Density of the B-tree when the build is complete: " << density_when_build_is_complete << std::endl;
        dynamic_bit_sequence.print_information_about_performance();
        dynamic_bit_sequence.print_memory_usage();
    }
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
    p.add<uint64_t>("query_num", 'q', "query_num", false, 1000000);
    p.add<uint64_t>("seed", 's', "seed", false, 0);

    p.parse_check(argc, argv);
    std::string index_name = p.get<std::string>("index_name");
    std::string query_type = p.get<std::string>("test");
    uint64_t item_num = p.get<uint64_t>("item_num");
    uint64_t query_num = p.get<uint64_t>("query_num");
    uint64_t seed = p.get<uint64_t>("seed");

    if (index_name == "BTreePlusAlpha")
    {
        stool::bptree::SimpleDynamicBitSequence dbs;
        dynamic_bit_operation_test(dbs, "stool::bptree::SimpleDynamicBitSequence", query_type, item_num, query_num, seed);
    }
    else if (index_name == "BTreePlusAlpha1")
    {
        stool::bptree::DynamicBitDequeSequence1 dbs;
        dynamic_bit_operation_test(dbs, "stool::bptree::DynamicBitDequeSequence1", query_type, item_num, query_num, seed);
    }
    else if (index_name == "BTreePlusAlpha2")
    {
        stool::bptree::DynamicBitDequeSequence2 dbs;
        dynamic_bit_operation_test(dbs, "stool::bptree::DynamicBitDequeSequence2", query_type, item_num, query_num, seed);
    }
    else if (index_name == "BTreePlusAlpha3")
    {
        stool::bptree::DynamicBitDequeSequence3 dbs;
        dynamic_bit_operation_test(dbs, "stool::bptree::DynamicBitDequeSequence3", query_type, item_num, query_num, seed);
    }
    else if (index_name == "BTreePlusAlpha4")
    {
        stool::bptree::DynamicBitDequeSequence4 dbs;
        dynamic_bit_operation_test(dbs, "stool::bptree::DynamicBitDequeSequence4", query_type, item_num, query_num, seed);
    }
    else if (index_name == "BTreePlusAlphaA")
    {
        stool::bptree::DynamicBitDequeSequenceA dbs;
        dynamic_bit_operation_test(dbs, "stool::bptree::DynamicBitDequeSequenceA", query_type, item_num, query_num, seed);
    }
    else if (index_name == "BTreePlusAlphaB")
    {
        stool::bptree::DynamicBitDequeSequenceB dbs;
        dynamic_bit_operation_test(dbs, "stool::bptree::DynamicBitDequeSequenceB", query_type, item_num, query_num, seed);
    }
    else if (index_name == "BTreePlusAlphaC")
    {
        stool::bptree::DynamicBitDequeSequenceC dbs;
        dynamic_bit_operation_test(dbs, "stool::bptree::DynamicBitDequeSequenceC", query_type, item_num, query_num, seed);
    }
    else if (index_name == "BTreePlusAlphaD")
    {
        stool::bptree::DynamicBitDequeSequenceD dbs;
        dynamic_bit_operation_test(dbs, "stool::bptree::DynamicBitDequeSequenceD", query_type, item_num, query_num, seed);
    }
    else if (index_name == "DYNAMIC")
    {
        DynSucBVWrapper dbs;
        dynamic_bit_operation_test(dbs, "DynSucBVWrapper", query_type, item_num, query_num, seed);
    }
    else if (index_name == "bit_vector")
    {
#if defined(__x86_64__)
        BVBVWrapper dbs;
        dynamic_bit_operation_test(dbs, "BVBVWrapper", query_type, item_num, query_num, seed);
        // dcc_dynamic_bit_test(item_num, query_num, seed);
#else
        std::cout << "This code is not supported on this architecture." << std::endl;
#endif
    }
}
