#pragma once
#include "../bp_tree/bp_tree.hpp"
#include "stool/include/light_stool.hpp"

namespace stool
{
    namespace permutation
    {
        ////////////////////////////////////////////////////////////////////////////////
        /// @class      PermutationItem
        /// @brief      The value stored in the BPTree of DynamicPermutation
        ///
        ////////////////////////////////////////////////////////////////////////////////
        struct PermutationItem
        {
            uint64_t pointer;
            uint8_t key;

            PermutationItem() {

            };
            PermutationItem(uint64_t _pointer, uint64_t _key) : pointer(_pointer), key(_key)
            {
            }
        };
    }
}