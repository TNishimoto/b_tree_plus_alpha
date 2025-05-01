#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>

#include "../include/b_tree_plus_alpha.hpp"
#include <random>

namespace stool
{
    using namespace prefix_sum;
    class BitTest
    {
    public:
        static std::deque<uint64_t> create_sequence(uint64_t num, std::mt19937_64 &mt64)
        {
            std::deque<uint64_t> r;
            std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, 1);
            while (r.size() < (size_t)num)
            {
                r.push_back(get_rand_uni_int(mt64));
            }
            return r;
        }
        static std::vector<bool> create_sequence2(uint64_t num, std::mt19937_64 &mt64)
        {
            std::vector<bool> r;
            std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, 1);
            while (r.size() < (size_t)num)
            {
                r.push_back(get_rand_uni_int(mt64));
            }
            return r;
        }

        static void test_iterator(stool::sequence::DynamicBitSequence &spsi_container)
        {
            auto r1 = spsi_container.to_vector();

            assert(r1.size() == spsi_container.size());
            for (uint64_t i = 0; i < spsi_container.size(); i++)
            {
                assert(r1[i] == spsi_container[i]);
            }
        }

        template <typename T>
        static void test_rank(T &spsi_container, bool value)
        {
            std::cout << "test_rank" << value << std::endl;
            std::vector<uint64_t> items = spsi_container.to_value_vector();
            for (uint64_t i = 0; i <= items.size(); i++)
            {
                uint64_t rank1 = spsi_container.rank(i, value);
                uint64_t rank2 = stool::StringFunctions::rank_query<uint64_t>(items, i, value);

                if (rank1 != rank2)
                {
                    std::cout << rank1 << "/" << rank2 << "/" << i << "/" << value << std::endl;
                    assert(false);
                    throw std::logic_error("Error");
                }
            }
        }
        /*
        template <typename T>
        static void test_rank1(T &spsi_container)
        {
            std::cout << "test_rank1" << std::endl;
            std::vector<uint64_t> items = spsi_container.to_value_vector();
            uint64_t counter = 0;
            for (uint64_t i = 0; i < items.size(); i++)
            {
                if (items[i] == 1)
                {
                    counter++;
                }
                uint64_t rank = spsi_container.rank1(i);
                if (rank != counter)
                {
                    assert(false);
                    throw std::logic_error("Error");
                }
            }
        }
        */

        template <typename T>
        static void test_select(T &spsi_container, bool bit_value)
        {
            std::cout << "test_select" << bit_value << std::endl;
            std::vector<uint64_t> items = spsi_container.to_value_vector();
            for (uint64_t i = 0; i < items.size(); i++)
            {
                if (items[i] == bit_value)
                {
                    uint64_t rank = stool::StringFunctions::rank_query<uint64_t>(items, i + 1, bit_value);
                    uint64_t nth2 = i;
                    uint64_t nth1 = spsi_container.select(rank - 1, bit_value);

                    if (nth1 != nth2)
                    {
                        std::cout << nth1 << "/" << nth2 << "/" << rank << std::endl;
                        assert(false);
                        throw std::logic_error("Error");
                    }
                }
            }
        }
        template <typename T>
        static void test_select0(T &spsi_container)
        {
            std::cout << "test_select0" << std::endl;
            std::vector<uint64_t> items = spsi_container.to_value_vector();
            uint64_t counter = 0;
            for (uint64_t i = 0; i < items.size(); i++)
            {
                if (items[i] == 0)
                {
                    counter++;
                    uint64_t nth = spsi_container.select0(counter);

                    if (nth != i)
                    {
                        assert(false);
                        throw std::logic_error("Error");
                    }
                }
            }
        }

        template <typename T>
        static void test_psum(T &spsi_container)
        {
            std::cout << "test_psum" << std::endl;
            std::vector<uint64_t> items = spsi_container.to_value_vector();
            uint64_t sum = 0;
            for (uint64_t i = 0; i < items.size(); i++)
            {
                sum += items[i];
                uint64_t psum = spsi_container.psum(i);

                if (sum != psum)
                {
                    assert(false);
                    throw std::logic_error("Error");
                }
            }
        }
        template <typename T>
        static void test_search(T &spsi)
        {
            std::cout << "test_search" << std::endl;

            // uint64_t psum = spsi.psum();
            uint64_t xsum = 1;

            std::vector<uint64_t> items = spsi.to_value_vector();

            for (uint64_t i = 0; i < items.size(); i++)
            {
                for (uint64_t j = 0; j < items[i]; j++)
                {
                    uint64_t nsum = xsum + j;
                    uint64_t idx = spsi.search(nsum);

                    if (idx != i)
                    {
                        assert(false);
                        throw std::logic_error("Error");
                    }
                }
                xsum += items[i];
            }
        }

        template <typename T>
        static void build_test(T &spsi, int64_t num, int64_t seed)
        {
            std::mt19937_64 mt64(seed);
            auto bits = create_sequence2(num, mt64);
            auto tmp = T::build(bits);            
            spsi.swap(tmp);

            assert(bits.size() == spsi.size());
            for(uint64_t i = 0; i < bits.size();i++){
                assert(bits[i] == spsi[i]);
            }

        }


        template <typename T>
        static void insert_and_delete_test(T &spsi, int64_t num, int64_t seed)
        {
            std::cout << "insert_and_delete_test" << std::endl;
            std::mt19937_64 mt64(seed);
            uint64_t checksum = 0;

            std::cout << spsi.to_string() << std::endl;

            while (spsi.size() < (size_t)num)
            {
                std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, spsi.size());
                std::uniform_int_distribution<uint64_t> get_rand_uni_int2(0, 1);
                uint64_t pos = get_rand_uni_int(mt64);
                uint64_t value = get_rand_uni_int2(mt64);
                assert(pos <= spsi.size());
                spsi.insert(pos, value);

                checksum += value;

                if (checksum != spsi.psum())
                {
                    throw std::runtime_error("insert_and_delete_test::Error");
                }
            }

            while (spsi.size() > 0)
            {
                std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, spsi.size() - 1);
                uint64_t pos = get_rand_uni_int(mt64);

                checksum -= spsi.at(pos);

                spsi.remove(pos);

                if (checksum != spsi.psum())
                {
                    throw std::runtime_error("Error");
                }
            }
        }

        static void load_write_test(stool::sequence::DynamicBitSequence &spsi)
        {
            std::vector<uint8_t> bytes;
            uint64_t pos = 0;

            std::cout << "save" << std::endl;
            stool::sequence::DynamicBitSequence::save(spsi, bytes, pos);

            pos = 0;
            std::cout << "load" << std::endl;

            stool::sequence::DynamicBitSequence spsi2 = stool::sequence::DynamicBitSequence::build_from_data(bytes, pos);

            if (spsi.size() != spsi2.size())
            {
                assert(false);
                throw -1;
            }

            for (uint64_t i = 0; i < spsi.size(); i++)
            {
                bool b1 = spsi[i];
                bool b2 = spsi2[i];
                if (b1 != b2)
                {
                    assert(false);
                    throw -1;
                }
            }
        }
        static void load_write_test2(stool::sequence::DynamicBitSequence &spsi)
        {
            {
                std::ofstream os;
                os.open("bit_text.bits", std::ios::binary);
                if (!os)
                {
                    std::cerr << "Error: Could not open file for writing." << std::endl;
                    throw std::runtime_error("File open error");
                }

                std::cout << "save" << std::endl;
                stool::sequence::DynamicBitSequence::save(spsi, os);
            }
            stool::sequence::DynamicBitSequence spsi2;

            {
                std::ifstream ifs;
                ifs.open("bit_text.bits", std::ios::binary);
                if (!ifs)
                {
                    std::cerr << "Error: Could not open file for reading." << std::endl;
                    throw std::runtime_error("File open error");
                }
                std::cout << "load" << std::endl;
                auto tmp = stool::sequence::DynamicBitSequence::build_from_data(ifs);
                spsi2.swap(tmp);

            }
            

            if (spsi.size() != spsi2.size())
            {
                assert(false);
                throw -1;
            }

            for (uint64_t i = 0; i < spsi.size(); i++)
            {
                bool b1 = spsi[i];
                bool b2 = spsi2[i];
                if (b1 != b2)
                {
                    assert(false);
                    throw -1;
                }
            }
        }

        template <typename T>
        static void insert_and_delete_test2(T &spsi, int64_t num, int64_t seed)
        {
            std::cout << "insert_and_delete_test2" << std::endl;
            std::mt19937_64 mt64(seed);
            uint64_t checksum = 0;

            std::cout << spsi.to_string() << std::endl;

            while (spsi.size() < (size_t)num)
            {
                std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, spsi.size());
                std::uniform_int_distribution<uint64_t> get_rand_uni_int2(0, 1);
                uint64_t pos = get_rand_uni_int(mt64);
                uint64_t value = get_rand_uni_int2(mt64);
                assert(pos <= spsi.size());
                spsi.insert(pos, value);

                checksum += value;

                if (checksum != spsi.psum())
                {
                    throw std::runtime_error("insert_and_delete_test::Error");
                }
            }

            spsi.sort_leaf_containers();
            while (spsi.size() > 0)
            {
                std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, spsi.size() - 1);
                uint64_t pos = get_rand_uni_int(mt64);

                checksum -= spsi.at(pos);

                spsi.remove(pos);

                if (checksum != spsi.psum())
                {
                    throw std::runtime_error("Error");
                }
            }
        }
    };

}
