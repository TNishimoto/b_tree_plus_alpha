#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>

#include "../../include/b_tree_plus_alpha.hpp"
#include <random>

namespace stool
{
    using namespace stool::bptree;
    class SPSITest
    {
    public:
        static std::deque<uint64_t> create_sequence(uint64_t num, int64_t max_value, std::mt19937_64 &mt64)
        {
            std::deque<uint64_t> r;
            std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, max_value - 1);
            while (r.size() < (size_t)num)
            {
                r.push_back(get_rand_uni_int(mt64));
            }
            return r;
        }

        template <typename A, uint64_t B, uint64_t C>
        static uint64_t random_build(DynamicPrefixSum<A, B, C> &spsi, uint64_t num, uint64_t max_value, [[maybe_unused]] uint64_t degree, int64_t seed)
        {
            std::mt19937_64 mt64(seed);
            //spsi.set_degree(degree);
            uint64_t checksum = 0;

            while (spsi.size() < (size_t)num)
            {
                std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, spsi.size());
                std::uniform_int_distribution<uint64_t> get_rand_uni_int2(0, max_value - 1);
                uint64_t pos = get_rand_uni_int(mt64);
                uint64_t value = get_rand_uni_int2(mt64);
                assert(pos <= spsi.size());
                spsi.insert(pos, value);
                spsi.verify();

                checksum += value;

                if (checksum != spsi.psum())
                {
                    throw std::runtime_error("insert_and_delete_test::Error");
                }
            }

            uint64_t checksum2 = 0;
            for(auto it : spsi){
                checksum2 += it;
            }
            assert(checksum == checksum2);
            return checksum;
        }

        template <typename A, uint64_t B, uint64_t C>
        static uint64_t random_remove(DynamicPrefixSum<A, B, C> &spsi, int64_t seed)
        {
            std::mt19937_64 mt64(seed);
            uint64_t checksum = 0;

            while (spsi.size() > 0)
            {
                std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, spsi.size() - 1);
                uint64_t pos = get_rand_uni_int(mt64);

                checksum += spsi.at(pos);

                spsi.remove(pos);
                spsi.verify();
            }
            return checksum;
        }

        template <typename A, uint64_t B, uint64_t C>
        static void load_write_bits_test(stool::bptree::DynamicPrefixSum<A, B, C> &spsi)
        {
            std::vector<uint8_t> bytes;
            uint64_t pos = 0;


            std::cout << "save" << std::endl;
            stool::bptree::DynamicPrefixSum<A, B, C>::save(spsi, bytes, pos);


            pos = 0;
            std::cout << "load" << std::endl;

            stool::bptree::DynamicPrefixSum<A, B, C> spsi2 = stool::bptree::DynamicPrefixSum<A, B, C>::load_from_bytes(bytes, pos);

            if (spsi.size() != spsi2.size())
            {
                assert(false);
                throw -1;
            }

            for (uint64_t i = 0; i < spsi.size(); i++)
            {
                uint64_t b1 = spsi[i];
                uint64_t b2 = spsi2[i];
                if (b1 != b2)
                {
                    assert(false);
                    throw -1;
                }
            }
        }
        template <typename A, uint64_t B, uint64_t C>
static void load_write_file_test(stool::bptree::DynamicPrefixSum<A, B, C> &spsi)
        {
            {
                std::ofstream os;
                os.open("spsi_text.bits", std::ios::binary);
                if (!os)
                {
                    std::cerr << "Error: Could not open file for writing." << std::endl;
                    throw std::runtime_error("File open error");
                }

                std::cout << "save" << std::endl;
                stool::bptree::DynamicPrefixSum<A, B, C>::save(spsi, os);
            }



            stool::bptree::DynamicPrefixSum<A, B, C> spsi2;

            {
                std::ifstream ifs;
                ifs.open("spsi_text.bits", std::ios::binary);
                if (!ifs)
                {
                    std::cerr << "Error: Could not open file for reading." << std::endl;
                    throw std::runtime_error("File open error");
                }
                std::cout << "load" << std::endl;
                auto tmp = stool::bptree::DynamicPrefixSum<A, B, C>::load_from_file(ifs);
                spsi2.swap(tmp);
            }
            std::remove("spsi_text.bits");

            if (spsi.size() != spsi2.size())
            {
                assert(false);
                throw -1;
            }

            for (uint64_t i = 0; i < spsi.size(); i++)
            {
                if (spsi.at(i) != spsi2.at(i))
                {
                    assert(false);
                    throw -1;
                }
            }
        }
        /*
        template <typename T>
        static void insert_and_delete_test(DynamicPrefixSum<T> &spsi, uint64_t num, uint64_t max_value, uint64_t degree, int64_t seed)
        {
            std::mt19937_64 mt64(seed);


            spsi.set_degree(degree);
            uint64_t checksum = 0;

            while (spsi.size() < (size_t)num)
            {
                std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, spsi.size());
                std::uniform_int_distribution<uint64_t> get_rand_uni_int2(0, max_value - 1);
                uint64_t pos = get_rand_uni_int(mt64);
                uint64_t value = get_rand_uni_int2(mt64);
                assert(pos <= spsi.size());
                spsi.insert(pos, value);
                spsi.verify();

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
                spsi.verify();

                if (checksum != spsi.psum())
                {
                    throw std::runtime_error("Error");
                }
            }
        }
        */

        template <typename T>
        static void test(int64_t num, int64_t max_value, uint64_t degree, int64_t seed)
        {
            std::mt19937_64 mt64(seed);
            std::deque<uint64_t> items = create_sequence(num, max_value, mt64);
            std::vector<uint64_t> items2;
            for (uint64_t p : items)
            {
                items2.push_back(p);
            }

            stool::DebugPrinter::print_integers(items, "Item");

            DynamicPrefixSum<T> spsi;
            spsi.build(items2, degree);

            spsi.get_tree().print_tree();
            spsi.verify();

            std::cout << "Size: " << spsi.size() << std::endl;
            // std::cout << spsi.tree.to_string() << std::endl;
            test_psum(spsi);
            test_search(spsi);

            std::cout << "INS TEST" << std::endl;

            for (int64_t i = 0; i < 30; i++)
            {
                std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, spsi.size() - 1);
                std::uniform_int_distribution<uint64_t> get_rand_uni_int2(0, max_value - 1);
                uint64_t pos = get_rand_uni_int(mt64);
                uint64_t value = get_rand_uni_int2(mt64);
                // std::cout << "Ins. pos = " << pos << ", value = " << value << std::endl;
                assert(pos <= spsi.size());
                spsi.insert(pos, value);
                // std::cout << "Size: " << spsi.size() << std::endl;
                // spsi.tree.print_tree();
                spsi.verify();
                // test_psum(spsi);
                // test_search(spsi);

                assert((int64_t)spsi.size() == num + (i + 1));
            }

            std::cout << "DEL TEST" << std::endl;

            uint64_t _num = spsi.size();
            while (spsi.size() > 0)
            {
                std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, spsi.size() - 1);
                uint64_t pos = get_rand_uni_int(mt64);
                spsi.remove(pos);
                spsi.verify();
                _num--;
                if (spsi.size() != _num)
                {
                    throw std::logic_error("error");
                }
            }
        }

        template <typename T>
        static void test_push_front(int64_t num, int64_t max_value, uint64_t degree, int64_t seed)
        {
            std::mt19937_64 mt64(seed);
            std::deque<uint64_t> items = create_sequence(num, max_value, mt64);

            DynamicPrefixSum<T> spsi;
            spsi.set_degree(degree);

            for (int64_t i = items.size() - 1; i >= 0; i--)
            {
                spsi.push_front(items[i]);
            }

            uint64_t sum = spsi.psum();

            while (spsi.size() > 0)
            {
                std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, spsi.size() - 1);
                uint64_t pos = get_rand_uni_int(mt64);
                sum -= spsi.at(pos);
                spsi.remove(pos);
                if (sum != spsi.psum())
                {
                    throw std::logic_error("Error");
                }
                spsi.verify();
            }
        }

        template <typename T>
        static void test2(int64_t num, int64_t max_value, uint64_t degree, int64_t seed)
        {
            std::mt19937_64 mt64(seed);
            std::deque<uint64_t> items = create_sequence(num, max_value, mt64);
            std::vector<uint64_t> items2;
            for (uint64_t p : items)
            {
                items2.push_back(p);
            }

            // stool::Printer::print("Item", items);

            DynamicPrefixSum<T> spsi;
            spsi.build(items2, degree);
            // std::cout << "Size: " << spsi.size() << std::endl;
            // spsi.tree.print_tree();
            // spsi.tree.verify();
            uint64_t sum = spsi.psum();

            while (spsi.tree.size() > 0)
            {
                std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, spsi.size() - 1);
                uint64_t pos = get_rand_uni_int(mt64);
                sum -= spsi.at(pos);
                // std::cout << "DEL, pos = " << pos << std::endl;
                spsi.remove(pos);
                if (sum != spsi.psum())
                {
                    throw std::logic_error("Error");
                }

                // spsi.tree.print_tree();
                spsi.tree.verify();
                // test_psum(spsi);
            }
        }

        template <typename T>
        static void test_psum(DynamicPrefixSum<T> &spsi)
        {
            std::vector<uint64_t> items = spsi.to_vector();
            uint64_t sum = 0;
            for (uint64_t i = 0; i < items.size(); i++)
            {
                sum += items[i];
                uint64_t psum = spsi.psum(i);

                if (sum != psum)
                {
                    assert(false);
                    throw std::logic_error("Error");
                }
            }
        }
        template <typename T>
        static void test_search(DynamicPrefixSum<T> &spsi)
        {
            // uint64_t psum = spsi.psum();
            uint64_t xsum = 1;

            std::vector<uint64_t> items = spsi.to_vector();

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
        static void test3(int64_t num, int64_t max_value, uint64_t degree, int64_t seed)
        {

            std::mt19937_64 mt64(seed);
            std::deque<uint64_t> items = create_sequence(num, max_value, mt64);

            std::vector<uint64_t> items2;
            for (uint64_t p : items)
            {
                items2.push_back(p);
            }

            DynamicPrefixSum<T> spsi;
            spsi.build(items2, degree);
            spsi.verify();

            test_psum(spsi);
            test_search(spsi);
        }

        template <typename T>
        static void test4(int64_t num, int64_t max_value, uint64_t degree, int64_t seed)
        {
            std::mt19937_64 mt64(seed);
            std::deque<uint64_t> items = create_sequence(num, max_value, mt64);

            DynamicPrefixSum<T> spsi;
            spsi.set_degree(degree);

            for (uint64_t x : items)
            {
                spsi.push_back(x);
            }

            std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, spsi.size() - 1);

            for (int64_t i = 0; i < num; i++)
            {
                uint64_t pos = get_rand_uni_int(mt64);
                uint64_t value = items[pos];
                std::uniform_int_distribution<uint64_t> get_rand_uni_delta(0, value);
                int64_t delta = get_rand_uni_delta(mt64);
                if (i % 2 == 0)
                {
                    spsi.increment(pos, delta);
                    items[pos] += delta;
                }
                else
                {
                    spsi.decrement(pos, delta);
                    items[pos] -= delta;
                }
            }

            std::vector<uint64_t> vec2 = spsi.to_vector();

            std::vector<uint64_t> vec1;
            for (uint64_t x : items)
            {
                vec1.push_back(x);
            }
            stool::EqualChecker::equal_check(vec1, vec2);
        }
    };

}
