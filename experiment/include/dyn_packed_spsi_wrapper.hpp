#pragma once
#include "stool/include/all.hpp"
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wunused-variable"
#include "../modules/DYNAMIC/include/dynamic/dynamic.hpp"
#pragma GCC diagnostic pop

class DynPackedSPSIWrapper
{
private:
    dyn::packed_spsi spsi;

public:
    DynPackedSPSIWrapper()
    {
    }

    void initialize([[maybe_unused]] uint64_t degree)
    {
    }

    uint64_t size() const
    {
        return this->spsi.size();
    }
    void push_back(uint64_t value)
    {
        this->spsi.push_back(value);
    }

    void insert(uint64_t pos, uint64_t value)
    {
        this->spsi.insert(pos, value);
    }
    void remove(uint64_t pos)
    {
        this->spsi.remove(pos);
    }

    uint64_t psum(uint64_t x) const
    {
        return this->spsi.psum(x);
    }
    uint64_t psum() const
    {
        return this->spsi.psum();
    }
    int64_t search(uint64_t sum) const
    {
        return this->spsi.search(sum);
    }
    uint64_t at(uint64_t pos) const
    {
        return this->spsi.at(pos);
    }
    void increment(uint64_t i, int64_t delta)
    {
        this->spsi.increment(i, delta);
    }
    void decrement(uint64_t i, int64_t delta)
    {
        this->spsi.increment(i, -delta);
    }

    static std::string name()
    {
        std::string s;
        s += "SPSI(";
        s += "dyn";
        s += ")";
        return s;
    }
    void push_many(const std::vector<uint64_t> &items)
    {
        for (uint64_t x : items)
        {
            this->spsi.push_back(x);
        }
    }

    uint64_t size_in_bytes() const
    {
        return 0;
    }
    std::vector<std::string> get_memory_usage_info() const
    {
        std::vector<std::string> r;
        return r;
    }
    void print_memory_usage() const
    {
        std::vector<std::string> log = this->get_memory_usage_info();
        for (std::string &s : log)
        {
            std::cout << s << std::endl;
        }
    }
};
