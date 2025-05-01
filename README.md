# B-tree_plus_alpha

This repository implements various dynamic data structures using B-trees. 

For now, this repository is being used as a personal library. Therefore, detailed documentation has not yet been created.
I will provide more detailed documentation shortly.

# download
The source codes in 'module' directory are maintained in different repositories. 
So, to download all the necessary source codes, do the following:

> git clone https://github.com/TNishimoto/b_tree_plus_alpha.git  
> cd b_tree_plus_alpha  
> git submodule init  
> git submodule update  

## examples

### VLCDequeSPSI class

[VLCDequeSPSI](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/include/prefix_sum/spsi.hpp) is a dynamic [prefix-sum](https://en.wikipedia.org/wiki/Prefix_sum "prefix-sum") data structure built on a non-negative integer sequence. 
An example of VLCDequeSPSI is described in [spsi_example.cpp](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/examples/spsi_example.cpp).  
The following message is displayed when this example is executed.  

| Name       | Order         |
|------------|---------------|
| Memory     | O(n) bytes    |
| Operations |               |
| insert     | O(log n) time |
| delete     | O(log n) time |
| push_back  | O(log n) time |
| push_front | O(log n) time |
| psum       | O(log n) time |
| search     | O(log n) time |
| at         | O(log n) time |
| increment  | O(log n) time |
| decrement  | O(log n) time |
| push_many  | O(log n) time |

% ./spsi_example.out

> Print the values stored in S  
> S: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]  
> The sum of S[0..3] is 10  
> Let x be the smallest index that satisfies psum(x) >= 10. Then, x = 3  
> Let y be the smallest index that satisfies psum(y) >= 1000. Then, y =-1  
> S[3] = 4  
> S[3] += 10  
> S[3] = 14  
> S[3] -= 10  
> S[3] = 4  
> Insert 100 into S at the fifth value  
> S: [1, 2, 3, 4, 100, 5, 6, 7, 8, 9, 10]  
> Delete the fifth value from S  
> S: [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]  
> Add 0 to S as the last value  
> Add 9 to S as the first value  
> S: [9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0]  
> Add 1, 2, 3, 4 to S as the last values  
> S: [9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 1, 2, 3, 4]  
> Write S to S.bin  
> Remove all values from S  
> S: []  
> Read S from S.bin  
> S: [9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 1, 2, 3, 4]  

### DynamicBitSequence class 

[DynamicBitSequence](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/include/sequence/dynamic_bit_sequence.hpp) is a dynamic data structure supporting rank and select queries on a bit sequence.

### DynamicSequence64 class 

[DynamicSequence64](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/include/sequence/dynamic_sequence64.hpp) is a dynamic data structure that maintains a 64-bit non-negative sequence.

### DynamicWaveletTree class 

[DynamicWaveletTree](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/include/sequence/dynamic_wavelet_tree.hpp) is a dynamic version of [Wavelet Tree](https://en.wikipedia.org/wiki/Wavelet_Tree "Wavelet Tree").

### DynamicPermutation class 

[DynamicPermutation](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/include/permutation/dynamic_permutation.hpp) is a dynamic data structure that maintains a permutation.


## API Documentation (in preparation)

[Doxygen](https://TNishimoto.github.io/b_tree_plus_alpha/html/index.html)
