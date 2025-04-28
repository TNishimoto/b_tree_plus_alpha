#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
// #include "qgram_tree.h"
// #include "include/debug/debug.hpp"
#include "../include/b_tree_plus_alpha.hpp"

void build_test(stool::sequence::DynamicWaveletTree &ds, stool::NaiveDynamicString &dyn_text, const std::vector<uint8_t> &text, const std::vector<uint8_t> &alphabet)
{

    auto tmp = stool::sequence::DynamicWaveletTree::build(text, alphabet);
    ds.swap(tmp);
    for (auto c : text)
    {
        dyn_text.push_back(c);
    }
    std::vector<uint8_t> test_str = ds.to_uint8_str();
    stool::equal_check(test_str, dyn_text.text);

}

void rank_test(const stool::sequence::DynamicWaveletTree &ds, const stool::NaiveDynamicString &text, const std::vector<uint8_t> &alphabet)
{
    for (auto c : alphabet)
    {
        for (int64_t i = 0; i <= text.size(); i++)
        {
            uint64_t rank1 = ds.rank(i, c);
            uint64_t rank2 = stool::StringFunctions::rank_query(text.text, i, c);
            assert(rank1 == rank2);
        }
    }
}
void select_test(const stool::sequence::DynamicWaveletTree &ds, const stool::NaiveDynamicString &text, [[maybe_unused]] const std::vector<uint8_t> &alphabet)
{

    for (int64_t i = 0; i < text.size(); i++)
    {
        uint8_t c = text.text[i];
        int64_t rank = stool::StringFunctions::rank_query(text.text, i + 1, c);
        assert(rank > 0);
        int64_t nth = ds.select(rank - 1, c);

        assert(nth == i);
    }

}
void insert_test(stool::sequence::DynamicWaveletTree &ds, stool::NaiveDynamicString &text, const std::vector<uint8_t> &alphabet, uint64_t insert_char_count, uint64_t seed)
{

    std::mt19937_64 mt64(seed);
    std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, text.size() + insert_char_count);
    for (uint64_t i = 0; i < insert_char_count; i++)
    {
        uint64_t c = alphabet[get_rand_uni_int(mt64) % alphabet.size()];
        uint64_t nth = get_rand_uni_int(mt64) % (text.size() + 1);
        text.insert_char(nth, c);
        ds.insert(nth, c);
    }

    std::vector<uint8_t> test_str = ds.to_uint8_str();
    stool::equal_check(test_str, text.text);

}
void remove_test(stool::sequence::DynamicWaveletTree &ds, stool::NaiveDynamicString &text, uint64_t seed)
{
    std::mt19937_64 mt64(seed);
    std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, text.size());
    while (text.size() > 0)
    {
        uint64_t nth = get_rand_uni_int(mt64) % (text.size());
        text.delete_char(nth);
        ds.remove(nth);

        if (text.size() % 100 == 0)
        {
            std::vector<uint8_t> test_str = ds.to_uint8_str();
            stool::equal_check(test_str, text.text);
        }
    }

}

void save_and_load_test(stool::sequence::DynamicWaveletTree &ds)
{
    std::string file_name = "byte_text" + std::to_string(ds.size()) + "_" + std::to_string(ds.get_alphabet_size()) + ".bits";
    {
        std::ofstream os;
        os.open(file_name, std::ios::binary);
        if (!os)
        {
            std::cerr << "Error: Could not open file for writing." << std::endl;
            throw std::runtime_error("File open error");
        }

        stool::sequence::DynamicWaveletTree::save(ds, os);
    }
    stool::sequence::DynamicWaveletTree ds2;

    {
        std::ifstream ifs;
        ifs.open(file_name, std::ios::binary);
        if (!ifs)
        {
            std::cerr << "Error: Could not open file for reading." << std::endl;
            throw std::runtime_error("File open error");
        }
        stool::sequence::DynamicWaveletTree tmp = stool::sequence::DynamicWaveletTree::build_from_data(ifs);

        ds2.swap(tmp);
    }

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

    if (mode == 0)
    {
    }
    else if (mode == 1)
    {
        uint64_t seed = 0;
        std::mt19937_64 mt64(seed);

        for (uint64_t type = 0; type <= stool::UInt8VectorGenerator::get_max_alphabet_type(); type++)
        {
            uint64_t len = 2;
            std::vector<uint8_t> chars = stool::UInt8VectorGenerator::create_alphabet(type);
            stool::Printer::print_chars("Alphabet", chars);
                

            while (len < 10000)
            {
                std::vector<uint8_t> text = stool::UInt8VectorGenerator::create_random_sequence(len, chars, seed);
                stool::NaiveDynamicString dyn_text;

                // stool::Printer::print_string(text);
                // stool::Printer::print_chars("Alphabet", chars);

                stool::sequence::DynamicWaveletTree ds;

                std::cout << "A" << std::flush;
                build_test(ds, dyn_text, text, chars);
                std::cout << "B" << std::flush;
                rank_test(ds, dyn_text, chars);
                std::cout << "C" << std::flush;
                select_test(ds, dyn_text, chars);
                std::cout << "D" << std::flush;
                save_and_load_test(ds);
                std::cout << "E" << std::flush;
                insert_test(ds, dyn_text, chars, 1000, seed++);
                std::cout << "F" << std::flush;
                remove_test(ds, dyn_text, seed++);
                std::cout << "G" << std::flush;
                len *= 4;
                seed++;
            }
                std::cout << std::endl;

        }
    }
}
