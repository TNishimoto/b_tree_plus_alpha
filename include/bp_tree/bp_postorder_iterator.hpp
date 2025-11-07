#pragma once
#include "./stack_node.hpp"
namespace stool
{
    namespace bptree
    {
        /**
         * @brief The iterator of a post-order traversal on BPTree [Unchecked AI's Comment]
         * \ingroup BPTreeClasses
         */
        template <typename LEAF_CONTAINER, typename VALUE, uint64_t MAX_DEGREE, bool USE_PSUM>
        class BPPostorderIterator
        {

        public:
            using SNode = StackNode<LEAF_CONTAINER, VALUE, MAX_DEGREE, USE_PSUM>;
            using NodePointer = BPNodePointer<LEAF_CONTAINER, VALUE, MAX_DEGREE, USE_PSUM>;
            using Node = BPInternalNode<LEAF_CONTAINER, VALUE, MAX_DEGREE, USE_PSUM>;
            std::vector<SNode> _st;
            uint64_t idx = 0;

            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;

            BPPostorderIterator()
            {
                this->idx = UINT64_MAX;
            }

            BPPostorderIterator(uint64_t leaf)
            {
                this->_st.push_back(SNode(NodePointer::build_leaf_pointer(leaf, -1), 0, false));
                this->idx = 0;
            }

            BPPostorderIterator(Node *_root)
            {
                if (_root != nullptr)
                {
                    BPPostorderIterator::initialize_stack(this->_st, *_root);                    
                    this->idx = 0;
                }
                else
                {
                    this->idx = UINT64_MAX;
                }
            }

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Public const Methods
            ///   The public const methods of this class.
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            NodePointer get_current_node() const
            {
                assert(this->_st.size() != 0);
                SNode top = this->_st[this->_st.size() - 1];
                return top.pointer;
            }

            bool is_end() const
            {
                return this->idx == UINT64_MAX;
            }

            void print() const
            {

                SNode top = this->_st[this->_st.size() - 1];
                std::cout << "[" << (uint64_t)top.pointer.get_node() << "/" << top.position << "]" << std::endl;
            }

            void copy_to(BPPostorderIterator &item) const
            {
                item._st.clear();
                for (auto &it : this->_st)
                {
                    item._st.push_back(it.copy());
                }
                item.idx = this->idx;
            }
            //@}
            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Operators
            ///   The operators of this class
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            NodePointer operator*() const
            {
                assert(this->_st.size() != 0);
                SNode top = this->_st[this->_st.size() - 1];
                return top.pointer;
            }

            static void initialize_stack(std::vector<SNode> &st, Node &_root)
            {
                st.push_back(SNode(NodePointer::build_internal_node_pointer(&_root, -1), 0, false));
                while (st.size() > 0)
                {
                    SNode top = st[st.size() - 1];
                    st.pop_back();
                    st.push_back(SNode(top.pointer, top.position, true));

                    NodePointer &np = top.pointer;
                    if (np.is_leaf())
                    {
                        break;
                    }
                    else
                    {
                        Node *node = np.get_node();
                        if (node->is_parent_of_leaves())
                        {
                            NodePointer child = NodePointer::build_leaf_pointer((uint64_t)node->get_child(0), 0);
                            st.push_back(SNode(child, 0, false));
                        }
                        else
                        {
                            NodePointer child = NodePointer::build_internal_node_pointer(node->get_child(0), 0);
                            st.push_back(SNode(child, 0, false));
                        }
                    }
                }
            }
            static bool proceed(std::vector<SNode> &st)
            {
                if (st.size() > 0)
                {
                    st.pop_back();
                    while (st.size() > 0)
                    {
                        SNode top = st[st.size() - 1];

                        NodePointer &np = top.pointer;
                        if (np.is_leaf())
                        {
                            break;
                        }
                        else
                        {
                            Node *node = np.get_node();
                            if (top.checked)
                            {
                                if (top.position + 1 == node->children_count())
                                {
                                    break;
                                }
                                else
                                {
                                    st.pop_back();
                                    st.push_back(SNode(np, top.position + 1, false));
                                }
                            }
                            else
                            {
                                st.pop_back();
                                st.push_back(SNode(np, top.position, true));
                                if (node->is_parent_of_leaves())
                                {
                                    NodePointer child = NodePointer::build_leaf_pointer((uint64_t)node->get_child(top.position), top.position);
                                    st.push_back(SNode(child, 0, false));
                                }
                                else
                                {
                                    NodePointer child = NodePointer::build_internal_node_pointer(node->get_child(top.position), top.position);
                                    st.push_back(SNode(child, 0, false));
                                }
                            }
                        }
                    }
                    if (st.size() > 0)
                    {
                        return true;
                    }
                    else
                    {
                        return false;
                    }
                }
                else
                {
                    return false;
                }
            }

            BPPostorderIterator &operator++()
            {
                assert(this->_st.size() > 0);
                bool b = proceed(this->_st);
                if (b)
                {
                    this->idx++;
                }
                else
                {
                    this->idx = UINT64_MAX;
                }
                return *this;
            }
            bool operator==(const BPPostorderIterator &other) const { return this->idx == other.idx; }
            bool operator!=(const BPPostorderIterator &other) const { return this->idx != other.idx; }
            bool operator<(const BPPostorderIterator &other) const { return this->idx < other.idx; }
            bool operator>(const BPPostorderIterator &other) const { return this->idx > other.idx; }
            bool operator<=(const BPPostorderIterator &other) const { return this->idx <= other.idx; }
            bool operator>=(const BPPostorderIterator &other) const { return this->idx >= other.idx; }

            //@}
        };
    }
}