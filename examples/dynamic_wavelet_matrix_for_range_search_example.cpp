#include "../include/all.hpp"

int main(int argc, char *argv[])
{
    /*
    _S is a permutation of [0.._S.size()-1].
    */
    std::vector<uint64_t> _S = {8, 1, 5, 2, 6, 3, 7, 4, 0};

    std::cout << "Build DynamicWaveletMatrixForRangeSearch S from [8, 1, 5, 2, 6, 3, 7, 4, 0]" << std::endl;
    stool::bptree::DynamicWaveletMatrixForRangeSearch S = stool::bptree::DynamicWaveletMatrixForRangeSearch::build(_S);
    std::cout << "Print the integers stored in S" << std::endl;
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "The number of elements in S is " << S.size() << std::endl;
    std::cout << "S[0] = " << S.access_x_rank(0) << std::endl;
    std::cout << "The occurrence position of 3 in S is " << S.access_y_rank(3) << std::endl;

    std::vector<uint64_t> reported_indexes;
    uint64_t number_of_reported_elements = S.range_report(0, 3, 1, 4, reported_indexes);
    std::cout << "The number of elements such that 1 <= S[i] <= 4 in S[0..3] is " << number_of_reported_elements << std::endl;
    stool::DebugPrinter::print_integers(reported_indexes, "Reported_indexes");

    std::cout << "Insert 2 into S at position 1" << std::endl;
    S.insert(1, 2);
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Delete an element $i$ such that $S[i] = 4$ from S" << std::endl;
    S.erase_y_rank(4);
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Write S to S.bin" << std::endl;
    {
        std::ofstream ofs("S.bin");    
        stool::bptree::DynamicWaveletMatrixForRangeSearch::store_to_file(S, ofs);
        ofs.close();
    }

    std::cout << "Remove all values from S" << std::endl;
    S.clear();
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Read S from S.bin" << std::endl;
    {
        std::ifstream ifs("S.bin");
        stool::bptree::DynamicWaveletMatrixForRangeSearch tmp = stool::bptree::DynamicWaveletMatrixForRangeSearch::load_from_file(ifs);
        ifs.close();
        S.swap(tmp);
    }
    std::cout << "S = " << S.to_string() << std::endl;


    
    /*

    std::cout << "The number of a in S[0..3] is " << S.one_based_rank(4, 'a') << std::endl;
    std::cout << "The number of b in S[0..3] is " << S.one_based_rank(4, 'b') << std::endl;

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
        stool::bptree::DynamicWaveletTree::store_to_file(S, ofs);
        ofs.close();
    }

    std::cout << "Remove all values from S" << std::endl;
    S.clear();
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Read S from S.bin" << std::endl;
    {
        std::ifstream ifs("S.bin");
        stool::bptree::DynamicWaveletTree tmp = stool::bptree::DynamicWaveletTree::load_from_file(ifs);
        ifs.close();
        S.swap(tmp);
    }
    std::cout << "S = " << S.to_string() << std::endl;
    */

}
