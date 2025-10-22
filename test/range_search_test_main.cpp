#include <iostream>
#include <string>
#include <memory>
#include <bitset>
#include <cassert>
#include <chrono>
#include <cstdio>
#include "../include/b_tree_plus_alpha.hpp"

//using RANGED_DYNAMIC_WAVELET_TREE = stool::bptree::DynamicWaveletTreeOnGrid;
using RANGED_DYNAMIC_WAVELET_TREE = stool::bptree::DynamicWaveletTreeForRangeSearch;


std::vector<uint64_t> create_random_rank_array(uint64_t size, uint64_t seed)
{
    std::mt19937_64 mt64(seed);
    std::vector<uint64_t> r;
    for (uint64_t i = 0; i < size; i++)
    {
        r.push_back(i);
    }
    std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, size - 1);

    for (uint64_t i = 0; i < size; i++)
    {
        uint64_t pos1 = get_rand_uni_int(mt64);
        uint64_t pos2 = get_rand_uni_int(mt64);
        std::swap(r[pos1], r[pos2]);
    }
    return r;
}

std::vector<uint64_t> sort_elements_in_x_order(const std::vector<uint64_t> &rank_array){
    std::vector<uint64_t> r;
    r.resize(rank_array.size(), UINT64_MAX);
    for(uint64_t i = 0; i < rank_array.size(); i++){
        r[rank_array[i]] = i;
    }
    return r;
}

void insert_element_to_rank_array(std::vector<uint64_t> &rank_array, uint64_t new_x_rank, uint64_t new_y_rank){
    for(uint64_t i = 0; i < rank_array.size(); i++){
        if(rank_array[i] >= new_x_rank){
            rank_array[i]++;
        }
    }
    rank_array.insert(rank_array.begin() + new_y_rank, new_x_rank);
}

void remove_element_from_rank_array(std::vector<uint64_t> &rank_array, uint64_t y_rank){
    uint64_t _rank = rank_array[y_rank];
    for(uint64_t i = 0; i < rank_array.size(); i++){
        if(rank_array[i] >= _rank){
            rank_array[i]--;
        }
    }
    rank_array.erase(rank_array.begin() + y_rank);
}


void insert_test(uint64_t size, bool detail_check, uint64_t seed)
{
    std::mt19937_64 mt64(seed);
    std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, UINT64_MAX);
    std::vector<uint64_t> rank_array;
    rank_array.clear();
    RANGED_DYNAMIC_WAVELET_TREE ds;
    ds.build(rank_array);

    for(uint64_t i = 0; i < size; i++){
        uint64_t new_x_rank = get_rand_uni_int(mt64) % (i+1);
        uint64_t new_y_rank = get_rand_uni_int(mt64) % (i+1);
        insert_element_to_rank_array(rank_array, new_x_rank, new_y_rank);
        ds.add(new_x_rank, new_y_rank);


        if(detail_check){
            std::vector<uint64_t> test_rank_array = ds.to_rank_elements_in_y_order();
            try{
                stool::EqualChecker::equal_check(rank_array, test_rank_array);
            }catch(const std::exception& e){
                std::cout << "Error: " << e.what() << std::endl;
                stool::DebugPrinter::print_integers(rank_array,      "rank_array     ");
                stool::DebugPrinter::print_integers(test_rank_array, "test_rank_array");
                ds.print_tree();
                throw e;
            }    
        }

    }

    {
        std::vector<uint64_t> test_rank_array = ds.to_rank_elements_in_y_order();
        try{
            stool::EqualChecker::equal_check(rank_array, test_rank_array);
        }catch(const std::exception& e){
            std::cout << "Error: " << e.what() << std::endl;
            stool::DebugPrinter::print_integers(rank_array,      "rank_array     ");
            stool::DebugPrinter::print_integers(test_rank_array, "test_rank_array");
            ds.print_tree();
            throw e;
        }
    
    }

}

void insert_test(uint64_t size, uint64_t number_of_trials, bool detail_check, uint64_t seed)
{
    std::cout << "INSERT TEST" << std::endl;
    for(uint64_t len = 1; len <= size; len*=2){
        std::cout << "\t len: " << len << ": " << std::flush;
        for(uint64_t i = 0; i < number_of_trials; i++){
            std::cout << "+" << std::flush;
            insert_test(len, detail_check, seed++);
        }    
        std::cout << std::endl;
    }
    std::cout << "[END]" << std::endl;
}

void remove_test(uint64_t size, bool detail_check, uint64_t seed)
{
    std::mt19937_64 mt64(seed);
    std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, UINT64_MAX);
    std::vector<uint64_t> rank_array = create_random_rank_array(size, seed++);
    RANGED_DYNAMIC_WAVELET_TREE ds;
    ds.build(rank_array);

    

    for(uint64_t i = 0; i < size; i++){
        uint64_t remove_y_rank = get_rand_uni_int(mt64) % rank_array.size();
        remove_element_from_rank_array(rank_array, remove_y_rank);
        ds.remove(remove_y_rank);


        if(detail_check){
            std::vector<uint64_t> test_rank_array = ds.to_rank_elements_in_y_order();
            try{
                stool::EqualChecker::equal_check(rank_array, test_rank_array);
            }catch(const std::exception& e){
                std::cout << "Error: " << e.what() << std::endl;
                stool::DebugPrinter::print_integers(rank_array,      "rank_array     ");
                stool::DebugPrinter::print_integers(test_rank_array, "test_rank_array");
                ds.print_tree();
                throw e;
            }    
        }

    }

    {
        std::vector<uint64_t> test_rank_array = ds.to_rank_elements_in_y_order();
        try{
            stool::EqualChecker::equal_check(rank_array, test_rank_array);
        }catch(const std::exception& e){
            std::cout << "Error: " << e.what() << std::endl;
            stool::DebugPrinter::print_integers(rank_array,      "rank_array     ");
            stool::DebugPrinter::print_integers(test_rank_array, "test_rank_array");
            ds.print_tree();
            throw e;
        }
    
    }

}


void remove_test(uint64_t size, uint64_t number_of_trials, bool detail_check, uint64_t seed)
{
    std::cout << "REMOVE TEST" << std::endl;
    for(uint64_t len = 1; len <= size; len*=2){
        std::cout << "\t len: " << len << ": " << std::flush;
        for(uint64_t i = 0; i < number_of_trials; i++){
            std::cout << "+" << std::flush;
            remove_test(len, detail_check, seed++);
        }    
        std::cout << std::endl;
    }
    std::cout << "[END]" << std::endl;
}




void build_test(uint64_t size, uint64_t seed)
{
    std::vector<uint64_t> rank_array = create_random_rank_array(size, seed);
    RANGED_DYNAMIC_WAVELET_TREE ds;
    ds.build(rank_array);

    std::vector<uint64_t> test_rank_array = ds.to_rank_elements_in_y_order();
    try{
        stool::EqualChecker::equal_check(rank_array, test_rank_array, "build_test(1)");
    }catch(const std::exception& e){
        std::cout << "Error: " << e.what() << std::endl;
        ds.print_tree();
        stool::DebugPrinter::print_integers(test_rank_array, "test_rank_array");
        stool::DebugPrinter::print_integers(rank_array, "rank_array");
        throw e;
    }

    std::vector<uint64_t> test_rank_array_in_x_order = ds.to_rank_elements_in_x_order();
    std::vector<uint64_t> rank_array_in_x_order = sort_elements_in_x_order(rank_array);
    try{
        stool::EqualChecker::equal_check(rank_array_in_x_order,test_rank_array_in_x_order, "build_test(2)");
    }catch(const std::exception& e){
        std::cout << "Error: " << e.what() << std::endl;
        stool::DebugPrinter::print_integers(rank_array, "rank_array");
        stool::DebugPrinter::print_integers(test_rank_array_in_x_order, "test_rank_array_in_x_order");
        stool::DebugPrinter::print_integers(rank_array_in_x_order, "rank_array_in_x_order");


        ds.print_tree();
        throw e;
    }
    
}
void build_test(uint64_t size, uint64_t number_of_trials, uint64_t seed)
{
    std::cout << "BUILD TEST" << std::endl;
    for(uint64_t len = 1; len <= size; len*=2){
        std::cout << "\t len: " << len << ": " << std::flush;
        for(uint64_t i = 0; i < number_of_trials; i++){
            std::cout << "+" << std::flush;
            build_test(len, seed++);
        }    
        std::cout << std::endl;
    }
    std::cout << "[END]" << std::endl;
}


void range_search_test_sub(uint64_t size, uint64_t number_of_trials, uint64_t seed)
{
    std::mt19937_64 mt64(seed);
    std::uniform_int_distribution<uint64_t> get_rand_uni_int(0, size - 1);
    std::vector<uint64_t> rank_array = create_random_rank_array(size, seed);
    RANGED_DYNAMIC_WAVELET_TREE ds;
    ds.build(rank_array);

    for(uint64_t i = 0; i < number_of_trials; i++){
        uint64_t x_min = get_rand_uni_int(mt64);
        uint64_t x_max = get_rand_uni_int(mt64);
        if(x_min > x_max){
            std::swap(x_min, x_max);
        }
        uint64_t y_min = get_rand_uni_int(mt64);
        uint64_t y_max = get_rand_uni_int(mt64);
        if(y_min > y_max){
            std::swap(y_min, y_max);
        }


        std::vector<uint64_t> expected_out;

        for(uint64_t y = y_min; y <= y_max; y++){
            if(rank_array[y] >= x_min && rank_array[y] <= x_max){
                expected_out.push_back(rank_array[y]);
            }
        }
        std::sort(expected_out.begin(), expected_out.end());

        std::vector<uint64_t> test_out;
        ds.range_report(x_min, x_max, y_min, y_max, test_out);
        std::sort(test_out.begin(), test_out.end());

        try{
            stool::EqualChecker::equal_check(expected_out, test_out, "range_search_test");
        }catch(const std::exception& e){
            std::cout << "Error: " << e.what() << std::endl;
            stool::DebugPrinter::print_integers(rank_array, "rank_array");
            std::cout << "x_min: " << x_min << ", x_max: " << x_max << ", y_min: " << y_min << ", y_max: " << y_max << std::endl;

            stool::DebugPrinter::print_integers(test_out, "test_out");
            stool::DebugPrinter::print_integers(expected_out, "expected_out");

            ds.print_tree();
            throw e;
        }
    }

}

void range_search_test(uint64_t max_size, uint64_t number_of_trials1, uint64_t number_of_trials2, uint64_t seed)
{
    std::cout << "RANGE SEARCH TEST" << std::endl;
    for(uint64_t len = 1; len <= max_size; len*=2){
        std::cout << "\tlen: " << len << ": " << std::flush;
        for(uint64_t i = 0; i < number_of_trials1; i++){
            std::cout << "+" << std::flush;
            range_search_test_sub(len, number_of_trials2 ,seed++);
        }
        std::cout << std::endl;
    }
    std::cout << "[END]" << std::endl;
}

void load_write_file_test(uint64_t size, std::string filepath, uint64_t seed)
{
    
    std::vector<uint64_t> rank_array = create_random_rank_array(size, seed);
    RANGED_DYNAMIC_WAVELET_TREE ds;
    ds.build(rank_array);

    {
        std::ofstream os;
        os.open(filepath, std::ios::binary);
        if (!os)
        {
            std::cerr << "Error: Could not open file for writing." << std::endl;
            throw std::runtime_error("File open error");
        }
        RANGED_DYNAMIC_WAVELET_TREE::store_to_file(ds, os);
    }

    RANGED_DYNAMIC_WAVELET_TREE ds2;
    {
        std::ifstream ifs;
        ifs.open(filepath, std::ios::binary);
        if (!ifs)
        {
            std::cerr << "Error: Could not open file for reading." << std::endl;
        }
        ds2 = RANGED_DYNAMIC_WAVELET_TREE::load_from_file(ifs);
    }

    std::vector<uint64_t> test_rank_array = ds.to_rank_elements_in_y_order();
    std::vector<uint64_t> test_rank_array2 = ds2.to_rank_elements_in_y_order();
    try{
        stool::EqualChecker::equal_check(test_rank_array, test_rank_array2, "load_write_file_test");
    }catch(const std::exception& e){
        std::cout << "Error: " << e.what() << std::endl;
        ds.print_tree();
        ds2.print_tree();
        throw e;
    }
}


void load_write_file_test(uint64_t size, uint64_t number_of_trials, std::string filepath, uint64_t seed)
{
    std::cout << "LOAD WRITE FILE TEST" << std::endl;
    for(uint64_t len = 1; len <= size; len*=2){
        std::cout << "\t len: " << len << ": " << std::flush;
        for(uint64_t i = 0; i < number_of_trials; i++){
            std::cout << "+" << std::flush;

            load_write_file_test(len, filepath, seed++);
        }    
        std::cout << std::endl;
    }
    std::cout << "[END]" << std::endl;
}

void load_write_bits_test(uint64_t size, uint64_t seed)
{
    
    std::vector<uint64_t> rank_array = create_random_rank_array(size, seed);
    RANGED_DYNAMIC_WAVELET_TREE ds;
    ds.build(rank_array);
    std::vector<uint8_t> bytes;
    uint64_t pos = 0;
    RANGED_DYNAMIC_WAVELET_TREE::store_to_bytes(ds, bytes, pos);
    pos = 0;

    RANGED_DYNAMIC_WAVELET_TREE ds2;
    ds2 = RANGED_DYNAMIC_WAVELET_TREE::load_from_bytes(bytes, pos);


    std::vector<uint64_t> test_rank_array = ds.to_rank_elements_in_y_order();
    std::vector<uint64_t> test_rank_array2 = ds2.to_rank_elements_in_y_order();
    try{
        stool::EqualChecker::equal_check(test_rank_array, test_rank_array2, "load_write_file_test");
    }catch(const std::exception& e){
        std::cout << "Error: " << e.what() << std::endl;
        ds.print_tree();
        ds2.print_tree();
        throw e;
    }
}


void load_write_bits_test(uint64_t size, uint64_t number_of_trials, uint64_t seed)
{
    std::cout << "LOAD WRITE BITS TEST" << std::endl;
    for(uint64_t len = 1; len <= size; len*=2){
        std::cout << "\t len: " << len << ": " << std::flush;
        for(uint64_t i = 0; i < number_of_trials; i++){
            std::cout << "+" << std::flush;

            load_write_bits_test(len, seed++);
        }    
        std::cout << std::endl;
    }
    std::cout << "[END]" << std::endl;
}



int main(int argc, char *argv[])
{
#ifdef DEBUG
    std::cout << "\033[41m";
    std::cout << "DEBUG MODE!" << std::endl;
    std::cout << "\e[m" << std::endl;
    // std::cout << "\033[30m" << std::endl;
#endif
#ifdef SLOWDEBUG
    std::cout << "\033[41m";
    std::cout << "SLOWDEBUG MODE!" << std::endl;
    std::cout << "\e[m" << std::endl;
    // std::cout << "\033[30m" << std::endl;
#endif

    cmdline::parser p;

    // p.add<std::string>("input_file", 'i', "input file name", true);
    p.add<uint>("mode", 'm', "mode", false, 0);
    p.add<uint64_t>("seed", 's', "seed", false, 0);

    p.parse_check(argc, argv);
    uint64_t seed = p.get<uint64_t>("seed");
    //uint64_t mode = p.get<uint>("mode");

    uint64_t text_size = 4000;

    //load_write_file_test(text_size, 10, "range_search_test.bits", seed);
    //load_write_bits_test(text_size, 10, seed);
    //remove_test(text_size, 10, false, seed);
    //insert_test(text_size, 10, true, seed);

    build_test(text_size, 100, seed);
    range_search_test(text_size, 50, 100, seed);






}