#pragma once
#include "../sequence/dynamic_bit_sequence.hpp"
#include "../prefix_sum/dynamic_prefix_sum.hpp"

namespace stool
{
    namespace bptree
    {
        /// \defgroup RangeSearchClasses Classes for dynamic wavelet tree for range search
        /// @{
        /// @}


        /**
         * @brief A dynamic data structure supporting range search [Unchecked AI's Comment]
         * \ingroup RangeSearchClasses
         */
        class DynamicWaveletTreeForRangeSearch
        {
            using BIT_SEQUENCE = SimpleDynamicBitSequence;
            using PREFIX_SUM = SimpleDynamicPrefixSum;

            std::vector<BIT_SEQUENCE> bits_seq;
            std::vector<PREFIX_SUM> length_seq;


            //std::vector<stool::NaiveFLCVector<false>> leaves;

            //inline static uint64_t LEAF_MAX_SIZE = 8;

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

            void clear()
            {
                for (uint64_t i = 0; i < this->bits_seq.size(); i++)
                {
                    this->bits_seq[i].clear();
                    this->length_seq[i].clear();
                }
                this->bits_seq.clear();
                this->length_seq.clear();
                //this->leaves.clear();

                /*
                for (uint64_t i = 0; i < this->leaves.size(); i++)
                {
                    this->leaves[i].clear();
                }
                this->leaves.push_back(stool::NaiveFLCVector<false>());
                */
            }

            uint64_t get_node_x_pos_in_bit_sequence(int64_t h, uint64_t h_node_id) const{
                if(h_node_id == 0){
                    return 0;
                }
                else{
                    return this->length_seq[h].psum(h_node_id-1);
                }
            }
            /*
            return the number of 0 in S[0..i];
            */
            uint64_t rank0_in_bit_sequence_of_node(uint64_t h, [[maybe_unused]] uint64_t h_node_id, uint64_t node_x_pos_in_bit_sequence, uint64_t i) const {
                assert(i <= this->length_seq[h].at(h_node_id));
                assert(node_x_pos_in_bit_sequence == this->get_node_x_pos_in_bit_sequence(h, h_node_id));
                return this->bits_seq[h].one_based_rank0(node_x_pos_in_bit_sequence + i + 1) - this->bits_seq[h].one_based_rank0(node_x_pos_in_bit_sequence);

            }
            /*
            return the number of 1 in S[0..i];
            */
            uint64_t rank1_in_bit_sequence_of_node(uint64_t h, [[maybe_unused]] uint64_t h_node_id, uint64_t node_x_pos_in_bit_sequence, uint64_t i) const {
                assert(i <= this->length_seq[h].at(h_node_id));
                assert(node_x_pos_in_bit_sequence == this->get_node_x_pos_in_bit_sequence(h, h_node_id));
                return this->bits_seq[h].one_based_rank1(node_x_pos_in_bit_sequence + i + 1) - this->bits_seq[h].one_based_rank1(node_x_pos_in_bit_sequence);

            }


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
                        uint64_t new_y_rank = y_rank > 0 ? this->rank0_in_bit_sequence_of_node(h, h_node_id, node_x_pos_in_bit_sequence, y_rank-1) : 0;
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

                    //uint64_t upper_size = this->get_upper_size_of_internal_node(h);
                    /*
                    if (this->is_unbalanced_node(h, h_node_id))
                    {
                        if(h + 5 < this->height()){
                            std::cout << "Rebuild internal node: h = " << h << ", h_node_id = " << h_node_id << ", H = " << this->height() << "/len = " << this->length_seq[h].at(h_node_id) << "/ s: " << this->get_upper_size_of_internal_node(h) << std::endl;
                        }
                        std::cout << "Rebuild internal node: h = " << h << ", h_node_id = " << h_node_id << ", H = " << this->height() << "/len = " << this->length_seq[h].at(h_node_id) << "/ s: " << this->get_upper_size_of_internal_node(h) << std::endl;
                        this->print_tree();
        
                        this->rebuild_internal_node(h, h_node_id);
                    }
                    */

                }
                else
                {
                    assert(this->get_bit_count_in_node(h, h_node_id) <= 1);
                    if(node_size == 0){
                        this->bits_seq[h].insert(node_x_pos_in_bit_sequence + y_rank, false);
                        this->length_seq[h].increment(h_node_id, 1);    
                    }else if(node_size == 1){
                        assert(x_rank <= 1);
                        if(x_rank == 0){
                            this->bits_seq[h].set_bit(node_x_pos_in_bit_sequence, true);
                            this->bits_seq[h].insert(node_x_pos_in_bit_sequence + y_rank, false);

                        }else{

                            this->bits_seq[h].insert(node_x_pos_in_bit_sequence + y_rank, true);
                        }
                        this->length_seq[h].increment(h_node_id, 1);
                    }else{
                        throw std::runtime_error("node_size > 1");
                    }
                }


            }

            void add(uint64_t x_rank, uint64_t y_rank)
            {
                

                if(this->size() > 0){
                    //std::cout << "Add: x_rank = " << x_rank << ", y_rank = " << y_rank << std::endl;

                    std::vector<uint64_t> output_path(this->height(), UINT64_MAX);
                    this->recursive_add(0, 0, x_rank, y_rank, output_path);
                    uint64_t upper_size = this->get_upper_size_of_internal_node(0);
                    if (this->size() >= upper_size)
                    {
                        /*
                        std::cout << "Rebuilding range reporting data structure ...: ";
                        std::chrono::system_clock::time_point st1, st2;                        
                        st1 = std::chrono::system_clock::now();        
                        */            
    
                        std::vector<uint64_t> rank_elements = this->to_rank_elements_in_y_order();
                        this->build(rank_elements);

                        /*
                        
                        st2 = std::chrono::system_clock::now();
                        uint64_t sec_time = std::chrono::duration_cast<std::chrono::seconds>(st2 - st1).count();
                        std::cout << "[DONE] Elapsed Time: " << sec_time << " sec, the number of elements: " << rank_elements.size() << std::endl;
                        */
                    }else{
                        uint64_t height = this->height();
                        for(uint64_t h = 0; h < height; h++){
                            uint64_t h_node_id = output_path[h];
                            if (this->is_unbalanced_node(h, h_node_id))
                            {
                                /*
                                if(h + 5 < this->height()){
                                    std::cout << "Rebuild internal node: h = " << h << ", h_node_id = " << h_node_id << ", H = " << this->height() << "/len = " << this->length_seq[h].at(h_node_id) << "/ s: " << this->get_upper_size_of_internal_node(h) << std::endl;
                                }
                                */
                
                                this->rebuild_internal_node(h, h_node_id);
                                break;
                            }

                        }
                    }
                    assert(this->verify());

                }else{
                    this->clear();
                    std::vector<uint64_t> rank_elements;
                    rank_elements.push_back(0);
                    this->build(rank_elements);
                    
                }



            }
            
            void remove(uint64_t y_rank)
            {
                int64_t height = this->height();
                if(height == 0){
                    throw std::runtime_error("Error: DynamicWaveletTreeForRangeSearch::remove(y_rank)");
                }else{
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
            

            /*
            static uint64_t get_full_size_of_tree(uint64_t height)
            {
                if(height == 0){
                    return 0;
                }else{
                    return 1 << (height - 1);
                }
            }
            */

            /*
            static uint64_t get_upper_size_of_tree(uint64_t height)
            {
                return _get_upper_size_of_internal_node(0, height);
            }
            */

            /*
            uint64_t get_full_size_of_internal_node(uint64_t h) const
            {
                uint64_t H = this->bits_seq.size();
                return 1 << (H - 1 - h);
            }
            */


            /*
            uint64_t get_upper_size_of_leaf() const
            {
                return LEAF_MAX_SIZE;
            }
            */

            static uint64_t _get_upper_size_of_root(uint64_t H)
            {
                return _get_upper_size_of_internal_node(0, H);
            }


            static uint64_t _get_upper_size_of_internal_node(uint64_t h, uint64_t H)
            {

                uint64_t u1 = 1;
                for (uint64_t p = h + 1; p < H; p++)
                {
                    u1 *= 2;
                }

                if(u1 > 4){
                    return u1 / 2;
                }else{
                    return u1;
                }
            }

            uint64_t get_upper_size_of_internal_node(uint64_t h) const
            {
                return _get_upper_size_of_internal_node(h, this->height());
            }
            uint64_t get_lower_size_of_internal_node(uint64_t h) const
            {
                uint64_t fsize = _get_upper_size_of_internal_node(h, this->height());
                return (fsize / 4);
            }

            /*
            void insert_element_into_internal_node(uint8_t h, uint64_t hnode_id, uint64_t pos, uint64_t rank){


            }
            */

            void build_h_bit_sequence(uint64_t h, const std::vector<uint64_t> &rank_elements, std::vector<uint64_t> &output_next_rank_elements, std::vector<uint64_t> &output_next_length_seq)
            {

                uint64_t h_node_count = 1ULL << h; 
                uint64_t counter = 0;
                uint64_t node_x_pos = 0;
                std::vector<bool> tmp_bit_sequence(rank_elements.size(), false);

                if((int64_t)(h + 1) < this->height()){
                    output_next_rank_elements.resize(rank_elements.size(), UINT64_MAX);                
                    output_next_length_seq.resize(h_node_count * 2, UINT64_MAX);    
                }


                
                for(uint64_t i = 0; i < h_node_count; i++){
                    uint64_t bit_size = this->get_bit_count_in_node(h, i);
                    uint64_t half_size = bit_size / 2;


                    // Processing left elements
                    if((int64_t)(h + 1) < this->height())
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

                        if((int64_t)(h + 1) < this->height()){
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
                        }else{
                            if(bit_size > 1){
                                throw std::runtime_error("Error in build_h_bit_sequence, bit_size > 1");
                            }
                        }
    
                    }

                    node_x_pos += bit_size;
                }
                this->bits_seq[h].clear();
                this->bits_seq[h].push_many(tmp_bit_sequence);                
            }

            void rebuild_h_bit_sequence(uint64_t h, uint64_t first_node_id, uint64_t local_h_node_count, const std::vector<uint64_t> &rank_elements, std::vector<uint64_t> &output_next_rank_elements, std::vector<uint64_t> &output_next_length_seq)
            {
                assert(first_node_id + local_h_node_count - 1 < this->length_seq[h].size());


                uint64_t counter = 0;
                uint64_t node_x_pos = this->get_node_x_pos_in_bit_sequence(h, first_node_id);
                const uint64_t first_node_x_pos = node_x_pos;
                std::vector<bool> tmp_bit_sequence(rank_elements.size(), false);

                if((int64_t)(h + 1) < this->height()){
                    output_next_rank_elements.resize(rank_elements.size(), UINT64_MAX);                
                    output_next_length_seq.resize(local_h_node_count * 2, UINT64_MAX);    
                }


                
                for(uint64_t node_id = first_node_id; node_id <= first_node_id + local_h_node_count - 1; node_id++){
                    uint64_t bit_size = this->get_bit_count_in_node(h, node_id);
                    uint64_t half_size = bit_size / 2;


                    // Processing left elements
                    if((int64_t)(h + 1) < this->height())
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

                        if((int64_t)(h + 1) < this->height()){
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
                        }else{
                            if(bit_size > 1){
                                throw std::runtime_error("Error in rebuild_h_bit_sequence, bit_size > 1");
                            }
                        }
    
                    }

                    node_x_pos += bit_size;
                }
                this->bits_seq[h].set_bits(first_node_x_pos, tmp_bit_sequence);
            }


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

                if(message_paragraph != stool::Message::NO_MESSAGE){
                    std::cout << stool::Message::get_paragraph_string(message_paragraph) << "Building wavelet tree for range search... " << "(input size = " << rank_elements.size() << ", tree height = " << height << ")" << std::endl;
                }
                std::chrono::system_clock::time_point st1, st2;
                st1 = std::chrono::system_clock::now();


                this->bits_seq.resize(height);
                this->length_seq.resize(height);
                for(uint64_t h = 0; h < height; h++){
                    this->bits_seq[h].clear();
                    this->length_seq[h].clear();
                }

                if(height > 0){
                    this->length_seq[0].push_back(rank_elements.size());
                    std::vector<uint64_t> tmp_rank_elements = rank_elements;

                    for(uint64_t h = 0; h < height; h++){

                        if(message_paragraph != stool::Message::NO_MESSAGE){
                            std::cout << stool::Message::get_paragraph_string(message_paragraph+1) << "Building the " << h << "-th bit sequence in the wavelet tree... " << std::endl;
                        }
                        std::vector<uint64_t> next_rank_elements;
                        std::vector<uint64_t> next_length_seq;
        
                        this->build_h_bit_sequence(h, tmp_rank_elements, next_rank_elements, next_length_seq);


                        tmp_rank_elements.swap(next_rank_elements);
                        if(h + 1 < height){
                            this->length_seq[h+1].push_many(next_length_seq);
                        }
                    }
                }

                assert(this->verify());

                st2 = std::chrono::system_clock::now();
                uint64_t sec_time = std::chrono::duration_cast<std::chrono::seconds>(st2 - st1).count();

                if(message_paragraph != stool::Message::NO_MESSAGE){
                    std::cout << stool::Message::get_paragraph_string(message_paragraph) << "[DONE] Elapsed Time: " << sec_time << " sec" << std::endl;
                }
            }
            
            uint64_t get_bit_count_in_node(uint64_t h, uint64_t h_node_id) const {
                assert(h < this->length_seq.size());
                assert(h_node_id < this->length_seq[h].size());
                return this->length_seq[h].at(h_node_id);
            }
            

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
                    /*
                    uint64_t left_node_id = 2 * h_node_id;
                    uint64_t right_node_id = 2 * h_node_id + 1;
                    uint64_t left_tree_size = this->leaves[left_node_id].size();
                    uint64_t right_tree_size = this->leaves[right_node_id].size();

                    bool unbalance_flag_LR = left_tree_size > (right_tree_size * 2) || right_tree_size > (left_tree_size * 2);
                    uint64_t child_upper_size = this->get_upper_size_of_leaf();
                    bool full_flag_L = left_tree_size >= child_upper_size;
                    bool full_flag_R = right_tree_size >= child_upper_size;

                    return unbalance_flag_LR || full_flag_L || full_flag_R;
                    */
                }
            }

            

            void rebuild_internal_node(uint8_t h, uint64_t h_node_id)
            {

                std::vector<uint64_t> rank_elements = this->to_local_rank_elements_in_y_order(h, h_node_id);


                uint64_t height = this->height();
                uint64_t current_node_id = h_node_id;
                uint64_t current_node_count = 1;
                for(uint64_t q = h; q < height; q++){
                    std::vector<uint64_t> next_rank_elements;
                    std::vector<uint64_t> next_length_seq;

                    this->rebuild_h_bit_sequence(q, current_node_id, current_node_count, rank_elements, next_rank_elements, next_length_seq);

                    rank_elements.swap(next_rank_elements);

                    current_node_count *= 2;
                    current_node_id *= 2;

                    if(q+1 < height){
                        this->length_seq[q+1].set_values(current_node_id, next_length_seq);
                    }

                }


                /*
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
                    if ((int64_t)(h + 1) < (int64_t)this->bits_seq.size())
                    {
                        this->rebuild_internal_node(h + 1, next_id, left_elements);
                        assert(left_elements.size() == this->get_bit_count_in_node(h + 1, next_id));
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
                    if ((int64_t)(h + 1) < (int64_t)this->bits_seq.size())
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
                    */
            }
            

            void swap(DynamicWaveletTreeForRangeSearch &item)
            {
                this->length_seq.swap(item.length_seq);
                this->bits_seq.swap(item.bits_seq);
            }

            /*
            void rebuild_leaf(uint64_t leaf_id, const std::vector<uint64_t> &rank_elements)
            {
                stool::NaiveFLCVector<false> tmp(rank_elements);
                this->leaves[leaf_id].swap(tmp);
            }
            */

            /*
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
            */

            /*
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
            */
            int64_t height() const
            {
                return this->bits_seq.size();
            }
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

            uint64_t access_x_rank(uint64_t y_rank) const
            {
                assert(y_rank < this->size());
                uint64_t x_rank = this->compute_local_x_rank(0, 0, y_rank);
                return x_rank;
            }
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
                    uint64_t left_tree_size = this->get_bit_count_in_node(h+1, 2 * current_node_id);

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
            std::vector<bool> get_bit_sequence(uint64_t h, uint64_t node_id) const{
                uint64_t x_pos = this->get_node_x_pos_in_bit_sequence(h, node_id);
                uint64_t node_size = this->get_bit_count_in_node(h, node_id);
                std::vector<bool> r;
                r.resize(node_size, false);
                for(uint64_t i = 0; i < node_size; i++){
                    r[i] = this->bits_seq[h].at(x_pos + i);
                }
                return r;
            }

            bool verify() const
            {

                for (uint64_t h = 0; h < this->bits_seq.size(); h++)
                {
                    uint64_t node_count = 1 << h;

                    if(h + 1 < this->bits_seq.size()){
                        for (uint64_t i = 0; i < node_count; i++)
                        {
                            std::vector<bool> bit_sequence = this->get_bit_sequence(h, i);
                            uint64_t countL = 0;
                            uint64_t countR = 0;
                            for(uint64_t j = 0; j < bit_sequence.size(); j++){
                                if(bit_sequence[j]){
                                    countR++;
                                }else{
                                    countL++;
                                }
                            }

                            uint64_t left_tree_size = this->get_bit_count_in_node(h+1, 2 * i);
                            uint64_t right_tree_size = this->get_bit_count_in_node(h+1, 2 * i + 1);

                            if(countL != left_tree_size){
                                this->print_tree();
                                throw std::runtime_error("Error: verify, countL != left_tree_size");
                            }

                            if(countR != right_tree_size){
                                this->print_tree();
                                throw std::runtime_error("Error: verify, countR != right_tree_size");
                            }
                        }
    
                    }else{
                        for (uint64_t i = 0; i < node_count; i++)
                        {
                            uint64_t bit_size = this->get_bit_count_in_node(h, i);
                            if(bit_size > 1){
                                this->print_tree();
                                throw std::runtime_error("Error: verify function, bit_size > 1");
                            }
                        }
                    }

                }
                return true;
                
            }

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
                    if(node_size == 1){
                        r[0] = 0;
                    }else if(node_size > 1){
                        for (uint64_t i = 0; i < node_size; i++)
                        {
                            r[i] = this->bits_seq[h].at(x_pos + i);
                        }
                    }
                }

                return r;
                
            }

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
            std::vector<uint64_t> to_rank_elements_in_x_order() const
            {
                std::vector<uint64_t> r;
                r.resize(this->size(), UINT64_MAX);
                uint64_t size = this->size();
                for(uint64_t i = 0; i < size; i++){
                    r[i] = this->access_y_rank(i);
                }

                return r;
            }

            /*
            std::vector<uint64_t> compute_local_rank_array_in_y_order(uint64_t node_y, uint64_t node_id) const
            {
                std::vector<uint64_t> r;
                uint64_t node_size = this->get_bit_count_in_node(node_y, node_id);
                r.resize(node_size, UINT64_MAX);
                for(uint64_t i = 0; i < node_size; i++){
                    r[i] = this->compute_local_x_rank(node_y, node_id, i);
                }
                return r;
            }
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
                        uint64_t left_tree_size = this->get_bit_count_in_node(h+1, 2 * h_node_id);
                        x_rank += left_tree_size;
                        local_y_rank -= this->rank0_in_bit_sequence_of_node(h, h_node_id, node_x_pos, local_y_rank);
                    }
                    else
                    {
                        local_y_rank -= this->rank1_in_bit_sequence_of_node(h, h_node_id, node_x_pos, local_y_rank);
                    }
                    h_node_id = next_node_id;
                }
                //x_rank += this->leaves[h_node_id][h_y_rank];
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
            /*
            template <typename APPENDABLE_VECTOR>
            uint64_t local_range_report_on_leaf(uint64_t leaf_id, uint64_t x_rank_gap, uint64_t x_min, uint64_t x_max, uint64_t hy_min, uint64_t hy_max, APPENDABLE_VECTOR &out) const
            {
                //assert(hy_max < this->leaves[leaf_id].size());

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
            */

            template <typename APPENDABLE_VECTOR>
            uint64_t recursive_range_report_on_internal_nodes(uint64_t h, uint64_t node_id, uint64_t node_x_pos, int64_t x_min, int64_t x_max, uint64_t hy_min, uint64_t hy_max, APPENDABLE_VECTOR &out) const
            {

                uint64_t found_elements_count = 0;
                int64_t node_size = this->get_bit_count_in_node(h, node_id);
                if (x_min <= (int64_t)node_x_pos && (int64_t)(node_x_pos + node_size - 1) <= x_max)
                {
                    uint64_t limitR = std::min((int64_t)hy_max, node_size-1);

                    if(hy_min <= limitR){
                        uint64_t _tmp = local_range_report_on_internal_node(h, node_id, node_x_pos, hy_min, limitR, out);
                        found_elements_count += _tmp;
                    }

                }
                else if((int64_t)(h+1) < this->height())
                {
                    uint64_t node_x_pos_L = node_x_pos;
                    uint64_t node_x_pos_R = node_x_pos + this->get_bit_count_in_node(h+1, 2 * node_id);

                    

                    /*
                    int64_t hy_max_0 = ((int64_t)this->bits_seq[h][node_id].rank0(hy_max + 1)) - 1;
                    int64_t hy_max_1 = ((int64_t)this->bits_seq[h][node_id].rank1(hy_max + 1)) - 1;
                    */
                   int64_t hy_max_0 = rank0_in_bit_sequence_of_node(h, node_id, node_x_pos_L, hy_max) - 1;
                   int64_t hy_max_1 = rank1_in_bit_sequence_of_node(h, node_id, node_x_pos_L, hy_max) - 1;

                   /*
                    int64_t hy_min_0 = hy_min > 0 ? ((int64_t)this->bits_seq[h][node_id].rank0(hy_min)) : 0;
                    int64_t hy_min_1 = hy_min > 0 ? ((int64_t)this->bits_seq[h][node_id].rank1(hy_min)) : 0;
                    */
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
                    std::vector<uint64_t> tmp_len_veq;
                    uint64_t counter = 0;
                    tmp_len_veq.push_back(0);
                    for(uint64_t i = 0; i < this->length_seq[h].size(); i++){
                        counter += this->length_seq[h].at(i);
                        tmp_len_veq.push_back(counter);
                    }

                    std::string s = "";
                    uint64_t tmp_p = 0;
                    for(uint64_t i = 0; i < this->bits_seq[h].size(); i++){
                        while(tmp_len_veq[tmp_p] <= i){
                            s.append("|");
                            tmp_p++;
                        }

                        bool b = this->bits_seq[h].at(i);

                        s.append(b ? "1" : "0");
                    }
                    s.append("|");
                    
                    r.push_back(s);
                }
                /*
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
                */

                std::cout << "===== TREE =====" << std::endl;

                for (uint64_t i = 0; i < r.size(); i++)
                {
                    std::cout << r[i] << std::endl;
                }
                std::cout << "===== [END] =====" << std::endl;
            }

            static void store_to_file(DynamicWaveletTreeForRangeSearch &item, std::ofstream &os)
            {
                uint64_t height = item.height();
                os.write(reinterpret_cast<const char *>(&height), sizeof(uint64_t));

                for (uint64_t h = 0; h < height; h++)
                {
                    BIT_SEQUENCE::store_to_file(item.bits_seq[h], os);
                    PREFIX_SUM::store_to_file(item.length_seq[h], os);
                }
            }
            static void store_to_bytes(DynamicWaveletTreeForRangeSearch &item, std::vector<uint8_t> &output, uint64_t &pos)
            {                
                uint64_t bytes = item.size_in_bytes();
                if(output.size() < pos + bytes){
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
            uint64_t size_in_bytes(bool only_extra_bytes = false) const
            {
                uint64_t sum = 0;
                sum += sizeof(uint64_t);
                for(int64_t h = 0; h < (int64_t)this->height(); h++){
                    sum += this->bits_seq[h].size_in_bytes(only_extra_bytes);
                    sum += this->length_seq[h].size_in_bytes(only_extra_bytes);
                }
                return sum;
            }

            static DynamicWaveletTreeForRangeSearch load_from_file(std::ifstream &ifs)
            {
                DynamicWaveletTreeForRangeSearch r;
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
            static DynamicWaveletTreeForRangeSearch load_from_bytes(const std::vector<uint8_t> &data, uint64_t &pos)
            {
                DynamicWaveletTreeForRangeSearch r;
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

            std::vector<std::string> get_memory_usage_info(int message_paragraph = stool::Message::SHOW_MESSAGE) const
            {

				std::vector<std::string> r;
                uint64_t size_in_bytes = this->size_in_bytes();
                uint64_t element_count = this->size();

                double bits_per_element = element_count > 0 ? ((double)size_in_bytes / (double)element_count) : 0;

                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "=DynamicWaveletTreeForRangeSearch: " + std::to_string(this->size_in_bytes()) 
                + " bytes, " + std::to_string(element_count) + " elements, " + std::to_string(bits_per_element)  + " bytes per element =");

                for(uint64_t h = 0; h < this->bits_seq.size(); h++){
                    uint64_t _sub_size = 0; 
                    _sub_size += this->bits_seq[h].size_in_bytes();
                    _sub_size += this->length_seq[h].size_in_bytes();

                    uint64_t _bits_per_element = element_count > 0 ? ((double)_sub_size / (double)element_count) : 0;
                    r.push_back(stool::Message::get_paragraph_string(message_paragraph+1) + "Level " + std::to_string(h) + " in range tree: " + std::to_string(_sub_size) + " bytes" + " (" + std::to_string(_bits_per_element) + " bytes per element)");
                }
                r.push_back(stool::Message::get_paragraph_string(message_paragraph) + "==");

				return r;
			}

        };
    }
}
