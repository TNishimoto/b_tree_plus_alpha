#include "../include/b_tree_plus_alpha.hpp"

int main(int argc, char *argv[])
{


    std::vector<bool> values = {1, 0, 1, 0, 1, 0, 1, 0, 1, 1};

    // Build spsi data structure S from sequence values.
    stool::sequence::DynamicBitSequence S = stool::sequence::DynamicBitSequence::build(values);
    std::cout << "Print the bits stored in S" << std::endl;
    S.print("S");

    std::cout << "The number of 1 in S[0..3] is " << S.rank1(4) << std::endl;
    std::cout << "The number of 0 in S[0..3] is " << S.rank0(4) << std::endl;

    std::cout << "The position of the third 1 in S is " << S.select1(2) << std::endl;
    std::cout << "The position of the third 0 in S is " << S.select0(2) << std::endl;
    std::cout << "The position of the fifth 0 in S is " << S.select0(4) << std::endl;

}
