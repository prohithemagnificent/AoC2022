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
#include <queue>




#define WIDTH 150
#define HEIGHT 200

struct coord{
    // row, col
    int row;
    int col;
};



enum direction { up, left, down, right };

void turn(direction& dir, int whichway){
    // side effect: changes dir

    // -1  -> R turn (clockwise)
    // -2  -> L turn (counter-clockwise)
    if (whichway == -1){
        // turn clockwise
        if (dir == up){
            dir = right;
        }else if(dir == left){
            dir = up;
        }else if (dir == down){
            dir = left;
        }else if (dir == right){
            dir = down;
        }   
    }else{
        // turn counter-clockwise
         if (dir == up){
            dir = left;
        }else if(dir == left){
            dir = down;
        }else if (dir == down){
            dir = right;
        }else if (dir == right){
            dir = up;
        }  

    }
    return;
}


void step(const std::vector<std::vector<char>>& map, coord& position,  const direction& dir, const int steps){
    // side effect: changes position
    for (int i = 0; i < steps; i++){
        coord new_pos_candidate = position;

        if (dir == up){
            new_pos_candidate.row--;
        }else if (dir == down){
            new_pos_candidate.row++;
        }else if (dir == left){
            new_pos_candidate.col--;
        }else{ // dir == right
            new_pos_candidate.col++;
        }

        bool weNeedToGoAround = false;
        // check if new position candidate is valid
        // map is indexed as [row][column]
        if (new_pos_candidate.row >= 0 && new_pos_candidate.row < HEIGHT &&
            new_pos_candidate.col >= 0 && new_pos_candidate.col < WIDTH){
            if (map[new_pos_candidate.row][new_pos_candidate.col] == '.'){
                // empty space, within bounds
                position = new_pos_candidate;
            }else if(map[new_pos_candidate.row][new_pos_candidate.col] == '#'){
                // wall, within bounds
                // -> nothing, no update, but we stop
                break;

            }else if(map[new_pos_candidate.row][new_pos_candidate.col] == 'T'){
                // out of map, within bounds
                weNeedToGoAround = true;
            }
        }else{
            // out of bounds
            weNeedToGoAround = true;
        }

        if (weNeedToGoAround){
            // try to go around, break if wall, newpos is on the other side if emptyspace
            // start from current position again
            new_pos_candidate = position;
            if (dir == up){
                // go down and find other edge
                while (new_pos_candidate.row < HEIGHT && map[new_pos_candidate.row][new_pos_candidate.col] != 'T'){
                    new_pos_candidate.row++;
                }
                // now we have arrived out of bounds or at the T
                // go back 1
                new_pos_candidate.row--;

                if (map[new_pos_candidate.row][new_pos_candidate.col] == '.'){
                    position = new_pos_candidate;
                }else{
                    break;
                }

                
            }else if (dir == down){
                // go up and find other edge
                while (new_pos_candidate.row >= 0 && map[new_pos_candidate.row][new_pos_candidate.col] != 'T'){
                    new_pos_candidate.row--;
                }
                // now we have arrived out of bounds or at the T
                // go back 1
                new_pos_candidate.row++;

                if (map[new_pos_candidate.row][new_pos_candidate.col] == '.'){
                    position = new_pos_candidate;
                }else{
                    break;
                }
                
            }else if (dir == left){
                // go right and find other edge
                while (new_pos_candidate.col < WIDTH && map[new_pos_candidate.row][new_pos_candidate.col] != 'T'){
                    new_pos_candidate.col++;
                }
                // now we have arrived out of bounds or at the T
                // go back 1
                new_pos_candidate.col--;

                if (map[new_pos_candidate.row][new_pos_candidate.col] == '.'){
                    position = new_pos_candidate;
                }else{
                    break;
                }
                
            }else{ // dir == right
                // go left and find other edge
                while (new_pos_candidate.col >= 0 && map[new_pos_candidate.row][new_pos_candidate.col] != 'T'){
                    new_pos_candidate.col--;
                }
                // now we have arrived out of bounds or at the T
                // go back 1
                new_pos_candidate.col++;

                if (map[new_pos_candidate.row][new_pos_candidate.col] == '.'){
                    position = new_pos_candidate;
                }else{
                    break;
                }
            }

        }
         
    }
}



template <typename T> // T must be printable with cout
void printMap(const std::vector<std::vector<T>>& map){
    for (auto line : map){
        for (auto c : line){
            std::cout << c;
        }
        std::cout << std::endl;
    }
}


int main(){

    auto startTime = std::chrono::steady_clock::now();

    
    std::vector<std::string> v;
    std::ifstream file("input_map.txt");
    if (file.is_open()){
        std::string line; 
        while (std::getline(file, line)){
            v.push_back(line);
        } 
    }


    int width = WIDTH;
    int height = HEIGHT;

    std::vector<std::vector<char>> map(height, std::vector<char>(width));
    /* map:
         T where there is no map, 
         # - wall, 
         . - empty space where we can walk
    */
    //printMap<char>(map);

    for (auto& row : map){
        std::fill(row.begin(), row.end(), 'T');
    }

    /*// map is indexed as [row][column]
    map[1][5] = 'T';*/  
    
    //std::cout << std::endl << std::endl << std::endl;
    //printMap<char>(map);

    for (int i = 0; i < height; i++){
        std::string line = v[i];
        int colidx = 0;
        for (auto c : line){
            if (c == '.'){
                map[i][colidx] = '.';
            }else if(c == '#'){
                map[i][colidx] = '#';
            }



            colidx++;
        }
    }

    //std::cout << std::endl << std::endl << std::endl;
    //printMap<char>(map);

    std::string instructions;
    std::ifstream file_2("input_instructions.txt");
    if (file_2.is_open()){
        std::string line; 
        while (std::getline(file_2, line)){
            instructions = line;
        } 
    }

    auto base = instructions.begin();
    auto runner = instructions.begin();

    std::vector<int> stepsAndTurns;  
    // > 0 -> step
    // -1  -> R turn (clockwise)
    // -2  -> L turn (counter-clockwise)




    // search the string, put steps and -1,-2 for turns in vector
    while (base != instructions.end()){
        if (*runner == 'R'){
            stepsAndTurns.push_back(-1);
            runner++;
            base = runner;
        }else if( *runner == 'L'){
            stepsAndTurns.push_back(-2);
            runner++;
            base = runner;
        }else{
            while (*runner != 'L' && *runner != 'R' ){
                runner++;
                if (runner == instructions.end()){
                    break;
                }
            }

            // now in the range [base, runner)  we have a number
        
        
            std::string  numberstring = instructions.substr(std::distance(instructions.begin(), base), std::distance(base, runner));



            stepsAndTurns.push_back(std::stoi(numberstring));

            base = runner;

        }
    }




    // moving      // coord: (row, col)
    coord pos {0, 50};


    // last one is a step, I use that information

    // -1  -> R turn (clockwise)
    // -2  -> L turn (counter-clockwise)
    direction dir = right;

    /*turn(dir, -1);
    turn(dir, -1);
    turn(dir, -1);
    turn(dir, -1);
    turn(dir, -2);*/

    for (int i = 0; i < stepsAndTurns.size(); i++){
        int current_thing = stepsAndTurns[i];

        if (current_thing == -1 || current_thing == -2){
            turn(dir, current_thing);
        }else{ // step
            step(map, pos, dir, current_thing);
        }

    }
    
            





    // row, column is 1 based!!! add 1 when calculating
    // Facing is 0 for right (>), 1 for down (v), 2 for left (<), and 3 for up (^). The final password is the sum of 1000 times the row, 4 times the column, and the facing.
    int scoreFromFacing = 0;
    if (dir == right){
        scoreFromFacing = 0;
    }else if (dir == down){
        scoreFromFacing = 1;
    }else if (dir == left){
        scoreFromFacing = 2;
    }else{
        scoreFromFacing = 3;
    }

    int pw = 1000 * (pos.row + 1) + 4 * (pos.col + 1) + scoreFromFacing;

    std::cout << "password: " << pw << std::endl;

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    std::cout << "Elapsed time (ms)= " << duration << std::endl;
}


/*
--- Day 22: Monkey Map ---
The monkeys take you on a surprisingly easy trail through the jungle. They're even going in roughly the right direction according to your handheld device's Grove Positioning System.

As you walk, the monkeys explain that the grove is protected by a force field. To pass through the force field, you have to enter a password; doing so involves tracing a specific path on a strangely-shaped board.

At least, you're pretty sure that's what you have to do; the elephants aren't exactly fluent in monkey.

The monkeys give you notes that they took when they last saw the password entered (your puzzle input).

For example:

        ...#
        .#..
        #...
        ....
...#.......#
........#...
..#....#....
..........#.
        ...#....
        .....#..
        .#......
        ......#.

10R5L5R10L4R5L5
The first half of the monkeys' notes is a map of the board. It is comprised of a set of open tiles (on which you can move, drawn .) and solid walls (tiles which you cannot enter, drawn #).

The second half is a description of the path you must follow. It consists of alternating numbers and letters:

A number indicates the number of tiles to move in the direction you are facing. If you run into a wall, you stop moving forward and continue with the next instruction.
A letter indicates whether to turn 90 degrees clockwise (R) or counterclockwise (L). Turning happens in-place; it does not change your current tile.
So, a path like 10R5 means "go forward 10 tiles, then turn clockwise 90 degrees, then go forward 5 tiles".

You begin the path in the leftmost open tile of the top row of tiles. Initially, you are facing to the right (from the perspective of how the map is drawn).

If a movement instruction would take you off of the map, you wrap around to the other side of the board. In other words, if your next tile is off of the board, you should instead look in the direction opposite of your current facing as far as you can until you find the opposite edge of the board, then reappear there.

For example, if you are at A and facing to the right, the tile in front of you is marked B; if you are at C and facing down, the tile in front of you is marked D:

        ...#
        .#..
        #...
        ....
...#.D.....#
........#...
B.#....#...A
.....C....#.
        ...#....
        .....#..
        .#......
        ......#.
It is possible for the next tile (after wrapping around) to be a wall; this still counts as there being a wall in front of you, and so movement stops before you actually wrap to the other side of the board.

By drawing the last facing you had with an arrow on each tile you visit, the full path taken by the above example looks like this:

        >>v#    
        .#v.    
        #.v.    
        ..v.    
...#...v..v#    
>>>v...>#.>>    
..#v...#....    
...>>>>v..#.    
        ...#....
        .....#..
        .#......
        ......#.
To finish providing the password to this strange input device, you need to determine numbers for your final row, column, and facing as your final position appears from the perspective of the original map. Rows start from 1 at the top and count downward; columns start from 1 at the left and count rightward. (In the above example, row 1, column 1 refers to the empty space with no tile on it in the top-left corner.) Facing is 0 for right (>), 1 for down (v), 2 for left (<), and 3 for up (^). The final password is the sum of 1000 times the row, 4 times the column, and the facing.

In the above example, the final row is 6, the final column is 8, and the final facing is 0. So, the final password is 1000 * 6 + 4 * 8 + 0: 6032.

Follow the path given in the monkeys' notes. What is the final password?

Your puzzle answer was 95358.

The first half of this puzzle is complete! It provides one gold star: *

--- Part Two ---
*/