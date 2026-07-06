#pragma once
#include "../dynamic_bit_sequence.hpp"
#include "..//dynamic_prefix_sum.hpp"

namespace stool
{
    namespace bptree
    {

        /**
         * @brief A dynamic wavelet matrix supporting insertion, deletion, and 2D range reporting on rank coordinates.
         *
         * The structure stores \p n elements as pairs of x-rank and y-rank coordinates.
         * Elements are ordered primarily by y-rank; x-rank is the associated value at each y-rank position.
         * \ingroup RangeSearchClasses
         */
        class DynamicWaveletMatrixForRangeSearch
        {
            using BIT_SEQUENCE = SimpleDynamicBitSequence;
            using PREFIX_SUM = SimpleDynamicPrefixSum;

            // H n-bit sequences
            std::vector<BIT_SEQUENCE> bits_seq;
            std::vector<PREFIX_SUM> length_seq;

            // std::vector<stool::NaiveFLCVector<false>> leaves;

            // inline static uint64_t LEAF_MAX_SIZE = 8;

        public:
            /**
             * @brief Random-access iterator traversing elements in y-rank order.
             *
             * Dereferencing yields the x-rank of the element at the current y-rank position.
             */
            class YRankIterator
            {
            public:
                using iterator_category = std::random_access_iterator_tag; // C++17
                using value_type = uint64_t;
                using difference_type = std::ptrdiff_t;

                const DynamicWaveletMatrixForRangeSearch *container = nullptr;
                uint64_t x_rank = 0;
                uint64_t y_rank = 0;

                /** @brief Default constructor creating an invalid end iterator. */
                YRankIterator() : container(nullptr), x_rank(0), y_rank(0) {}

                /**
                 * @brief Construct an iterator at a given position.
                 * @param container Pointer to the parent structure.
                 * @param x_rank The x-rank at the current position.
                 * @param y_rank The y-rank position.
                 */
                YRankIterator(const DynamicWaveletMatrixForRangeSearch *container, uint64_t x_rank, uint64_t y_rank) : container(container), x_rank(x_rank), y_rank(y_rank) {}

                /** @brief Return the x-rank at the current y-rank position. */
                uint64_t operator*() const noexcept { return this->x_rank; }

                /** @brief Pre-increment: advance to the next y-rank and update the x-rank. */
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
                /** @brief Post-increment: advance to the next y-rank and return the previous iterator. */
                YRankIterator operator++(int) noexcept
                {
                    YRankIterator t = *this;
                    ++*this;
                    return t;
                }
                /** @brief Pre-decrement: move to the previous y-rank and update the x-rank. */
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
                /** @brief Post-decrement: move to the previous y-rank and return the previous iterator. */
                YRankIterator operator--(int) noexcept
                {
                    YRankIterator t = *this;
                    --*this;
                    return t;
                }

                /**
                 * @brief Advance the iterator by \p n y-rank positions.
                 * @param n Number of positions to advance (may be negative).
                 */
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
                /**
                 * @brief Move the iterator back by \p n y-rank positions.
                 * @param n Number of positions to retreat.
                 */
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
                /** @brief Return an iterator advanced by \p n y-rank positions. */
                friend YRankIterator operator+(YRankIterator it, difference_type n) noexcept { return YRankIterator(it.container, it.y_rank + n, it.x_rank); }
                /** @brief Return an iterator advanced by \p n y-rank positions. */
                friend YRankIterator operator+(difference_type n, YRankIterator it) noexcept { return it + n; }
                /** @brief Return an iterator moved back by \p n y-rank positions. */
                friend YRankIterator operator-(YRankIterator it, difference_type n) noexcept { return YRankIterator(it.container, it.y_rank - n, it.x_rank); }
                /** @brief Return the difference in y-rank between two iterators. */
                friend difference_type operator-(YRankIterator a, YRankIterator b) noexcept { return a.y_rank - b.y_rank; }

                /** @brief Equality comparison based on y-rank. */
                friend bool operator==(YRankIterator a, YRankIterator b) noexcept { return a.y_rank == b.y_rank; }
                /** @brief Inequality comparison based on y-rank. */
                friend bool operator!=(YRankIterator a, YRankIterator b) noexcept { return !(a == b); }
                /** @brief Less-than comparison based on y-rank. */
                friend bool operator<(YRankIterator a, YRankIterator b) noexcept { return a.y_rank < b.y_rank; }
                /** @brief Greater-than comparison based on y-rank. */
                friend bool operator>(YRankIterator a, YRankIterator b) noexcept { return b < a; }
                /** @brief Less-than-or-equal comparison based on y-rank. */
                friend bool operator<=(YRankIterator a, YRankIterator b) noexcept { return !(b < a); }
                /** @brief Greater-than-or-equal comparison based on y-rank. */
                friend bool operator>=(YRankIterator a, YRankIterator b) noexcept { return !(a < b); }
            };

            /**
             * @brief Random-access iterator traversing elements in x-rank order.
             *
             * Dereferencing yields the y-rank of the element at the current x-rank position.
             */
            class XRankIterator
            {
            public:
                using iterator_category = std::random_access_iterator_tag; // C++17
                using value_type = uint64_t;
                using difference_type = std::ptrdiff_t;

                const DynamicWaveletMatrixForRangeSearch *container = nullptr;
                uint64_t x_rank = 0;
                uint64_t y_rank = 0;

                /** @brief Default constructor creating an invalid end iterator. */
                XRankIterator() : container(nullptr), x_rank(0), y_rank(0) {}

                /**
                 * @brief Construct an iterator at a given position.
                 * @param container Pointer to the parent structure.
                 * @param x_rank The x-rank position.
                 * @param y_rank The y-rank at the current position.
                 */
                XRankIterator(const DynamicWaveletMatrixForRangeSearch *container, uint64_t x_rank, uint64_t y_rank) : container(container), x_rank(x_rank), y_rank(y_rank) {}

                /** @brief Return the y-rank at the current x-rank position. */
                uint64_t operator*() const noexcept { return this->y_rank; }

                /** @brief Pre-increment: advance to the next x-rank and update the y-rank. */
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
                /** @brief Post-increment: advance to the next x-rank and return the previous iterator. */
                XRankIterator operator++(int) noexcept
                {
                    XRankIterator t = *this;
                    ++*this;
                    return t;
                }
                /** @brief Pre-decrement: move to the previous x-rank and update the y-rank. */
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
                /** @brief Post-decrement: move to the previous x-rank and return the previous iterator. */
                XRankIterator operator--(int) noexcept
                {
                    XRankIterator t = *this;
                    --*this;
                    return t;
                }

                /**
                 * @brief Advance the iterator by \p n x-rank positions.
                 * @param n Number of positions to advance (may be negative).
                 */
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
                /**
                 * @brief Move the iterator back by \p n x-rank positions.
                 * @param n Number of positions to retreat.
                 */
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
                /** @brief Return an iterator advanced by \p n x-rank positions. */
                friend XRankIterator operator+(XRankIterator it, difference_type n) noexcept { return XRankIterator(it.container, it.y_rank + n, it.x_rank); }
                /** @brief Return an iterator advanced by \p n x-rank positions. */
                friend XRankIterator operator+(difference_type n, XRankIterator it) noexcept { return it + n; }
                /** @brief Return an iterator moved back by \p n x-rank positions. */
                friend XRankIterator operator-(XRankIterator it, difference_type n) noexcept { return XRankIterator(it.container, it.y_rank - n, it.x_rank); }
                /** @brief Return the difference in x-rank between two iterators. */
                friend difference_type operator-(XRankIterator a, XRankIterator b) noexcept { return a.y_rank - b.y_rank; }

                /** @brief Equality comparison based on x-rank. */
                friend bool operator==(XRankIterator a, XRankIterator b) noexcept { return a.y_rank == b.y_rank; }
                /** @brief Inequality comparison based on x-rank. */
                friend bool operator!=(XRankIterator a, XRankIterator b) noexcept { return !(a == b); }
                /** @brief Less-than comparison based on x-rank. */
                friend bool operator<(XRankIterator a, XRankIterator b) noexcept { return a.y_rank < b.y_rank; }
                /** @brief Greater-than comparison based on x-rank. */
                friend bool operator>(XRankIterator a, XRankIterator b) noexcept { return b < a; }
                /** @brief Less-than-or-equal comparison based on x-rank. */
                friend bool operator<=(XRankIterator a, XRankIterator b) noexcept { return !(b < a); }
                /** @brief Greater-than-or-equal comparison based on x-rank. */
                friend bool operator>=(XRankIterator a, XRankIterator b) noexcept { return !(a < b); }
            };

            /** @brief Default constructor. Initializes an empty structure. */
            DynamicWaveletMatrixForRangeSearch()
            {
                this->clear();
            }

        private:
            /**
             * @brief Return the starting index of a node's bit sequence at level \p h.
             * @param h Level in the wavelet tree (0 is the root level).
             * @param h_node_id Node identifier at level \p h.
             * @return The offset in \p bits_seq[h] where this node's bits begin.
             * @warning O(log n) time
             */
            uint64_t get_node_x_pos_in_bit_sequence(int64_t h, uint64_t h_node_id) const
            {
                if (h_node_id == 0)
                {
                    return 0;
                }
                else
                {
                    return this->length_seq[h].psum(h_node_id - 1);
                }
            }

            /**
             * @brief Return the number of zeros in the prefix \p S[0..i] of a node's bit sequence.
             * @param h Level in the wavelet tree.
             * @param h_node_id Node identifier at level \p h.
             * @param node_x_pos_in_bit_sequence Starting index of the node's bits in \p bits_seq[h].
             * @param i Inclusive end index within the node (0-based).
             * @return The rank of zero up to position \p i within the node.
             * @warning O(log n) time
             */
            uint64_t rank0_in_bit_sequence_of_node(uint64_t h, [[maybe_unused]] uint64_t h_node_id, uint64_t node_x_pos_in_bit_sequence, uint64_t i) const
            {
                assert(i <= this->length_seq[h].at(h_node_id));
                assert(node_x_pos_in_bit_sequence == this->get_node_x_pos_in_bit_sequence(h, h_node_id));
                return this->bits_seq[h].one_based_rank0(node_x_pos_in_bit_sequence + i + 1) - this->bits_seq[h].one_based_rank0(node_x_pos_in_bit_sequence);
            }

            /**
             * @brief Return the number of ones in the prefix \p S[0..i] of a node's bit sequence.
             * @param h Level in the wavelet tree.
             * @param h_node_id Node identifier at level \p h.
             * @param node_x_pos_in_bit_sequence Starting index of the node's bits in \p bits_seq[h].
             * @param i Inclusive end index within the node (0-based).
             * @return The rank of one up to position \p i within the node.
             * @warning O(log n) time
             */
            uint64_t rank1_in_bit_sequence_of_node(uint64_t h, [[maybe_unused]] uint64_t h_node_id, uint64_t node_x_pos_in_bit_sequence, uint64_t i) const
            {
                assert(i <= this->length_seq[h].at(h_node_id));
                assert(node_x_pos_in_bit_sequence == this->get_node_x_pos_in_bit_sequence(h, h_node_id));
                return this->bits_seq[h].one_based_rank1(node_x_pos_in_bit_sequence + i + 1) - this->bits_seq[h].one_based_rank1(node_x_pos_in_bit_sequence);
            }

            /**
             * @brief Recursively insert an element at local coordinates \p (x_rank, y_rank) within a subtree.
             * @param h Current level in the wavelet tree.
             * @param h_node_id Node identifier at level \p h.
             * @param x_rank Local x-rank within the subtree rooted at \p h_node_id.
             * @param y_rank Local y-rank within the subtree rooted at \p h_node_id.
             * @param output_path Output vector recording the node path taken during insertion.
             * @warning O(h log n) time
             */
            void recursive_add(int64_t h, uint64_t h_node_id, uint64_t x_rank, uint64_t y_rank, std::vector<uint64_t> &output_path)
            {
                output_path[h] = h_node_id;
                uint64_t node_size = this->length_seq[h].at(h_node_id);
                uint64_t node_x_pos_in_bit_sequence = this->get_node_x_pos_in_bit_sequence(h, h_node_id);

                if (h + 1 < this->height())
                {
                    uint64_t left_node_id = 2 * h_node_id;
                    uint64_t right_node_id = 2 * h_node_id + 1;
                    uint64_t left_tree_size = this->length_seq[h + 1].at(left_node_id);

                    if (x_rank <= left_tree_size)
                    {
                        uint64_t new_y_rank = y_rank > 0 ? this->rank0_in_bit_sequence_of_node(h, h_node_id, node_x_pos_in_bit_sequence, y_rank - 1) : 0;
                        this->recursive_add(h + 1, left_node_id, x_rank, new_y_rank, output_path);
                        this->bits_seq[h].insert(node_x_pos_in_bit_sequence + y_rank, false);
                        this->length_seq[h].increment(h_node_id, 1);
                    }
                    else
                    {
                        uint64_t new_y_rank = y_rank > 0 ? this->rank1_in_bit_sequence_of_node(h, h_node_id, node_x_pos_in_bit_sequence, y_rank - 1) : 0;
                        uint64_t new_x_rank = x_rank - left_tree_size;

                        this->recursive_add(h + 1, right_node_id, new_x_rank, new_y_rank, output_path);
                        this->bits_seq[h].insert(node_x_pos_in_bit_sequence + y_rank, true);
                        this->length_seq[h].increment(h_node_id, 1);
                    }
                }
                else
                {
                    assert(this->get_bit_count_in_node(h, h_node_id) <= 1);
                    if (node_size == 0)
                    {
                        this->bits_seq[h].insert(node_x_pos_in_bit_sequence + y_rank, false);
                        this->length_seq[h].increment(h_node_id, 1);
                    }
                    else if (node_size == 1)
                    {
                        assert(x_rank <= 1);
                        if (x_rank == 0)
                        {
                            this->bits_seq[h].set_bit(node_x_pos_in_bit_sequence, true);
                            this->bits_seq[h].insert(node_x_pos_in_bit_sequence + y_rank, false);
                        }
                        else
                        {

                            this->bits_seq[h].insert(node_x_pos_in_bit_sequence + y_rank, true);
                        }
                        this->length_seq[h].increment(h_node_id, 1);
                    }
                    else
                    {
                        throw std::runtime_error("node_size > 1");
                    }
                }
            }
            /**
             * @brief Return the upper capacity threshold for the root node of a tree with height \p H.
             * @param H Tree height.
             * @return Maximum recommended size before the root subtree should be expanded or rebuilt.
             */
            static uint64_t _get_upper_size_of_root(uint64_t H)
            {
                return _get_upper_size_of_internal_node(0, H);
            }

            /**
             * @brief Return the upper capacity threshold for an internal node at level \p h in a tree of height \p H.
             * @param h Level of the internal node.
             * @param H Tree height.
             * @return Maximum recommended size for the subtree rooted at level \p h.
             */
            static uint64_t _get_upper_size_of_internal_node(uint64_t h, uint64_t H)
            {

                uint64_t u1 = 1;
                for (uint64_t p = h + 1; p < H; p++)
                {
                    u1 *= 2;
                }

                if (u1 > 4)
                {
                    return u1 / 2;
                }
                else
                {
                    return u1;
                }
            }

            /**
             * @brief Return the upper capacity threshold for an internal node at level \p h.
             * @param h Level of the internal node.
             * @return Maximum recommended size for the subtree rooted at level \p h.
             */
            uint64_t get_upper_size_of_internal_node(uint64_t h) const
            {
                return _get_upper_size_of_internal_node(h, this->height());
            }

            /**
             * @brief Return the lower capacity threshold for an internal node at level \p h.
             * @param h Level of the internal node.
             * @return One quarter of the upper capacity threshold at level \p h.
             */
            uint64_t get_lower_size_of_internal_node(uint64_t h) const
            {
                uint64_t fsize = _get_upper_size_of_internal_node(h, this->height());
                return (fsize / 4);
            }

            /**
             * @brief Build the bit sequence at level \p h from rank elements in y-order.
             * @param h Level to build.
             * @param rank_elements Rank elements of the subtree at level \p h, ordered by y-rank.
             * @param output_next_rank_elements Output rank elements for level \p h+1.
             * @param output_next_length_seq Output child node sizes for level \p h+1.
             * @warning O(n log n) time
             */
            void build_h_bit_sequence(uint64_t h, const std::vector<uint64_t> &rank_elements, std::vector<uint64_t> &output_next_rank_elements, std::vector<uint64_t> &output_next_length_seq)
            {

                uint64_t h_node_count = 1ULL << h;
                uint64_t counter = 0;
                uint64_t node_x_pos = 0;
                std::vector<bool> tmp_bit_sequence(rank_elements.size(), false);

                if ((int64_t)(h + 1) < this->height())
                {
                    output_next_rank_elements.resize(rank_elements.size(), UINT64_MAX);
                    output_next_length_seq.resize(h_node_count * 2, UINT64_MAX);
                }

                for (uint64_t i = 0; i < h_node_count; i++)
                {
                    uint64_t bit_size = this->get_bit_count_in_node(h, i);
                    uint64_t half_size = bit_size / 2;

                    // Processing left elements
                    if ((int64_t)(h + 1) < this->height())
                    {
                        uint64_t left_counter = 0;
                        for (uint64_t j = 0; j < bit_size; j++)
                        {
                            if (rank_elements[node_x_pos + j] < half_size)
                            {

                                output_next_rank_elements[counter++] = rank_elements[node_x_pos + j];
                                left_counter++;
                            }
                        }
                        output_next_length_seq[i * 2] = left_counter;
                    }

                    // Processing right elements
                    {
                        uint64_t right_counter = 0;

                        if ((int64_t)(h + 1) < this->height())
                        {
                            for (uint64_t j = 0; j < bit_size; j++)
                            {
                                if (rank_elements[node_x_pos + j] >= half_size)
                                {

                                    tmp_bit_sequence[node_x_pos + j] = true;
                                    output_next_rank_elements[counter++] = rank_elements[node_x_pos + j] - half_size;
                                    right_counter++;
                                }
                            }
                            output_next_length_seq[(i * 2) + 1] = right_counter;
                        }
                        else
                        {
                            if (bit_size > 1)
                            {
                                throw std::runtime_error("Error in build_h_bit_sequence, bit_size > 1");
                            }
                        }
                    }

                    node_x_pos += bit_size;
                }
                this->bits_seq[h].clear();
                this->bits_seq[h].push_many(tmp_bit_sequence);
            }

            /**
             * @brief Rebuild the bit sequence at level \p h for a contiguous range of nodes.
             * @param h Level to rebuild.
             * @param first_node_id First node identifier at level \p h in the rebuild range.
             * @param local_h_node_count Number of consecutive nodes to rebuild.
             * @param rank_elements Rank elements of the subtree, ordered by y-rank.
             * @param output_next_rank_elements Output rank elements for level \p h+1.
             * @param output_next_length_seq Output child node sizes for level \p h+1.
             * @warning O(n log n) time
             */
            void rebuild_h_bit_sequence(uint64_t h, uint64_t first_node_id, uint64_t local_h_node_count, const std::vector<uint64_t> &rank_elements, std::vector<uint64_t> &output_next_rank_elements, std::vector<uint64_t> &output_next_length_seq)
            {
                assert(first_node_id + local_h_node_count - 1 < this->length_seq[h].size());

                uint64_t counter = 0;
                uint64_t node_x_pos = this->get_node_x_pos_in_bit_sequence(h, first_node_id);
                const uint64_t first_node_x_pos = node_x_pos;
                std::vector<bool> tmp_bit_sequence(rank_elements.size(), false);

                if ((int64_t)(h + 1) < this->height())
                {
                    output_next_rank_elements.resize(rank_elements.size(), UINT64_MAX);
                    output_next_length_seq.resize(local_h_node_count * 2, UINT64_MAX);
                }

                for (uint64_t node_id = first_node_id; node_id <= first_node_id + local_h_node_count - 1; node_id++)
                {
                    uint64_t bit_size = this->get_bit_count_in_node(h, node_id);
                    uint64_t half_size = bit_size / 2;

                    // Processing left elements
                    if ((int64_t)(h + 1) < this->height())
                    {
                        uint64_t left_counter = 0;
                        for (uint64_t j = 0; j < bit_size; j++)
                        {
                            if (rank_elements[(node_x_pos - first_node_x_pos) + j] < half_size)
                            {
                                output_next_rank_elements[counter++] = rank_elements[(node_x_pos - first_node_x_pos) + j];
                                left_counter++;
                            }
                        }
                        output_next_length_seq[(node_id - first_node_id) * 2] = left_counter;
                    }

                    // Processing right elements
                    {
                        uint64_t right_counter = 0;

                        if ((int64_t)(h + 1) < this->height())
                        {
                            for (uint64_t j = 0; j < bit_size; j++)
                            {
                                if (rank_elements[(node_x_pos - first_node_x_pos) + j] >= half_size)
                                {

                                    tmp_bit_sequence[(node_x_pos - first_node_x_pos) + j] = true;
                                    output_next_rank_elements[counter++] = rank_elements[(node_x_pos - first_node_x_pos) + j] - half_size;
                                    right_counter++;
                                }
                            }
                            output_next_length_seq[(node_id - first_node_id) * 2 + 1] = right_counter;
                        }
                        else
                        {
                            if (bit_size > 1)
                            {
                                throw std::runtime_error("Error in rebuild_h_bit_sequence, bit_size > 1");
                            }
                        }
                    }

                    node_x_pos += bit_size;
                }
                this->bits_seq[h].set_bits(first_node_x_pos, tmp_bit_sequence);
            }

            /**
             * @brief Check whether a node is unbalanced and should be rebuilt.
             * @param h Level of the node.
             * @param h_node_id Node identifier at level \p h.
             * @return True if the left/right child sizes are too skewed, a child is full, or a leaf has size >= 2.
             */
            bool is_unbalanced_node(uint8_t h, uint64_t h_node_id) const
            {
                if (h + 1 < this->height())
                {
                    uint64_t left_node_id = 2 * h_node_id;
                    uint64_t right_node_id = 2 * h_node_id + 1;
                    uint64_t left_tree_size = this->get_bit_count_in_node(h + 1, left_node_id);
                    uint64_t right_tree_size = this->get_bit_count_in_node(h + 1, right_node_id);
                    bool unbalance_flag_LR = left_tree_size > (right_tree_size * 2) || right_tree_size > (left_tree_size * 2);
                    uint64_t child_upper_size = this->get_upper_size_of_internal_node(h + 1);
                    bool full_flag_L = left_tree_size >= child_upper_size;
                    bool full_flag_R = right_tree_size >= child_upper_size;
                    return unbalance_flag_LR || full_flag_L || full_flag_R;
                }
                else
                {
                    return this->length_seq[h].at(h_node_id) >= 2;
                }
            }

        public:
            /** @brief Clear all bit sequences, length sequences, and reset the structure to empty. */
            void clear()
            {
                for (uint64_t i = 0; i < this->bits_seq.size(); i++)
                {
                    this->bits_seq[i].clear();
                    this->length_seq[i].clear();
                }
                this->bits_seq.clear();
                this->length_seq.clear();
            }

            /**
             * @brief Insert an element at global coordinates \p (x_rank, y_rank).
             *
             * After insertion, the structure may be partially or fully rebuilt
             * if capacity or balance thresholds are exceeded.
             * @param x_rank The x-rank of the element to insert.
             * @param y_rank The y-rank position at which to insert.
             * @warning amortized O(H log n) time
             */
            void add(uint64_t x_rank, uint64_t y_rank)
            {

                if (this->size() > 0)
                {
                    std::vector<uint64_t> output_path(this->height(), UINT64_MAX);
                    this->recursive_add(0, 0, x_rank, y_rank, output_path);
                    uint64_t upper_size = this->get_upper_size_of_internal_node(0);
                    if (this->size() >= upper_size)
                    {

                        std::vector<uint64_t> rank_elements = this->to_rank_elements_in_y_order();
                        this->build(rank_elements);
                    }
                    else
                    {
                        uint64_t height = this->height();
                        for (uint64_t h = 0; h < height; h++)
                        {
                            uint64_t h_node_id = output_path[h];
                            if (this->is_unbalanced_node(h, h_node_id))
                            {
                                this->rebuild_internal_node(h, h_node_id);
                                break;
                            }
                        }
                    }
                    assert(this->verify());
                }
                else
                {
                    this->clear();
                    std::vector<uint64_t> rank_elements;
                    rank_elements.push_back(0);
                    this->build(rank_elements);
                }
            }

            /**
             * @brief Remove the element at global y-rank \p y_rank.
             *
             * After removal, the structure may be rebuilt if the size falls below
             * half of the root node's upper capacity threshold.
             * @param y_rank The y-rank of the element to remove.
             * @throws std::runtime_error if the structure is empty.
             * @warning amortized O(H log n) time
             */
            void remove(uint64_t y_rank)
            {
                int64_t height = this->height();
                if (height == 0)
                {
                    throw std::runtime_error("Error: DynamicWaveletMatrixForRangeSearch::remove(y_rank)");
                }
                else
                {
                    uint64_t h_y_rank = y_rank;
                    uint64_t h_node_id = 0;

                    for (int64_t h = 0; h < height; h++)
                    {
                        uint64_t node_x_pos = this->get_node_x_pos_in_bit_sequence(h, h_node_id);
                        bool b = this->bits_seq[h].at(node_x_pos + h_y_rank);
                        uint64_t next_node_id = (2 * h_node_id) + (uint64_t)b;
                        if (b)
                        {
                            uint64_t rmv_y_rank = this->rank0_in_bit_sequence_of_node(h, h_node_id, node_x_pos, h_y_rank);
                            this->bits_seq[h].remove(node_x_pos + h_y_rank);
                            this->length_seq[h].decrement(h_node_id, 1);
                            h_y_rank -= rmv_y_rank;
                        }
                        else
                        {
                            uint64_t rmv_y_rank = this->rank1_in_bit_sequence_of_node(h, h_node_id, node_x_pos, h_y_rank);
                            this->bits_seq[h].remove(node_x_pos + h_y_rank);
                            this->length_seq[h].decrement(h_node_id, 1);
                            h_y_rank -= rmv_y_rank;
                        }
                        h_node_id = next_node_id;
                    }

                    uint64_t upper_size = this->get_upper_size_of_internal_node(0);
                    uint64_t size = this->size();
                    if (size < upper_size / 2)
                    {
                        auto rank_elements = this->to_rank_elements_in_y_order();
                        this->build(rank_elements);
                    }

                    assert(this->verify());
                }
            }

            /**
             * @brief Build the entire structure from rank elements ordered by y-rank.
             * @param rank_elements Vector of x-ranks in y-rank order.
             * @param message_paragraph Indentation level for progress messages (use stool::Message::NO_MESSAGE to suppress).
             * @warning O(n log^2 n) time
             */
            void build(const std::vector<uint64_t> &rank_elements, int message_paragraph = stool::Message::NO_MESSAGE)
            {

                this->clear();

                uint64_t height = 0;
                while (true)
                {
                    uint64_t fsize = _get_upper_size_of_root(height);
                    if (rank_elements.size() < fsize)
                    {
                        break;
                    }
                    else
                    {
                        height++;
                    }
                }

                if (message_paragraph != stool::Message::NO_MESSAGE)
                {
                    std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Building wavelet tree for range search... " << "(input size = " << rank_elements.size() << ", tree height = " << height << ")" << std::endl;
                }
                std::chrono::system_clock::time_point st1, st2;
                st1 = std::chrono::system_clock::now();

                this->bits_seq.resize(height);
                this->length_seq.resize(height);
                for (uint64_t h = 0; h < height; h++)
                {
                    this->bits_seq[h].clear();
                    this->length_seq[h].clear();
                }

                if (height > 0)
                {
                    this->length_seq[0].push_back(rank_elements.size());
                    std::vector<uint64_t> tmp_rank_elements = rank_elements;

                    for (uint64_t h = 0; h < height; h++)
                    {

                        if (message_paragraph != stool::Message::NO_MESSAGE)
                        {
                            std::cout << stool::Message::get_paragraph_string(message_paragraph + 1) << "Building the " << h << "-th bit sequence in the wavelet tree... " << std::endl;
                        }
                        std::vector<uint64_t> next_rank_elements;
                        std::vector<uint64_t> next_length_seq;

                        this->build_h_bit_sequence(h, tmp_rank_elements, next_rank_elements, next_length_seq);

                        tmp_rank_elements.swap(next_rank_elements);
                        if (h + 1 < height)
                        {
                            this->length_seq[h + 1].push_many(next_length_seq);
                        }
                    }
                }

                assert(this->verify());

                st2 = std::chrono::system_clock::now();
                uint64_t sec_time = std::chrono::duration_cast<std::chrono::seconds>(st2 - st1).count();

                if (message_paragraph != stool::Message::NO_MESSAGE)
                {
                    std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[DONE] Elapsed Time: " << sec_time << " sec" << std::endl;
                }
            }

            /**
             * @brief Return the number of bits (elements) stored in a node.
             * @param h Level in the wavelet tree.
             * @param h_node_id Node identifier at level \p h.
             * @return The size of the node.
             */
            uint64_t get_bit_count_in_node(uint64_t h, uint64_t h_node_id) const
            {
                assert(h < this->length_seq.size());
                assert(h_node_id < this->length_seq[h].size());
                return this->length_seq[h].at(h_node_id);
            }

            /**
             * @brief Rebuild the subtree rooted at node \p h_node_id on level \p h.
             * @param h Level of the root node of the subtree to rebuild.
             * @param h_node_id Node identifier at level \p h.
             */
            void rebuild_internal_node(uint8_t h, uint64_t h_node_id)
            {

                std::vector<uint64_t> rank_elements = this->to_local_rank_elements_in_y_order(h, h_node_id);

                uint64_t height = this->height();
                uint64_t current_node_id = h_node_id;
                uint64_t current_node_count = 1;
                for (uint64_t q = h; q < height; q++)
                {
                    std::vector<uint64_t> next_rank_elements;
                    std::vector<uint64_t> next_length_seq;

                    this->rebuild_h_bit_sequence(q, current_node_id, current_node_count, rank_elements, next_rank_elements, next_length_seq);

                    rank_elements.swap(next_rank_elements);

                    current_node_count *= 2;
                    current_node_id *= 2;

                    if (q + 1 < height)
                    {
                        this->length_seq[q + 1].set_values(current_node_id, next_length_seq);
                    }
                }
            }

            /**
             * @brief Swap the contents of this structure with \p item.
             * @param item Another DynamicWaveletMatrixForRangeSearch instance.
             */
            void swap(DynamicWaveletMatrixForRangeSearch &item)
            {
                this->length_seq.swap(item.length_seq);
                this->bits_seq.swap(item.bits_seq);
            }

            /**
             * @brief Return the height of the wavelet tree.
             * @return Number of levels in \p bits_seq (0 if empty).
             * @warning O(1) time
             */
            int64_t height() const
            {
                return this->bits_seq.size();
            }

            /**
             * @brief Return the number of stored elements.
             * @return Size of the root-level bit sequence, or 0 if empty.
             * @warning O(1) time
             */
            uint64_t size() const
            {
                if (this->height() > 0)
                {
                    return this->bits_seq[0].size();
                }
                else
                {
                    return 0;
                }
            }

            /**
             * @brief Return the x-rank of the element at global y-rank \p y_rank.
             * @param y_rank Global y-rank position (0-based).
             * @return The corresponding x-rank.
             */
            uint64_t access_x_rank(uint64_t y_rank) const
            {
                assert(y_rank < this->size());
                uint64_t x_rank = this->compute_local_x_rank(0, 0, y_rank);
                return x_rank;
            }

            /**
             * @brief Return the leaf node identifier for global x-rank \p x_rank.
             * @param x_rank Global x-rank position (0-based).
             * @return Node identifier at the leaf level.
             * @throws std::runtime_error if \p x_rank is out of range.
             * @warning O(H log n) time
             */
            uint64_t find_leaf_index(uint64_t x_rank) const
            {

                if (x_rank >= this->size())
                {
                    throw std::runtime_error("ERROR in find_leaf_index: x_rank is out of range");
                }

                uint64_t current_x_rank = x_rank;
                uint64_t current_node_id = 0;
                uint64_t height = this->height();

                for (uint64_t h = 0; h + 1 < height; h++)
                {
                    uint64_t left_tree_size = this->get_bit_count_in_node(h + 1, 2 * current_node_id);

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
                return current_node_id;
            }

            /**
             * @brief Return the y-rank of the element at global x-rank \p x_rank.
             * @param x_rank Global x-rank position (0-based).
             * @return The corresponding y-rank.
             * @warning O(H log n) time
             */
            uint64_t access_y_rank(uint64_t x_rank) const
            {
                uint64_t leaf_index = this->find_leaf_index(x_rank);
                uint64_t current_y_rank = 0;
                uint64_t prev_node_id = leaf_index;
                int64_t height = this->height();
                for (int64_t h = height - 2; h >= 0; h--)
                {
                    uint64_t next_node_id = prev_node_id / 2;
                    uint64_t next_x_pos = this->get_node_x_pos_in_bit_sequence(h, next_node_id);

                    if (prev_node_id % 2 == 0)
                    {
                        uint64_t count_zero_offset = this->bits_seq[h].one_based_rank0(next_x_pos);
                        uint64_t next_y_rank = this->bits_seq[h].select0(current_y_rank + count_zero_offset) - next_x_pos;
                        current_y_rank = next_y_rank;
                        prev_node_id = next_node_id;
                    }
                    else
                    {

                        uint64_t count_one_offset = this->bits_seq[h].one_based_rank1(next_x_pos);
                        int64_t select_result = this->bits_seq[h].select1(current_y_rank + count_one_offset);
                        assert(select_result >= 0);
                        uint64_t next_y_rank = select_result - next_x_pos;

                        current_y_rank = next_y_rank;
                        prev_node_id = next_node_id;
                    }
                }
                return current_y_rank;
            }

            /**
             * @brief Return the bit sequence stored in a node as a vector.
             * @param h Level in the wavelet tree.
             * @param node_id Node identifier at level \p h.
             * @return Copy of the node's bits in y-rank order.
             */
            std::vector<bool> get_bit_sequence(uint64_t h, uint64_t node_id) const
            {
                uint64_t x_pos = this->get_node_x_pos_in_bit_sequence(h, node_id);
                uint64_t node_size = this->get_bit_count_in_node(h, node_id);
                std::vector<bool> r;
                r.resize(node_size, false);
                for (uint64_t i = 0; i < node_size; i++)
                {
                    r[i] = this->bits_seq[h].at(x_pos + i);
                }
                return r;
            }


            /**
             * @brief Extract rank elements in y-order for the subtree rooted at \p (h, node_id).
             * @param h Level of the subtree root.
             * @param node_id Node identifier at level \p h.
             * @return Vector of x-ranks in y-rank order within the subtree.
             */
            std::vector<uint64_t> to_local_rank_elements_in_y_order(uint64_t h, uint64_t node_id) const
            {
                uint64_t height = this->height();
                uint64_t node_size = this->get_bit_count_in_node(h, node_id);
                std::vector<uint64_t> r;
                r.resize(node_size, UINT64_MAX);
                uint64_t x_pos = this->get_node_x_pos_in_bit_sequence(h, node_id);

                if (h + 1 < height)
                {
                    uint64_t counterL = 0;
                    uint64_t counterR = 0;
                    uint64_t leaf_id_L = 2 * node_id;
                    uint64_t leaf_id_R = 2 * node_id + 1;
                    uint64_t left_tree_size = this->get_bit_count_in_node(h + 1, leaf_id_L);

                    std::vector<uint64_t> left_elements = this->to_local_rank_elements_in_y_order(h + 1, leaf_id_L);
                    std::vector<uint64_t> right_elements = this->to_local_rank_elements_in_y_order(h + 1, leaf_id_R);

                    assert(left_elements.size() + right_elements.size() == node_size);

                    for (uint64_t i = 0; i < node_size; i++)
                    {
                        bool b = this->bits_seq[h].at(x_pos + i);
                        if (b)
                        {
                            assert(counterR < right_elements.size());

                            r[i] = right_elements[counterR++] + left_tree_size;
                        }
                        else
                        {
                            assert(counterL < left_elements.size());
                            r[i] = left_elements[counterL++];
                        }
                    }
                }
                else
                {
                    if (node_size == 1)
                    {
                        r[0] = 0;
                    }
                    else if (node_size > 1)
                    {
                        for (uint64_t i = 0; i < node_size; i++)
                        {
                            r[i] = this->bits_seq[h].at(x_pos + i);
                        }
                    }
                }

                return r;
            }

            /**
             * @brief Extract all rank elements in y-order for the entire structure.
             * @return Vector of x-ranks in global y-rank order, or an empty vector if empty.
             */
            std::vector<uint64_t> to_rank_elements_in_y_order() const
            {

                if (this->height() > 0)
                {
                    return this->to_local_rank_elements_in_y_order(0, 0);
                }
                else
                {
                    std::vector<uint64_t> r;
                    return r;
                }
            }

            /**
             * @brief Extract all y-ranks in x-order for the entire structure.
             * @return Vector where \p r[i] is the y-rank of the element with x-rank \p i.
             */
            std::vector<uint64_t> to_rank_elements_in_x_order() const
            {
                std::vector<uint64_t> r;
                r.resize(this->size(), UINT64_MAX);
                uint64_t size = this->size();
                for (uint64_t i = 0; i < size; i++)
                {
                    r[i] = this->access_y_rank(i);
                }

                return r;
            }

            /**
             * @brief Compute the global x-rank of the element at local y-rank \p local_y_rank within a subtree.
             * @param node_y Level of the subtree root.
             * @param node_id Node identifier at level \p node_y.
             * @param local_y_rank Local y-rank within the subtree.
             * @return The global x-rank of the element.
             * @warning O(H log n) time
             */
            uint64_t compute_local_x_rank(uint64_t node_y, uint64_t node_id, uint64_t local_y_rank) const
            {
                assert(local_y_rank < this->length_seq[node_y].at(node_id));

                uint64_t x_rank = 0;
                uint64_t h_node_id = node_id;
                int64_t height = this->height();
                for (int64_t h = node_y; h + 1 < height; h++)
                {
                    uint64_t node_x_pos = this->get_node_x_pos_in_bit_sequence(h, h_node_id);

                    assert(node_x_pos + local_y_rank < this->bits_seq[h].size());

                    bool b = this->bits_seq[h].at(node_x_pos + local_y_rank);
                    uint64_t next_node_id = (2 * h_node_id) + (uint64_t)b;
                    if (b)
                    {
                        uint64_t left_tree_size = this->get_bit_count_in_node(h + 1, 2 * h_node_id);
                        x_rank += left_tree_size;
                        local_y_rank -= this->rank0_in_bit_sequence_of_node(h, h_node_id, node_x_pos, local_y_rank);
                    }
                    else
                    {
                        local_y_rank -= this->rank1_in_bit_sequence_of_node(h, h_node_id, node_x_pos, local_y_rank);
                    }
                    h_node_id = next_node_id;
                }
                // x_rank += this->leaves[h_node_id][h_y_rank];
                return x_rank;
            }

            /**
             * @brief Report all x-ranks in a local y-rank range on a single internal node.
             * @tparam APPENDABLE_VECTOR Vector type supporting \p push_back (e.g. std::vector).
             * @param h Level of the node.
             * @param node_id Node identifier at level \p h.
             * @param x_rank_gap Offset added to each reported local x-rank.
             * @param hy_min Minimum local y-rank (inclusive).
             * @param hy_max Maximum local y-rank (inclusive).
             * @param out Output vector to which x-ranks are appended.
             * @return Number of elements reported.
             * @warning O((hy_max - hy_min + 1) * H log n) time
             */
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

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Main Queries
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            public:
            /**
             * @brief Report all x-ranks of elements whose coordinates lie in the given rectangle.
             * @tparam APPENDABLE_VECTOR Vector type supporting \p push_back (e.g. std::vector).
             * @param x_min Minimum x-rank (inclusive).
             * @param x_max Maximum x-rank (inclusive).
             * @param y_min Minimum y-rank (inclusive).
             * @param y_max Maximum y-rank (inclusive).
             * @param out Output vector to which matching x-ranks are appended.
             * @return Number of m elements reported.
             * @warning O((m + 1) log^2 n) time
             */
            template <typename APPENDABLE_VECTOR>
            uint64_t range_report(uint64_t x_min, uint64_t x_max, uint64_t y_min, uint64_t y_max, APPENDABLE_VECTOR &out) const
            {
                uint64_t found_elements_count = 0;
                if (this->height() > 0)
                {
                    found_elements_count = this->recursive_range_report_on_internal_nodes(0, 0, 0, x_min, x_max, y_min, y_max, out);
                }
                return found_elements_count;
            }

        private:
            /**
             * @brief Recursively report x-ranks in a 2D range on internal nodes.
             * @tparam APPENDABLE_VECTOR Vector type supporting \p push_back (e.g. std::vector).
             * @param h Current level in the wavelet tree.
             * @param node_id Node identifier at level \p h.
             * @param node_x_pos Global x-rank offset of the node's left boundary.
             * @param x_min Minimum global x-rank (inclusive).
             * @param x_max Maximum global x-rank (inclusive).
             * @param hy_min Minimum local y-rank within the node (inclusive).
             * @param hy_max Maximum local y-rank within the node (inclusive).
             * @param out Output vector to which x-ranks are appended.
             * @return Number of elements reported.
             */
            template <typename APPENDABLE_VECTOR>
            uint64_t recursive_range_report_on_internal_nodes(uint64_t h, uint64_t node_id, uint64_t node_x_pos, int64_t x_min, int64_t x_max, uint64_t hy_min, uint64_t hy_max, APPENDABLE_VECTOR &out) const
            {

                uint64_t found_elements_count = 0;
                int64_t node_size = this->get_bit_count_in_node(h, node_id);
                if (x_min <= (int64_t)node_x_pos && (int64_t)(node_x_pos + node_size - 1) <= x_max)
                {
                    uint64_t limitR = std::min((int64_t)hy_max, node_size - 1);

                    if (hy_min <= limitR)
                    {
                        uint64_t _tmp = local_range_report_on_internal_node(h, node_id, node_x_pos, hy_min, limitR, out);
                        found_elements_count += _tmp;
                    }
                }
                else if ((int64_t)(h + 1) < this->height())
                {
                    uint64_t node_x_pos_L = node_x_pos;
                    uint64_t node_x_pos_R = node_x_pos + this->get_bit_count_in_node(h + 1, 2 * node_id);

                    int64_t hy_max_0 = rank0_in_bit_sequence_of_node(h, node_id, node_x_pos_L, hy_max) - 1;
                    int64_t hy_max_1 = rank1_in_bit_sequence_of_node(h, node_id, node_x_pos_L, hy_max) - 1;

                    int64_t hy_min_0 = hy_min > 0 ? rank0_in_bit_sequence_of_node(h, node_id, node_x_pos_L, hy_min - 1) : 0;
                    int64_t hy_min_1 = hy_min > 0 ? rank1_in_bit_sequence_of_node(h, node_id, node_x_pos_L, hy_min - 1) : 0;

                    uint64_t next_node_id_L = 2 * node_id;
                    uint64_t next_node_id_R = next_node_id_L + 1;

                    if (x_min < (int64_t)node_x_pos_R && hy_min_0 <= hy_max_0)
                    {
                        found_elements_count += this->recursive_range_report_on_internal_nodes(h + 1, next_node_id_L, node_x_pos_L, x_min, x_max, hy_min_0, hy_max_0, out);
                    }

                    if (x_max >= (int64_t)node_x_pos_R && hy_min_1 <= hy_max_1)
                    {
                        found_elements_count += this->recursive_range_report_on_internal_nodes(h + 1, next_node_id_R, node_x_pos_R, x_min, x_max, hy_min_1, hy_max_1, out);
                    }
                }
                return found_elements_count;
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Print and verification functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            public:
            /**
             * @brief Verify structural consistency of the wavelet tree.
             *
             * Checks that zero/one counts in each internal node match the sizes
             * of its left and right children, and that leaf nodes contain at most one bit.
             * @return True if the structure is valid.
             * @throws std::runtime_error if an inconsistency is detected.
             */
            bool verify() const
            {

                for (uint64_t h = 0; h < this->bits_seq.size(); h++)
                {
                    uint64_t node_count = 1 << h;

                    if (h + 1 < this->bits_seq.size())
                    {
                        for (uint64_t i = 0; i < node_count; i++)
                        {
                            std::vector<bool> bit_sequence = this->get_bit_sequence(h, i);
                            uint64_t countL = 0;
                            uint64_t countR = 0;
                            for (uint64_t j = 0; j < bit_sequence.size(); j++)
                            {
                                if (bit_sequence[j])
                                {
                                    countR++;
                                }
                                else
                                {
                                    countL++;
                                }
                            }

                            uint64_t left_tree_size = this->get_bit_count_in_node(h + 1, 2 * i);
                            uint64_t right_tree_size = this->get_bit_count_in_node(h + 1, 2 * i + 1);

                            if (countL != left_tree_size)
                            {
                                this->print_tree();
                                throw std::runtime_error("Error: verify, countL != left_tree_size");
                            }

                            if (countR != right_tree_size)
                            {
                                this->print_tree();
                                throw std::runtime_error("Error: verify, countR != right_tree_size");
                            }
                        }
                    }
                    else
                    {
                        for (uint64_t i = 0; i < node_count; i++)
                        {
                            uint64_t bit_size = this->get_bit_count_in_node(h, i);
                            if (bit_size > 1)
                            {
                                this->print_tree();
                                throw std::runtime_error("Error: verify function, bit_size > 1");
                            }
                        }
                    }
                }
                return true;
            }            
            /** @brief Print a textual representation of the wavelet tree to standard output (for debugging). */
            void print_tree() const
            {
                std::vector<std::string> r;
                for (uint64_t h = 0; h < this->bits_seq.size(); h++)
                {
                    std::vector<uint64_t> tmp_len_veq;
                    uint64_t counter = 0;
                    tmp_len_veq.push_back(0);
                    for (uint64_t i = 0; i < this->length_seq[h].size(); i++)
                    {
                        counter += this->length_seq[h].at(i);
                        tmp_len_veq.push_back(counter);
                    }

                    std::string s = "";
                    uint64_t tmp_p = 0;
                    for (uint64_t i = 0; i < this->bits_seq[h].size(); i++)
                    {
                        while (tmp_len_veq[tmp_p] <= i)
                        {
                            s.append("|");
                            tmp_p++;
                        }

                        bool b = this->bits_seq[h].at(i);

                        s.append(b ? "1" : "0");
                    }
                    s.append("|");

                    r.push_back(s);
                }

                std::cout << "===== TREE =====" << std::endl;

                for (uint64_t i = 0; i < r.size(); i++)
                {
                    std::cout << r[i] << std::endl;
                }
                std::cout << "===== [END] =====" << std::endl;
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Iterators
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            /** @brief Return an iterator to the first element in x-rank order. */
            XRankIterator x_rank_begin() const
            {
                if (this->size() == 0)
                {
                    return XRankIterator(this, this->size(), this->size());
                }
                else
                {
                    return XRankIterator(this, 0, this->access_y_rank(0));
                }
            }

            /** @brief Return an iterator past the last element in x-rank order. */
            XRankIterator x_rank_end() const
            {
                return XRankIterator(this, this->size(), this->size());
            }

            /** @brief Return an iterator to the first element in y-rank order. */
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

            /** @brief Return an iterator past the last element in y-rank order. */
            YRankIterator y_rank_end() const
            {
                return YRankIterator(this, this->size(), this->size());
            }

            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Load, save, and builder functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            /**
             * @brief Serialize the structure to a binary output stream.
             * @param item Structure to serialize.
             * @param os Output file stream.
             */
            static void store_to_file(DynamicWaveletMatrixForRangeSearch &item, std::ofstream &os)
            {
                uint64_t height = item.height();
                os.write(reinterpret_cast<const char *>(&height), sizeof(uint64_t));

                for (uint64_t h = 0; h < height; h++)
                {
                    BIT_SEQUENCE::store_to_file(item.bits_seq[h], os);
                    PREFIX_SUM::store_to_file(item.length_seq[h], os);
                }
            }

            /**
             * @brief Serialize the structure to a byte buffer.
             * @param item Structure to serialize.
             * @param output Byte buffer; resized if necessary.
             * @param pos Read/write position in \p output; advanced on return.
             */
            static void store_to_bytes(DynamicWaveletMatrixForRangeSearch &item, std::vector<uint8_t> &output, uint64_t &pos)
            {
                uint64_t bytes = item.size_in_bytes();
                if (output.size() < pos + bytes)
                {
                    output.resize(pos + bytes);
                }
                uint64_t height = item.height();

                std::memcpy(output.data() + pos, &height, sizeof(height));
                pos += sizeof(height);

                for (int64_t h = 0; h < (int64_t)height; h++)
                {
                    BIT_SEQUENCE::store_to_bytes(item.bits_seq[h], output, pos);
                    PREFIX_SUM::store_to_bytes(item.length_seq[h], output, pos);
                }
            }

            /**
             * @brief Return the serialized size of this structure in bytes.
             * @param only_extra_bytes If true, count only dynamically allocated extra bytes in sub-structures.
             * @return Total byte size when serialized.
             */
            uint64_t size_in_bytes(bool only_extra_bytes = false) const
            {
                uint64_t sum = 0;
                sum += sizeof(uint64_t);
                for (int64_t h = 0; h < (int64_t)this->height(); h++)
                {
                    sum += this->bits_seq[h].size_in_bytes(only_extra_bytes);
                    sum += this->length_seq[h].size_in_bytes(only_extra_bytes);
                }
                return sum;
            }

            /**
             * @brief Deserialize a structure from a binary input stream.
             * @param ifs Input file stream positioned at the serialized data.
             * @return Reconstructed DynamicWaveletMatrixForRangeSearch instance.
             */
            static DynamicWaveletMatrixForRangeSearch load_from_file(std::ifstream &ifs)
            {
                DynamicWaveletMatrixForRangeSearch r;
                uint64_t _height = 0;
                ifs.read(reinterpret_cast<char *>(&_height), sizeof(uint64_t));

                r.bits_seq.resize(_height);
                r.length_seq.resize(_height);
                for (uint64_t h = 0; h < _height; h++)
                {
                    SimpleDynamicBitSequence bits = BIT_SEQUENCE::load_from_file(ifs);
                    r.bits_seq[h].swap(bits);

                    SimpleDynamicPrefixSum length_seq = PREFIX_SUM::load_from_file(ifs);
                    r.length_seq[h].swap(length_seq);
                }
                return r;
            }
            /**
             * @brief Deserialize a structure from a byte buffer.
             * @param data Byte buffer containing serialized data.
             * @param pos Read position in \p data; advanced on return.
             * @return Reconstructed DynamicWaveletMatrixForRangeSearch instance.
             */
            static DynamicWaveletMatrixForRangeSearch load_from_bytes(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                DynamicWaveletMatrixForRangeSearch r;
                uint64_t _height;
                std::memcpy(&_height, data.data() + pos, sizeof(_height));
                pos += sizeof(_height);

                r.bits_seq.resize(_height);
                r.length_seq.resize(_height);
                for (uint64_t h = 0; h < _height; h++)
                {
                    SimpleDynamicBitSequence bits = BIT_SEQUENCE::load_from_bytes(data, pos);
                    r.bits_seq[h].swap(bits);

                    SimpleDynamicPrefixSum length_seq = PREFIX_SUM::load_from_bytes(data, pos);
                    r.length_seq[h].swap(length_seq);
                }
                return r;
            }

            /**
             * @brief Return human-readable memory usage statistics for this structure.
             * @param message_paragraph Indentation level for output lines (default: stool::Message::SHOW_MESSAGE).
             * @return Vector of formatted strings describing total and per-level memory usage.
             */
            std::vector<std::string> get_memory_usage_info(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {

                std::vector<std::string> r;
                uint64_t size_in_bytes = this->size_in_bytes();
                uint64_t element_count = this->size();

                double bits_per_element = element_count > 0 ? ((double)size_in_bytes / (double)element_count) : 0;

                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "=DynamicWaveletMatrixForRangeSearch: " + std::to_string(this->size_in_bytes()) + " bytes, " + std::to_string(element_count) + " elements, " + std::to_string(bits_per_element) + " bytes per element =");

                for (uint64_t h = 0; h < this->bits_seq.size(); h++)
                {
                    uint64_t _sub_size = 0;
                    _sub_size += this->bits_seq[h].size_in_bytes();
                    _sub_size += this->length_seq[h].size_in_bytes();

                    uint64_t _bits_per_element = element_count > 0 ? ((double)_sub_size / (double)element_count) : 0;
                    r.push_back(stool::Message::get_paragraph_string(message_paragraph + 1) + "Level " + std::to_string(h) + " in range tree: " + std::to_string(_sub_size) + " bytes" + " (" + std::to_string(_bits_per_element) + " bytes per element)");
                }
                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "==");

                return r;
            }
            //@}
        };
    }
}
