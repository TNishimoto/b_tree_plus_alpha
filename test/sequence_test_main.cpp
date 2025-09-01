#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cstdio>
#include "../include/b_tree_plus_alpha.hpp"
#include "stool/test/sources/template/dynamic_integer_test.hpp"

using SEQ = stool::bptree::DynamicSequence64<stool::NaiveFLCVector<>, 62, 256>;

void build_test(SEQ &ds, std::vector<uint64_t> &dyn_text, const std::vector<uint64_t> &text)
{

    auto tmp = SEQ::build(text);
    ds.swap(tmp);
    for (auto c : text)
    {
        dyn_text.push_back(c);
    }
    std::vector<uint64_t> test_str = ds.to_vector();
    stool::EqualChecker::equal_check(test_str, dyn_text);

}

void insert_test(SEQ &ds, std::vector<uint64_t> &text, uint64_t insert_char_count, uint64_t seed)
{

    std::mt19937_64 mt64(seed);
    std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, text.size() + insert_char_count);
    for (uint64_t i = 0; i < insert_char_count; i++)
    {
        uint64_t c = get_rand_uni_int(mt64);
        uint64_t nth = get_rand_uni_int(mt64) % (text.size() + 1);
        text.insert(text.begin() + nth, c);
        ds.insert(nth, c);
    }

    std::vector<uint64_t> test_str = ds.to_vector();
    stool::EqualChecker::equal_check(test_str, text);

}
void remove_test(SEQ &ds, std::vector<uint64_t> &text, uint64_t seed)
{
    std::mt19937_64 mt64(seed);
    std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, text.size());
    while (text.size() > 0)
    {
        uint64_t nth = get_rand_uni_int(mt64) % (text.size());
        text.erase(text.begin() + nth);
        ds.remove(nth);

        if (text.size() % 100 == 0)
        {
            std::vector<uint64_t> test_str = ds.to_vector();
            stool::EqualChecker::equal_check(test_str, text);
        }
    }

}

void save_and_load_test(SEQ &ds)
{
    std::string file_name = "byte_text" + std::to_string(ds.size()) + ".bits";
    {
        std::ofstream os;
        os.open(file_name, std::ios::binary);
        if (!os)
        {
            std::cerr << "Error: Could not open file for writing." << std::endl;
            throw std::runtime_error("File open error");
        }

        SEQ::save(ds, os);
    }
    SEQ ds2;

    {
        std::ifstream ifs;
        ifs.open(file_name, std::ios::binary);
        if (!ifs)
        {
            std::cerr << "Error: Could not open file for reading." << std::endl;
            throw std::runtime_error("File open error");
        }
        SEQ tmp = SEQ::build_from_data(ifs);

        ds2.swap(tmp);
    }
    std::remove(file_name.c_str());

    if (ds.size() != ds2.size())
    {
        assert(false);
        throw -1;
    }

    for (uint64_t i = 0; i < ds.size(); i++)
    {
        bool b1 = ds[i];
        bool b2 = ds2[i];
        if (b1 != b2)
        {
            assert(false);
            throw -1;
        }
    }
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
    p.add<uint>("mode", 'm', "mode", false, 1);

    p.parse_check(argc, argv);
    uint64_t mode = p.get<uint>("mode");

    uint64_t seed = 0;
    uint64_t seq_len = 10000;
    uint64_t number_of_trials = 100;
    uint64_t max_value = 1000000;

    using SEQ = stool::bptree::SimpleDynamicSequence64;

    stool::DynamicIntegerTest::build_test<SEQ>(seq_len, max_value, number_of_trials, seed);
    //stool::DynamicIntegerTest::psum_test<SEQ>(seq_len, max_value, number_of_trials, seed);
    //stool::DynamicIntegerTest::search_test<SEQ>(seq_len, max_value, number_of_trials, seed);
    stool::DynamicIntegerTest::load_write_file_test<SEQ>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::load_write_bits_test<SEQ>(seq_len, max_value, number_of_trials, seed, false);

    stool::DynamicIntegerTest::push_and_pop_test<SEQ>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::insert_test<SEQ>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::remove_test<SEQ>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::replace_test<SEQ>(seq_len, max_value, number_of_trials, seed, false);
    stool::DynamicIntegerTest::random_test<SEQ, false>(seq_len, max_value, number_of_trials, 100, seed, false);


}
