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
    int index;
    int value;

    // constructor
    perry(int idx, int val){
        index = idx;
        value = val;
    }
};

void addNToArrayBetween2indices(std::vector<perry>& array, int n, int i_start, int i_end){
    // including i_start anmd i_end, add to array[i].index
    auto it = array.begin();
    while (it != array.end()){
        int idx = (*it).index;
        if (idx >= i_start && idx <= i_end){
            (*it).index += n;
        }
        it++;
    }
}



int main(){

    auto startTime = std::chrono::steady_clock::now();

    std::vector<int> v;
    std::ifstream file("input.txt");
    if (file.is_open()){
        std::string line; 
        while (std::getline(file, line)){
            v.push_back(std::stoi(line));
        } 
    }

    //std::set<int> s {v.begin(), v.end()};  // set size not same as v size -> there are multiples of some elements

    const int N = v.size();
    // vector of indices and values
    std::vector<perry> array;
    for (int i = 0; i < N; i++){
        perry currP = perry(i, v[i]);
        array.push_back(currP);
    }
    

    for (int i = 0; i < N; i++){
        int idx_from = array[i].index;
        int val = array[i].value;

        
        int newpos;

        if (val > 0){
            newpos = idx_from + val;
            while (newpos >= N){
                newpos -= N;
                newpos++;
            }
        }else{  // val < 0
            newpos = idx_from + val;
            while (newpos < 0){
                newpos += N;
                newpos--;
            }
        }
        int diff = newpos - idx_from;

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
    

    int res = 0;
    int idx_of_0 = -1;
    for (int i = 0; i < N; i++){
        if (array[i].value == 0){
            idx_of_0 = array[i].index;
            break;
        }
    }

    for (int i = 0; i < N; i++){
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



    std::cout << res << std::endl; // 10649 toohigh


    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    std::cout << "Elapsed time (ms)= " << duration << std::endl;
}



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

The first half of this puzzle is complete! It provides one gold star: *
*/