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

/*#define SIZE_OF_MAP 2000   // input will be offset by  OFFSET
#define OFFSET 950*/
// now all 2598 of them can fit in 1 single row lol
#define SIZE_OF_MAP 20
#define OFFSET 5

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
    std::ifstream file("input_test.txt");
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

    const int offset = OFFSET;
    for (int row = 0; row < v.size(); row++){
        for (int col = 0; col < v[0].size(); col++){
            if (v[row][col] == '#'){
                map[row + offset][col + offset] = true;
            }
        }
    }


    std::cout << "before starting" << std::endl;
    printVecofVec<bool>(map);
    std::cout << std::endl << std::endl << std::endl;

    std::vector<std::vector<int>> proposed_map_counts_INIT (size_of_map, std::vector<int>(size_of_map));
    for (auto& vec : proposed_map_counts_INIT){
        for (auto& n : vec){
            n = 0;
        }    
    }


    int i = 0;
    for (i = 0; i < 1000; i++){
        // i % 4 = 0 : north, 1 : south, 2 : west, 3 : east
        // 10 moves like this

        // counts map of proposed solutions, always filled with 0 at the beginning of round (cheap copy constructor)
        std::vector<std::vector<int>> proposed_map_counts = proposed_map_counts_INIT;

        bool didanyelvesmove = false;

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
                        didanyelvesmove = true;
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
                            map[row][col] = false;  // remove old
                            map[proposition.row][proposition.col] = true;  // add new
                            
                        } 
                        
                    }                   

                } // if elf in map
            }  // for col
        }  // for row
        
        std::cout << "at round i = "  << i << std::endl;
        printVecofVec<bool>(map);
        std::cout << std::endl << std::endl << std::endl;
        if (!didanyelvesmove){
            break;
        }
    }  // for i (10 steps)


    
    
    std::cout << "round where we didn't move at all: " << i + 1 << std::endl;  // 648 too low


    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    std::cout << "Elapsed time (ms)= " << duration << std::endl;



}
