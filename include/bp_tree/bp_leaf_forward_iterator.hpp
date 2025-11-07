#pragma once
#include "./bp_postorder_iterator.hpp"
namespace stool
{
    namespace bptree
    {
        /**
         * @brief A forward iterator for traversing the leaves of a BP-tree. [Unchecked AI's Comment] 
         * \ingroup BPTreeClasses
         */
        template <typename LEAF_CONTAINER, typename VALUE, uint64_t MAX_DEGREE, bool USE_PSUM>
        class BPLeafForwardIterator
        {

        public:
            using SNode = StackNode<LEAF_CONTAINER, VALUE, MAX_DEGREE, USE_PSUM>;
            using NodePointer = BPNodePointer<LEAF_CONTAINER, VALUE, MAX_DEGREE, USE_PSUM>;
            using Node = BPInternalNode<LEAF_CONTAINER, VALUE, MAX_DEGREE, USE_PSUM>;
            using BASE_ITE = BPPostorderIterator<LEAF_CONTAINER, VALUE, MAX_DEGREE, USE_PSUM>;

            std::vector<SNode> _st;
            uint64_t idx = 0;

            using iterator_category = std::bidirectional_iterator_tag;
            using difference_type = std::ptrdiff_t;

            BPLeafForwardIterator()
            {
                this->idx = UINT64_MAX;
            }
            /*
            BPLeafForwardIterator(uint64_t leaf)
            {
                this->_st.push_back(SNode(NodePointer::build_leaf_pointer(leaf, -1), 0, false));
                this->idx = 0;
            }
            */

            BPLeafForwardIterator(Node *_root)
            {
                if (_root != nullptr)
                {
                    BASE_ITE::initialize_stack(this->_st, *_root);                            
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

            uint64_t get_current_leaf_container_index() const
            {                
                assert(this->_st.size() != 0);
                SNode top = this->_st[this->_st.size() - 1];
                return top.pointer.get_leaf_container_index();
            }

            bool is_end() const
            {
                return this->idx == UINT64_MAX;
            }
            void copy_to(BPLeafForwardIterator &item) const
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
            uint64_t operator*() const
            {
                return this->get_current_leaf_container_index();
            }

            BPLeafForwardIterator &operator++()
            {
                while (true)
                {
                    bool b = BASE_ITE::proceed(this->_st);
                    if(b){
                        SNode top = this->_st[this->_st.size() - 1];
                        if(top.pointer.is_leaf()){
                            this->idx++;
                            break;
                        }
                    }else{
                        this->idx = UINT64_MAX;
                        break;
                    }
                }                
                return *this;
            }
            bool operator==(const BPLeafForwardIterator &other) const { return this->idx == other.idx; }
            bool operator!=(const BPLeafForwardIterator &other) const { return this->idx != other.idx; }
            bool operator<(const BPLeafForwardIterator &other) const { return this->idx < other.idx; }
            bool operator>(const BPLeafForwardIterator &other) const { return this->idx > other.idx; }
            bool operator<=(const BPLeafForwardIterator &other) const { return this->idx <= other.idx; }
            bool operator>=(const BPLeafForwardIterator &other) const { return this->idx >= other.idx; }

            //@}
        };
    }
}