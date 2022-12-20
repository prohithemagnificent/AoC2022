#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include <string>
#include <regex>
#include <deque>
#include <chrono>
#include <numeric>
#include <set>

struct perry{
    long long int index;
    long long int value;

    // constructor
    perry(long long int idx, long long int val){
        index = idx;
        value = val;
    }
};

void addNToArrayBetween2indices(std::vector<perry>& array, long long int n, long long int i_start, long long int i_end){
    // including i_start anmd i_end, add to array[i].index
    auto it = array.begin();
    while (it != array.end()){
        long long int idx = (*it).index;
        if (idx >= i_start && idx <= i_end){
            (*it).index += n;
        }
        it++;
    }
}



int main(){

    auto startTime = std::chrono::steady_clock::now();

    const long long int decription_key = 811589153;
    std::vector<long long int> v;
    std::ifstream file("input.txt");
    if (file.is_open()){
        std::string line; 
        while (std::getline(file, line)){
            v.push_back(decription_key * std::stoi(line));
        } 
    }

    //std::set<long long int> s {v.begin(), v.end()};  // set size not same as v size -> there are multiples of some elements

    const long long int N = v.size();
    // vector of indices and values
    std::vector<perry> array;
    for (long long int i = 0; i < N; i++){
        perry currP = perry(i, v[i]);
        array.push_back(currP);
    }
    
    for (long long int p2 = 0; p2 < 10; p2++){
        std::cout << "p2 n of cycle: " << p2 << std::endl;
        for (long long int i = 0; i < N; i++){
            std::cout << "p2: " << p2 << ", i: " << i << ", of N: " << N << std::endl;
            long long int idx_from = array[i].index;
            long long int val = array[i].value;

            
            long long int newpos;

            if (val > 0){
                newpos = idx_from + val;
                long long int np = newpos / N;
                newpos = newpos - np*N + np;
                while (newpos >= N){
                    newpos -= N;
                    newpos++;
                }
            }else{  // val < 0
                newpos = idx_from + val;
                long long int np = newpos / N;
                newpos = newpos - np*N + np;
                while (newpos < 0){
                    newpos += N;
                    newpos--;
                }
            }
            long long int diff = newpos - idx_from;

            if (val == 0){
                // nothing
            }else if (diff > 0){
                
                addNToArrayBetween2indices(array, -1, idx_from + 1, idx_from + diff);
                array[i].index  += diff;
            }else{   // diff < 0
                
                addNToArrayBetween2indices(array, +1, idx_from + diff, idx_from - 1);
                array[i].index  += diff;
            }

        }
    }

    long long int res = 0;
    long long int idx_of_0 = -1;
    for (long long int i = 0; i < N; i++){
        if (array[i].value == 0){
            idx_of_0 = array[i].index;
            break;
        }
    }

    for (long long int i = 0; i < N; i++){
        if (array[i].index == (idx_of_0 + 1000) % N){
            res += array[i].value;
        }
        if (array[i].index == (idx_of_0 + 2000) % N){
            res += array[i].value;
        }
        if (array[i].index == (idx_of_0 + 3000) % N){
            res += array[i].value;
        }
    }



    std::cout << res << std::endl; 


    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    std::cout << "Elapsed time (ms)= " << duration << std::endl;
}

/*
8798438007673  <- OK
Elapsed time (ms)= 87 653   ( = 87 s)
*/



/*
--- Day 20: Grove Positioning System ---
It's finally time to meet back up with the Elves. When you try to contact them, however, you get no reply. Perhaps you're out of range?

You know they're headed to the grove where the star fruit grows, so if you can figure out where that is, you should be able to meet back up with them.

Fortunately, your handheld device has a file (your puzzle input) that contains the grove's coordinates! Unfortunately, the file is encrypted - just in case the device were to fall into the wrong hands.

Maybe you can decrypt it?

When you were still back at the camp, you overheard some Elves talking about coordinate file encryption. The main operation involved in decrypting the file is called mixing.

The encrypted file is a list of numbers. To mix the file, move each number forward or backward in the file a number of positions equal to the value of the number being moved. The list is circular, so moving a number off one end of the list wraps back around to the other end as if the ends were connected.

For example, to move the 1 in a sequence like 4, 5, 6, 1, 7, 8, 9, the 1 moves one position forward: 4, 5, 6, 7, 1, 8, 9. To move the -2 in a sequence like 4, -2, 5, 6, 7, 8, 9, the -2 moves two positions backward, wrapping around: 4, 5, 6, 7, 8, -2, 9.

The numbers should be moved in the order they originally appear in the encrypted file. Numbers moving around during the mixing process do not change the order in which the numbers are moved.

Consider this encrypted file:

1
2
-3
3
-2
0
4
Mixing this file proceeds as follows:

Initial arrangement:
1, 2, -3, 3, -2, 0, 4

1 moves between 2 and -3:
2, 1, -3, 3, -2, 0, 4

2 moves between -3 and 3:
1, -3, 2, 3, -2, 0, 4

-3 moves between -2 and 0:
1, 2, 3, -2, -3, 0, 4

3 moves between 0 and 4:
1, 2, -2, -3, 0, 3, 4

-2 moves between 4 and 1:
1, 2, -3, 0, 3, 4, -2

0 does not move:
1, 2, -3, 0, 3, 4, -2

4 moves between -3 and 0:
1, 2, -3, 4, 0, 3, -2
Then, the grove coordinates can be found by looking at the 1000th, 2000th, and 3000th numbers after the value 0, wrapping around the list as necessary. In the above example, the 1000th number after 0 is 4, the 2000th is -3, and the 3000th is 2; adding these together produces 3.

Mix your encrypted file exactly once. What is the sum of the three numbers that form the grove coordinates?

Your puzzle answer was 8028.

--- Part Two ---
The grove coordinate values seem nonsensical. While you ponder the mysteries of Elf encryption, you suddenly remember the rest of the decryption routine you overheard back at camp.

First, you need to apply the decryption key, 811589153. Multiply each number by the decryption key before you begin; this will produce the actual list of numbers to mix.

Second, you need to mix the list of numbers ten times. The order in which the numbers are mixed does not change during mixing; the numbers are still moved in the order they appeared in the original, pre-mixed list. (So, if -3 appears fourth in the original list of numbers to mix, -3 will be the fourth number to move during each round of mixing.)

Using the same example as above:

Initial arrangement:
811589153, 1623178306, -2434767459, 2434767459, -1623178306, 0, 3246356612

After 1 round of mixing:
0, -2434767459, 3246356612, -1623178306, 2434767459, 1623178306, 811589153

After 2 rounds of mixing:
0, 2434767459, 1623178306, 3246356612, -2434767459, -1623178306, 811589153

After 3 rounds of mixing:
0, 811589153, 2434767459, 3246356612, 1623178306, -1623178306, -2434767459

After 4 rounds of mixing:
0, 1623178306, -2434767459, 811589153, 2434767459, 3246356612, -1623178306

After 5 rounds of mixing:
0, 811589153, -1623178306, 1623178306, -2434767459, 3246356612, 2434767459

After 6 rounds of mixing:
0, 811589153, -1623178306, 3246356612, -2434767459, 1623178306, 2434767459

After 7 rounds of mixing:
0, -2434767459, 2434767459, 1623178306, -1623178306, 811589153, 3246356612

After 8 rounds of mixing:
0, 1623178306, 3246356612, 811589153, -2434767459, 2434767459, -1623178306

After 9 rounds of mixing:
0, 811589153, 1623178306, -2434767459, 3246356612, 2434767459, -1623178306

After 10 rounds of mixing:
0, -2434767459, 1623178306, 3246356612, -1623178306, 2434767459, 811589153
The grove coordinates can still be found in the same way. Here, the 1000th number after 0 is 811589153, the 2000th is 2434767459, and the 3000th is -1623178306; adding these together produces 1623178306.

Apply the decryption key and mix your encrypted file ten times. What is the sum of the three numbers that form the grove coordinates?

Your puzzle answer was 8798438007673.

Both parts of this puzzle are complete! They provide two gold stars: **

*/