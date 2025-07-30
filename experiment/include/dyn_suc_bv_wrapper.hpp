#pragma once
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunknown-pragmas"

#include "../modules/DYNAMIC/include/dynamic/dynamic.hpp"
#pragma GCC diagnostic pop

class DynSucBVWrapper
{

public:
    dyn::suc_bv bits;

    DynSucBVWrapper()
    {
    }
    DynSucBVWrapper([[maybe_unused]] uint64_t degree)
    {
    }
    DynSucBVWrapper(std::vector<bool> &_items, [[maybe_unused]] uint64_t degree)
    {
        this->push_many(_items);
    }

    void print_statistics([[maybe_unused]] int message_paragraph = stool::Message::SHOW_MESSAGE) const
    {
    }
    void initialize([[maybe_unused]] uint64_t degree)
    {
    }
    void swap(DynSucBVWrapper &item)
    {
        this->bits = std::move(item.bits);
        // this->bits.swap(item.bits);
    }
    void clear()
    {
        while (this->bits.size() > 0)
        {
            this->bits.remove(this->bits.size() - 1);
        }
    }
    /*
void build(std::vector<bool> &_items)
{
    this->push_many(_items);
}
*/

    uint64_t size() const
    {
        return this->bits.size();
    }
    void push_back(uint64_t value)
    {
        this->bits.push_back(value > 0);
    }
    void push_front(uint64_t value)
    {
        this->bits.push_front(value > 0);
    }

    void insert(uint64_t pos, uint64_t value)
    {
        this->bits.insert(pos, value > 0);
    }
    void remove(uint64_t pos)
    {
        this->bits.remove(pos);
    }
    std::vector<uint64_t> to_vector() const
    {
        std::vector<uint64_t> r;
        for (uint64_t i = 0; i < this->bits.size(); i++)
        {
            uint64_t x = this->bits.at(i) ? 1 : 0;
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
        return this->bits.at(pos);
    }

    static std::string name()
    {
        std::string s;
        s += "DynamicSequence(";
        s += ")";
        return s;
    }
    static DynSucBVWrapper build(const std::vector<bool> &_items)
    {
        DynSucBVWrapper r;
        r.push_many(_items);
        return r;
    }
    void push_many(const std::vector<bool> &_items)
    {
        for (uint64_t i = 0; i < _items.size(); i++)
        {
            bits.push_back(_items[i]);
        }
    }

    uint64_t count_c(bool c) const
    {
        return this->bits.rank(this->bits.size(), c);
    }

    uint64_t size_in_bytes() const
    {
        return 0;
    }
    std::vector<std::string> get_memory_usage_info() const
    {
        std::vector<std::string> log1;
        // = this->tree.get_memory_usage_info();

        std::vector<std::string> r;
        r.push_back("=SPSI: " + std::to_string(this->size_in_bytes()) + " bytes =");
        for (std::string &s : log1)
        {
            r.push_back(" " + s);
        }
        r.push_back("==");
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
    std::string to_string() const
    {
        std::string s;
        std::vector<uint64_t> bits = this->to_value_vector();
        s.push_back('[');
        for (uint64_t i = 0; i < bits.size(); i++)
        {
            s += std::to_string(bits[i]);
            if (i + 1 < bits.size())
            {
                s += ", ";
            }
        }
        s.push_back(']');
        return s;
    }

    int64_t rank1(uint64_t i) const
    {
        return this->bits.rank1(i);
    }
    int64_t rank0(uint64_t i) const
    {
        return this->bits.rank0(i);
    }

    int64_t select1(uint64_t i) const
    {
        return this->bits.select1(i);
    }
    int64_t select0(uint64_t i) const
    {
        return this->bits.select0(i);
    }
};
