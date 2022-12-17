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


    std::vector<std::pair<int,int>> sensors;  // x,y pairs
    std::vector<std::pair<int,int>> beacons;  // x,y pairs


    std::regex re1 { R"([A-Za-z =,:]+([\d-]+)[A-Za-z =,:]+([\d-]+)[A-Za-z =,:]+([\d-]+)[A-Za-z =,:]+([\d-]+))" };

    for (auto line : v){
        std::smatch matchResults;
        
        std::regex_match(line, matchResults, re1);


        sensors.push_back({std::stoi(matchResults[1]), std::stoi(matchResults[2])});
        beacons.push_back({std::stoi(matchResults[3]), std::stoi(matchResults[4])});
    }

    int res_x = -69;
    int res_y = -69;

/////////////////////////////////////////  
/*  
    int dist = 1;
    while(true){
        
        std::cout << "dist: " << dist << std::endl;
        for (int dx = -dist; dx <= dist; dx++){
            for (int dy = -dist; dy <= dist; dy++){
                for (int c = 0; c < beacons.size(); c++){
                    int current_beacon_x = beacons[c].first;
                    int current_beacon_y = beacons[c].second;

                    // if point is further from all sensors than their beacon, then it is the point we are looking for
                    bool further = true;
                    for (int i = 0; i < beacons.size(); i++){
                        int distance = std::abs(sensors[i].first - beacons[i].first) + std::abs(sensors[i].second - beacons[i].second);
                        int distance_TOCHECK = std::abs(beacons[i].first - (current_beacon_x + dx)) + std::abs(beacons[i].second - (current_beacon_y + dy));
                        if (distance_TOCHECK <= distance){
                            further = false;
                        }
                    }
                    

                    if (further){
                        res_x = current_beacon_x + dx;
                        res_y = current_beacon_y + dy;
                        goto FOUNDSOLUTION;
                    }
                }
                
            }
        }
        dist++;

    }
    */
/////////////////////////////////////////
    std::vector<std::pair<int,int>> solutions;

    // check around sensor range edges
    for (int i = 0; i < sensors.size(); i++){
        std::cout << "i: " << i << std::endl;
        int current_distance = std::abs(sensors[i].first - beacons[i].first) + std::abs(sensors[i].second - beacons[i].second);

        int current_sensor_x = sensors[i].first;
        int current_sensor_y = sensors[i].second;

        // check right to up ------------------------------------------------------------------------------------
        int x_tocheck = current_sensor_x + current_distance;
        int y_tocheck = current_sensor_y;
        
        while(x_tocheck >= current_sensor_x - 3){  // don't want to calculate actual range lol
            for (int dx : {1,2}){

                // if point further from all sensors than their beacon
                bool further = true;
                for (int j = 0; j < beacons.size(); j++){
                    int distance = std::abs(sensors[j].first - beacons[j].first) + std::abs(sensors[j].second - beacons[j].second);
                    int distance_TOCHECK = std::abs(sensors[j].first - (x_tocheck + dx)) + std::abs(sensors[j].second - (y_tocheck));
                    if (distance_TOCHECK <= distance){
                        further = false;
                    }
                }
                if (further){
                    if (x_tocheck + dx >= 0 && y_tocheck >= 0 && x_tocheck + dx <= 4000000 && y_tocheck <= 4000000){
                        solutions.push_back({x_tocheck+dx, y_tocheck});
                    }
                }
            }

            x_tocheck--;
            y_tocheck++;
        }
        // END OF check right to up ------------------------------------------------------------------------------------

        // check left to down ------------------------------------------------------------------------------------
        x_tocheck = current_sensor_x - current_distance;
        y_tocheck = current_sensor_y;
        
        while(x_tocheck <= current_sensor_x + 3){  // don't want to calculate actual range lol
            for (int dx : {-1, -2}){

                // if point further from all sensors than their beacon
                bool further = true;
                for (int j = 0; j < beacons.size(); j++){
                    int distance = std::abs(sensors[j].first - beacons[j].first) + std::abs(sensors[j].second - beacons[j].second);
                    int distance_TOCHECK = std::abs(sensors[j].first - (x_tocheck + dx)) + std::abs(sensors[j].second - (y_tocheck));
                    if (distance_TOCHECK <= distance){
                        further = false;
                    }
                }
                if (further){
                    if (x_tocheck + dx >= 0 && y_tocheck >= 0 && x_tocheck + dx <= 4000000 && y_tocheck <= 4000000){
                        solutions.push_back({x_tocheck+dx, y_tocheck});
                    }
                    
                }
            }

            x_tocheck++;
            y_tocheck--;
        }
        // END OF check left to down ------------------------------------------------------------------------------------
            
        // check down to right ------------------------------------------------------------------------------------
        x_tocheck = current_sensor_x ;
        y_tocheck = current_sensor_y - current_distance;
        
        while(y_tocheck <= current_sensor_y + 3){  // don't want to calculate actual range lol
            for (int dy : {-1, -2}){

                // if point further from all sensors than their beacon
                bool further = true;
                for (int j = 0; j < beacons.size(); j++){
                    int distance = std::abs(sensors[j].first - beacons[j].first) + std::abs(sensors[j].second - beacons[j].second);
                    int distance_TOCHECK = std::abs(sensors[j].first - (x_tocheck)) + std::abs(sensors[j].second - (y_tocheck + dy));
                    if (distance_TOCHECK <= distance){
                        further = false;
                    }
                }
                if (further){
                    if (x_tocheck >= 0 && y_tocheck + dy >= 0 && x_tocheck <= 4000000 && y_tocheck + dy <= 4000000){
                        solutions.push_back({x_tocheck, y_tocheck + dy});
                    }
                    
                }
            }

            x_tocheck++;
            y_tocheck++;
        }
        // END OF check down to right ------------------------------------------------------------------------------------


        // check up to left ------------------------------------------------------------------------------------
        x_tocheck = current_sensor_x ;
        y_tocheck = current_sensor_y + current_distance;
        
        while(y_tocheck >= current_sensor_y - 3){  // don't want to calculate actual range lol
            for (int dy : {+1, +2}){

                // if point further from all sensors than their beacon
                bool further = true;
                for (int j = 0; j < beacons.size(); j++){
                    int distance = std::abs(sensors[j].first - beacons[j].first) + std::abs(sensors[j].second - beacons[j].second);
                    int distance_TOCHECK = std::abs(sensors[j].first - (x_tocheck)) + std::abs(sensors[j].second - (y_tocheck + dy));
                    if (distance_TOCHECK <= distance){
                        further = false;
                    }
                }
                if (further){
                    if (x_tocheck >= 0 && y_tocheck + dy >= 0 && x_tocheck <= 4000000 && y_tocheck + dy <= 4000000){
                        solutions.push_back({x_tocheck, y_tocheck + dy});
                    }
                    
                }
            }

            x_tocheck--;
            y_tocheck--;
        }
        // END OF check up to left ------------------------------------------------------------------------------------
    
    }


    // if point is further from all sensors than their beacon, then it is the point we are looking for
                    


    long long int res = (long long int)solutions[0].first* (long long int)4000000 + (long long int)solutions[0].second;


    std::cout << std::endl<< std::endl<< "result p2: " << res << std::endl;
    // 18446744072411047380  too high
    // 13734006908372 ok


    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    std::cout << "Elapsed time (ms)= " << duration << std::endl;

}



/*
--- Day 15: Beacon Exclusion Zone ---
You feel the ground rumble again as the distress signal leads you to a large network of subterranean tunnels. You don't have time to search them all, but you don't need to: your pack contains a set of deployable sensors that you imagine were originally built to locate lost Elves.

The sensors aren't very powerful, but that's okay; your handheld device indicates that you're close enough to the source of the distress signal to use them. You pull the emergency sensor system out of your pack, hit the big button on top, and the sensors zoom off down the tunnels.

Once a sensor finds a spot it thinks will give it a good reading, it attaches itself to a hard surface and begins monitoring for the nearest signal source beacon. Sensors and beacons always exist at integer coordinates. Each sensor knows its own position and can determine the position of a beacon precisely; however, sensors can only lock on to the one beacon closest to the sensor as measured by the Manhattan distance. (There is never a tie where two beacons are the same distance to a sensor.)

It doesn't take long for the sensors to report back their positions and closest beacons (your puzzle input). For example:

Sensor at x=2, y=18: closest beacon is at x=-2, y=15
Sensor at x=9, y=16: closest beacon is at x=10, y=16
Sensor at x=13, y=2: closest beacon is at x=15, y=3
Sensor at x=12, y=14: closest beacon is at x=10, y=16
Sensor at x=10, y=20: closest beacon is at x=10, y=16
Sensor at x=14, y=17: closest beacon is at x=10, y=16
Sensor at x=8, y=7: closest beacon is at x=2, y=10
Sensor at x=2, y=0: closest beacon is at x=2, y=10
Sensor at x=0, y=11: closest beacon is at x=2, y=10
Sensor at x=20, y=14: closest beacon is at x=25, y=17
Sensor at x=17, y=20: closest beacon is at x=21, y=22
Sensor at x=16, y=7: closest beacon is at x=15, y=3
Sensor at x=14, y=3: closest beacon is at x=15, y=3
Sensor at x=20, y=1: closest beacon is at x=15, y=3
So, consider the sensor at 2,18; the closest beacon to it is at -2,15. For the sensor at 9,16, the closest beacon to it is at 10,16.

Drawing sensors as S and beacons as B, the above arrangement of sensors and beacons looks like this:

               1    1    2    2
     0    5    0    5    0    5
 0 ....S.......................
 1 ......................S.....
 2 ...............S............
 3 ................SB..........
 4 ............................
 5 ............................
 6 ............................
 7 ..........S.......S.........
 8 ............................
 9 ............................
10 ....B.......................
11 ..S.........................
12 ............................
13 ............................
14 ..............S.......S.....
15 B...........................
16 ...........SB...............
17 ................S..........B
18 ....S.......................
19 ............................
20 ............S......S........
21 ............................
22 .......................B....
This isn't necessarily a comprehensive map of all beacons in the area, though. Because each sensor only identifies its closest beacon, if a sensor detects a beacon, you know there are no other beacons that close or closer to that sensor. There could still be beacons that just happen to not be the closest beacon to any sensor. Consider the sensor at 8,7:

               1    1    2    2
     0    5    0    5    0    5
-2 ..........#.................
-1 .........###................
 0 ....S...#####...............
 1 .......#######........S.....
 2 ......#########S............
 3 .....###########SB..........
 4 ....#############...........
 5 ...###############..........
 6 ..#################.........
 7 .#########S#######S#........
 8 ..#################.........
 9 ...###############..........
10 ....B############...........
11 ..S..###########............
12 ......#########.............
13 .......#######..............
14 ........#####.S.......S.....
15 B........###................
16 ..........#SB...............
17 ................S..........B
18 ....S.......................
19 ............................
20 ............S......S........
21 ............................
22 .......................B....
This sensor's closest beacon is at 2,10, and so you know there are no beacons that close or closer (in any positions marked #).

None of the detected beacons seem to be producing the distress signal, so you'll need to work out where the distress beacon is by working out where it isn't. For now, keep things simple by counting the positions where a beacon cannot possibly be along just a single row.

So, suppose you have an arrangement of beacons and sensors like in the example above and, just in the row where y=10, you'd like to count the number of positions a beacon cannot possibly exist. The coverage from all sensors near that row looks like this:

                 1    1    2    2
       0    5    0    5    0    5
 9 ...#########################...
10 ..####B######################..
11 .###S#############.###########.
In this example, in the row where y=10, there are 26 positions where a beacon cannot be present.

Consult the report from the sensors you just deployed. In the row where y=2000000, how many positions cannot contain a beacon?

Your puzzle answer was 5147333.

--- Part Two ---
Your handheld device indicates that the distress signal is coming from a beacon nearby. The distress beacon is not detected by any sensor, but the distress beacon must have x and y coordinates each no lower than 0 and no larger than 4000000.

To isolate the distress beacon's signal, you need to determine its tuning frequency, which can be found by multiplying its x coordinate by 4000000 and then adding its y coordinate.

In the example above, the search space is smaller: instead, the x and y coordinates can each be at most 20. With this reduced search area, there is only a single position that could have a beacon: x=14, y=11. The tuning frequency for this distress beacon is 56000011.

Find the only possible position for the distress beacon. What is its tuning frequency?

Your puzzle answer was 13734006908372.

Both parts of this puzzle are complete! They provide two gold stars: **
*/