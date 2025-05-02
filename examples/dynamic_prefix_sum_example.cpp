#include "../include/b_tree_plus_alpha.hpp"

int main(int argc, char *argv[])
{


    std::vector<uint64_t> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    std::cout << "Build DynamicPrefixSum S from sequence [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]" << std::endl;
    stool::bptree::DynamicPrefixSum<> S = stool::bptree::DynamicPrefixSum<>::build(values);

    std::cout << "Print the values stored in S" << std::endl;
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "The sum of S[0..3] is " << S.psum(3) << std::endl; 
    std::cout << "Let x be the smallest index that satisfies psum(x) >= 10. Then, x = " << S.search(9) << std::endl; 
    std::cout << "Let y be the smallest index that satisfies psum(y) >= 1000. Then, y =" << S.search(1000) << std::endl; 
    std::cout << "S[3] = " << S[3] << std::endl;

    std::cout << "S[3] += 10" << std::endl;
    S.increment(3, 10);
    std::cout << "S[3] = " << S[3] << std::endl;
    std::cout << "S[3] -= 10" << std::endl;
    S.decrement(3, 10);
    std::cout << "S[3] = " << S[3] << std::endl;

    std::cout << "Insert 100 into S at position 4" << std::endl;
    S.insert(4, 100);
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Delete S[4] from S" << std::endl;
    S.remove(4);
    std::cout << "S = " << S.to_string() << std::endl;


    std::cout << "Add 0 to the tail of S" << std::endl;
    S.push_back(0);
    std::cout << "Add 9 to the head of S" << std::endl;
    S.push_front(9);
    std::cout << "S = " << S.to_string() << std::endl;


    std::cout << "Add 1, 2, 3, 4 to the tail of S" << std::endl;
    std::vector<uint64_t> values2 = {1, 2, 3, 4};
    S.push_many(values2);
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Write S to S.bin" << std::endl;
    {
        std::ofstream ofs("S.bin");    
        stool::bptree::DynamicPrefixSum<>::save(S, ofs);
        ofs.close();
    }

    std::cout << "Remove all values from S" << std::endl;
    S.clear();
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Read S from S.bin" << std::endl;
    {
        std::ifstream ifs("S.bin");
        stool::bptree::DynamicPrefixSum<> tmp = stool::bptree::DynamicPrefixSum<>::build_from_data(ifs);
        ifs.close();
        S.swap(tmp);
    }
    std::cout << "S = " << S.to_string() << std::endl;



}
