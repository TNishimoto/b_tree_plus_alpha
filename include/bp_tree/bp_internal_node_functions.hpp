#pragma once
#include "./bp_internal_node.hpp"

namespace stool
{
    namespace bptree
    {

        ////////////////////////////////////////////////////////////////////////////////
        /// @class      BPInternalNodeFunctions
        /// @brief      Helper functions of BPInternalNode
        ///
        ////////////////////////////////////////////////////////////////////////////////
        template <typename LEAF_CONTAINER, typename VALUE, bool USE_PARENT_FIELD, bool USE_PSUM, uint64_t MAX_DEGREE>
        class BPInternalNodeFunctions
        {
            using InternalNode = BPInternalNode<LEAF_CONTAINER, VALUE, MAX_DEGREE, USE_PSUM>;

        public:
            ////////////////////////////////////////////////////////////////////////////////
            ///   @name SPSI functions
            ///   SPSI functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            static uint64_t psum(const InternalNode &node)
            {
                assert(node.use_psum());
                const auto &deq = node.get_value_sum_deque();
                return deq.psum();

                /*
                uint64_t sum = 0;
                for (uint64_t p : deq)
                {
                    sum += p;
                }
                return sum;
                */
            }

            static uint64_t psum(const InternalNode &node, uint64_t i, const std::vector<LEAF_CONTAINER> &leaf_container_vec)
            {

                InternalNode *current_node = const_cast<InternalNode *>(&node);
                uint64_t current_i = i;
                bool _is_leaf = false;
                uint64_t sum = 0;


                while (!_is_leaf)
                {
                    const auto &count_deq = current_node->get_value_count_deque();
                    const auto &sum_deq = current_node->get_value_sum_deque();
                    uint64_t tmp_i = 0;
                    int64_t search_result = count_deq.search(current_i+1, tmp_i);

                    if (search_result != -1)
                    {
                        _is_leaf = current_node->is_parent_of_leaves();
                        current_node = current_node->get_child(search_result);


                        if (search_result != 0)
                        {                            
                            sum += sum_deq.psum(search_result - 1);
                        }

                        current_i -= tmp_i;
                    }
                    else
                    {
                        throw std::invalid_argument("BPInternalNodeFunctions::psum(), psum error");
                    }
                }

                uint64_t x = (uint64_t)current_node;
                assert(x < leaf_container_vec.size());
                assert(current_i < leaf_container_vec[x].size());

                uint64_t leaf_psum = leaf_container_vec[x].psum(current_i);

                return sum + leaf_psum;
            }

            static int64_t select0(const InternalNode &node, uint64_t i, const std::vector<LEAF_CONTAINER> &leaf_container_vec)
            {
                uint64_t nth = i + 1;
                InternalNode *current_node = const_cast<InternalNode *>(&node);
                uint64_t result = 0;
                uint64_t current_psum = 0;
                bool _is_leaf = false;

                while (!_is_leaf)
                {
                    bool b = false;
                    // assert(nth <= current_psum + current_node->get_value_sum());
                    assert(current_psum <= nth);
                    const auto &sum_deq = current_node->get_value_sum_deque();
                    const auto &count_deq = current_node->get_value_count_deque();

                    for (uint64_t x = 0; x < current_node->children_count(); x++)
                    {
                        int64_t sub_count0 = count_deq[x] - sum_deq[x];
                        if (current_psum + sub_count0 >= nth)
                        {
                            _is_leaf = current_node->is_parent_of_leaves();
                            current_node = current_node->get_child(x);
                            b = true;
                            break;
                        }
                        else
                        {
                            current_psum += sub_count0;
                            result += count_deq[x];
                        }
                    }
                    if (!b)
                    {
                        throw std::invalid_argument("psum error");
                    }
                }
                uint64_t x = (uint64_t)current_node;
                return result + leaf_container_vec[x].select0(i - current_psum);
            }

            static int64_t search(const InternalNode &node, uint64_t sum, const std::vector<LEAF_CONTAINER> &leaf_container_vec)
            {
                InternalNode *current_node = const_cast<InternalNode *>(&node);
                uint64_t result = 0;
                uint64_t current_psum = 0;
                bool _is_leaf = false;

                while (!_is_leaf)
                {
                    // bool b = false;
                    assert(sum <= current_psum + current_node->get_value_sum());
                    assert(current_psum <= sum);
                    const auto &sum_deq = current_node->get_value_sum_deque();
                    const auto &count_deq = current_node->get_value_count_deque();
                    uint64_t tmp_psum = 0;
                    uint64_t search_sum = sum - current_psum;
                    int64_t search_result = sum_deq.search(search_sum, tmp_psum);
                    if (search_result != -1)
                    {
                        _is_leaf = current_node->is_parent_of_leaves();
                        current_node = current_node->get_child(search_result);
                        current_psum += tmp_psum;
                        if (search_result != 0)
                        {
                            result += count_deq.psum(search_result - 1);
                        }

                    }
                    else
                    {
                        std::cout << "sum: " << sum << " == True sum: " << current_psum << std::endl;
                        throw std::invalid_argument("BPInternalNodeFunctions::search(), psum error");
                    }
                }

                uint64_t x = (uint64_t)current_node;
                return result + leaf_container_vec[x].search(sum - current_psum);
            }

            inline static uint64_t time_count = 0;
            static std::pair<int64_t, uint64_t> access_child_index_by_value_index(const InternalNode &node, uint64_t value_index)
            {
                assert(value_index <= node.get_value_count());

                const auto &count_deq = node.get_value_count_deque();

                uint64_t sum = 0;



                //auto st1 = std::chrono::system_clock::now();
                int64_t search_result = count_deq.search(value_index+1, sum);
                //auto st2 = std::chrono::system_clock::now();
                //time_count += std::chrono::duration_cast<std::chrono::nanoseconds>(st2 - st1).count();

               
                auto pair = std::pair<int64_t, uint64_t>(search_result, value_index - sum);

                return pair;
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Conversion functions
            ///   Conversion functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            static void get_leaf_container_index_vector(const InternalNode &node, std::vector<uint64_t> &output)
            {
                if (!node.is_parent_of_leaves())
                {
                    for (uint64_t i = 0; i < node.children_count(); i++)
                    {
                        InternalNode *child = node.get_child(i);
                        BPInternalNodeFunctions::get_leaf_container_index_vector(*child, output);
                    }
                }
                else
                {
                    for (uint64_t i = 0; i < node.children_count(); i++)
                    {
                        InternalNode *child = node.get_child(i);
                        output.push_back((uint64_t)child);
                    }
                }
            }
            static void to_value_vector(const InternalNode &node, std::vector<VALUE> &output, const std::vector<LEAF_CONTAINER> &leaf_container_vec)
            {
                std::vector<uint64_t> index_vector;
                BPInternalNodeFunctions::get_leaf_container_index_vector(node, index_vector);

                for (uint64_t i : index_vector)
                {
                    std::vector<VALUE> tmp;
                    leaf_container_vec[i].to_values(tmp);

                    // std::vector<VALUE> tmp = leaf_container_vec[i].to_value_vector();
                    for (uint64_t j : tmp)
                    {
                        output.push_back(j);
                    }
                }
            }

            static std::string to_string(const InternalNode &node, const std::vector<LEAF_CONTAINER> &leaf_container_vec)
            {
                std::vector<uint64_t> tmp;
                BPInternalNodeFunctions::get_leaf_container_index_vector(node, tmp);

                std::string s;
                for (uint64_t idx : tmp)
                {
                    s += leaf_container_vec[idx].to_string();
                }
                return s;
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Print and verification functions
            ///   Print and verification functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{

            static uint64_t get_tree_string(const InternalNode &node, std::vector<std::string> &output, const std::vector<LEAF_CONTAINER> &leaf_container_vec)
            {
                uint64_t h = node.get_height();
                while (output.size() <= h)
                {
                    output.push_back("");
                }

                uint64_t width = 0;

                if (node.is_parent_of_leaves())
                {
                    std::string s1;
                    std::vector<uint64_t> tmp;
                    BPInternalNodeFunctions::get_leaf_container_index_vector(node, tmp);
                    for (uint64_t idx : tmp)
                    {
                        s1 += "([" + std::to_string(idx) + "]" + leaf_container_vec[idx].to_string() + "])";
                    }
                    output[0] += s1;
                    width = s1.size();
                }
                else
                {
                    // uint64_t width = 0;

                    for (uint64_t i = 0; i < node.children_count(); i++)
                    {
                        InternalNode *child = node.get_child(i);
                        width += BPInternalNodeFunctions::get_tree_string(*child, output, leaf_container_vec);
                    }
                }
                std::string s = "";

                std::string id_str;
#if DEBUG
                id_str = "[" + std::to_string(node.id) + "]";
#endif

                while (s.size() < width)
                {
                    if (s.size() == 0)
                    {
                        s.push_back('[');
                        if (s.size() + id_str.size() < width)
                        {
                            s += id_str;
                        }
                    }
                    else if (s.size() + 1 == width)
                    {
                        s.push_back(']');
                    }
                    else
                    {

                        s.push_back('-');
                    }
                }

                output[h] += s;
                return width;
            }

            static void print_tree(const InternalNode &node, const std::vector<LEAF_CONTAINER> &leaf_container_vec)
            {
                std::vector<std::string> tree;
                BPInternalNodeFunctions::get_tree_string(node, tree, leaf_container_vec);
                for (int64_t i = tree.size() - 1; i >= 0; i--)
                {
                    std::cout << tree[i] << std::endl;
                }
            }
            static bool verify(const InternalNode &node, const std::vector<LEAF_CONTAINER> &leaf_container_vec, uint64_t max_degree, bool is_root)
            {
#if DEBUG
                if (node.id > (uint64_t)InternalNode::ID_COUNTER)
                {
                    throw std::logic_error("Error(6): BPInternalNode::verify()");
                }
#endif

                uint64_t value_count_sum = 0;
                uint64_t value_sum_sum = 0;
                bool b1 = true;

                if (!is_root)
                {
                    if (node.get_degree() > max_degree || node.get_degree() < (max_degree / 2))
                    {
                        throw std::logic_error("Error(3): BPInternalNode::verify()");
                    }
                }
                else
                {
                    if (node.get_degree() > max_degree)
                    {
                        throw std::logic_error("Error(4): BPInternalNode::verify()");
                    }
                }

                if (node.is_parent_of_leaves())
                {
                    for (uint64_t i = 0; i < node.children_count(); i++)
                    {
                        uint64_t x = (uint64_t)node.get_child(i);
                        value_count_sum += leaf_container_vec[x].size();
                        value_sum_sum += leaf_container_vec[x].psum();
                    }
                }
                else
                {
                    bool p = node.get_child(0)->is_parent_of_leaves();
                    const stool::SimpleDeque16<InternalNode *> &children = node.get_children();

                    for (InternalNode *child : children)
                    {
                        value_count_sum += child->get_value_count();
                        value_sum_sum += child->get_value_sum();

                        if (p != child->is_parent_of_leaves())
                        {
                            b1 = false;
                        }
                    }
                }

                if (node.use_psum())
                {
                    if (node.is_parent_of_leaves())
                    {
                        for (uint64_t i = 0; i < node.children_count(); i++)
                        {
                            uint64_t x = (uint64_t)node.get_child(i);
                            uint64_t psum = leaf_container_vec[x].psum();
                            uint64_t psum2 = node.get_value_sum_deque()[i];

                            if (psum != psum2)
                            {
                                std::cout << "Error: psum = " << psum << " == True psum: " << psum2 << std::endl;
                                throw std::logic_error("Error(5): BPInternalNode::verify()");
                            }
                        }
                    }
                }

                if (!(value_count_sum == node.get_value_count()))
                {
                    std::cout << "Error: count = " << node.get_value_count() << " == True count: " << value_count_sum << std::endl;
                    // stool::Printer::print("Count VEC", node.children_value_count_deque_.to_deque());

                    // this->print_tree(leaf_container_vec);
                    throw std::logic_error("Error(1): BPInternalNode::verify()");
                }

                if (!(value_sum_sum == node.get_value_sum()))
                {
                    std::cout << "Error: sum = " << node.get_value_sum() << " == True sum: " << value_sum_sum << std::endl;
                    // this->print_tree(leaf_container_vec);
                    throw std::logic_error("Error(2): BPInternalNode::verify()");
                }

                if (node.get_degree() > max_degree)
                {
                    throw std::logic_error("Error(3): BPInternalNode::verify()");
                }
                if (!b1)
                {
                    throw std::logic_error("Error(4): BPInternalNode::verify()");
                }

                return true;
            }
            //@}

            ////////////////////////////////////////////////////////////////////////////////
            ///   @name Non-const functions
            ///   Non-const functions
            ////////////////////////////////////////////////////////////////////////////////
            //@{
            static void move_right(InternalNode &left_node, InternalNode &right_node, uint64_t len, InternalNode *parent, int64_t parent_edge_index, std::vector<InternalNode *> &parent_vec)
            {

                assert(left_node.is_parent_of_leaves() == right_node.is_parent_of_leaves());

                if constexpr (USE_PSUM)
                {
                    // stool::SimpleDeque16<uint64_t> &left_sum_deq = left_node.get_value_sum_deque();
                    // stool::SimpleDeque16<uint64_t> &right_sum_deq = right_node.get_value_sum_deque();
                    // assert(left_sum_deq.size() >= len);

                    int64_t sum_delta = 0;
                    for (uint64_t i = 0; i < len; i++)
                    {
                        uint64_t _sum = left_node.__last_item_on_sum_deque();
                        // uint64_t _sum = left_sum_deq[left_sum_deq.size() - 1];
                        left_node.__pop_back_on_sum_deque();
                        right_node.__push_front_on_sum_deque(_sum);
                        // left_sum_deq.pop_back();
                        // right_sum_deq.push_front(_sum);
                        sum_delta += _sum;
                    }

                    if (parent != nullptr)
                    {
                        parent->__increment_a_value_of_sum_deque(parent_edge_index, -sum_delta);
                        parent->__increment_a_value_of_sum_deque(parent_edge_index + 1, sum_delta);

                        // stool::SimpleDeque16<uint64_t> &parent_sum_deq = parent->get_value_sum_deque();
                        // parent_sum_deq[parent_edge_index] -= sum_delta;
                        // parent_sum_deq[parent_edge_index + 1] += sum_delta;
                    }
                }

                {
                    auto &left_count_deq = left_node.get_value_count_deque();
                    auto &right_count_deq = right_node.get_value_count_deque();
                    int64_t count_delta = 0;

                    for (uint64_t i = 0; i < len; i++)
                    {
                        assert(left_node.children_count() > 0);
                        uint64_t _count = left_count_deq[left_count_deq.size() - 1];
                        left_count_deq.pop_back();
                        right_count_deq.push_front(_count);
                        count_delta += _count;
                    }

                    if (parent != nullptr)
                    {
                        auto &parent_count_deq = parent->get_value_count_deque();
                        parent_count_deq.decrement(parent_edge_index, count_delta);
                        parent_count_deq.increment(parent_edge_index + 1, count_delta);
                    }
                }

                stool::SimpleDeque16<InternalNode *> &left_children = left_node.get_children();
                stool::SimpleDeque16<InternalNode *> &right_children = right_node.get_children();
                for (uint64_t i = 0; i < len; i++)
                {
                    assert(left_node.children_count() > 0);
                    InternalNode *top = left_children[left_children.size() - 1];
                    left_children.pop_back();
                    right_children.push_front(top);
                    if (USE_PARENT_FIELD)
                    {
                        if (left_node.is_parent_of_leaves())
                        {
                            parent_vec[(uint64_t)top] = &right_node;
                        }
                        else
                        {
                            top->set_parent(&right_node);
                        }
                    }
                }
            }

            static void move_left(InternalNode &left_node, InternalNode &right_node, uint64_t len, InternalNode *parent, int64_t parent_edge_index, std::vector<InternalNode *> &parent_vec)
            {

                assert(right_node.is_parent_of_leaves() == left_node.is_parent_of_leaves());

                if constexpr (USE_PSUM)
                {
                    // stool::SimpleDeque16<uint64_t> &left_sum_deq = left_node.get_value_sum_deque();
                    // stool::SimpleDeque16<uint64_t> &right_sum_deq = right_node.get_value_sum_deque();

                    int64_t sum_delta = 0;
                    for (uint64_t i = 0; i < len; i++)
                    {
                        uint64_t _sum = right_node.get_value_sum_deque()[0];
                        right_node.__pop_front_on_sum_deque();
                        left_node.__push_back_on_sum_deque(_sum);
                        sum_delta += _sum;
                    }

                    if (parent != nullptr)
                    {
                        // stool::SimpleDeque16<uint64_t> &parent_sum_deq = parent->get_value_sum_deque();
                        parent->__increment_a_value_of_sum_deque(parent_edge_index, -sum_delta);
                        parent->__increment_a_value_of_sum_deque(parent_edge_index - 1, sum_delta);

                        // parent_sum_deq[parent_edge_index] -= sum_delta;
                        // parent_sum_deq[parent_edge_index - 1] += sum_delta;
                    }
                }

                {
                    auto &left_count_deq = left_node.get_value_count_deque();
                    auto &right_count_deq = right_node.get_value_count_deque();

                    int64_t count_delta = 0;
                    for (uint64_t i = 0; i < len; i++)
                    {
                        uint64_t _count = right_count_deq[0];
                        right_count_deq.pop_front();
                        left_count_deq.push_back(_count);
                        count_delta += _count;
                    }
                    if (parent != nullptr)
                    {
                        auto &parent_count_deq = parent->get_value_count_deque();
                        parent_count_deq.decrement(parent_edge_index, count_delta);
                        parent_count_deq.increment(parent_edge_index - 1, count_delta);
                    }
                }

                stool::SimpleDeque16<InternalNode *> &left_children = left_node.get_children();
                stool::SimpleDeque16<InternalNode *> &right_children = right_node.get_children();

                for (uint64_t i = 0; i < len; i++)
                {
                    assert(right_node.children_count() > 0);
                    InternalNode *top = right_children[0];
                    right_children.pop_front();
                    left_children.push_back(top);
                    if (USE_PARENT_FIELD)
                    {
                        if (right_node.is_parent_of_leaves())
                        {
                            parent_vec[(uint64_t)top] = &left_node;
                        }
                        else
                        {
                            top->set_parent(&left_node);
                        }
                    }
                }
            }

            //@}
        };
    }

}
