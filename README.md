# B-tree_plus_alpha

This repository implements the following five dynamic data structures using B-trees:

- A dynamic prefix-sum data structure for a sequence of non-negative integers
- A dynamic bit-vector supporting access, rank, and select queries
- A dynamic data structure supporting access queries on a sequence of non-negative integers
- A dynamic wavelet tree for strings
- A dynamic data structure supporting access queries on a permutation

## Download
The source code in the **modules** directory is maintained in separate repositories.
To download all the required source code, follow the steps below:

```
 git clone https://github.com/TNishimoto/b_tree_plus_alpha.git  
 cd b_tree_plus_alpha  
 git submodule init  
 git submodule update  
```

## Compile

B-tree_plus_alpha is a header-only library.
To use this library, (i) include the **include/b_tree_plus_alpha.hpp** file, and (ii) add the **modules** directory to the include path.
The examples directory contains a **CMakeLists.txt** file for compiling example programs.

## Dynamic Data Structures

### DynamicPrefixSum class

[DynamicPrefixSum](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/include/prefix_sum/dynamic_prefix_sum.hpp) is a dynamic [prefix-sum](https://en.wikipedia.org/wiki/Prefix_sum "prefix-sum") data structure built on a sequence of non-negative integers. 
This class is implemented based on the technique described in Section 2.2 of [this paper](https://doi.org/10.1016/j.jda.2018.11.002).

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

Here, S is a non-negative integer sequence stored in DynamicPrefixSum; n is the number of values in S; M is the sum of the values in S.  
See [this page](https://tnishimoto.github.io/b_tree_plus_alpha/html/classstool_1_1prefix__sum_1_1_s_p_s_i.html) for the member functions supported by DynamicPrefixSum.

#### Example

An example usage of DynamicPrefixSum is provided in [dynamic_prefix_sum_example.cpp](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/examples/dynamic_prefix_sum_example.cpp).
When this example is executed, the following message is displayed:

```
% ./dynamic_prefix_sum_example.out  
  
 Build DynamicPrefixSum S from sequence [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]  
 Print the values stored in S  
 S = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]  
 The sum of S[0..3] is 10  
 Let x be the smallest index that satisfies psum(x) >= 10. Then, x = 3  
 Let y be the smallest index that satisfies psum(y) >= 1000. Then, y =-1  
 S[3] = 4  
 S[3] += 10  
 S[3] = 14  
 S[3] -= 10  
 S[3] = 4  
 Insert 100 into S at position 4  
 S = [1, 2, 3, 4, 100, 5, 6, 7, 8, 9, 10]  
 Delete S[4] from S  
 S = [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]  
 Add 0 to the tail of S  
 Add 9 to the head of S  
 S = [9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0]  
 Add 1, 2, 3, 4 to the tail of S  
 S = [9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 1, 2, 3, 4]  
 Write S to S.bin  
 Remove all values from S  
 S = []  
 Read S from S.bin  
 S = [9, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 0, 1, 2, 3, 4]
```

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

#### Example

An example usage of DynamicBitSequence is provided in [dynamic_bit_example.cpp](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/examples/dynamic_bit_example.cpp).
When this example is executed, the following message is displayed:

```
% ./dynamic_bit_example.out  
  
 Build data structure S from bit sequence [1, 0, 1, 0, 1, 0, 1, 0, 1, 1]  
 Print the bits stored in S  
 S = [1010101011]  
 The number of 1 in S[0..3] is 2  
 The number of 0 in S[0..3] is 2  
 The position of the third 1 in S is 4  
 The position of the third 0 in S is 5  
 The position of the fifth 0 in S is -1  
 Insert 1 into S at position 4  
 S = [10101101011]  
 Delete S[4] from S  
 S = [1010101011]  
 Add 0 to the tail of S  
 Add 1 to the head of S  
 S = [110101010110]  
 Add 1010 to the tail of S  
 S = [1101010101101010]  
 Write S to S.bin  
 Remove all values from S  
 S = []  
 Read S from S.bin  
 S = [1101010101101010]  
```

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

#### Example

An example usage of DynamicSequence64 is provided in [dynamic_sequence_64_example.cpp](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/examples/dynamic_sequence_64_example.cpp).  
When this example is executed, the following message is displayed:

```
 ./dynamic_sequence_64_example.out  
 
 Build DynamicSequence64 S from integer sequence [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]  
 Print the integers stored in S  
 S = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]  
 Insert 1 into S at position 4  
 S = [10, 20, 30, 40, 1, 50, 60, 70, 80, 90, 100]  
 Delete S[4] from S  
 S = [10, 20, 30, 40, 50, 60, 70, 80, 90, 100]  
 Add 0 to the tail of S  
 Add 1 to the head of S  
 S = [1, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 0]  
 Add 1, 2, 3, 4 to the tail of S  
 S = [1, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 0, 1, 2, 3, 4]  
 Write S to S.bin  
 Remove all values from S  
 S = []  
 Read S from S.bin  
 S = [1, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 0, 1, 2, 3, 4]  
```


### DynamicWaveletTree class 

[DynamicWaveletTree](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/include/sequence/dynamic_wavelet_tree.hpp) is a dynamic version of [Wavelet Tree](https://en.wikipedia.org/wiki/Wavelet_Tree "Wavelet Tree").

#### Table for update operations and queries

| Category         | Name              | Order                         | Description                                                   |
|------------------|-------------------|-------------------------------|---------------------------------------------------------------|
| Memory           |                   | O(n log σ) bytes              |                                                               |
| Update Operation | S.insert(i, v)    | amortized O(log σ log n) time | Insert v into S as the i-th character                         |
|                  | S.remove(i)       | amortized O(log σ log n) time | Remove the i-th character from S                              |
|                  | S.push_back(v)    | amortized O(log σ log n) time | Add v to S as the last character                              |
|                  | S.push_many(i, P) | amortized O(log σ log n) time | Add the characters in sequence P to S as the last characters  |
| Query            | S.at(i)           | O(log σ log n) time           | Return S[i]                                                   |
|                  | S.rank(i, c)      | O(log σ log n) time           | Return the number of c in S[0..i]                             |
|                  | S.select(i, c)    | O(log σ log n) time           | Return the position of the (i+1)-th c in S                    |

Here, S is a string stored in DynamicWaveletTree; n is the length of S; σ is the the alphabet size of S.  
See [this page](https://tnishimoto.github.io/b_tree_plus_alpha/html/classstool_1_1sequence_1_1_dynamic_wavelet_tree.html) for the member functions supported by DynamicWaveletTree.

#### Example

An example usage of DynamicWaveletTree is provided in [dynamic_wavelet_tree_example.cpp](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/examples/dynamic_wavelet_tree_example.cpp).  
When this example is executed, the following message is displayed:

```
% ./dynamic_wavelet_tree_example.out  
  
 Print the integers stored in S  
 S = ababababab  
 The number of a in S[0..3] is 2  
 The number of b in S[0..3] is 2  
 The position of the third a in S is 4  
 The position of the third b in S is 5  
 The position of the 10-th b in S is -1  
 The position of the first c in S is -1  
 Insert c into S at position 4  
 S = ababcababab  
 Delete S[4] from S  
 S = ababababab  
 Add c to the tail of S  
 S = abababababc  
 Add bbbb to the tail of S  
 S = abababababcbbbb  
 Write S to S.bin  
 Remove all values from S  
 S =   
 Read S from S.bin  
 S = abababababcbbbb  
```

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

Here, S is a permutation of n integers 0, 1, ..., n-1 stored in DynamicPermutation.  
See [this page](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/include/permutation/dynamic_permutation.hpp) for the member functions supported by DynamicPermutation.

#### Example

An example usage of DynamicPermutation is provided in [dynamic_permutation_example.cpp](https://github.com/TNishimoto/b_tree_plus_alpha/blob/main/examples/dynamic_permutation_example.cpp).  
When this example is executed, the following message is displayed:

```
% ./dynamic_permutation_example.out  
  
 Build DynamicPermutation S from permutation [0, 3, 1, 2, 5, 4, 9, 8, 7, 6]  
 Constructing Dynamic Permutation... (input size = 10)  
   Processing... [0KB]   
 [END], the number of nodes = 1, Elapsed Time: 0 sec (0 ms/MB)  
 Print the integers stored in S  
 S = [0, 3, 1, 2, 5, 4, 9, 8, 7, 6]  
 S[3] = 2  
 S^{-1}[3] = 1  
 Insert 2 into S at position 1  
 S = [0, 2, 4, 1, 3, 6, 5, 10, 9, 8, 7]  
 Delete S[4] from S  
 S = [0, 2, 3, 1, 5, 4, 9, 8, 7, 6]  
 Move S[1] in S to position 0  
 S = [2, 0, 3, 1, 5, 4, 9, 8, 7, 6]  
 Write S to S.bin  
 Remove all values from S  
 S = []  
 Read S from S.bin  
 S = [2, 0, 3, 1, 5, 4, 9, 8, 7, 6]  
```

## API Documentation (in preparation)

[Doxygen](https://TNishimoto.github.io/b_tree_plus_alpha/html/index.html)

