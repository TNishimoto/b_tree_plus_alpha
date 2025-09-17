#include "../include/b_tree_plus_alpha.hpp"

int main(int argc, char *argv[])
{


    std::vector<bool> values = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1};

    std::cout << "Build SimpleDynamicBitSequence S from bit sequence [1, 0, 1, 0, 1, 0, 1, 0, 1, 1]" << std::endl;
    stool::bptree::SimpleDynamicBitSequence S = stool::bptree::SimpleDynamicBitSequence::build(values);
    std::cout << "Print the bits stored in S" << std::endl;
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "The number of 1 in S[0..3] is " << S.rank1(4) << std::endl;
    std::cout << "The number of 0 in S[0..3] is " << S.rank0(4) << std::endl;

    std::cout << "The position of the third 1 in S is " << S.select1(2) << std::endl;
    std::cout << "The position of the third 0 in S is " << S.select0(2) << std::endl;
    std::cout << "The position of the fifth 0 in S is " << S.select0(4) << std::endl;

    std::cout << "Insert 1 into S at position 4" << std::endl;
    S.insert(4, 1);
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Delete S[4] from S" << std::endl;
    S.remove(4);
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Add 0 to the tail of S" << std::endl;
    S.push_back(0);
    std::cout << "Add 1 to the head of S" << std::endl;
    S.push_front(1);
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Add 1010 to the tail of S" << std::endl;
    std::vector<bool> values2 = {1, 0, 1, 0};
    S.push_many(values2);
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Write S to S.bin" << std::endl;
    {
        std::ofstream ofs("S.bin");    
        stool::bptree::SimpleDynamicBitSequence::store_to_file(S, ofs);
        ofs.close();
    }

    std::cout << "Remove all values from S" << std::endl;
    S.clear();
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Read S from S.bin" << std::endl;
    {
        std::ifstream ifs("S.bin");
        stool::bptree::SimpleDynamicBitSequence tmp = stool::bptree::SimpleDynamicBitSequence::load_from_file(ifs);
        ifs.close();
        S.swap(tmp);
    }
    std::cout << "S = " << S.to_string() << std::endl;


}
