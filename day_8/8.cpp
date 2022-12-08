#include <iostream>
#include <fstream>

#include <string>

#include <algorithm>

#include <vector>

    
    
int main(){
    
    std::vector<std::string> v;
    std::ifstream file("input_8.txt");
    if (file.is_open()){
        std::string line; 
        while (std::getline(file, line)){
            v.push_back(line);
        } 
    }


    std::vector<std::vector<int>> vint;

    for (auto line : v){
        std::vector<int> lineint;
        for (auto c : line){
            lineint.push_back(c - '0');
        }
        vint.push_back(lineint);
    }


    std::vector<std::vector<bool>> visible (v.size(), (std::vector<bool>(v[0].size(), false)));

    // left to right
    for (int i = 0; i < vint.size(); i++){
        int currMax = -1; // there are 0 big trees lol
        for (int j = 0; j < vint[i].size(); j++){
            if (vint[i][j] > currMax){
                visible[i][j] = true;
                currMax = vint[i][j];
            }
        }
    }


    // right to left
    for (int i = 0; i < vint.size(); i++){
        int currMax = -1; // there are 0 big trees lol
        for (int j = vint[i].size() - 1; j >= 0; j--){
            if (vint[i][j] > currMax){
                visible[i][j] = true;
                currMax = vint[i][j];
            }
        }
    }


    // top to bottom
    for (int j = 0; j < vint[0].size(); j++){
        int currMax = -1; // there are 0 big trees lol
        for (int i = 0; i < vint.size(); i++){
            if (vint[i][j] > currMax){
                visible[i][j] = true;
                currMax = vint[i][j];
            }
        }
    }


    // bottom to top
    for (int j = 0; j < vint[0].size(); j++){
        int currMax = -1; // there are 0 big trees lol
        for (int i = vint.size() - 1; i >= 0; i--){
            if (vint[i][j] > currMax){
                visible[i][j] = true;
                currMax = vint[i][j];
            }
        }
    }


    // count visibles
    int countOfVisible = 0;
    for (auto v1 : visible){
        for (bool b : v1){
            if (b){
                countOfVisible++;
            }
        }
    }
    // subtract sides:
    //countOfVisible = countOfVisible - v.size() * 2 - v[0].size() * 2 + 4;  // +4: 4 corners subtracted twice


    std::cout << countOfVisible << "\n";



    /*
    *
    *
    * 
    * -------------------
    * PART 2
    * -------------------
    * 
    * 
    * 
    * */

    // only check trees which are nto on the edge 
    int maxScenicScore = 0;
    for (int i = 1; i < vint.size() - 1; i++){
        for (int j = 1; j < vint[0].size() - 1; j++){
            int currentScore = 0;
            
            // look left
            int score_LEFT = 0;
            int idx = 1;
            bool toBreak = false;
            while (true){
                if ((i - idx) < 0 || toBreak){
                    break;
                }
                if (vint[i - idx][j] < vint[i][j]){
                    score_LEFT++;        
                }else{
                    score_LEFT++;
                    toBreak = true;                    
                }
                idx++;
            }
            // look right
            int score_RIGHT = 0;
            idx = 1;
            toBreak = false;
            while (true){
                if ((i + idx) >= vint.size() || toBreak){
                    break;
                }
                if (vint[i + idx][j] < vint[i][j]){
                    score_RIGHT++;
                }else{
                    score_RIGHT++;
                    toBreak = true;
                }
                idx++;
            }
            // look up
            int score_UP = 0;
            idx = 1;
            toBreak = false;
            while (true){
                if ((j - idx) < 0 || toBreak){
                    break;
                }
                if (vint[i][j-idx] < vint[i][j]){
                    score_UP++;
                }else{
                    score_UP++;
                    toBreak = true;
                }
                idx++;
            }
            // look down
            int score_DOWN = 0;
            idx = 1;
            toBreak = false;
            while (true){
                if ((j + idx) >= vint[0].size() || toBreak){
                    break;
                }
                if (vint[i][j+idx] < vint[i][j]){
                    score_DOWN++;
                }else{
                    score_DOWN++;
                    toBreak = true;
                }
                idx++;
            }
            //std::cout << "i j" << i <<" " << j << ", vint[i][j]: " << vint[i][j] << std::endl;
            currentScore = score_LEFT * score_UP * score_RIGHT * score_DOWN;
            maxScenicScore = std::max(maxScenicScore, currentScore);
        }

    }
    std::cout << "max scenic score: " << maxScenicScore << std::endl; 
}


/*
--- Day 8: Treetop Tree House ---
The expedition comes across a peculiar patch of tall trees all planted carefully in a grid. The Elves explain that a previous expedition planted these trees as a reforestation effort. Now, they're curious if this would be a good location for a tree house.

First, determine whether there is enough tree cover here to keep a tree house hidden. To do this, you need to count the number of trees that are visible from outside the grid when looking directly along a row or column.

The Elves have already launched a quadcopter to generate a map with the height of each tree (your puzzle input). For example:

30373
25512
65332
33549
35390
Each tree is represented as a single digit whose value is its height, where 0 is the shortest and 9 is the tallest.

A tree is visible if all of the other trees between it and an edge of the grid are shorter than it. Only consider trees in the same row or column; that is, only look up, down, left, or right from any given tree.

All of the trees around the edge of the grid are visible - since they are already on the edge, there are no trees to block the view. In this example, that only leaves the interior nine trees to consider:

The top-left 5 is visible from the left and top. (It isn't visible from the right or bottom since other trees of height 5 are in the way.)
The top-middle 5 is visible from the top and right.
The top-right 1 is not visible from any direction; for it to be visible, there would need to only be trees of height 0 between it and an edge.
The left-middle 5 is visible, but only from the right.
The center 3 is not visible from any direction; for it to be visible, there would need to be only trees of at most height 2 between it and an edge.
The right-middle 3 is visible from the right.
In the bottom row, the middle 5 is visible, but the 3 and 4 are not.
With 16 trees visible on the edge and another 5 visible in the interior, a total of 21 trees are visible in this arrangement.

Consider your map; how many trees are visible from outside the grid?

Your puzzle answer was 1782.

--- Part Two ---
Content with the amount of tree cover available, the Elves just need to know the best spot to build their tree house: they would like to be able to see a lot of trees.

To measure the viewing distance from a given tree, look up, down, left, and right from that tree; stop if you reach an edge or at the first tree that is the same height or taller than the tree under consideration. (If a tree is right on the edge, at least one of its viewing distances will be zero.)

The Elves don't care about distant trees taller than those found by the rules above; the proposed tree house has large eaves to keep it dry, so they wouldn't be able to see higher than the tree house anyway.

In the example above, consider the middle 5 in the second row:

30373
25512
65332
33549
35390
Looking up, its view is not blocked; it can see 1 tree (of height 3).
Looking left, its view is blocked immediately; it can see only 1 tree (of height 5, right next to it).
Looking right, its view is not blocked; it can see 2 trees.
Looking down, its view is blocked eventually; it can see 2 trees (one of height 3, then the tree of height 5 that blocks its view).
A tree's scenic score is found by multiplying together its viewing distance in each of the four directions. For this tree, this is 4 (found by multiplying 1 * 1 * 2 * 2).

However, you can do even better: consider the tree of height 5 in the middle of the fourth row:

30373
25512
65332
33549
35390
Looking up, its view is blocked at 2 trees (by another tree with a height of 5).
Looking left, its view is not blocked; it can see 2 trees.
Looking down, its view is also not blocked; it can see 1 tree.
Looking right, its view is blocked at 2 trees (by a massive tree of height 9).
This tree's scenic score is 8 (2 * 2 * 1 * 2); this is the ideal spot for the tree house.

Consider each tree on your map. What is the highest scenic score possible for any tree?

Your puzzle answer was 474606.

Both parts of this puzzle are complete! They provide two gold stars: **

At this point, you should return to your Advent calendar and try another puzzle.

If you still want to see it, you can get your puzzle input.

You can also [Share] this puzzle.

*/