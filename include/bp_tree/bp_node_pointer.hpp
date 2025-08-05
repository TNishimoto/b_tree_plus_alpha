#pragma once
#include "./bp_internal_node.hpp"

namespace stool
{
    namespace bptree
    {
        ////////////////////////////////////////////////////////////////////////////////
        /// @class      BPNodePointer
        /// @brief      A pointer to a node of BPTree
        ///
        ////////////////////////////////////////////////////////////////////////////////
        template <typename LEAF_CONTAINER, typename VALUE, uint64_t MAX_DEGREE>
        class BPNodePointer
        {
            using Node = BPInternalNode<LEAF_CONTAINER, VALUE, MAX_DEGREE>;
            Node *node_;
            int16_t parent_edge_index_;
            bool is_leaf_;

        public:
            BPNodePointer()
            {
            }

            static BPNodePointer build_internal_node_pointer(Node *_node, int16_t _parent_edge_index)
            {
                BPNodePointer r;
                r.node_ = _node;
                r.parent_edge_index_ = _parent_edge_index;
                r.is_leaf_ = false;
                return r;
            }
            static BPNodePointer build_leaf_pointer(uint64_t _leaf_container_index, int16_t _parent_edge_index)
            {
                BPNodePointer r;
                r.node_ = (Node *)_leaf_container_index;
                r.parent_edge_index_ = _parent_edge_index;
                r.is_leaf_ = true;
                return r;
            }

            uint64_t get_parent_edge_index() const
            {
                return this->parent_edge_index_;
            }
            Node *get_node() const
            {
                return this->node_;
            }

            bool is_leaf() const
            {
                return this->is_leaf_;
            }

            uint64_t get_leaf_container_index() const
            {
                return (uint64_t)this->node_;
            }
            uint64_t get_degree(const std::vector<LEAF_CONTAINER> &leaf_container_vec) const
            {
                if (this->is_leaf())
                {
                    return leaf_container_vec[this->get_leaf_container_index()].size();
                }
                else
                {
                    return this->get_node()->get_degree();
                }
            }
        };
    }
}