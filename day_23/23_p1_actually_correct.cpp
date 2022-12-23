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


#define SIZE_OF_SQUARE_INPUT 72

#define SIZE_OF_MAP 122  // input will be offset by 20,20

struct pos{
    int row;
    int col;
};


pos proposeNewPos(const std::vector<std::vector<bool>>& map, const pos original_pos, int original_dir){
    // i % 4 = 0 : north, 1 : south, 2 : west, 3 : east
    int original_row = original_pos.row;
    int original_col = original_pos.col;

    // return can be the same
    int proposed_row = original_row;
    int proposed_col = original_col;

    for (int i = original_dir; i < original_dir + 4; i++){
        if (i % 4 == 0){
            // north
            bool canmove = true;
            for (int ofs : {-1,0,1}){
                if (map[original_row - 1][original_col + ofs]){
                    // if there is a neighbor
                    canmove = false;
                }
            }
            if (canmove){
                proposed_row = original_row - 1;
                proposed_col = original_col;
                break;
            }
        }else if (i % 4 == 1){
            // south
            bool canmove = true;
            for (int ofs : {-1,0,1}){
                if (map[original_row + 1][original_col + ofs]){
                    // if there is a neighbor
                    canmove = false;
                }
            }
            if (canmove){
                proposed_row = original_row + 1;
                proposed_col = original_col;
                break;
            }
        }else if (i % 4 == 2){
            // west
            bool canmove = true;
            for (int ofs : {-1,0,1}){
                if (map[original_row + ofs][original_col - 1]){
                    // if there is a neighbor
                    canmove = false;
                }
            }
            if (canmove){
                proposed_row = original_row ;
                proposed_col = original_col - 1;
                break;
            }
        }else if (i % 4 == 3){
            // east
            bool canmove = true;
            for (int ofs : {-1,0,1}){
                if (map[original_row + ofs][original_col + 1]){
                    // if there is a neighbor
                    canmove = false;
                }
            }
            if (canmove){
                proposed_row = original_row ;
                proposed_col = original_col + 1;
                break;
            }
        }

    }

    return {proposed_row, proposed_col};
}

// T must be printable with cout
template <typename T>
void printVecofVec(const std::vector<std::vector<T>>& map){
    for (auto row : map){
        for (T el : row){
            //std::cout << el;
            if (el){
                std::cout << '#';
            }else{
                std::cout << '.';
            }
            
        }
        std::cout << std::endl;
    }

}



int main(){

    auto startTime = std::chrono::steady_clock::now();

    std::vector<std::string> v;
    std::ifstream file("input.txt");
    if (file.is_open()){
        std::string line; 
        while (std::getline(file, line)){
            v.push_back(line);
        } 
    }

    // # -> elf == ture
    // . -> space == false
    const int size_of_map = SIZE_OF_MAP;
    std::vector<std::vector<bool>> map(size_of_map, std::vector<bool>(size_of_map));

    // fill map with false
    for (auto& vec : map){
        for (auto& b : vec){
            b = false;
        }    
    }

    /*printVecofVec<bool>(map);
    std::cout << std::endl << std::endl << std::endl;*/

    const int offset = 20;
    for (int row = 0; row < v.size(); row++){
        for (int col = 0; col < v[0].size(); col++){
            if (v[row][col] == '#'){
                map[row + offset][col + offset] = true;
            }
        }
    }


    /*std::cout << "before starting" << std::endl;
    printVecofVec<bool>(map);
    std::cout << std::endl << std::endl << std::endl;*/

    std::vector<std::vector<int>> proposed_map_counts_INIT (size_of_map, std::vector<int>(size_of_map));
    for (auto& vec : proposed_map_counts_INIT){
        for (auto& n : vec){
            n = 0;
        }    
    }



    for (int i = 0; i < 10; i++){
        // i % 4 = 0 : north, 1 : south, 2 : west, 3 : east
        // 10 moves like this

        // counts map of proposed solutions, always filled with 0 at the beginning of round (cheap copy constructor)
        std::vector<std::vector<int>> proposed_map_counts = proposed_map_counts_INIT;


         std::vector<std::vector<bool>> new_map = map;

        // go through each elf and create proposition map
        for (int row = 0; row < size_of_map; row++){
            for (int col = 0; col < size_of_map; col++){
                if (map[row][col]){
                    // there is an elf here

                    // ---- STEP 1 ----
                    // create propositions

                    // OPTION 1: no neighbors -> don't move at all
                    bool thereareneighbors = false;
                    for (int r_ofs : {-1,0,1}){
                        for (int c_ofs : {-1,0,1}){
                            if (r_ofs == 0 && c_ofs == 0){
                                // nothing
                            }else{
                                if (map[row + r_ofs][col + c_ofs]){
                                    thereareneighbors = true;
                                }
                            }
                        }
                    }

                    if (!thereareneighbors){
                        // nothing
                    }else{
                        // OPTION 2: there are neighbors -> try to move
                        //  1. fill proposed map 
                        pos proposition = proposeNewPos(map, {row, col}, i);
                        proposed_map_counts[proposition.row][proposition.col]++; 
                    }                   

                } // if elf in map
            }  // for col
        }  // for row


        // go through each elf again and move if possible
        for (int row = 0; row < size_of_map; row++){
            for (int col = 0; col < size_of_map; col++){
                if (map[row][col]){
                    // there is an elf here


                    // OPTION 1: no neighbors -> don't move at all
                    bool thereareneighbors = false;
                    for (int r_ofs : {-1,0,1}){
                        for (int c_ofs : {-1,0,1}){
                            if (r_ofs == 0 && c_ofs == 0){
                                // nothing
                            }else{
                                if (map[row + r_ofs][col + c_ofs]){
                                    thereareneighbors = true;
                                }
                            }
                        }
                    }

                    if (!thereareneighbors){
                        // nothing
                    }else{
                        // ---- STEP 2 ----
                        // move to propositions if possible
                        pos proposition = proposeNewPos(map, {row, col}, i);
                        if (proposed_map_counts[proposition.row][proposition.col] == 1 && (proposition.row != row || proposition.col != col) ){

                            // we can move (only move if we actually would move)
                            new_map[row][col] = false;  // remove old
                            new_map[proposition.row][proposition.col] = true;  // add new
                            
                        } 
                        
                    }                   

                } // if elf in map
            }  // for col
        }  // for row
        
        map = new_map;
        std::cout << "at round i = "  << i << std::endl;
        /*printVecofVec<bool>(map);
        std::cout << std::endl << std::endl << std::endl;*/
    }  // for i (10 steps)


    int min_row = -1;
    int max_row = -1;
    int min_col = -1;
    int max_col = -1;

    for (int r = 0; r < size_of_map; r++){
        for (int c = 0; c < size_of_map; c++){
            if (map[r][c]){
                // found element
                min_row = r;
                goto MIN_ROW_FOUND;
            }
        }
    }
MIN_ROW_FOUND:
    for (int r = size_of_map - 1; r >= 0 ; r--){
        for (int c = 0; c < size_of_map; c++){
            if (map[r][c]){
                // found element
                max_row = r;
                goto MAX_ROW_FOUND;
            }
        }
    }
MAX_ROW_FOUND:
    for (int c = 0; c < size_of_map; c++){
        for (int r = 0; r < size_of_map; r++){
            if (map[r][c]){
                // found element
                min_col = c;
                goto MIN_COL_FOUND;
            }
        }
    }
MIN_COL_FOUND:
    for (int c = size_of_map - 1; c > 0 ; c--){
        for (int r = 0; r < size_of_map; r++){
            if (map[r][c]){
                // found element
                max_col = c;
                goto MAX_COL_FOUND;
            }
        }
    }
MAX_COL_FOUND:

    int count = 0;
    for (int r = min_row; r <= max_row; r++){
        for (int c = min_col; c <= max_col; c++){
            if (!map[r][c]){
                count++;
            }
        }
    }

    std::cout << "count of empty space: " << count << std::endl;  


    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    std::cout << "Elapsed time (ms)= " << duration << std::endl;



}



/*
--- Day 23: Unstable Diffusion ---
You enter a large crater of gray dirt where the grove is supposed to be. All around you, plants you imagine were expected to be full of fruit are instead withered and broken. A large group of Elves has formed in the middle of the grove.

"...but this volcano has been dormant for months. Without ash, the fruit can't grow!"

You look up to see a massive, snow-capped mountain towering above you.

"It's not like there are other active volcanoes here; we've looked everywhere."

"But our scanners show active magma flows; clearly it's going somewhere."

They finally notice you at the edge of the grove, your pack almost overflowing from the random star fruit you've been collecting. Behind you, elephants and monkeys explore the grove, looking concerned. Then, the Elves recognize the ash cloud slowly spreading above your recent detour.

"Why do you--" "How is--" "Did you just--"

Before any of them can form a complete question, another Elf speaks up: "Okay, new plan. We have almost enough fruit already, and ash from the plume should spread here eventually. If we quickly plant new seedlings now, we can still make it to the extraction point. Spread out!"

The Elves each reach into their pack and pull out a tiny plant. The plants rely on important nutrients from the ash, so they can't be planted too close together.

There isn't enough time to let the Elves figure out where to plant the seedlings themselves; you quickly scan the grove (your puzzle input) and note their positions.

For example:

....#..
..###.#
#...#.#
.#...##
#.###..
##.#.##
.#..#..
The scan shows Elves # and empty ground .; outside your scan, more empty ground extends a long way in every direction. The scan is oriented so that north is up; orthogonal directions are written N (north), S (south), W (west), and E (east), while diagonal directions are written NE, NW, SE, SW.

The Elves follow a time-consuming process to figure out where they should each go; you can speed up this process considerably. The process consists of some number of rounds during which Elves alternate between considering where to move and actually moving.

During the first half of each round, each Elf considers the eight positions adjacent to themself. If no other Elves are in one of those eight positions, the Elf does not do anything during this round. Otherwise, the Elf looks in each of four directions in the following order and proposes moving one step in the first valid direction:

If there is no Elf in the N, NE, or NW adjacent positions, the Elf proposes moving north one step.
If there is no Elf in the S, SE, or SW adjacent positions, the Elf proposes moving south one step.
If there is no Elf in the W, NW, or SW adjacent positions, the Elf proposes moving west one step.
If there is no Elf in the E, NE, or SE adjacent positions, the Elf proposes moving east one step.
After each Elf has had a chance to propose a move, the second half of the round can begin. Simultaneously, each Elf moves to their proposed destination tile if they were the only Elf to propose moving to that position. If two or more Elves propose moving to the same position, none of those Elves move.

Finally, at the end of the round, the first direction the Elves considered is moved to the end of the list of directions. For example, during the second round, the Elves would try proposing a move to the south first, then west, then east, then north. On the third round, the Elves would first consider west, then east, then north, then south.

As a smaller example, consider just these five Elves:

.....
..##.
..#..
.....
..##.
.....
The northernmost two Elves and southernmost two Elves all propose moving north, while the middle Elf cannot move north and proposes moving south. The middle Elf proposes the same destination as the southwest Elf, so neither of them move, but the other three do:

..##.
.....
..#..
...#.
..#..
.....
Next, the northernmost two Elves and the southernmost Elf all propose moving south. Of the remaining middle two Elves, the west one cannot move south and proposes moving west, while the east one cannot move south or west and proposes moving east. All five Elves succeed in moving to their proposed positions:

.....
..##.
.#...
....#
.....
..#..
Finally, the southernmost two Elves choose not to move at all. Of the remaining three Elves, the west one proposes moving west, the east one proposes moving east, and the middle one proposes moving north; all three succeed in moving:

..#..
....#
#....
....#
.....
..#..
At this point, no Elves need to move, and so the process ends.

The larger example above proceeds as follows:

== Initial State ==
..............
..............
.......#......
.....###.#....
...#...#.#....
....#...##....
...#.###......
...##.#.##....
....#..#......
..............
..............
..............

== End of Round 1 ==
..............
.......#......
.....#...#....
...#..#.#.....
.......#..#...
....#.#.##....
..#..#.#......
..#.#.#.##....
..............
....#..#......
..............
..............

== End of Round 2 ==
..............
.......#......
....#.....#...
...#..#.#.....
.......#...#..
...#..#.#.....
.#...#.#.#....
..............
..#.#.#.##....
....#..#......
..............
..............

== End of Round 3 ==
..............
.......#......
.....#....#...
..#..#...#....
.......#...#..
...#..#.#.....
.#..#.....#...
.......##.....
..##.#....#...
...#..........
.......#......
..............

== End of Round 4 ==
..............
.......#......
......#....#..
..#...##......
...#.....#.#..
.........#....
.#...###..#...
..#......#....
....##....#...
....#.........
.......#......
..............

== End of Round 5 ==
.......#......
..............
..#..#.....#..
.........#....
......##...#..
.#.#.####.....
...........#..
....##..#.....
..#...........
..........#...
....#..#......
..............
After a few more rounds...

== End of Round 10 ==
.......#......
...........#..
..#.#..#......
......#.......
...#.....#..#.
.#......##....
.....##.......
..#........#..
....#.#..#....
..............
....#..#..#...
..............
To make sure they're on the right track, the Elves like to check after round 10 that they're making good progress toward covering enough ground. To do this, count the number of empty ground tiles contained by the smallest rectangle that contains every Elf. (The edges of the rectangle should be aligned to the N/S/E/W directions; the Elves do not have the patience to calculate arbitrary rectangles.) In the above example, that rectangle is:

......#.....
..........#.
.#.#..#.....
.....#......
..#.....#..#
#......##...
....##......
.#........#.
...#.#..#...
............
...#..#..#..
In this region, the number of empty ground tiles is 110.

Simulate the Elves' process and find the smallest rectangle that contains the Elves after 10 rounds. How many empty ground tiles does that rectangle contain?

Your puzzle answer was 3882.

The first half of this puzzle is complete! It provides one gold star: *

*/