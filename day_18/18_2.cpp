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

#define SEARCHSIZE 30
using arr212121 = bool[SEARCHSIZE][SEARCHSIZE][SEARCHSIZE];

struct p3d{
int x;
int y;
int z;
};

bool checkOutOfCounds(int x, int y, int z){
    if (x < 0 || x > SEARCHSIZE - 1||
        y < 0 || y > SEARCHSIZE - 1||
        z < 0 || z > SEARCHSIZE - 1){
            return true;
        }else{
            return false;
        }
}

void floodFill(arr212121& arr, int x, int y, int z){
    // flood fills array arr, from point (x,y,z) contained in point
    if (checkOutOfCounds(x,y,z)){
        return;
    }
    

    if (arr[x][y][z]){
        return;
    }
    
    arr[x][y][z] = true;

    floodFill(arr, x+1, y, z);
    floodFill(arr, x-1, y, z);
    floodFill(arr, x, y+1, z);
    floodFill(arr, x, y-1, z);
    floodFill(arr, x, y, z+1);
    floodFill(arr, x, y, z-1);
    return;


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

    std::regex re1 { R"((\d+),(\d+),(\d+))" };

    std::vector<p3d> cubes;

    for (std::string line : v){
        std::smatch matchResults;
        
        std::regex_match(line, matchResults, re1);

        p3d point;
        point.x = std::stoi(matchResults[1])+3;
        point.y = std::stoi(matchResults[2])+3;
        point.z = std::stoi(matchResults[3])+3;

        cubes.push_back(point);
    }


    // represent array
    const int maxcoordinate = 19;
    const int maxcoordinate_search = SEARCHSIZE; // YOLO

    bool space[maxcoordinate_search][maxcoordinate_search][maxcoordinate_search];
    for (int i = 0; i < maxcoordinate_search; i++){
        for (int j = 0; j < maxcoordinate_search; j++){
            for (int k = 0; k < maxcoordinate_search; k++){
                space[i][j][k] = false;
            }
        }
    }

    for (auto p : cubes){
        space[p.x][p.y][p.z] = true;
    }

    // flood fill from 21,21,21 point all air
    floodFill(space, maxcoordinate_search-1, maxcoordinate_search-1, maxcoordinate_search-1);


    // what remains empty (false) are inside pockets -> add these to the list
    std::vector<p3d> cubes_part2 = cubes;

    for (int i = 0; i < maxcoordinate_search; i++){
        for (int j = 0; j < maxcoordinate_search; j++){
            for (int k = 0; k < maxcoordinate_search; k++){
                if (!space[i][j][k]){
                    p3d point_p2;
                    point_p2.x = i;
                    point_p2.y = j;
                    point_p2.z = k;
                    cubes_part2.push_back(point_p2);
                }
            }
        }
    }
    

    int totalsides = 0;
    // brute force baby
    cubes = cubes_part2; // lazy solution lol
    for (int i = 0; i < cubes.size(); i++){
        int currentfreesides = 6;
        for (int j = 0; j < cubes.size(); j++){
            if (i != j){
                if (((cubes[i].x == cubes[j].x - 1 || cubes[i].x == cubes[j].x + 1) && cubes[i].y == cubes[j].y && cubes[i].z == cubes[j].z)
                 || ((cubes[i].y == cubes[j].y - 1 || cubes[i].y == cubes[j].y + 1) && cubes[i].x == cubes[j].x && cubes[i].z == cubes[j].z)
                 || ((cubes[i].z == cubes[j].z - 1 || cubes[i].z == cubes[j].z + 1) && cubes[i].x == cubes[j].x && cubes[i].y == cubes[j].y)){
                    currentfreesides--;
                  }

            }
        }
        totalsides += currentfreesides;
    }

    std::cout << "total free surface area: " << totalsides << std::endl; 
    // 5158 toohigh
    // 3258 toohigh
    // 2126 toohigh

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    std::cout << "Elapsed time (ms)= " << duration << std::endl;

}



/*
--- Part Two ---
Something seems off about your calculation. The cooling rate depends on exterior surface area, but your calculation also included the surface area of air pockets trapped in the lava droplet.

Instead, consider only cube sides that could be reached by the water and steam as the lava droplet tumbles into the pond. The steam will expand to reach as much as possible, completely displacing any air on the outside of the lava droplet but never expanding diagonally.

In the larger example above, exactly one cube of air is trapped within the lava droplet (at 2,2,5), so the exterior surface area of the lava droplet is 58.

What is the exterior surface area of your scanned lava droplet?

Your puzzle answer was 2000.

Both parts of this puzzle are complete! They provide two gold stars: **
*/