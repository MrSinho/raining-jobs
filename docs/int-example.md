# Raining jobs example with integers

Given the following set of unsigned integers, with a resolution of 8 bits, we can set the numbers in descending order using bitwise operations only:

 * `7`
 * `49`
 * `-113`
 * `-128`
 * `98`

The source buffer length must be double of the length of the numbers to compare. There has to be also a separate destination buffer.

```python
----------> first half of source buffer, containing inputs
    [0] 7
    [1] 49
    [2] -113
    [3] -128
    [4] 98
----------> second half of source buffer (required)
    [5] 0
    [6] 0
    [7] 0
    [8] 0
    [9] 0
----------> destination buffer (required)
    [0] 0
    [1] 0
    [2] 0
    [3] 0
    [4] 0
----------
```

It's going to be very useful to keep in mind the binary representation of these numbers:

|decimal | binary                          |
|--------|---------------------------------|
|`7`     |`0b00000111`                     |
|`49`    |`0b00110001`                     |
|`-113`  |`0b10001111` (second complement) |
|`-128`  |`0b10000000` (second complement) |

```python
0
        7  --&--( 1 << 7 )--┐ = 0                          ┍--> -128  --&--( 1 << 5 )--┐ = 0                          ┍-----> -128  --&--( 1 << 3 )--┐ = 0
1                           |                              |                           |                              |                              |
       49  --&--( 1 << 7 )--┤ = 0                          ├--> -113  --&--( 1 << 5 )--┤ = 0                          ├-----> -113  --&--( 1 << 3 )--┤ = ( 1 << 3 )
2                           |                              |                           |                              |                              |
     -113  --&--( 1 << 7 )--┤ = ( 1 << 7 )                 ├-->   98  ---------OK------┤                              ├----->   98  ---------OK------┤
3                           |                              |                           |                              |                              |
     -128  --&--( 1 << 7 )--┤ = ( 1 << 7 )                 ├-->    7  --&--( 1 << 5 )--┤ = 0                          ├----->   49  ---------OK------┤
4                           |                              |                           |                              |                              |
       98  --&--( 1 << 7 )--┤ = 0                          ├-->   49  --&--( 1 << 5 )--┤ = ( 1 << 5 )                 ├----->    7  ---------OK------┤                                 ┍------------------------------┐
                            |                              |                           |                              |                              |                                 |                              |
                            |                              |                           |                              |                              | COPY TO DESTINATION MEMORY      |  ┍------------------------┐  |
5                           |                              |                           |                              |                              |                                 |  |                        |  |
                            ├-----> -113  --&--( 1 << 6 )--┤ = 0                       ├-----> -113  --&--( 1 << 4 )--┤ = 0                          ├-----> -113  ---------OK------┬--┙  |  ┍-------->  98        |  |
6                           |                              |                           |                              |                              |                              |     |  |                     |  |
                            ├-----> -128  --&--( 1 << 6 )--┤ = 0                       ├-----> -128  --&--( 1 << 4 )--┤ = 0                          ├-----> -128  ---------OK------┼-----┙  |  ┍----->  49        |  |
7                           |                              |                           |                              |                              |                              |        |  |                  |  |
                            ├----->   98  --&--( 1 << 6 )--┤ = (1 << 6)                ├----->   98  ---------OK------┤                              ├----->   98  ---------OK------┼--------┙  |  ┍-->   7        |  |
8                           |                              |                           |                              |                              |                              |           |  |               |  |
                            ├----->   49  --&--( 1 << 6 )--┤ = 0                       ├----->   49  ---------OK------┤                              ├----->   49  ---------OK------┼-----------┙  |    -113 <-----|--┙
9                           |                              |                           |                              |                              |                              |              |               |   
                            ┕----->    7  --&--( 1 << 6 )--┙ = 0                       ┕----->    7  ---------OK------┙                              ┕----->    7  ---------OK------┷--------------┙    -128 <-----┙   
```

The program begins to check if the most significant bit (`1 << 7`) is "ON", until we reach the least significant bits, if necessary.

`-113` and `-128` have the `bit 7` (first bit is `bit 0`) `ON` (which is also the sign bit for 8 bit signed integers), so they will be copied in the same order they appear from the first half of the source buffer, to the next half, starting from index `5` (the left side) to `6` (since they are two numbers), whereas the numbers `7`, `49` and `98` will be copied to the next next half of the source buffer starting from index `9` (right side) to `7`.

The comparison of the numbers began with a single workgroup (function is `rainingWorkGroup`), but after checking for the 7th bit we created a "left side" of memory, made of numbers which always have a higher _absolute binary value_, not decimal, since they have the most significant bits:

|decimal | binary      |
|--------|-------------|
|`7`     |0b`0`0000111 |
|`49`    |0b`0`0110001 |
|`-113`  |0b`1`0001111 |
|`-128`  |0b`1`0000000 |

| left side       | right side       |
|-----------------|------------------|
| `-113`, `-128`  | `7`, `49`, `98`  |

```python
0
        7  --&--( 1 << 7 )--┐ = 0                                        ┍--------------> ...
1                           |                                            |               
       49  --&--( 1 << 7 )--┤ = 0                                        |  ┍-----------> ...
2                           |                                            |  |            
     -113  --&--( 1 << 7 )--┤ = ( 1 << 7 ) --> move to left workgroup    |  |  ┍--------> ...
3                           |                                            |  |  |         
     -128  --&--( 1 << 7 )--┤ = ( 1 << 7 ) --> move to left workgroup    |  |  |  ┍-----> ...
4                           |                                            |  |  |  |      
       98  --&--( 1 << 7 )--┤ = 0                                        |  |  |  |  ┍--> ...
                            |                                            |  |  |  |  |
                            |                                            |  |  |  |  |
5                           |                                            |  |  |  |  |
                            ├-----> -113  -----┐                      ┍--┙  |  |  |  |
6                           |                  |--> LEFT  workgroup --┤     |  |  |  |
                            ├-----> -128  -----┙                      ┕-----┙  |  |  |
7                           |                                                  |  |  |
                            ├----->   98  -----┐                      ┍--------┙  |  |
8                           |                  |                      |           |  |
                            ├----->   49       |--> RIGHT workgroup --┼-----------┙  |
9                           |                  |                      |              |
                            ┕----->    7  -----┙                      ┕--------------┙
```

Now we need to compare  `-113` with `-128`, and separately, `7`, `49`, and `98`. So the program will submit a new workgroup with left values, and a workgroup with the values on the right.

We will eventually find out `-113` has a higher binary value than `-128` because it has `bit 3` `ON`. So now we can finally copy `-113` to the left side of the minor workgroup, copy to the destination memory and fix the `dst_index` at `src_index % src_original_length = src_index % 5`. The same process applies for `7`, `49`, and `98`, by creating smaller workgroups.

```python

0   ┍-----> -128  --&--( 1 << 3 )--┐ = 0             move to DESTINATION, index = 0 % 5
    |                              |
1   ├-----> -113  --&--( 1 << 3 )--┤ = ( 1 << 3 )    move to DESTINATION, index = 1 % 5
    |                              |
2   ├----->  ...  -----------------┤
    |                              |
3   ├----->  ...  -----------------┤
    |                              |
4   ├----->  ...  -----------------┤              
```

At the end we get the numbers ordered this way:

```
DESTINATION MEMORY

0
    -113
1 
    -128
2    
      98
3     
      49
4 
       7
```

Positive numbers are going to be shifted up by 2 places, while the negative numbers will be moved down, starting from index `3` (function `rainingSegregate`):

```
DESTINATION MEMORY

0     
      98
1 
      49
2    
       7
3     
    -113
4 
    -128
```

The same process is valid also with floating point numbers, (and numbers in general) of any type. The only extra process in the end is the separation of negative numbers from positive numbers.