#pragma once
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "bit_vector/bv.hpp"
#pragma GCC diagnostic pop

class BVBVWrapper
{
    bv::bv bv;

public:
    void print_statistics([[maybe_unused]] int message_paragraph = 1) const
    {
    }
    void initialize([[maybe_unused]] uint64_t degree)
    {
    }
    void swap([[maybe_unused]] BVBVWrapper &item)
    {
        throw -1;
        // this->bv = std::move(item.bv);
    }
    void clear()
    {
        while (this->bv.size() > 0)
        {
            this->bv.remove(this->bv.size() - 1);
        }
    }

    uint64_t size() const
    {
        return this->bv.size();
    }
    void push_back(uint64_t value)
    {
        this->bv.insert(this->bv.size(), value > 0);
    }
    void push_front(uint64_t value)
    {
        this->bv.insert(0, value > 0);
    }

    void insert(uint64_t pos, uint64_t value)
    {
        this->bv.insert(pos, value > 0);
    }
    void remove(uint64_t pos)
    {
        this->bv.remove(pos);
    }
    uint64_t count_c(bool c) const
    {
        return this->bv.rank(c, this->bv.size());
    }
    std::vector<uint64_t> to_vector() const
    {
        std::vector<uint64_t> r;
        for (uint64_t i = 0; i < this->bv.size(); i++)
        {
            uint64_t x = this->bv.at(i) ? 1 : 0;
            r.push_back(x);
        }
        return r;
    }
    std::vector<uint64_t> to_value_vector() const
    {
        throw -1;
    }
    uint64_t at(uint64_t pos) const
    {
        return this->bv.at(pos);
    }

    static std::string name()
    {
        std::string s;
        s += "DynamicSequence(";
        s += ")";
        return s;
    }

    static BVBVWrapper build([[maybe_unused]] const std::vector<bool> &_items)
    {
        throw -1;
    }

    void push_many(const std::vector<bool> &_items)
    {
        for (uint64_t i = 0; i < _items.size(); i++)
        {
            uint64_t s = this->bv.size();
            this->bv.insert(s, _items[i]);
        }
    }
    int64_t rank1(uint64_t i) const
    {
        return this->bv.rank(1, i);
    }
    int64_t rank0(uint64_t i) const
    {
        return this->bv.rank0(i);
    }

    void print_memory_usage() const
    {
    }
    int64_t select1(uint64_t i) const
    {
        return this->bv.select(1, i+1);
    }
    int64_t select0(uint64_t i) const
    {
        return this->bv.select0(i+1);
    }
};