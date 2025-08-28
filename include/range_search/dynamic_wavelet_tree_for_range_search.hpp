#pragma once
#include "../sequence/dynamic_bit_sequence.hpp"

namespace stool
{
    namespace bptree
    {

        class DynamicWaveletTreeForRangeSearch{
            using BIT_SEQUENCE = SimpleDynamicBitSequence;
            std::vector<std::vector<BIT_SEQUENCE>> bits_seq;
            std::vector<stool::NaiveFLCVector<false>> leaves;

            inline static uint64_t LEAF_MAX_SIZE = 8;

            public:

            DynamicWaveletTreeForRangeSearch(){
                this->bits_seq.clear();
                this->leaves.clear();
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


            void add(uint64_t x_rank, uint64_t y_rank){
                uint64_t h_tree_pos = 0;
                uint64_t h_element_pos = y_rank;
                uint64_t h_x_rank = x_rank;

                int64_t height = this->bits_seq.size();
                for(int64_t h = 0; h < height - 1; h++){
                    uint64_t left_tree_size = this->bits_seq[h][h_tree_pos].count0();
                    if(h_x_rank < left_tree_size){
                        this->bits_seq[h][h_tree_pos].insert(h_element_pos, 0);
                        h_element_pos -= this->bits_seq[h][h_tree_pos].rank1(h_element_pos);
                        h_tree_pos = 2 * h_tree_pos;
                    }else{
                        this->bits_seq[h][h_tree_pos].insert(h_element_pos, 1);
                        h_element_pos -= this->bits_seq[h][h_tree_pos].rank0(h_element_pos);
                        h_tree_pos = 2 * h_tree_pos + 1;
                        h_x_rank -= left_tree_size;
                    }
                }

                this->bits_seq[height - 1][h_tree_pos].insert(h_element_pos, 0);
            }
            static uint64_t get_full_size_of_tree(uint64_t height) {
                return LEAF_MAX_SIZE << height;
            }

            uint64_t get_full_size_of_internal_node(uint64_t h) const {
                uint64_t H = this->bits_seq.size();
                return LEAF_MAX_SIZE << (H - h);
            }
            uint64_t get_upper_size_of_internal_node(uint64_t h) const {
                uint64_t fsize = this->get_full_size_of_internal_node(h);
                return (fsize / 4) * 3;
            }
            uint64_t get_lower_size_of_internal_node(uint64_t h) const {
                uint64_t fsize = this->get_full_size_of_internal_node(h);
                return (fsize / 4);
            }


            /*
            void insert_element_into_internal_node(uint8_t h, uint64_t hnode_id, uint64_t pos, uint64_t rank){


            }
            */



            void build(const std::vector<uint64_t> &rank_elements){
                uint64_t height = 0;
                while(true){
                    uint64_t fsize = this->get_full_size_of_tree(height);
                    if(rank_elements.size() <= fsize){
                        break;
                    }else{
                        height++;
                    }
                }

                uint64_t leaf_count = 1ULL << height;
                this->leaves.resize(leaf_count);
                for(uint64_t i = 0; i< leaf_count; i++){
                    this->leaves[i].clear();
                }

                this->bits_seq.clear();
                this->bits_seq.resize(height);
                for(uint64_t h = 0; h < height;h++){
                    uint64_t h_node_count = 1ULL << h;
                    this->bits_seq[h].resize(h_node_count);
                    for(uint64_t i = 0; i < h_node_count; i++){
                        this->bits_seq[h][i].clear();
                    }
                }

                if(height > 0){
                    this->rebuild_internal_node(0, 0, rank_elements);
                }else{
                    this->rebuild_leaf(0, rank_elements);
                }

                assert(this->verify());
            }

            void rebuild_internal_node(uint8_t h, uint64_t h_node_id, const std::vector<uint64_t> &rank_elements){
                assert(h < this->bits_seq.size());
                uint64_t full_size = this->get_full_size_of_internal_node(h);                
                assert(rank_elements.size() <= full_size);
                uint64_t left_counter = 0;

                uint64_t half_size = rank_elements.size() / 2;
                {
                    std::vector<uint64_t> left_elements(half_size, 0);
                    for(uint64_t i = 0; i < rank_elements.size(); i++){
                        if(rank_elements[i] < half_size){
                            left_elements[left_counter++] = rank_elements[i];
                        }
                    }
                    uint64_t next_id = 2 * h_node_id;
                    if(h + 1 < this->bits_seq.size()){
                        this->rebuild_internal_node(h + 1, next_id, left_elements);
                        assert(left_elements.size() == this->bits_seq[h + 1][next_id].size());
                    }else{
                        this->rebuild_leaf(next_id, left_elements);
                        assert(left_elements.size() == this->leaves[next_id].size());
                    }
                }
                {
                    std::vector<uint64_t> right_elements(rank_elements.size() - half_size, 0);
                    std::vector<bool> tmp_bit_sequence(rank_elements.size(), false);
                    uint64_t right_counter = 0;


                    for(uint64_t i = 0; i < rank_elements.size(); i++){
                        assert(i < rank_elements.size());
                        if(rank_elements[i] >= half_size){                            
                            assert(right_counter < right_elements.size());
                            tmp_bit_sequence[i] = true;
                            right_elements[right_counter++] = rank_elements[i] - half_size;
                        }
                    }
                    uint64_t next_id = (2 * h_node_id) + 1;
                    if(h + 1 < this->bits_seq.size()){
                        this->rebuild_internal_node(h + 1, next_id, right_elements);
                        assert(right_elements.size() == this->bits_seq[h + 1][next_id].size());
                    }else{
                        this->rebuild_leaf(next_id, right_elements);
                        assert(right_elements.size() == this->leaves[next_id].size());
                    }
                    assert(h < this->bits_seq.size());
                    assert(h_node_id < this->bits_seq[h].size());

                    this->bits_seq[h][h_node_id].clear();
                    this->bits_seq[h][h_node_id].push_many(tmp_bit_sequence);    
                }
            }
            void rebuild_leaf(uint64_t leaf_id, const std::vector<uint64_t> &rank_elements){
                stool::NaiveFLCVector<false> tmp(rank_elements);
                this->leaves[leaf_id].swap(tmp);
            }



            void insert_element_into_leaf(uint64_t leaf_id, uint64_t pos, uint64_t rank){
                uint64_t size = this->leaves[leaf_id].size();
                for(uint64_t i = 0; i < size; i++){
                    uint64_t v = this->leaves[leaf_id][i];
                    if(v >= rank){
                        this->leaves[leaf_id].increment(i, 1);
                    }
                }
                this->leaves[leaf_id].insert(pos, rank);
            }
            void remove_element_from_leaf(uint64_t leaf_id, uint64_t pos){
                uint64_t size = this->leaves[leaf_id].size();
                uint64_t rank = this->leaves[leaf_id][pos];
                for(uint64_t i = 0; i < size; i++){
                    uint64_t v = this->leaves[leaf_id][i];
                    if(v > rank){
                        this->leaves[leaf_id].increment(i, -1);
                    }
                }
                this->leaves[leaf_id].remove(pos);
            }
            int64_t height() const {
                return this->bits_seq.size();
            }
            uint64_t size() const {
                if(this->height() == 0){
                    return this->leaves[0].size();
                }else{
                    return this->bits_seq[0][0].size();
                }
            }

            uint64_t access_x_rank(uint64_t y_rank) const{
                assert(y_rank < this->size());
                uint64_t x_rank = 0;
                uint64_t h_y_rank = y_rank;
                uint64_t node_id = 0;
                int64_t height = this->height();
                for(int64_t h = 0; h < height; h++){
                    bool b = this->bits_seq[h][node_id].at(h_y_rank);
                    uint64_t next_node_id = (2 * node_id) + (uint64_t)b;
                    if(b){
                        uint64_t left_tree_size = this->bits_seq[h][node_id].count0();
                        x_rank += left_tree_size;
                        h_y_rank -= this->bits_seq[h][node_id].rank0(h_y_rank);
                    }else{
                        h_y_rank -= this->bits_seq[h][node_id].rank1(h_y_rank);
                    }
                    node_id = next_node_id;
                }
                assert(node_id < this->leaves.size());
                if(h_y_rank >= this->leaves[node_id].size()){
                    std::cout << "y_rank:" << y_rank << std::endl;
                    std::cout << "x_rank:" << x_rank << std::endl;
                    std::cout << "h_y_rank:" << h_y_rank << std::endl;
                    std::cout << "node_id:" << node_id << std::endl;
                    std::cout << this->leaves[node_id].to_string() << std::endl;

                    throw std::runtime_error("Error in access_x_rank: not found, y_rank: " + std::to_string(y_rank));
                }


                assert(h_y_rank < this->leaves[node_id].size());
                return x_rank + this->leaves[node_id][h_y_rank];
            }
            bool verify() const {
                for(uint64_t h = 0; h < this->bits_seq.size(); h++){
                    for(uint64_t i = 0; i < this->bits_seq[h].size(); i++){
                        uint64_t left_tree_size = this->bits_seq[h][i].count0();
                        uint64_t right_tree_size = this->bits_seq[h][i].count1();
                        if(h + 1 < this->bits_seq.size()){
                            assert(left_tree_size == this->bits_seq[h + 1][2 * i].size());
                            assert(right_tree_size == this->bits_seq[h + 1][2 * i + 1].size());    
                        }else{
                            assert(left_tree_size == this->leaves[2 * i].size());
                            assert(right_tree_size == this->leaves[2 * i + 1].size());
                        }
                    }
                }
                return true;
                
            }
            /*
            uint64_t access_y_rank(uint64_t x_rank) const{
                uint64_t h_x_rank = x_rank;
                uint64_t y_rank = 0;
                uint64_t node_id = 0;
                int64_t height = this->height();
                for(int64_t h = 0; h < height; h++){
                    uint64_t left_tree_size = this->bits_seq[h][node_id].count0();
                    bool b = h_x_rank < left_tree_size;
                    uint64_t next_node_id = (2 * node_id) + (uint64_t)b;

                    if(b){
                        h_x_rank -= left_tree_size;
                        uint64_t p = this->bits_seq[h][node_id].select1(h_x_rank + 1);


                    }else{

                    }


                    bool b = this->bits_seq[h][node_id].at(h_y_rank);
                    if(b){
                        uint64_t left_tree_size = this->bits_seq[h][node_id].count0();
                        x_rank += left_tree_size;
                        h_y_rank -= this->bits_seq[h][node_id].rank0(h_y_rank);
                    }        
                }
                uint64_t leaf_size = this->leaves[node_id].size();
                for(uint64_t i = 0; i < leaf_size; i++){
                    uint64_t v = this->leaves[node_id][i];
                    if(v == h_y_rank){
                        return x_rank + i;
                    }
                }
                throw std::runtime_error("Not found");

            }
            */

            std::vector<uint64_t> to_rank_elements() const {
                std::vector<uint64_t> rank_elements;
                uint64_t size = this->size();
                rank_elements.resize(size, UINT64_MAX);

                for(uint64_t y = 0; y < size; y++){
                    uint64_t x = this->access_x_rank(y);
                    rank_elements[y] = x;
                }
                return rank_elements;
            }


            void print_tree() const {
                std::vector<std::string> r;
                for(uint64_t h = 0; h < this->bits_seq.size(); h++){
                    std::string s = "";
                    for(uint64_t i = 0; i < this->bits_seq[h].size(); i++){
                        s.append(this->bits_seq[h][i].to_string());
                    }
                    r.push_back(s);
                }
                std::string s = "";
                std::vector<char> char_vec{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

                for(uint64_t i = 0; i < this->leaves.size(); i++){
                    s.append("[");

                    for(uint64_t j = 0; j < this->leaves[i].size(); j++){
                        uint64_t v = this->leaves[i][j];
                        if(v < char_vec.size()){
                            s.push_back(char_vec[v]);
                        }else{
                            s.push_back('?');
                        }
                    }
                    s.append("]");
                }
                r.push_back(s);


                for(uint64_t i = 0; i < r.size(); i++){
                    std::cout << r[i] << std::endl;
                }
            }

        };
    }
}
