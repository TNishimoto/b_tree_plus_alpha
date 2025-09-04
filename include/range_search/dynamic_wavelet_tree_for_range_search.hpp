#pragma once
#include "../sequence/dynamic_bit_sequence.hpp"

namespace stool
{
    namespace bptree
    {

        class DynamicWaveletTreeForRangeSearch
        {
            using BIT_SEQUENCE = SimpleDynamicBitSequence;
            std::vector<std::vector<BIT_SEQUENCE>> bits_seq;
            std::vector<stool::NaiveFLCVector<false>> leaves;

            inline static uint64_t LEAF_MAX_SIZE = 8;

        public:
            class YRankIterator
            {
            public:
                using iterator_category = std::random_access_iterator_tag; // C++17
                using value_type = uint64_t;
                using difference_type = std::ptrdiff_t;

                const DynamicWaveletTreeForRangeSearch *container = nullptr;
                uint64_t x_rank = 0;            
                uint64_t y_rank = 0;
                YRankIterator() : container(nullptr), x_rank(0), y_rank(0) {}
                YRankIterator(const DynamicWaveletTreeForRangeSearch *container, uint64_t x_rank, uint64_t y_rank) : container(container), x_rank(x_rank), y_rank(y_rank) {}

                uint64_t operator*() const noexcept { return this->x_rank; }

                // 前後インクリメント
                YRankIterator &operator++() noexcept
                {
                    ++this->y_rank;
                    if (this->y_rank < this->container->size())
                    {
                        this->x_rank = this->container->access_x_rank(this->y_rank);
                    }
                    else
                    {
                        this->x_rank = this->container->size();
                    }
                    return *this;
                }
                YRankIterator operator++(int) noexcept
                {
                    YRankIterator t = *this;
                    ++*this;
                    return t;
                }
                YRankIterator &operator--() noexcept
                {
                    if (this->y_rank > 0)
                    {
                        this->y_rank--;
                        this->x_rank = this->container->access_x_rank(this->y_rank);
                    }
                    else
                    {
                        this->x_rank = this->container->size();
                    }
                    return *this;
                }
                YRankIterator operator--(int) noexcept
                {
                    YRankIterator t = *this;
                    --*this;
                    return t;
                }

                // 加減算
                YRankIterator &operator+=(difference_type n) noexcept
                {
                    this->y_rank += n;
                    if (this->y_rank < this->container->size())
                    {
                        this->x_rank = this->container->access_x_rank(this->y_rank);
                    }
                    else
                    {
                        this->x_rank = this->container->size();
                    }
                    return *this;
                }
                YRankIterator &operator-=(difference_type n) noexcept
                {
                    this->y_rank -= n;
                    if (this->y_rank < this->container->size())
                    {
                        this->x_rank = this->container->access_x_rank(this->y_rank);
                    }
                    else
                    {
                        this->x_rank = this->container->size();
                    }
                    return *this;
                }
                friend YRankIterator operator+(YRankIterator it, difference_type n) noexcept { return YRankIterator(it.container, it.y_rank + n, it.x_rank); }
                friend YRankIterator operator+(difference_type n, YRankIterator it) noexcept { return it + n; }
                friend YRankIterator operator-(YRankIterator it, difference_type n) noexcept { return YRankIterator(it.container, it.y_rank - n, it.x_rank); }
                friend difference_type operator-(YRankIterator a, YRankIterator b) noexcept { return a.y_rank - b.y_rank; }

                // 比較
                friend bool operator==(YRankIterator a, YRankIterator b) noexcept { return a.y_rank == b.y_rank; }
                friend bool operator!=(YRankIterator a, YRankIterator b) noexcept { return !(a == b); }
                friend bool operator<(YRankIterator a, YRankIterator b) noexcept { return a.y_rank < b.y_rank; }
                friend bool operator>(YRankIterator a, YRankIterator b) noexcept { return b < a; }
                friend bool operator<=(YRankIterator a, YRankIterator b) noexcept { return !(b < a); }
                friend bool operator>=(YRankIterator a, YRankIterator b) noexcept { return !(a < b); }
            };

            class XRankIterator
            {
            public:
                using iterator_category = std::random_access_iterator_tag; // C++17
                using value_type = uint64_t;
                using difference_type = std::ptrdiff_t;

                const DynamicWaveletTreeForRangeSearch *container = nullptr;
                uint64_t x_rank = 0;            
                uint64_t y_rank = 0;
                XRankIterator() : container(nullptr), x_rank(0), y_rank(0) {}
                XRankIterator(const DynamicWaveletTreeForRangeSearch *container, uint64_t x_rank, uint64_t y_rank) : container(container), x_rank(x_rank), y_rank(y_rank) {}

                uint64_t operator*() const noexcept { return this->y_rank; }

                // 前後インクリメント
                XRankIterator &operator++() noexcept
                {
                    ++this->x_rank;
                    if (this->x_rank < this->container->size())
                    {
                        this->y_rank = this->container->access_y_rank(this->x_rank);
                    }
                    else
                    {
                        this->y_rank = this->container->size();
                    }
                    return *this;
                }
                XRankIterator operator++(int) noexcept
                {
                    XRankIterator t = *this;
                    ++*this;
                    return t;
                }
                XRankIterator &operator--() noexcept
                {
                    if (this->x_rank > 0)
                    {
                        this->x_rank--;
                        this->y_rank = this->container->access_y_rank(this->x_rank);
                    }
                    else
                    {
                        this->y_rank = this->container->size();
                    }
                    return *this;
                }
                XRankIterator operator--(int) noexcept
                {
                    XRankIterator t = *this;
                    --*this;
                    return t;
                }

                // 加減算
                XRankIterator &operator+=(difference_type n) noexcept
                {
                    this->x_rank += n;
                    if (this->x_rank < this->container->size())
                    {
                        this->y_rank = this->container->access_y_rank(this->x_rank);
                    }
                    else
                    {
                        this->y_rank = this->container->size();
                    }
                    return *this;
                }
                XRankIterator &operator-=(difference_type n) noexcept
                {
                    this->x_rank -= n;
                    if (this->x_rank < this->container->size())
                    {
                        this->y_rank = this->container->access_y_rank(this->x_rank);
                    }
                    else
                    {
                        this->y_rank = this->container->size();
                    }
                    return *this;
                }
                friend XRankIterator operator+(XRankIterator it, difference_type n) noexcept { return XRankIterator(it.container, it.y_rank + n, it.x_rank); }
                friend XRankIterator operator+(difference_type n, XRankIterator it) noexcept { return it + n; }
                friend XRankIterator operator-(XRankIterator it, difference_type n) noexcept { return XRankIterator(it.container, it.y_rank - n, it.x_rank); }
                friend difference_type operator-(XRankIterator a, XRankIterator b) noexcept { return a.y_rank - b.y_rank; }

                // 比較
                friend bool operator==(XRankIterator a, XRankIterator b) noexcept { return a.y_rank == b.y_rank; }
                friend bool operator!=(XRankIterator a, XRankIterator b) noexcept { return !(a == b); }
                friend bool operator<(XRankIterator a, XRankIterator b) noexcept { return a.y_rank < b.y_rank; }
                friend bool operator>(XRankIterator a, XRankIterator b) noexcept { return b < a; }
                friend bool operator<=(XRankIterator a, XRankIterator b) noexcept { return !(b < a); }
                friend bool operator>=(XRankIterator a, XRankIterator b) noexcept { return !(a < b); }
            };

            DynamicWaveletTreeForRangeSearch()
            {
                this->clear();
            }

            void clear(){
                for(uint64_t i = 0; i < this->bits_seq.size(); i++){
                    for(uint64_t j = 0; j < this->bits_seq[i].size(); j++){
                        this->bits_seq[i][j].clear();
                    }
                }
                for(uint64_t i = 0; i < this->leaves.size(); i++){
                    this->leaves[i].clear();
                }
                this->bits_seq.clear();
                this->leaves.clear();

                this->leaves.push_back(stool::NaiveFLCVector<false>());
            }

            /*
            std::array<uint64_t, 64> get_path(uint64_t x_rank, uint64_t y_rank){
                std::array<uint64_t, 64> path;
                for(uint64_t i = 0; i < 64; i++){
                    path[i] = 0;
                }
                return path;
            }
            */

            void add(int64_t h, uint64_t h_node_id, uint64_t x_rank, uint64_t y_rank)
            {
                uint64_t node_size = this->bits_seq[h][h_node_id].size();
                if (h + 1 < this->height())
                {
                    uint64_t left_node_id = 2 * h_node_id;
                    uint64_t right_node_id = 2 * h_node_id + 1;
                    uint64_t left_tree_size = this->bits_seq[h + 1][left_node_id].size();
                    if (x_rank <= left_tree_size)
                    {
                        uint64_t new_y_rank = y_rank < node_size ? this->bits_seq[h][h_node_id].rank0(y_rank) : left_tree_size;
                        this->add(h + 1, left_node_id, x_rank, new_y_rank);

                        this->bits_seq[h][h_node_id].insert(y_rank, false);
                    }
                    else
                    {
                        uint64_t new_y_rank = y_rank < node_size ? this->bits_seq[h][h_node_id].rank1(y_rank) : (node_size - left_tree_size);
                        uint64_t new_x_rank = x_rank - left_tree_size;
                        this->add(h + 1, right_node_id, new_x_rank, new_y_rank);

                        this->bits_seq[h][h_node_id].insert(y_rank, true);
                    }
                }
                else
                {
                    uint64_t left_leaf_id = 2 * h_node_id;
                    uint64_t right_leaf_id = 2 * h_node_id + 1;
                    uint64_t left_tree_size = this->leaves[left_leaf_id].size();

                    if (x_rank <= left_tree_size)
                    {
                        uint64_t new_y_rank = y_rank < node_size ? this->bits_seq[h][h_node_id].rank0(y_rank) : left_tree_size;
                        this->insert_element_into_leaf(left_leaf_id, new_y_rank, x_rank);
                        this->bits_seq[h][h_node_id].insert(y_rank, false);
                    }
                    else
                    {
                        uint64_t new_y_rank = y_rank < node_size ? this->bits_seq[h][h_node_id].rank1(y_rank) : (node_size - left_tree_size);
                        uint64_t new_x_rank = x_rank - left_tree_size;
                        this->insert_element_into_leaf(right_leaf_id, new_y_rank, new_x_rank);
                        this->bits_seq[h][h_node_id].insert(y_rank, true);
                    }
                }

                uint64_t upper_size = this->get_upper_size_of_internal_node(h);
                if (this->size() < upper_size && this->is_unbalanced_node(h, h_node_id))
                {
                    std::vector<uint64_t> rank_elements = this->to_rank_elements(h, h_node_id);
                    this->rebuild_internal_node(h, h_node_id, rank_elements);
                }
            }

            void add(uint64_t x_rank, uint64_t y_rank)
            {
                if (this->height() > 0)
                {
                    this->add(0, 0, x_rank, y_rank);
                    uint64_t upper_size = this->get_upper_size_of_internal_node(0);
                    if (this->size() >= upper_size)
                    {
                        std::vector<uint64_t> rank_elements = this->to_rank_elements();
                        this->build(rank_elements);
                    }
                }
                else
                {
                    this->insert_element_into_leaf(0, y_rank, x_rank);
                    uint64_t upper_size = this->get_upper_size_of_leaf();
                    if (this->size() >= upper_size)
                    {
                        std::vector<uint64_t> rank_elements = this->to_rank_elements();
                        this->build(rank_elements);
                    }
                }
            }
            void remove(uint64_t y_rank)
            {
                int64_t height = this->height();
                if (height > 0)
                {
                    uint64_t h_y_rank = y_rank;
                    uint64_t h_node_id = 0;

                    for (int64_t h = 0; h < height; h++)
                    {
                        bool b = this->bits_seq[h][h_node_id].at(h_y_rank);
                        uint64_t next_node_id = (2 * h_node_id) + (uint64_t)b;
                        if (b)
                        {
                            uint64_t rmv_y_rank = this->bits_seq[h][h_node_id].rank0(h_y_rank);
                            this->bits_seq[h][h_node_id].remove(h_y_rank);
                            h_y_rank -= rmv_y_rank;
                        }
                        else
                        {
                            uint64_t rmv_y_rank = this->bits_seq[h][h_node_id].rank1(h_y_rank);
                            this->bits_seq[h][h_node_id].remove(h_y_rank);
                            h_y_rank -= rmv_y_rank;
                        }
                        h_node_id = next_node_id;
                    }
                    this->remove_element_from_leaf(h_node_id, h_y_rank);

                    uint64_t upper_size = this->get_upper_size_of_internal_node(0);
                    uint64_t size = this->size();
                    if (size < upper_size / 2)
                    {
                        auto rank_elements = this->to_rank_elements();
                        this->build(rank_elements);
                    }
                }
                else
                {
                    this->remove_element_from_leaf(0, y_rank);
                }
            }

            static uint64_t get_full_size_of_tree(uint64_t height)
            {
                return LEAF_MAX_SIZE << height;
            }
            static uint64_t get_upper_size_of_tree(uint64_t height)
            {
                return _get_upper_size_of_internal_node(0, height);
            }

            uint64_t get_full_size_of_internal_node(uint64_t h) const
            {
                uint64_t H = this->bits_seq.size();
                return LEAF_MAX_SIZE << (H - h);
            }
            uint64_t get_upper_size_of_leaf() const
            {
                return LEAF_MAX_SIZE;
            }

            static uint64_t _get_upper_size_of_internal_node(uint64_t h, uint64_t H)
            {
                uint64_t u1 = 1;
                uint64_t u2 = 1;
                for (uint64_t p = h; p < H; p++)
                {
                    u1 *= 3;
                    u2 *= 2;
                }
                uint64_t r = (u1 * LEAF_MAX_SIZE) / u2;
                return r;
            }

            uint64_t get_upper_size_of_internal_node(uint64_t h) const
            {
                return _get_upper_size_of_internal_node(h, this->height());
            }
            uint64_t get_lower_size_of_internal_node(uint64_t h) const
            {
                uint64_t fsize = this->get_full_size_of_internal_node(h);
                return (fsize / 4);
            }

            /*
            void insert_element_into_internal_node(uint8_t h, uint64_t hnode_id, uint64_t pos, uint64_t rank){


            }
            */

            void build(const std::vector<uint64_t> &rank_elements)
            {
                uint64_t height = 0;
                while (true)
                {
                    uint64_t fsize = get_upper_size_of_tree(height);
                    if (rank_elements.size() < fsize)
                    {
                        break;
                    }
                    else
                    {
                        height++;
                    }
                }

                uint64_t leaf_count = 1ULL << height;
                this->leaves.resize(leaf_count);
                for (uint64_t i = 0; i < leaf_count; i++)
                {
                    this->leaves[i].clear();
                }

                this->bits_seq.clear();
                this->bits_seq.resize(height);
                for (uint64_t h = 0; h < height; h++)
                {
                    uint64_t h_node_count = 1ULL << h;
                    this->bits_seq[h].resize(h_node_count);
                    for (uint64_t i = 0; i < h_node_count; i++)
                    {
                        this->bits_seq[h][i].clear();
                    }
                }

                if (height > 0)
                {
                    assert(rank_elements.size() < this->get_upper_size_of_internal_node(0));
                    this->rebuild_internal_node(0, 0, rank_elements);
                }
                else
                {
                    this->rebuild_leaf(0, rank_elements);
                }

                assert(this->verify());
            }

            bool is_unbalanced_node(uint8_t h, uint64_t h_node_id) const
            {
                if (h + 1 < this->height())
                {
                    uint64_t left_node_id = 2 * h_node_id;
                    uint64_t right_node_id = 2 * h_node_id + 1;
                    uint64_t left_tree_size = this->bits_seq[h + 1][left_node_id].size();
                    uint64_t right_tree_size = this->bits_seq[h + 1][right_node_id].size();
                    bool unbalance_flag_LR = left_tree_size > (right_tree_size * 2) || right_tree_size > (left_tree_size * 2);
                    uint64_t child_upper_size = this->get_upper_size_of_internal_node(h + 1);
                    bool full_flag_L = left_tree_size >= child_upper_size;
                    bool full_flag_R = right_tree_size >= child_upper_size;
                    return unbalance_flag_LR || full_flag_L || full_flag_R;
                }
                else
                {
                    uint64_t left_node_id = 2 * h_node_id;
                    uint64_t right_node_id = 2 * h_node_id + 1;
                    uint64_t left_tree_size = this->leaves[left_node_id].size();
                    uint64_t right_tree_size = this->leaves[right_node_id].size();

                    bool unbalance_flag_LR = left_tree_size > (right_tree_size * 2) || right_tree_size > (left_tree_size * 2);
                    uint64_t child_upper_size = this->get_upper_size_of_leaf();
                    bool full_flag_L = left_tree_size >= child_upper_size;
                    bool full_flag_R = right_tree_size >= child_upper_size;

                    return unbalance_flag_LR || full_flag_L || full_flag_R;
                }
            }

            void rebuild_internal_node(uint8_t h, uint64_t h_node_id, const std::vector<uint64_t> &rank_elements)
            {
                assert(h < this->bits_seq.size());
                uint64_t upper_size = this->get_upper_size_of_internal_node(h);

                if (rank_elements.size() > upper_size)
                {
                    std::cout << "Rebuild: h = " << (int)h << "/ H = " << (int)this->height() << ", h_node_id = " << h_node_id << ", rank_elements.size() = " << rank_elements.size() << ", upper_size = " << upper_size << std::endl;
                    this->print_tree();
                }

                assert(rank_elements.size() <= upper_size);
                uint64_t left_counter = 0;

                uint64_t half_size = rank_elements.size() / 2;
                {
                    std::vector<uint64_t> left_elements(half_size, 0);
                    for (uint64_t i = 0; i < rank_elements.size(); i++)
                    {
                        if (rank_elements[i] < half_size)
                        {
                            left_elements[left_counter++] = rank_elements[i];
                        }
                    }
                    uint64_t next_id = 2 * h_node_id;
                    if (h + 1 < this->bits_seq.size())
                    {
                        this->rebuild_internal_node(h + 1, next_id, left_elements);
                        assert(left_elements.size() == this->bits_seq[h + 1][next_id].size());
                    }
                    else
                    {
                        this->rebuild_leaf(next_id, left_elements);
                        assert(left_elements.size() == this->leaves[next_id].size());
                    }
                }
                {
                    std::vector<uint64_t> right_elements(rank_elements.size() - half_size, 0);
                    std::vector<bool> tmp_bit_sequence(rank_elements.size(), false);
                    uint64_t right_counter = 0;

                    for (uint64_t i = 0; i < rank_elements.size(); i++)
                    {
                        assert(i < rank_elements.size());
                        if (rank_elements[i] >= half_size)
                        {
                            assert(right_counter < right_elements.size());
                            tmp_bit_sequence[i] = true;
                            right_elements[right_counter++] = rank_elements[i] - half_size;
                        }
                    }
                    uint64_t next_id = (2 * h_node_id) + 1;
                    if (h + 1 < this->bits_seq.size())
                    {
                        this->rebuild_internal_node(h + 1, next_id, right_elements);
                        assert(right_elements.size() == this->bits_seq[h + 1][next_id].size());
                    }
                    else
                    {
                        this->rebuild_leaf(next_id, right_elements);
                        assert(right_elements.size() == this->leaves[next_id].size());
                    }
                    assert(h < this->bits_seq.size());
                    assert(h_node_id < this->bits_seq[h].size());

                    this->bits_seq[h][h_node_id].clear();
                    this->bits_seq[h][h_node_id].push_many(tmp_bit_sequence);
                }
            }
            void rebuild_leaf(uint64_t leaf_id, const std::vector<uint64_t> &rank_elements)
            {
                stool::NaiveFLCVector<false> tmp(rank_elements);
                this->leaves[leaf_id].swap(tmp);
            }

            void insert_element_into_leaf(uint64_t leaf_id, uint64_t pos, uint64_t rank)
            {
                uint64_t size = this->leaves[leaf_id].size();
                for (uint64_t i = 0; i < size; i++)
                {
                    uint64_t v = this->leaves[leaf_id][i];
                    if (v >= rank)
                    {
                        this->leaves[leaf_id].increment(i, 1);
                    }
                }
                this->leaves[leaf_id].insert(pos, rank);
            }
            void remove_element_from_leaf(uint64_t leaf_id, uint64_t pos)
            {
                uint64_t size = this->leaves[leaf_id].size();
                uint64_t rank = this->leaves[leaf_id][pos];
                for (uint64_t i = 0; i < size; i++)
                {
                    uint64_t v = this->leaves[leaf_id][i];
                    if (v > rank)
                    {
                        this->leaves[leaf_id].increment(i, -1);
                    }
                }
                this->leaves[leaf_id].remove(pos);
            }
            int64_t height() const
            {
                return this->bits_seq.size();
            }
            uint64_t size() const
            {
                assert(this->leaves.size() > 0);
                if (this->height() == 0)
                {
                    return this->leaves[0].size();
                }
                else
                {
                    return this->bits_seq[0][0].size();
                }
            }

            uint64_t access_x_rank(uint64_t y_rank) const
            {
                assert(y_rank < this->size());
                uint64_t x_rank = this->compute_local_x_rank(0, 0, y_rank);
                return x_rank;
            }
            std::pair<uint64_t, uint64_t> locus_element(uint64_t x_rank) const
            {
                if(x_rank >= this->size()){
                    throw std::runtime_error("ERROR in locus_element: x_rank is out of range");
                }

                uint64_t current_x_rank = x_rank;
                uint64_t current_node_id = 0;
                uint64_t height = this->height();

                for (uint64_t h = 0; h < height; h++)
                {
                    uint64_t left_tree_size = this->bits_seq[h][current_node_id].count0();

                    if (current_x_rank < left_tree_size)
                    {
                        current_node_id = 2 * current_node_id;
                    }
                    else
                    {
                        current_x_rank = current_x_rank - left_tree_size;
                        current_node_id = 2 * current_node_id + 1;
                    }
                }
                uint64_t leaf_size = this->leaves[current_node_id].size();
                for (uint64_t i = 0; i < leaf_size; i++)
                {
                    uint64_t v = this->leaves[current_node_id][i];
                    if (v == current_x_rank)
                    {
                        return std::make_pair(current_node_id, i);
                    }
                }

                this->print_tree();
                std::cout << "size: " << this->size() << std::endl;
                std::cout << "x_rank = " << x_rank << std::endl;
                std::cout << "locus_element not found" << std::endl;
                throw std::runtime_error("Locus element not found");
            }
            uint64_t access_y_rank(uint64_t x_rank) const
            {
                std::pair<uint64_t, uint64_t> locus = this->locus_element(x_rank);
                uint64_t current_y_rank = locus.second;
                uint64_t prev_node_id = locus.first;
                uint64_t height = this->height();
                for (int64_t h = height - 1; h >= 0; h--)
                {
                    if (prev_node_id % 2 == 0)
                    {
                        uint64_t next_node_id = prev_node_id / 2;
                        uint64_t next_y_rank = this->bits_seq[h][next_node_id].select0(current_y_rank);
                        current_y_rank = next_y_rank;
                        prev_node_id = next_node_id;
                    }
                    else
                    {
                        uint64_t next_node_id = prev_node_id / 2;
                        uint64_t next_y_rank = this->bits_seq[h][next_node_id].select1(current_y_rank);
                        current_y_rank = next_y_rank;
                        prev_node_id = next_node_id;
                    }
                }
                return current_y_rank;
            }

            bool verify() const
            {
                for (uint64_t h = 0; h < this->bits_seq.size(); h++)
                {
                    for (uint64_t i = 0; i < this->bits_seq[h].size(); i++)
                    {
                        uint64_t left_tree_size = this->bits_seq[h][i].count0();
                        uint64_t right_tree_size = this->bits_seq[h][i].count1();
                        if (h + 1 < this->bits_seq.size())
                        {
                            if(left_tree_size != this->bits_seq[h + 1][2 * i].size()){
                                throw std::runtime_error("Error: verify, left_tree_size != this->bits_seq[h + 1][2 * i].size()");
                            }
                            if(right_tree_size != this->bits_seq[h + 1][2 * i + 1].size()){
                                throw std::runtime_error("Error: verify, right_tree_size != this->bits_seq[h + 1][2 * i + 1].size()");
                            }
                        }
                        else
                        {
                            if(left_tree_size != this->leaves[2 * i].size()){
                                throw std::runtime_error("Error: verify, left_tree_size != this->leaves[2 * i].size()");
                            }
                            if(right_tree_size != this->leaves[2 * i + 1].size()){
                                throw std::runtime_error("Error: verify, right_tree_size != this->leaves[2 * i + 1].size()");
                            }
                        }
                    }
                }
                return true;
            }

            std::vector<uint64_t> to_rank_elements(uint64_t h, uint64_t node_id) const
            {
                uint64_t height = this->height();
                uint64_t node_size = this->bits_seq[h][node_id].size();
                std::vector<uint64_t> r;
                r.resize(node_size, UINT64_MAX);
                if (h + 1 < height)
                {
                    uint64_t counterL = 0;
                    uint64_t counterR = 0;
                    uint64_t leaf_id_L = 2 * node_id;
                    uint64_t leaf_id_R = 2 * node_id + 1;
                    uint64_t left_tree_size = this->bits_seq[h + 1][leaf_id_L].size();

                    std::vector<uint64_t> left_elements = this->to_rank_elements(h + 1, leaf_id_L);
                    std::vector<uint64_t> right_elements = this->to_rank_elements(h + 1, leaf_id_R);

                    for (uint64_t i = 0; i < node_size; i++)
                    {
                        bool b = this->bits_seq[h][node_id].at(i);
                        if (b)
                        {
                            r[i] = right_elements[counterR++] + left_tree_size;
                        }
                        else
                        {
                            r[i] = left_elements[counterL++];
                        }
                    }
                }
                else
                {
                    uint64_t counterL = 0;
                    uint64_t counterR = 0;
                    uint64_t leaf_id_L = 2 * node_id;
                    uint64_t leaf_id_R = 2 * node_id + 1;
                    uint64_t left_tree_size = this->leaves[leaf_id_L].size();

                    for (uint64_t i = 0; i < node_size; i++)
                    {
                        bool b = this->bits_seq[h][node_id].at(i);
                        if (b)
                        {
                            r[i] = this->leaves[leaf_id_R][counterR++] + left_tree_size;
                        }
                        else
                        {
                            r[i] = this->leaves[leaf_id_L][counterL++];
                        }
                    }
                }
                return r;
            }

            std::vector<uint64_t> to_rank_elements() const
            {
                if (this->height() > 0)
                {
                    return this->to_rank_elements(0, 0);
                }
                else
                {
                    std::vector<uint64_t> r;
                    r.resize(this->size(), UINT64_MAX);
                    for (uint64_t i = 0; i < this->size(); i++)
                    {
                        r[i] = this->leaves[0][i];
                    }
                    return r;
                }
            }
            std::vector<uint64_t> to_rank_elements_in_x_order() const
            {
                std::vector<uint64_t> r;
                r.resize(this->size(), UINT64_MAX);
                for (auto it = this->x_rank_begin(); it != this->x_rank_end(); ++it)
                {
                    r[*it] = this->access_y_rank(*it);
                }
                /*
                for (uint64_t i = 0; i < this->size(); i++)
                {
                    r[i] = this->access_y_rank(i);
                }
                */
                return r;
            }

            uint64_t compute_local_x_rank(uint64_t node_y, uint64_t node_id, uint64_t element_position_in_node) const
            {
                uint64_t x_rank = 0;
                uint64_t h_y_rank = element_position_in_node;
                uint64_t h_node_id = node_id;
                int64_t height = this->height();
                for (int64_t h = node_y; h < height; h++)
                {
                    bool b = this->bits_seq[h][h_node_id].at(h_y_rank);
                    uint64_t next_node_id = (2 * h_node_id) + (uint64_t)b;
                    if (b)
                    {
                        uint64_t left_tree_size = this->bits_seq[h][h_node_id].count0();
                        x_rank += left_tree_size;
                        h_y_rank -= this->bits_seq[h][h_node_id].rank0(h_y_rank);
                    }
                    else
                    {
                        h_y_rank -= this->bits_seq[h][h_node_id].rank1(h_y_rank);
                    }
                    h_node_id = next_node_id;
                }
                x_rank += this->leaves[h_node_id][h_y_rank];
                return x_rank;
            }

            template <typename APPENDABLE_VECTOR>
            uint64_t local_range_report_on_internal_node(uint64_t h, uint64_t node_id, uint64_t x_rank_gap, uint64_t hy_min, uint64_t hy_max, APPENDABLE_VECTOR &out) const
            {
                for (uint64_t i = hy_min; i <= hy_max; i++)
                {
                    uint64_t x = this->compute_local_x_rank(h, node_id, i) + x_rank_gap;
                    out.push_back(x);
                }
                return hy_max - hy_min + 1;
            }
            template <typename APPENDABLE_VECTOR>
            uint64_t local_range_report_on_leaf(uint64_t leaf_id, uint64_t x_rank_gap, uint64_t x_min, uint64_t x_max, uint64_t hy_min, uint64_t hy_max, APPENDABLE_VECTOR &out) const
            {
                assert(hy_max < this->leaves[leaf_id].size());

                for (uint64_t i = hy_min; i <= hy_max; i++)
                {
                    uint64_t x = this->leaves[leaf_id][i] + x_rank_gap;
                    if (x >= x_min && x <= x_max)
                    {
                        out.push_back(x);
                    }
                }
                return hy_max - hy_min + 1;
            }

            template <typename APPENDABLE_VECTOR>
            uint64_t recursive_range_report_on_internal_nodes(uint64_t h, uint64_t node_id, uint64_t x_rank_gap, int64_t x_min, int64_t x_max, uint64_t hy_min, uint64_t hy_max, APPENDABLE_VECTOR &out) const
            {
                uint64_t found_elements_count = 0;
                int64_t node_x_min = x_rank_gap;
                int64_t node_size = this->bits_seq[h][node_id].size();
                if (x_min <= node_x_min && (node_x_min + node_size - 1) <= x_max)
                {
                    found_elements_count += local_range_report_on_internal_node(h, node_id, node_x_min, hy_min, hy_max, out);
                }
                else
                {
                    uint64_t node_x_min_L = node_x_min;
                    uint64_t node_x_min_R = node_x_min + this->bits_seq[h][node_id].count0();

                    int64_t hy_max_0 = ((int64_t)this->bits_seq[h][node_id].rank0(hy_max + 1)) - 1;
                    int64_t hy_max_1 = ((int64_t)this->bits_seq[h][node_id].rank1(hy_max + 1)) - 1;

                    int64_t hy_min_0 = hy_min > 0 ? ((int64_t)this->bits_seq[h][node_id].rank0(hy_min)) : 0;
                    int64_t hy_min_1 = hy_min > 0 ? ((int64_t)this->bits_seq[h][node_id].rank1(hy_min)) : 0;

                    uint64_t next_node_id_L = 2 * node_id;
                    uint64_t next_node_id_R = next_node_id_L + 1;

                    if ((int64_t)h + 1 < this->height())
                    {
                        if (hy_min_0 >= 0 && hy_min_0 <= hy_max_0)
                        {
                            found_elements_count += this->recursive_range_report_on_internal_nodes(h + 1, next_node_id_L, node_x_min_L, x_min, x_max, hy_min_0, hy_max_0, out);
                        }
                        if (hy_min_1 >= 0 && hy_min_1 <= hy_max_1)
                        {
                            found_elements_count += this->recursive_range_report_on_internal_nodes(h + 1, next_node_id_R, node_x_min_R, x_min, x_max, hy_min_1, hy_max_1, out);
                        }
                    }
                    else
                    {
                        if (hy_min_0 >= 0 && hy_min_0 <= hy_max_0)
                        {
                            assert(hy_max_0 < (int64_t)this->leaves[next_node_id_L].size());
                            found_elements_count += this->local_range_report_on_leaf(next_node_id_L, node_x_min_L, x_min, x_max, hy_min_0, hy_max_0, out);
                        }
                        if (hy_min_1 >= 0 && hy_min_1 <= hy_max_1)
                        {
                            assert(hy_max_1 < (int64_t)this->leaves[next_node_id_R].size());
                            found_elements_count += this->local_range_report_on_leaf(next_node_id_R, node_x_min_R, x_min, x_max, hy_min_1, hy_max_1, out);
                        }
                    }
                }
                return found_elements_count;
            }

            template <typename APPENDABLE_VECTOR>
            uint64_t range_report(uint64_t x_min, uint64_t x_max, uint64_t y_min, uint64_t y_max, APPENDABLE_VECTOR &out) const
            {
                uint64_t found_elements_count = 0;
                if (this->height() > 0)
                {
                    found_elements_count = this->recursive_range_report_on_internal_nodes(0, 0, 0, x_min, x_max, y_min, y_max, out);
                }
                else
                {
                    found_elements_count = this->local_range_report_on_leaf(0, 0, x_min, x_max, y_min, y_max, out);
                }
                return found_elements_count;
            }

            XRankIterator x_rank_begin() const
            {
                if(this->size() == 0){
                    return XRankIterator(this, this->size(), this->size());

                }else{
                    return XRankIterator(this, 0, this->access_y_rank(0));
                }
            }
            XRankIterator x_rank_end() const
            {
                return XRankIterator(this, this->size(), this->size());
            }
            YRankIterator y_rank_begin() const
            {
                if (this->size() == 0)
                {
                    return YRankIterator(this, this->size(), this->size());
                }
                else
                {
                    return YRankIterator(this, this->access_x_rank(0), 0);
                }
            }
            YRankIterator y_rank_end() const
            {
                return YRankIterator(this, this->size(), this->size());
            }

            void print_tree() const
            {
                std::vector<std::string> r;
                for (uint64_t h = 0; h < this->bits_seq.size(); h++)
                {
                    std::string s = "";
                    for (uint64_t i = 0; i < this->bits_seq[h].size(); i++)
                    {
                        s.append(this->bits_seq[h][i].to_string());
                    }
                    r.push_back(s);
                }
                std::string s = "";
                std::vector<char> char_vec{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

                for (uint64_t i = 0; i < this->leaves.size(); i++)
                {
                    s.append("[");

                    for (uint64_t j = 0; j < this->leaves[i].size(); j++)
                    {
                        uint64_t v = this->leaves[i][j];
                        if (v < char_vec.size())
                        {
                            s.push_back(char_vec[v]);
                        }
                        else
                        {
                            s.push_back('?');
                        }
                    }
                    s.append("]");
                }
                r.push_back(s);

                std::cout << "===== TREE =====" << std::endl;

                for (uint64_t i = 0; i < r.size(); i++)
                {
                    std::cout << r[i] << std::endl;
                }
                std::cout << "===== [END] =====" << std::endl;
            }
        };
    }
}
