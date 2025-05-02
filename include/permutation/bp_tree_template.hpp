#pragma once
#include "./permutation_container.hpp"
#include "./bp_internal_node_template.hpp"

namespace stool
{
    using ___PermutationTree = bptree::BPTree<bptree::PermutationContainer, bptree::PermutationItem, true, false>;
    using ___PermutationNode = ___PermutationTree::Node;

    template <>
    void ___PermutationTree::move_values_right(___PermutationNode *left_node, ___PermutationNode *right_node, uint64_t len, bool is_leaf, ___PermutationNode *parent, int64_t parent_edge_index_of_left_node)
    {
        using BPTree = BPTree<bptree::PermutationContainer, bptree::PermutationItem, true, false>;
        if (is_leaf)
        {
            uint64_t left_leaf = (uint64_t)left_node;
            uint64_t right_leaf = (uint64_t)right_node;
            assert(left_leaf < this->leaf_container_vec.size());
            assert(right_leaf < this->leaf_container_vec.size());

            if (parent != nullptr)
            {

                stool::SimpleDeque16<uint64_t> &parent_count_deq = parent->get_value_count_deque();

                parent_count_deq[parent_edge_index_of_left_node] -= len;
                parent_count_deq[parent_edge_index_of_left_node + 1] += len;
            }

            /*
            this->print_leaves();
            if(this->linked_tree_ != nullptr){
                this->linked_tree_->print_leaves();
            }
            */
            for (uint64_t i = 0; i < len; i++)
            {
                bptree::PermutationItem item = this->leaf_container_vec[left_leaf].pop_back();
                this->leaf_container_vec[right_leaf].push_front(item);
                if (this->linked_tree_ != nullptr && item.pointer < this->linked_tree_->get_leaf_container_vector_size())
                {
                    leaf_container_vec[right_leaf].update_linked_tree(0, right_leaf, left_leaf, *this->linked_tree_);
                }
            }
            /*
            std::vector<PermutationItem> items = this->leaf_container_vec[left_leaf].pop_back(len);
            this->leaf_container_vec[right_leaf].push_front(items);
            */
        }
        else
        {
            BPTree::BPFunctions::move_right(*left_node, *right_node, len, parent, parent_edge_index_of_left_node, this->parent_vec);
        }
    }
    template <>
    void ___PermutationTree::preprocess_for_the_exchange_of_two_leaves(uint64_t leaf_index1, uint64_t leaf_index2)
    {
        //std::cout << "Exchange: " << leaf_index1 << " / " << leaf_index2 << std::endl;

        uint64_t dummy_index = this->leaf_container_vec.size();
        if (this->parent_vec[leaf_index1] != nullptr)
        {
            bptree::PermutationContainer &leaf1 = this->leaf_container_vec[leaf_index1];

            uint64_t x_idnex = this->parent_vec[leaf_index2] == nullptr ? leaf_index2 : dummy_index;
            for (const bptree::PermutationItem &it : leaf1)
            {
                //std::cout << "1 Process: leaf_index1 = " << leaf_index1 << " = [" << it.pointer << ", " << (uint64_t)it.key << "]" << std::endl;

                bptree::PermutationContainer &leaf3 = this->linked_tree_->get_leaf_container(it.pointer);
                int64_t idx = leaf3.get_index(bptree::PermutationItem(leaf_index1, it.key));
                assert(idx != -1);
                stool::bptree::PermutationItem tmp = leaf3.at(idx);

                //std::cout << "---[" << tmp.pointer << ", " << (uint64_t)tmp.key << "]" << " -> [" << x_idnex << ",  " << (uint64_t)tmp.key << "]" << std::endl;
                assert(tmp.pointer == leaf_index1);

                leaf3.set_value(idx, stool::bptree::PermutationItem(x_idnex, tmp.key));
            }

            if (this->parent_vec[leaf_index2] != nullptr)
            {
                bptree::PermutationContainer &leaf2 = this->leaf_container_vec[leaf_index2];
                for (const bptree::PermutationItem &it : leaf2)
                {
                    //std::cout << "2 Process: leaf_index2 = " << leaf_index2 << " = [" << it.pointer << ", " << (uint64_t)it.key << "]" << std::endl;

                    bptree::PermutationContainer &leaf3 = this->linked_tree_->get_leaf_container(it.pointer);
                    int64_t idx = leaf3.get_index(bptree::PermutationItem(leaf_index2, it.key));
                    if(idx == -1){
                        this->linked_tree_->print_leaves();
                    }
                    assert(idx != -1);

                    stool::bptree::PermutationItem tmp = leaf3.at(idx);
                    //std::cout << "---[" << tmp.pointer << ", " << (uint64_t)tmp.key << "]" << " -> [" << leaf_index1 << ",  " << (uint64_t)tmp.key << "]" << std::endl;

                    assert(tmp.pointer == leaf_index2);

                    leaf3.set_value(idx, stool::bptree::PermutationItem(leaf_index1, tmp.key));
                }

                for (const bptree::PermutationItem &it : leaf1)
                {
                    //std::cout << "3 Process: leaf_index1 = " << leaf_index1 << " = [" << it.pointer << ", " << (uint64_t)it.key << "]" << std::endl;
                    bptree::PermutationContainer &leaf3 = this->linked_tree_->get_leaf_container(it.pointer);
                    int64_t idx = leaf3.get_index(bptree::PermutationItem(dummy_index, it.key));
                    assert(idx != -1);
                    stool::bptree::PermutationItem tmp = leaf3.at(idx);
                    //std::cout << "---[" << tmp.pointer << ", " << (uint64_t)tmp.key << "]" << " -> [" << leaf_index2 << ",  " << (uint64_t)tmp.key << "]" << std::endl;
                    assert(tmp.pointer == dummy_index);
                    leaf3.set_value(idx, stool::bptree::PermutationItem(leaf_index2, tmp.key));
                }
            }
        }
        else
        {
            if (this->parent_vec[leaf_index2] != nullptr)
            {
                ___PermutationTree::preprocess_for_the_exchange_of_two_leaves(leaf_index2, leaf_index1);
            }
        }

    }

    template <>
    void ___PermutationTree::move_values_left(___PermutationNode *left_node, ___PermutationNode *right_node, uint64_t len, bool is_leaf, ___PermutationNode *parent, int64_t parent_edge_index_of_right_node)
    {
        if (is_leaf)
        {
            uint64_t left_leaf = (uint64_t)left_node;
            uint64_t right_leaf = (uint64_t)right_node;
            assert(left_leaf < this->leaf_container_vec.size());

            if (parent != nullptr)
            {
                stool::SimpleDeque16<uint64_t> &parent_count_deq = parent->get_value_count_deque();
                parent_count_deq[parent_edge_index_of_right_node] -= len;
                parent_count_deq[parent_edge_index_of_right_node - 1] += len;
            }

            for (uint64_t i = 0; i < len; i++)
            {
                bptree::PermutationItem item = this->leaf_container_vec[right_leaf].pop_front();
                this->leaf_container_vec[left_leaf].push_back(item);
                uint64_t _size = leaf_container_vec[left_leaf].size();
                if (this->linked_tree_ != nullptr && item.pointer < this->linked_tree_->get_leaf_container_vector_size())
                {
                    leaf_container_vec[left_leaf].update_linked_tree(_size - 1, left_leaf, right_leaf, *this->linked_tree_);
                }
            }

            /*
            std::vector<PermutationItem> items = this->leaf_container_vec[right_leaf].pop_front(len);
            this->leaf_container_vec[left_leaf].push_back(items);
            */
        }
        else
        {
            BPTree::BPFunctions::move_left(*left_node, *right_node, len, parent, parent_edge_index_of_right_node, this->parent_vec);
        }
    }

}