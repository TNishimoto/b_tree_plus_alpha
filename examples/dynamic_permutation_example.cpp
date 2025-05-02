#include "../include/b_tree_plus_alpha.hpp"

int main(int argc, char *argv[])
{


    std::vector<uint64_t> values = {0, 3, 1, 2, 5, 4, 9, 8, 7, 6};

    std::cout << "Build DynamicPermutation S from permutation [0, 3, 1, 2, 5, 4, 9, 8, 7, 6]" << std::endl;
    stool::bptree::DynamicPermutation S; 
    S.build(values.begin(), values.end(), values.size());
    std::cout << "Print the integers stored in S" << std::endl;
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "S[3] = " << S.access(3) << std::endl;
    std::cout << "S^{-1}[3] = " << S.inverse(3) << std::endl;

    
    std::cout << "Insert 2 into S at position 1" << std::endl;
    S.insert(1, 2);
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Delete S[4] from S" << std::endl;
    S.erase(4);
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Move S[1] in S to position 0" << std::endl;
    S.move_pi_index(1, 0);
    std::cout << "S = " << S.to_string() << std::endl;

    
 

    std::cout << "Write S to S.bin" << std::endl;
    {
        std::ofstream ofs("S.bin");    
        stool::bptree::DynamicPermutation::save(S, ofs);
        ofs.close();
    }

    std::cout << "Remove all values from S" << std::endl;
    S.clear();
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Read S from S.bin" << std::endl;
    {
        std::ifstream ifs("S.bin");
        stool::bptree::DynamicPermutation tmp = stool::bptree::DynamicPermutation::build_from_data(ifs);
        ifs.close();
        S.swap(tmp);
    }
    std::cout << "S = " << S.to_string() << std::endl;


}
