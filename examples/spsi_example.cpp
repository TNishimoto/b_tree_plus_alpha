#include "../include/b_tree_plus_alpha.hpp"

int main(int argc, char *argv[])
{


    std::vector<uint64_t> values = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    // Build spsi data structure S from sequence values.
    stool::prefix_sum::VLCDequeSPSI S = stool::prefix_sum::VLCDequeSPSI::build(values);

    std::cout << "Print the values stored in S" << std::endl;
    S.print("S");

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

    std::cout << "Insert 100 into S at the fifth value" << std::endl;
    S.insert(4, 100);
    S.print("S");

    std::cout << "Delete the fifth value from S" << std::endl;
    S.remove(4);
    S.print("S");

    std::cout << "Add 0 to S as the last value" << std::endl;
    S.push_back(0);
    std::cout << "Add 9 to S as the first value" << std::endl;
    S.push_front(9);
    S.print("S");

    std::cout << "Add 1, 2, 3, 4 to S as the last values" << std::endl;
    std::vector<uint64_t> values2 = {1, 2, 3, 4};
    S.push_many(values2);
    S.print("S");

    std::cout << "Write S to S.bin" << std::endl;
    {
        std::ofstream ofs("S.bin");    
        stool::prefix_sum::VLCDequeSPSI::save(S, ofs);
        ofs.close();
    }

    std::cout << "Remove all values from S" << std::endl;
    S.clear();
    S.print("S");

    std::cout << "Read S from S.bin" << std::endl;
    stool::prefix_sum::VLCDequeSPSI S2;
    {
        std::ifstream ifs("S.bin");
        stool::prefix_sum::VLCDequeSPSI tmp = stool::prefix_sum::VLCDequeSPSI::build_from_data(ifs);
        ifs.close();
        S2.swap(tmp);
    }
    S2.print("S");



}
