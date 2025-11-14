#include "../include/all.hpp"

int main(int argc, char *argv[])
{


    std::vector<uint64_t> values = {10, 20, 30, 40, 50, 60, 70, 80, 90, 100};

    std::cout << "Build DynamicSequence64 S from integer sequence [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]" << std::endl;
    stool::bptree::SimpleDynamicSequence64 S = stool::bptree::SimpleDynamicSequence64::build(values);
    std::cout << "Print the integers stored in S" << std::endl;
    std::cout << "S = " << S.to_string() << std::endl;


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

    
    std::cout << "Add 1, 2, 3, 4 to the tail of S" << std::endl;
    std::vector<uint64_t> values2 = {1, 2, 3, 4};
    S.push_many(values2);
    std::cout << "S = " << S.to_string() << std::endl;
    

    std::cout << "Write S to S.bin" << std::endl;
    {
        std::ofstream ofs("S.bin");    
        stool::bptree::SimpleDynamicSequence64::store_to_file(S, ofs);
        ofs.close();
    }

    std::cout << "Remove all values from S" << std::endl;
    S.clear();
    std::cout << "S = " << S.to_string() << std::endl;

    std::cout << "Read S from S.bin" << std::endl;
    {
        std::ifstream ifs("S.bin");
        stool::bptree::SimpleDynamicSequence64 tmp = stool::bptree::SimpleDynamicSequence64::load_from_file(ifs);
        ifs.close();
        S.swap(tmp);
    }
    std::cout << "S = " << S.to_string() << std::endl;


}
