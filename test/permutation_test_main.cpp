#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
#include "stool/include/light_stool.hpp"

#include "include/spsi_test.hpp"
#include "../include/b_tree_plus_alpha.hpp"
#include "include/naive_permutation.hpp"

std::vector<uint64_t> create_random_permutation(uint64_t num, std::mt19937_64 &mt64)
        {
            std::vector<uint64_t> r;
            for (uint64_t i = 0; i < num; i++)
            {
                r.push_back(i);
            }
            std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, num - 1);

            for (uint64_t i = 0; i < num; i++)
            {
                uint64_t pos1 = get_rand_uni_int(mt64);
                uint64_t pos2 = get_rand_uni_int(mt64);
                std::swap(r[pos1], r[pos2]);
            }
            return r;
        }
        bool equal_check(stool::NaivePermutation &npom, stool::bptree::DynamicPermutation &pom)
        {
            pom.get_pi_tree().verify();
            pom.get_inverse_pi_tree().verify();

            std::vector<uint64_t> pom_pi_vector = pom.get_pi_vector();
            std::vector<uint64_t> pom_inv_pi_vector = pom.get_inverse_pi_vector();

            stool::EqualChecker::equal_check(pom_pi_vector, npom.pi_list, "Check1");
            stool::EqualChecker::equal_check(pom_inv_pi_vector, npom.inverse_pi_list, "Check2");
            return true;
        }

void permutation_test_random_delete(stool::NaivePermutation &npom, stool::bptree::DynamicPermutation &pom, bool verification, std::mt19937_64 &mt64)
{
    uint64_t size = npom.pi_list.size();
    std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, size - 1);
    uint64_t pi_index = get_rand_uni_int(mt64);

    // std::cout << "Delete: pi = " << pi_index << std::endl;

    npom.erase(pi_index);
    pom.erase(pi_index);

    if (verification)
    {
        equal_check(npom, pom);
    }
}
void permutation_test_random_insert(stool::NaivePermutation &npom, stool::bptree::DynamicPermutation &pom, bool verification, std::mt19937_64 &mt64)
{
    uint64_t size = npom.pi_list.size();
    std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, size);
    uint64_t pi_index = get_rand_uni_int(mt64);
    uint64_t rev_pi_index = get_rand_uni_int(mt64);

    // pom.print();
    // std::cout << "pi_index: " << pi_index << " rev_pi_index: " << rev_pi_index << std::endl;

    npom.insert(pi_index, rev_pi_index);
    pom.insert(pi_index, rev_pi_index);

    if (verification)
    {
        try
        {
            equal_check(npom, pom);
        }
        catch (std::logic_error &e)
        {
            pom.print();
            throw e;
        }
    }

    // stool::EqualChecker::equal_check(pom_value_vector, npom.value_list);
}

void permutation_builder_test(uint64_t max_element_count, uint64_t trial_count, [[maybe_unused]] bool detailed_check, uint64_t seed)
{

    std::cout << "permutation_builder_test: " << std::flush;
    for (uint64_t num = 16; num < max_element_count; num *= 2)
    {
        std::cout << "+" << std::flush;

        for (uint64_t x = 0; x < trial_count; x++)
        {
            stool::bptree::DynamicPermutation dp;
            std::mt19937_64 mt64(seed++);

            std::vector<uint64_t> permutation = create_random_permutation(num, mt64);
            std::vector<uint64_t> inv_permutation;
            inv_permutation.resize(permutation.size());
            for (uint64_t i = 0; i < permutation.size(); i++)
            {
                inv_permutation[permutation[i]] = i;
            }

            // bptree::DynamicPermutation dp;
            stool::bptree::DynamicPermutationBuilder dpb;
            dpb.initialize(dp, permutation.size());

            for (int64_t i = inv_permutation.size() - 1; i >= 0; i--)
            {
                dpb.push_front(inv_permutation[i]);
            }
            dpb.finish();

            std::vector<uint64_t> result = dp.get_pi_vector();

            stool::EqualChecker::equal_check(permutation, result, "PermutationBuilderTest");
        }
    }
    std::cout << "[DONE]" << std::endl;
}

void random_insert_and_remove_test2(uint64_t max_element_count, uint64_t trial_count, [[maybe_unused]] bool detailed_check, uint64_t seed)
{
    std::cout << "random_insert_and_remove_test(2): " << std::flush;
    for (uint64_t num = 16; num < max_element_count; num *= 2)
    {
        std::cout << "+" << std::flush;

        for (uint64_t x = 0; x < trial_count; x++)
        {
            stool::bptree::DynamicPermutation dp;
            stool::NaivePermutation npom;
            std::mt19937_64 mt64(seed++);
            std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, 2);

            for (uint64_t i = 0; i < num; i++)
            {
                bool b = get_rand_uni_int(mt64) <= 1;
                if (b)
                {
                    permutation_test_random_insert(npom, dp, false, mt64);
                }
                else if (!b && npom.size() != 0)
                {
                    permutation_test_random_delete(npom, dp, false, mt64);
                }
            }
            equal_check(npom, dp);
        }
    }
    std::cout << "[DONE]" << std::endl;
}

void random_insert_and_remove_test1(uint64_t max_element_count, uint64_t trial_count, [[maybe_unused]] bool detailed_check, uint64_t seed)
{

    std::cout << "random_insert_test(1): " << std::flush;
    for (uint64_t num = 16; num < max_element_count; num *= 2)
    {
        std::cout << "+" << std::flush;

        for (uint64_t x = 0; x < trial_count; x++)
        {
            std::mt19937_64 mt64(seed++);
            stool::bptree::DynamicPermutation dp;
            stool::NaivePermutation npom;
            for (uint64_t i = 0; i < num; i++)
            {
                permutation_test_random_insert(npom, dp, detailed_check, mt64);
            }
            dp.sort_leaf_containers();

            try
            {
                equal_check(npom, dp);
            }
            catch (std::logic_error &e)
            {
                dp.print();
                throw e;
            }
            while (npom.pi_list.size() > 0)
            {
                permutation_test_random_delete(npom, dp, detailed_check, mt64);
            }
            equal_check(npom, dp);
        }
    }
    std::cout << "[DONE]" << std::endl;
}

void random_build_test(uint64_t max_element_count, uint64_t trial_count, uint64_t seed)
{
    std::cout << "random_build_test: " << std::flush;
    std::mt19937_64 mt64(seed);

    for (uint64_t num = 16; num < max_element_count; num *= 2)
    {
        std::cout << "+" << std::flush;

        for (uint64_t x = 0; x < trial_count; x++)
        {
            stool::bptree::DynamicPermutation dp;
            std::vector<uint64_t> permutation = create_random_permutation(num, mt64);
            dp.build(permutation.begin(), permutation.end(), permutation.size(), stool::Message::NO_MESSAGE);
            std::vector<uint64_t> result = dp.get_pi_vector();

            try
            {
                stool::EqualChecker::equal_check(permutation, result);
            }
            catch (std::logic_error &e)
            {
                std::cout << std::endl;
                stool::DebugPrinter::print_integers(permutation, "permutation");
                stool::DebugPrinter::print_integers(result, "test_permutation");
                dp.print();

                std::cout << "Error: " << e.what() << std::endl;
                throw e;
            }
        }
    }
    std::cout << "[DONE]" << std::endl;

    // bptree::DynamicPermutation dp;
}

void load_write_file_test(uint64_t max_element_count, uint64_t trial_count, [[maybe_unused]] bool detailed_check, uint64_t seed)
{
    std::cout << "load_write_file_test: " << std::flush;
    std::mt19937_64 mt64(seed);

    for (uint64_t num = 16; num < max_element_count; num *= 2)
    {
        std::cout << "+" << std::flush;

        for (uint64_t x = 0; x < trial_count; x++)
        {
            stool::bptree::DynamicPermutation dp;
            std::vector<uint64_t> permutation = create_random_permutation(num, mt64);
            dp.build(permutation.begin(), permutation.end(), permutation.size(), stool::Message::NO_MESSAGE);

            {
                std::ofstream os;
                os.open("permutation_text.bits", std::ios::binary);
                if (!os)
                {
                    std::cerr << "Error: Could not open file for writing." << std::endl;
                    throw std::runtime_error("File open error");
                }
                stool::bptree::DynamicPermutation::save(dp, os);
            }

            stool::bptree::DynamicPermutation dp2;

            {
                std::ifstream ifs;
                ifs.open("permutation_text.bits", std::ios::binary);
                if (!ifs)
                {
                    std::cerr << "Error: Could not open file for reading." << std::endl;
                    throw std::runtime_error("File open error");
                }

                auto tmp = stool::bptree::DynamicPermutation::build_from_data(ifs);
                dp2.swap(tmp);
            }
            std::remove("permutation_text.bits");

            if (dp.size() != dp2.size())
            {
                assert(false);
                throw -1;
            }

            for (uint64_t i = 0; i < dp.size(); i++)
            {
                if (dp.access(i) != dp2.access(i))
                {
                    assert(false);
                    throw -1;
                }
                if (dp.inverse(i) != dp2.inverse(i))
                {
                    assert(false);
                    throw -1;
                }
            }
        }
    }
    std::cout << "[DONE]" << std::endl;
}
void load_write_bits_test(uint64_t max_element_count, uint64_t trial_count, [[maybe_unused]] bool detailed_check, uint64_t seed)
{
    std::cout << "load_write_bits_test: " << std::flush;
    std::mt19937_64 mt64(seed);

    for (uint64_t num = 16; num < max_element_count; num *= 2)
    {
        std::cout << "+" << std::flush;

        for (uint64_t x = 0; x < trial_count; x++)
        {
            stool::bptree::DynamicPermutation dp;
            std::vector<uint64_t> permutation = create_random_permutation(num, mt64);
            dp.build(permutation.begin(), permutation.end(), permutation.size(), stool::Message::NO_MESSAGE);

            std::vector<uint8_t> bytes;
            uint64_t pos = 0;

            // std::cout << "save" << std::endl;
            // dp.print();

            stool::bptree::DynamicPermutation::save(dp, bytes, pos);
            // dp.print();

            pos = 0;
            // std::cout << "load" << std::endl;

            stool::bptree::DynamicPermutation dp2 = stool::bptree::DynamicPermutation::build_from_data(bytes, pos);

            if (dp.size() != dp2.size())
            {
                assert(false);
                throw -1;
            }

            // dp.print();
            // dp2.print();

            for (uint64_t i = 0; i < dp.size(); i++)
            {
                if (dp.access(i) != dp2.access(i))
                {
                    assert(false);
                    throw -1;
                }
                if (dp.inverse(i) != dp2.inverse(i))
                {
                    assert(false);
                    throw -1;
                }
            }
        }
    }
    std::cout << "[DONE]" << std::endl;
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

    p.parse_check(argc, argv);
    // uint64_t mode = p.get<uint>("mode");

    uint64_t max_element_count = 10000;
    uint64_t trial_count = 10;
    uint64_t seed = 0;

    random_insert_and_remove_test1(max_element_count, trial_count, false, seed);
    random_insert_and_remove_test2(max_element_count, trial_count, false, seed);
    random_build_test(max_element_count, trial_count, seed);
    permutation_builder_test(max_element_count, trial_count, false, seed);
    load_write_file_test(max_element_count, trial_count, false, seed);
    load_write_bits_test(max_element_count, trial_count, false, seed);

}
