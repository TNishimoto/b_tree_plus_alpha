#include "../include/b_tree_plus_alpha.hpp"

int main(int argc, char *argv[])
{


    std::vector<uint8_t> text = {'a', 'b', 'a', 'b', 'a', 'b', 'a', 'b', 'a', 'b'};
    std::vector<uint8_t> alphabet = {'a', 'b', 'c'};

    std::cout << "Build DynamicWaveletTree S from text ababababab with alphabet {a, b, c}" << std::endl;
    stool::sequence::DynamicWaveletTree S = stool::sequence::DynamicWaveletTree::build(text, alphabet);
    std::cout << "Print the integers stored in S" << std::endl;
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "The number of a in S[0..3] is " << S.rank(4, 'a') << std::endl;
    std::cout << "The number of b in S[0..3] is " << S.rank(4, 'b') << std::endl;

    std::cout << "The position of the third a in S is " << S.select(2, 'a') << std::endl;
    std::cout << "The position of the third b in S is " << S.select(2, 'b') << std::endl;
    std::cout << "The position of the 10-th b in S is " << S.select(9, 'b') << std::endl;
    std::cout << "The position of the first c in S is " << S.select(0, 'c') << std::endl;

    std::cout << "Insert c into S at position 4" << std::endl;
    S.insert(4, 'c');
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Delete S[4] from S" << std::endl;
    S.remove(4);
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Add c to the tail of S" << std::endl;
    S.push_back('c');
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Add bbbb to the tail of S" << std::endl;
    std::vector<uint8_t> values2 = {'b', 'b', 'b', 'b'};
    S.push_many(values2);
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Write S to S.bin" << std::endl;
    {
        std::ofstream ofs("S.bin");    
        stool::sequence::DynamicWaveletTree::save(S, ofs);
        ofs.close();
    }

    std::cout << "Remove all values from S" << std::endl;
    S.clear();
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Read S from S.bin" << std::endl;
    {
        std::ifstream ifs("S.bin");
        stool::sequence::DynamicWaveletTree tmp = stool::sequence::DynamicWaveletTree::build_from_data(ifs);
        ifs.close();
        S.swap(tmp);
    }
    std::cout << "S = " << S.to_string() << std::endl;

}
