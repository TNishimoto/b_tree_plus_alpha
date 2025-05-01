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

VLCDequeSPSI is a dynamic [prefix-sum](https://en.wikipedia.org/wiki/Prefix_sum "prefix-sum") data structure. 

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

See also XXX.