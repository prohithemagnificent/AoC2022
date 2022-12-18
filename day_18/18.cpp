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


struct p3d{
int x;
int y;
int z;
};


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

    int maxcoordinate = -1;
    for (std::string line : v){
        std::smatch matchResults;
        
        std::regex_match(line, matchResults, re1);

        p3d point;
        point.x = std::stoi(matchResults[1]);
        point.y = std::stoi(matchResults[2]);
        point.z = std::stoi(matchResults[3]);

        maxcoordinate = std::max({point.x, point.y, point.z, maxcoordinate});

        cubes.push_back(point);
    }

    std::cout << "max coordinate in any direction: " << maxcoordinate << std::endl;

    int totalsides = 0;
    // brute force baby
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

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    std::cout << "Elapsed time (ms)= " << duration << std::endl;



}


/*
--- Day 18: Boiling Boulders ---
You and the elephants finally reach fresh air. You've emerged near the base of a large volcano that seems to be actively erupting! Fortunately, the lava seems to be flowing away from you and toward the ocean.

Bits of lava are still being ejected toward you, so you're sheltering in the cavern exit a little longer. Outside the cave, you can see the lava landing in a pond and hear it loudly hissing as it solidifies.

Depending on the specific compounds in the lava and speed at which it cools, it might be forming obsidian! The cooling rate should be based on the surface area of the lava droplets, so you take a quick scan of a droplet as it flies past you (your puzzle input).

Because of how quickly the lava is moving, the scan isn't very good; its resolution is quite low and, as a result, it approximates the shape of the lava droplet with 1x1x1 cubes on a 3D grid, each given as its x,y,z position.

To approximate the surface area, count the number of sides of each cube that are not immediately connected to another cube. So, if your scan were only two adjacent cubes like 1,1,1 and 2,1,1, each cube would have a single side covered and five sides exposed, a total surface area of 10 sides.

Here's a larger example:

2,2,2
1,2,2
3,2,2
2,1,2
2,3,2
2,2,1
2,2,3
2,2,4
2,2,6
1,2,5
3,2,5
2,1,5
2,3,5
In the above example, after counting up all the sides that aren't connected to another cube, the total surface area is 64.

What is the surface area of your scanned lava droplet?

Your puzzle answer was 3530.

The first half of this puzzle is complete! It provides one gold star: *
*/