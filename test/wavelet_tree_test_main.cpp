#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
#include "../include/b_tree_plus_alpha.hpp"
#include "../modules/stool/test/sources/template/dynamic_string_test.hpp"
#include "../modules/stool/test/sources/template/string_test.hpp"


void build_test(stool::bptree::DynamicWaveletTree &ds, stool::NaiveDynamicString &dyn_text, const std::vector<uint8_t> &text, const std::vector<uint8_t> &alphabet)
{

    auto tmp = stool::bptree::DynamicWaveletTree::build(text, alphabet);
    ds.swap(tmp);
    for (auto c : text)
    {
        dyn_text.push_back(c);
    }
    std::vector<uint8_t> test_str = ds.to_u8_vector();
    stool::EqualChecker::equal_check(test_str, dyn_text.text);

}

void rank_test(const stool::bptree::DynamicWaveletTree &ds, const stool::NaiveDynamicString &text, const std::vector<uint8_t> &alphabet)
{
    for (auto c : alphabet)
    {
        for (int64_t i = 0; i <= text.size(); i++)
        {
            uint64_t rank1 = ds.one_based_rank(i, c);
            uint64_t rank2 = stool::StringFunctions::one_based_rank_query(text.text, i, c);
            if (rank1 != rank2)
            {
                std::cout << "rank1: " << rank1 << ", rank2: " << rank2 << std::endl;
                throw -1;
            }
            assert(rank1 == rank2);
        }
    }
}
void select_test(const stool::bptree::DynamicWaveletTree &ds, const stool::NaiveDynamicString &text, [[maybe_unused]] const std::vector<uint8_t> &alphabet)
{

    for (int64_t i = 0; i < text.size(); i++)
    {
        uint8_t c = text.text[i];
        int64_t rank = stool::StringFunctions::one_based_rank_query(text.text, i + 1, c);
        assert(rank > 0);
        int64_t nth = ds.select(rank - 1, c);
        if (nth != i)
        {
            std::cout << "nth: " << nth << ", i: " << i << std::endl;
            throw -1;
        }
        assert(nth == i);
    }

}
void insert_test(stool::bptree::DynamicWaveletTree &ds, stool::NaiveDynamicString &text, const std::vector<uint8_t> &alphabet, uint64_t insert_char_count, uint64_t seed)
{

    std::mt19937_64 mt64(seed);
    std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, text.size() + insert_char_count);
    for (uint64_t i = 0; i < insert_char_count; i++)
    {
        uint64_t c = alphabet[get_rand_uni_int(mt64) % alphabet.size()];
        uint64_t nth = get_rand_uni_int(mt64) % (text.size() + 1);
        text.insert_string(nth, c);
        ds.insert(nth, c);
    }

    std::vector<uint8_t> test_str = ds.to_u8_vector();
    stool::EqualChecker::equal_check(test_str, text.text);

}
void remove_test(stool::bptree::DynamicWaveletTree &ds, stool::NaiveDynamicString &text, uint64_t seed)
{
    std::mt19937_64 mt64(seed);
    std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, text.size());
    while (text.size() > 0)
    {
        uint64_t nth = get_rand_uni_int(mt64) % (text.size());
        text.delete_string(nth, 1);
        ds.remove(nth);

        if (text.size() % 100 == 0)
        {
            std::vector<uint8_t> test_str = ds.to_u8_vector();
            stool::EqualChecker::equal_check(test_str, text.text);
        }
    }
}

void save_and_load_test(stool::bptree::DynamicWaveletTree &ds)
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

        stool::bptree::DynamicWaveletTree::store_to_file(ds, os);
    }
    stool::bptree::DynamicWaveletTree ds2;

    {
        std::ifstream ifs;
        ifs.open(file_name, std::ios::binary);
        if (!ifs)
        {
            std::cerr << "Error: Could not open file for reading." << std::endl;
            throw std::runtime_error("File open error");
        }
        stool::bptree::DynamicWaveletTree tmp = stool::bptree::DynamicWaveletTree::load_from_file(ifs);

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

    if (mode == 0)
    {
    }
    else if (mode == 1)
    {
        uint64_t seed = 0;
        std::mt19937_64 mt64(seed);

        /*
        bool detailed_check = false;
        uint64_t max_text_size = 10000;
        uint64_t number_of_access = 100;
        uint64_t number_of_insertion = 100;
        uint64_t number_of_removal = 100;
        */

        std::function<void(stool::bptree::DynamicWaveletTree &, const std::vector<uint8_t> &, const std::vector<uint8_t> &)> builder_function = [](stool::bptree::DynamicWaveletTree &dwt, const std::vector<uint8_t> &text, const std::vector<uint8_t> &alphabet){
            auto _tmp = stool::bptree::DynamicWaveletTree::build(text, alphabet);
            dwt.swap(_tmp);
        };

        {
            //stool::bptree::DynamicWaveletTree dwt;
            /*
            stool::StringTest::access_character_test(max_text_size, number_of_access, 100, builder_function, seed);
            stool::DynamicStringTest::insert_character_test(max_text_size, number_of_insertion, 100, builder_function, detailed_check, seed);
            stool::DynamicStringTest::remove_character_test(max_text_size, number_of_removal, 100, builder_function, detailed_check, seed);
            */

        }

        for (uint64_t type = 0; type <= stool::Alphabet::get_max_alphabet_type(); type++)
        {
            uint64_t len = 2;
            std::vector<uint8_t> chars = stool::Alphabet::create_alphabet(type);
            stool::DebugPrinter::print_characters(chars, "Alphabet");
                

            while (len < 10000)
            {
                std::vector<uint8_t> text = stool::RandomString::create_random_sequence<uint8_t>(len, chars, seed);
                stool::NaiveDynamicString dyn_text;

                // stool::Printer::print_string(text);
                // stool::Printer::print_chars("Alphabet", chars);

                stool::bptree::DynamicWaveletTree ds;

                std::cout << "A" << std::flush;
                build_test(ds, dyn_text, text, chars);
                std::cout << "B" << std::flush;
                rank_test(ds, dyn_text, chars);
                std::cout << "C" << std::flush;
                select_test(ds, dyn_text, chars);
                std::cout << "D" << std::flush;
                save_and_load_test(ds);
                //std::cout << "E" << std::flush;
                //insert_test(ds, dyn_text, chars, 1000, seed++);
                std::cout << "F" << std::flush;
                //remove_test(ds, dyn_text, seed++);
                //std::cout << "G" << std::flush;
                len *= 4;
                seed++;
            }
                std::cout << std::endl;

        }
    }
}
