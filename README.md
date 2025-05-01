# B-tree_plus_alpha

This repository implements various dynamic data structures using B-trees. 

For now, this repository is being used as a personal library. Therefore, detailed documentation has not yet been created.
I will provide more detailed documentation shortly.

## Download
The source codes in 'module' directory are maintained in different repositories. 
So, to download all the necessary source codes, do the following:

> git clone https://github.com/TNishimoto/b_tree_plus_alpha.git  
> cd b_tree_plus_alpha  
> git submodule init  
> git submodule update  

## Dynamic Data Structures

### VLCDequeSPSI class

[VLCDequeSPSI](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/include/prefix_sum/spsi.hpp) is a dynamic [prefix-sum](https://en.wikipedia.org/wiki/Prefix_sum "prefix-sum") data structure built on a non-negative integer sequence. 
This class is implemented using the technique in Section 2.2 of [this paper](https://doi.org/10.1016/j.jda.2018.11.002).

#### Table for update operations and queries

| Category         | Name              | Order                   | Description                                               |
|------------------|-------------------|-------------------------|-----------------------------------------------------------|
| Memory           |                   | O(n log (M/n)) bytes    |                                                           |
| Update Operation | S.insert(i, v)    | amortized O(log n) time | Insert v into S as the i-th value                         |
|                  | S.remove(i)       | amortized O(log n) time | Remove the i-th value from S                              |
|                  | S.push_back(v)    | amortized O(log n) time | Add v to S as the last value                              |
|                  | S.push_front(v)   | amortized O(log n) time | Add v to S as the first value                             |
|                  | S.increment(i, v) | amortized O(log n) time | S[i] += v                                                 |
|                  | S.decrement(i, v) | amortized O(log n) time | S[i] -= v                                                 |
|                  | S.push_many(i, P) | amortized O(log n) time | Add the values in sequence P to S as the last values      |
| Query            | S.psum(i)         | O(log n) time           | Return the sum of S[0..i]                                 |
|                  | S.search(v)       | O(log n) time           | Return the smallest index x that satisfies S.psum(x) >= v |
|                  | S.at(i)           | O(log n) time           | Return S[i]                                               |

Here, S is a non-negative integer sequence stored in VLCDequeSPSI; n is the number of values in S; M is the sum of the values in S.  
See [this page](https://tnishimoto.github.io/b_tree_plus_alpha/html/classstool_1_1prefix__sum_1_1_s_p_s_i.html) for the member functions supported by VLCDequeSPSI.

#### Example

An example of VLCDequeSPSI is described in [spsi_example.cpp](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/examples/spsi_example.cpp).  
The following message is displayed when this example is executed.  

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

#### Table for update operations and queries  

| Category         | Name              | Order                   | Description                                               |
|------------------|-------------------|-------------------------|-----------------------------------------------------------|
| Memory           |                   | O(n) bytes              |                                                           |
| Update Operation | S.insert(i, v)    | amortized O(log n) time | Insert v into S as the i-th value                         |
|                  | S.remove(i)       | amortized O(log n) time | Remove the i-th value from S                              |
|                  | S.push_back(v)    | amortized O(log n) time | Add v to S as the last value                              |
|                  | S.push_front(v)   | amortized O(log n) time | Add v to S as the first value                             |
|                  | S.set_bit(i, v)   | amortized O(log n) time | S[i] = v                                                  |
|                  | S.push_many(i, P) | amortized O(log n) time | Add the values in sequence P to S as the last values      |
| Query            | S.psum(i)         | O(log n) time           | Return the sum of S[0..i]                                 |
|                  | S.search(v)       | O(log n) time           | Return the smallest index x that satisfies S.psum(x) >= v |
|                  | S.at(i)           | O(log n) time           | Return S[i]                                               |
|                  | rank1(i)          | O(log n) time           | Return the number of 1 in S[0..i]                         |
|                  | rank0(i)          | O(log n) time           | Return the number of 0 in S[0..i]                         |
|                  | select1(i)        | O(log n) time           | Return the position of the (i+1)-th 1 in S                |
|                  | select0(i)        | O(log n) time           | Return the position of the (i+1)-th 0 in S                |

Here, S is a bit sequence stored in DynamicBitSequence; n is the number of values in S.  
See [this page](https://tnishimoto.github.io/b_tree_plus_alpha/html/classstool_1_1sequence_1_1_dynamic_bit_sequence.html) for the member functions supported by DynamicBitSequence.

### DynamicSequence64 class 

[DynamicSequence64](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/include/sequence/dynamic_sequence64.hpp) is a dynamic data structure that maintains a 64-bit non-negative sequence.

#### Table for update operations and queries

| Category         | Name              | Order                   | Description                                           |
|------------------|-------------------|-------------------------|-------------------------------------------------------|
| Memory           |                   | O(n log (M/n)) bytes    |                                                       |
| Update Operation | S.insert(i, v)    | amortized O(log n) time | Insert v into S as the i-th value                     |
|                  | S.remove(i)       | amortized O(log n) time | Remove the i-th value from S                          |
|                  | S.push_back(v)    | amortized O(log n) time | Add v to S as the last value                          |
|                  | S.push_front(v)   | amortized O(log n) time | Add v to S as the first value                         |
|                  | S.increment(i, v) | amortized O(log n) time | S[i] += v                                             |
|                  | S.decrement(i, v) | amortized O(log n) time | S[i] -= v                                             |
|                  | S.set_value(i, v) | amortized O(log n) time | S[i] = v                                              |
|                  | S.push_many(i, P) | amortized O(log n) time | Add the values in sequence P to S as the last values  |
| Query            | S.at(i)           | O(log n) time           | Return S[i]                                           |

Here, S is a non-negative integer sequence stored in DynamicSequence64; n is the number of values in S; M is the sum of the values in S.  
See [this page](https://tnishimoto.github.io/b_tree_plus_alpha/html/classstool_1_1sequence_1_1_dynamic_sequence64.html) for the member functions supported by DynamicSequence64.

### DynamicWaveletTree class 

[DynamicWaveletTree](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/include/sequence/dynamic_wavelet_tree.hpp) is a dynamic version of [Wavelet Tree](https://en.wikipedia.org/wiki/Wavelet_Tree "Wavelet Tree").

#### Table for update operations and queries

| Category         | Name              | Order                   | Description                                                   |
|------------------|-------------------|-------------------------|---------------------------------------------------------------|
| Memory           |                   | O(n log σ) bytes        |                                                               |
| Update Operation | S.insert(i, v)    | amortized O(log n) time | Insert v into S as the i-th character                         |
|                  | S.remove(i)       | amortized O(log n) time | Remove the i-th character from S                              |
|                  | S.push_back(v)    | amortized O(log n) time | Add v to S as the last character                              |
|                  | S.push_many(i, P) | amortized O(log n) time | Add the characters in sequence P to S as the last characters  |
| Query            | S.at(i)           | O(log n) time           | Return S[i]                                                   |
|                  | S.rank(i, c)      | O(log n) time           | Return the number of c in S[0..i]                             |
|                  | S.select(i, c)    | O(log n) time           | Return the position of the (i+1)-th c in S                    |

Here, S is a string stored in DynamicWaveletTree; n is the length of S; sigma is the the alphabet size of S.  
See [this page](https://tnishimoto.github.io/b_tree_plus_alpha/html/classstool_1_1sequence_1_1_dynamic_wavelet_tree.html) for the member functions supported by DynamicWaveletTree.

### DynamicPermutation class 

[DynamicPermutation](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/include/permutation/dynamic_permutation.hpp) is a dynamic data structure that maintains a permutation.

#### Table for update operations and queries

| Category         | Name                  | Order                   | Description                                          |
|------------------|-----------------------|-------------------------|------------------------------------------------------|
| Memory           |                       | O(n log n) bytes        |                                                      |
| Update Operation | S.insert(i, j)        | amortized O(log n) time | Insert the j-th smallest value into S at position i  |
|                  | S.erase(i)            | amortized O(log n) time | Remove the i-th value from S                         |
|                  | S.move_pi_index(p, q) | amortized O(log n) time | Move the p-th value in S to the q-th value           |
| Query            | S.access(i)           | O(log n) time           | Return S[i]                                          |
|                  | S.inverse(i)          | O(log n) time           | Return the index j that satisfies S[j] = i           |

Here, S is a permutation of n integers 0, 1, ..., n-1 stored in DynamicWaveletTree.  
See [this page](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/include/permutation/dynamic_permutation.hpp) for the member functions supported by DynamicWaveletTree.


## API Documentation (in preparation)

[Doxygen](https://TNishimoto.github.io/b_tree_plus_alpha/html/index.html)

