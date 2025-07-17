#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>

#include "../../include/b_tree_plus_alpha.hpp"

#include "./naive_permutation.hpp"
#include <random>

namespace stool
{
    class PermutationTest
    {
    public:
        static std::vector<uint64_t> create_random_permutation(uint64_t num, std::mt19937_64 &mt64)
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
        static bool equal_check(NaivePermutation &npom, bptree::DynamicPermutation &pom)
        {
            pom.get_pi_tree().verify();
            pom.get_inverse_pi_tree().verify();

            std::vector<uint64_t> pom_pi_vector = pom.get_pi_vector();
            std::vector<uint64_t> pom_inv_pi_vector = pom.get_inverse_pi_vector();

            stool::EqualChecker::equal_check(pom_pi_vector, npom.pi_list, "Check1");
            stool::EqualChecker::equal_check(pom_inv_pi_vector, npom.inverse_pi_list, "Check2");
            return true;
        }
        static void permutation_test_random_insert(NaivePermutation &npom, bptree::DynamicPermutation &pom, bool verification, std::mt19937_64 &mt64)
        {
            uint64_t size = npom.pi_list.size();
            std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, size);
            uint64_t pi_index = get_rand_uni_int(mt64);
            uint64_t rev_pi_index = get_rand_uni_int(mt64);

            npom.insert(pi_index, rev_pi_index);
            pom.insert(pi_index, rev_pi_index);

            if (verification)
            {
                PermutationTest::equal_check(npom, pom);
            }

            // stool::EqualChecker::equal_check(pom_value_vector, npom.value_list);
        }
        static void permutation_test_random_delete(stool::NaivePermutation &npom, stool::bptree::DynamicPermutation &pom, bool verification, std::mt19937_64 &mt64)
        {
            uint64_t size = npom.pi_list.size();
            std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, size - 1);
            uint64_t pi_index = get_rand_uni_int(mt64);

            // std::cout << "Delete: pi = " << pi_index << std::endl;

            npom.erase(pi_index);
            pom.erase(pi_index);

            if (verification)
            {
                PermutationTest::equal_check(npom, pom);
            }
        }

        static void random_build_test(bptree::DynamicPermutation &dp, uint64_t num, uint64_t seed)
        {
            std::mt19937_64 mt64(seed);
            std::vector<uint64_t> permutation = create_random_permutation(num, mt64);

            // bptree::DynamicPermutation dp;
            dp.build(permutation.begin(), permutation.end(), permutation.size(), stool::Message::NO_MESSAGE);
            std::vector<uint64_t> result = dp.get_pi_vector();

            stool::EqualChecker::equal_check(permutation, result);
        }
        static void random_insert_test(bptree::DynamicPermutation &dp, stool::NaivePermutation &npom, uint64_t num, uint64_t seed)
        {
            std::mt19937_64 mt64(seed);
            dp.clear();
            npom.clear();

            for (uint64_t i = 0; i < num; i++)
            {
                permutation_test_random_insert(npom, dp, false, mt64);
            }

            // std::cout << "##" << std::endl;

            // dp.print();

            dp.sort_leaf_containers();

            // dp.print();

            PermutationTest::equal_check(npom, dp);
        }
        static void random_remove_test(bptree::DynamicPermutation &dp, stool::NaivePermutation &npom, uint64_t seed)
        {
            std::mt19937_64 mt64(seed);
            while (npom.pi_list.size() > 0)
            {
                permutation_test_random_delete(npom, dp, false, mt64);
            }
            PermutationTest::equal_check(npom, dp);
        }
        static void random_insert_and_remove_test(bptree::DynamicPermutation &dp, stool::NaivePermutation &npom, uint64_t num, uint64_t seed)
        {
            std::mt19937_64 mt64(seed);
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
            PermutationTest::equal_check(npom, dp);
        }

        /*
        static void permutation_test1(uint64_t num, uint64_t seed)
        {
            std::mt19937_64 mt64(seed);
            std::vector<uint64_t> permutation = create_random_permutation(num, mt64);

            bptree::DynamicPermutation dp;
            dp.build(permutation.begin(), permutation.end(), permutation.size(), 16);
            std::vector<uint64_t> result = dp.get_pi_vector();

            //Printer::print(result);
            stool::EqualChecker::equal_check(permutation, result);
        }
        */

        static void permutation_builder_test(bptree::DynamicPermutation &dp, uint64_t num, uint64_t seed)
        {
            std::mt19937_64 mt64(seed);
            std::vector<uint64_t> permutation = create_random_permutation(num, mt64);
            std::vector<uint64_t> inv_permutation;
            inv_permutation.resize(permutation.size());
            for (uint64_t i = 0; i < permutation.size(); i++)
            {
                inv_permutation[permutation[i]] = i;
            }

            // bptree::DynamicPermutation dp;
            bptree::DynamicPermutationBuilder dpb;
            dpb.initialize(dp, permutation.size(), dp.get_max_degree());

            for (int64_t i = inv_permutation.size() - 1; i >= 0; i--)
            {
                dpb.push_front(inv_permutation[i]);
            }
            dpb.finish();

            std::vector<uint64_t> result = dp.get_pi_vector();

            stool::EqualChecker::equal_check(permutation, result, "PermutationBuilderTest");
        }
        static void load_write_file_test(stool::bptree::DynamicPermutation &dp)
        {
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

        static void load_write_bits_test(stool::bptree::DynamicPermutation &dp)
        {
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
    };

}