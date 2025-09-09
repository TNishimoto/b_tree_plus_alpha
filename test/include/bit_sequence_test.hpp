#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cstdio>
#include "../../include/b_tree_plus_alpha.hpp"
#include <random>

namespace stool
{
    // using namespace stool::bptree;
    class BitSequenceTest
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

        template <typename BIT_SEQUENCE>
        static void test_iterator(BIT_SEQUENCE &spsi_container, int message_paragraph = stool::Message::SHOW_MESSAGE)
        {
            if (message_paragraph != stool::Message::NO_MESSAGE)
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Testing iterator..." << std::endl;
            }
            auto r1 = spsi_container.to_vector();

            if (r1.size() != spsi_container.size())
            {
                throw std::runtime_error("test_iterator::Error(1)");
            }

            for (uint64_t i = 0; i < spsi_container.size(); i++)
            {
                if (r1[i] != spsi_container[i])
                {
                    std::cout << stool::DebugPrinter::to_integer_string(r1) << std::endl;
                    std::cout << spsi_container.to_string() << std::endl;
                    std::cout << "i: " << i << std::endl;
                    throw std::runtime_error("test_iterator::Error(2)");
                }
            }
        }

        template <typename BIT_SEQUENCE>
        static void test_rank(BIT_SEQUENCE &spsi_container, bool value, int message_paragraph = stool::Message::SHOW_MESSAGE)
        {
            if (message_paragraph != stool::Message::NO_MESSAGE)
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "test_rank" << value << std::endl;
            }
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

        template <typename BIT_SEQUENCE>
        static void test_select(BIT_SEQUENCE &spsi_container, bool bit_value, int message_paragraph = stool::Message::SHOW_MESSAGE)
        {
            if (message_paragraph != stool::Message::NO_MESSAGE)
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "test_select" << bit_value << std::endl;
            }
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
        template <typename BIT_SEQUENCE>
        static void test_select0(BIT_SEQUENCE &spsi_container, int message_paragraph = stool::Message::SHOW_MESSAGE)
        {
            if (message_paragraph != stool::Message::NO_MESSAGE)
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "test_select0" << std::endl;
            }
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

        template <typename BIT_SEQUENCE>
        static void test_psum(BIT_SEQUENCE &spsi_container, int message_paragraph = stool::Message::SHOW_MESSAGE)
        {
            if (message_paragraph != stool::Message::NO_MESSAGE)
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "test_psum" << std::endl;
            }
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
        template <typename BIT_SEQUENCE>
        static void test_search(BIT_SEQUENCE &spsi, int message_paragraph = stool::Message::SHOW_MESSAGE)
        {
            if (message_paragraph != stool::Message::NO_MESSAGE)
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "test_search" << std::endl;
            }

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

        template <typename BIT_SEQUENCE>
        static void build_test(BIT_SEQUENCE &spsi, int64_t num, int64_t seed, int message_paragraph = stool::Message::SHOW_MESSAGE)
        {
            if (message_paragraph != stool::Message::NO_MESSAGE)
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Building test..." << std::endl;
            }
            std::mt19937_64 mt64(seed);
            auto bits = create_sequence2(num, mt64);
            auto tmp = BIT_SEQUENCE::build(bits);
            spsi.swap(tmp);

            assert(bits.size() == spsi.size());
            for (uint64_t i = 0; i < bits.size(); i++)
            {
                assert(bits[i] == spsi[i]);
            }
        }

        template <typename BIT_SEQUENCE>
        static void insert_and_delete_test(BIT_SEQUENCE &spsi, int64_t num, int64_t seed, int message_paragraph = stool::Message::SHOW_MESSAGE)
        {
            if (message_paragraph != stool::Message::NO_MESSAGE)
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "insert_and_delete_test: " << std::flush;
            }
            std::mt19937_64 mt64(seed);
            uint64_t checksum = 0;

            //std::cout << spsi.to_string() << std::endl;

            std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, UINT32_MAX);

            
            while (spsi.size() < (size_t)num)
            {
                uint64_t pos = get_rand_uni_int(mt64) % (spsi.size() + 1);
                uint64_t value = get_rand_uni_int(mt64) % 2;
                assert(pos <= spsi.size());

                spsi.insert(pos, value);

                spsi.verify();

                if(spsi.size() % 1000 == 0){
                    std::cout << "+" << std::flush;
                }

                checksum += value;

                if (checksum != spsi.psum())
                {
                    throw std::runtime_error("insert_and_delete_test(1)::Error, checksum error, " + std::to_string(checksum) + "/" + std::to_string(spsi.psum()));
                }
            }

            while (spsi.size() > 0)
            {
                if(spsi.size() % 1000 == 0){
                    std::cout << "-" << std::flush;
                }

                uint64_t pos = get_rand_uni_int(mt64) % spsi.size();

                checksum -= spsi.at(pos);

                uint64_t sum1 = spsi.psum();
                std::string old_bits = spsi.to_string();
                spsi.remove(pos);
                uint64_t sum2 = spsi.psum();
                std::string new_bits = spsi.to_string();
                spsi.verify();

                if (checksum != spsi.psum())
                {
                    std::cout << "pos: " << pos << ", sum1: " << sum1 << ", sum2: " << sum2 << std::endl;

                    std::cout << "old_bits: " << old_bits << std::endl;
                    std::cout << "new_bits: " << new_bits << std::endl;
                    throw std::runtime_error("insert_and_delete_test(2)::Error, checksum error, " + std::to_string(checksum) + "/" + std::to_string(spsi.psum()));
                }
            }
            std::cout << "[DONE]" << std::endl;
        }

        template <typename BIT_SEQUENCE>
        static void load_write_test(BIT_SEQUENCE &spsi, int message_paragraph = stool::Message::SHOW_MESSAGE)
        {
            if (message_paragraph != stool::Message::NO_MESSAGE)
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Loading and writing test..." << std::endl;
            }
            std::vector<uint8_t> bytes;
            uint64_t pos = 0;

            BIT_SEQUENCE::store_to_bytes(spsi, bytes, pos);

            pos = 0;

            BIT_SEQUENCE spsi2 = BIT_SEQUENCE::load_from_bytes(bytes, pos);

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
        template <typename BIT_SEQUENCE>
        static void load_write_test2(BIT_SEQUENCE &spsi, int message_paragraph = stool::Message::SHOW_MESSAGE)
        {
            if (message_paragraph != stool::Message::NO_MESSAGE)
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Loading and writing test..." << std::endl;
            }
            {
                std::ofstream os;
                os.open("bit_text.bits", std::ios::binary);
                if (!os)
                {
                    std::cerr << "Error: Could not open file for writing." << std::endl;
                    throw std::runtime_error("File open error");
                }

                std::cout << "save" << std::endl;
                BIT_SEQUENCE::store_to_file(spsi, os);
            }
            BIT_SEQUENCE spsi2;

            {
                std::ifstream ifs;
                ifs.open("bit_text.bits", std::ios::binary);
                if (!ifs)
                {
                    std::cerr << "Error: Could not open file for reading." << std::endl;
                    throw std::runtime_error("File open error");
                }
                std::cout << "load" << std::endl;
                auto tmp = BIT_SEQUENCE::load_from_file(ifs);
                spsi2.swap(tmp);
            }

            std::remove("bit_text.bits");

            if (spsi.size() != spsi2.size())
            {
                std::cout << "size1: " << spsi.size() << ", size2: " << spsi2.size() << std::endl;
                throw std::runtime_error("Error in load_write_test2: size mismatch");
            }

            for (uint64_t i = 0; i < spsi.size(); i++)
            {
                bool b1 = spsi[i];
                bool b2 = spsi2[i];
                if (b1 != b2)
                {
                    throw std::runtime_error("load_write_test2::Error(2)");
                }
            }
        }

        template <typename BIT_SEQUENCE>
        static void insert_and_delete_test2(BIT_SEQUENCE &spsi, int64_t num, int64_t seed, int message_paragraph = stool::Message::SHOW_MESSAGE)
        {
            if (message_paragraph != stool::Message::NO_MESSAGE)
            {
                std::cout << stool::Message::get_paragraph_string(message_paragraph) << "insert_and_delete_test2" << std::endl;
            }
            std::mt19937_64 mt64(seed);
            uint64_t checksum = 0;

            std::cout << spsi.to_string() << std::endl;
            std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, UINT32_MAX);

            while (spsi.size() < (size_t)num)
            {
                uint64_t pos = get_rand_uni_int(mt64) % (spsi.size() + 1);
                uint64_t value = get_rand_uni_int(mt64) % 2;
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
                uint64_t pos = get_rand_uni_int(mt64) % spsi.size();

                checksum -= spsi.at(pos);
                spsi.remove(pos);

                if (checksum != spsi.psum())
                {
                    std::cout << "checksum: " << checksum << ", psum: " << spsi.psum() << std::endl;
                    throw std::runtime_error("Error in insert_and_delete_test2: checksum mismatch");
                }
            }
        }
    };

}
