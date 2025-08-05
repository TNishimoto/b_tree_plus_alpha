#pragma once
#include "./bp_node_pointer.hpp"
namespace stool
{
    namespace bptree
    {
        ////////////////////////////////////////////////////////////////////////////////
        /// @class      StackNode
        /// @brief      The item of the stack for traversing BPTree 
        ///
        ////////////////////////////////////////////////////////////////////////////////
        template <typename LEAF_CONTAINER, typename VALUE, uint64_t MAX_DEGREE>
        struct StackNode
        {
        public:
            BPNodePointer<LEAF_CONTAINER, VALUE, MAX_DEGREE> pointer;
            uint64_t position;
            bool checked;

            StackNode()
            {
            }
            StackNode(BPNodePointer<LEAF_CONTAINER, VALUE, MAX_DEGREE> _pointer, uint64_t _position, bool _checked) : pointer(_pointer), position(_position), checked(_checked)
            {
            }

            StackNode copy() const {
                return StackNode(pointer, position, checked);

            }

            std::string to_string() const
            {
                std::string s;
                s += "[";
                s += std::to_string((uint64_t)this->pointer.get_node());
                s += ", ";
                s += std::to_string(this->position);
                if (this->checked)
                {
                    s += "T";
                }
                else
                {
                    s += "F";
                }
                s += "]";
                return s;
            }
        };
    }
}