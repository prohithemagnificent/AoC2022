#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <regex>
#include <deque>
#include <chrono>


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


    std::map<std::string, std::pair<int, std::vector<std::string>>> graph;

    //std::regex re1 { R"(^Valve ([A-Z]{2}) has flow rate\=(\d+); tunnels? lead to valve.*)"};  
    std::regex re1 { R"(Valve ([A-Z]{2}) has flow rate\=(\d+); .*)"};
    
    // 

    /*std::vector<std::string> nodes;
    std::vector<int> flowrates;
    std::vector<std::vector<std::string>> neighbors; */

    for (std::string line : v){
        std::smatch matchResults;
        std::smatch matchResults2;
        std::regex_match(line, matchResults, re1);

        /*nodes.push_back(matchResults[1]);*/
        /*flowrates.push_back(std::stoi(matchResults[2]));*/
        std::string currentnode = matchResults[1];
        int currentflowrate = std::stoi(matchResults[2]);


        auto idx = line.find("valve");
        std::string endpart = line.substr(idx+6, line.size());

        auto endBEG = endpart.begin();
        const auto endEND = endpart.end();
        
        std::vector<std::string> current_neighbors;
        while (endBEG != endEND){
            if (*endBEG == ',' || *endBEG == ' '){
                endBEG++;
            }else{
                std::string s;
                s += *endBEG;
                endBEG++;
                s += *endBEG;
                endBEG++;
                current_neighbors.push_back(s);
            }
        }
        /*neighbors.push_back(current_neighbors);*/
        graph[currentnode] = {currentflowrate, current_neighbors};
        

    }

    std::map<std::string, bool> opened;
    std::map<std::string, bool> visited;

 
    



    // find all possible paths
    std::vector<std::vector<std::string>> allpaths;
    allpaths.push_back(std::vector<std::string> {"AA"});
    visited["AA"] = true;
    
    int idx = 0;

    while (allpaths.back().size() <= 30){
        const std::vector<std::string> currentpath = allpaths[idx];
        visited[currentpath.back()] = true;

        for (auto& neighbor : graph[currentpath.back()].second){
            if (std::find(currentpath.begin(), currentpath.end(), neighbor) == currentpath.end()){
                std::vector<std::string> current_to_add = currentpath;
                current_to_add.push_back(neighbor);
                allpaths.push_back(current_to_add);
            }
            
        }
        idx++;     
        
    }

    

    // calculate score for each route, keep track of max
    int maxscore = -1;
    

    


    for (auto& path : allpaths){
        if (path.size() <= 1){
            continue;
        }

        int minutes = 30;
        int totalflowrate = 0;
        int totalflow = 0;

        std::vector<bool> turnedon (path.size());

        // steph 1 from A always
        int i = 1;
        minutes--;

        while (minutes > 0){
            totalflow += totalflowrate;



            if (graph[path[i]].first > 0 && !turnedon[i]){
                // open current valve
                totalflowrate += graph[path[i]].first;
                turnedon[i] = true;                
            }else if (i+1 < path.size()){
                // step to next  cave
                i++;                
            }

            minutes--;
        }

        maxscore = std::max(maxscore, totalflow);
    }

    
    std::cout << maxscore << std::endl;
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();

    std::cout << "Elapsed time (ms)= " << duration << std::endl;


    int lol;
    
}


/*
--- Day 16: Proboscidea Volcanium ---
The sensors have led you to the origin of the distress signal: yet another handheld device, just like the one the Elves gave you. However, you don't see any Elves around; instead, the device is surrounded by elephants! They must have gotten lost in these tunnels, and one of the elephants apparently figured out how to turn on the distress signal.

The ground rumbles again, much stronger this time. What kind of cave is this, exactly? You scan the cave with your handheld device; it reports mostly igneous rock, some ash, pockets of pressurized gas, magma... this isn't just a cave, it's a volcano!

You need to get the elephants out of here, quickly. Your device estimates that you have 30 minutes before the volcano erupts, so you don't have time to go back out the way you came in.

You scan the cave for other options and discover a network of pipes and pressure-release valves. You aren't sure how such a system got into a volcano, but you don't have time to complain; your device produces a report (your puzzle input) of each valve's flow rate if it were opened (in pressure per minute) and the tunnels you could use to move between the valves.

There's even a valve in the room you and the elephants are currently standing in labeled AA. You estimate it will take you one minute to open a single valve and one minute to follow any tunnel from one valve to another. What is the most pressure you could release?

For example, suppose you had the following scan output:

Valve AA has flow rate=0; tunnels lead to valves DD, II, BB
Valve BB has flow rate=13; tunnels lead to valves CC, AA
Valve CC has flow rate=2; tunnels lead to valves DD, BB
Valve DD has flow rate=20; tunnels lead to valves CC, AA, EE
Valve EE has flow rate=3; tunnels lead to valves FF, DD
Valve FF has flow rate=0; tunnels lead to valves EE, GG
Valve GG has flow rate=0; tunnels lead to valves FF, HH
Valve HH has flow rate=22; tunnel leads to valve GG
Valve II has flow rate=0; tunnels lead to valves AA, JJ
Valve JJ has flow rate=21; tunnel leads to valve II
All of the valves begin closed. You start at valve AA, but it must be damaged or jammed or something: its flow rate is 0, so there's no point in opening it. However, you could spend one minute moving to valve BB and another minute opening it; doing so would release pressure during the remaining 28 minutes at a flow rate of 13, a total eventual pressure release of 28 * 13 = 364. Then, you could spend your third minute moving to valve CC and your fourth minute opening it, providing an additional 26 minutes of eventual pressure release at a flow rate of 2, or 52 total pressure released by valve CC.

Making your way through the tunnels like this, you could probably open many or all of the valves by the time 30 minutes have elapsed. However, you need to release as much pressure as possible, so you'll need to be methodical. Instead, consider this approach:

== Minute 1 ==
No valves are open.
You move to valve DD.

== Minute 2 ==
No valves are open.
You open valve DD.

== Minute 3 ==
Valve DD is open, releasing 20 pressure.
You move to valve CC.

== Minute 4 ==
Valve DD is open, releasing 20 pressure.
You move to valve BB.

== Minute 5 ==
Valve DD is open, releasing 20 pressure.
You open valve BB.

== Minute 6 ==
Valves BB and DD are open, releasing 33 pressure.
You move to valve AA.

== Minute 7 ==
Valves BB and DD are open, releasing 33 pressure.
You move to valve II.

== Minute 8 ==
Valves BB and DD are open, releasing 33 pressure.
You move to valve JJ.

== Minute 9 ==
Valves BB and DD are open, releasing 33 pressure.
You open valve JJ.

== Minute 10 ==
Valves BB, DD, and JJ are open, releasing 54 pressure.
You move to valve II.

== Minute 11 ==
Valves BB, DD, and JJ are open, releasing 54 pressure.
You move to valve AA.

== Minute 12 ==
Valves BB, DD, and JJ are open, releasing 54 pressure.
You move to valve DD.

== Minute 13 ==
Valves BB, DD, and JJ are open, releasing 54 pressure.
You move to valve EE.

== Minute 14 ==
Valves BB, DD, and JJ are open, releasing 54 pressure.
You move to valve FF.

== Minute 15 ==
Valves BB, DD, and JJ are open, releasing 54 pressure.
You move to valve GG.

== Minute 16 ==
Valves BB, DD, and JJ are open, releasing 54 pressure.
You move to valve HH.

== Minute 17 ==
Valves BB, DD, and JJ are open, releasing 54 pressure.
You open valve HH.

== Minute 18 ==
Valves BB, DD, HH, and JJ are open, releasing 76 pressure.
You move to valve GG.

== Minute 19 ==
Valves BB, DD, HH, and JJ are open, releasing 76 pressure.
You move to valve FF.

== Minute 20 ==
Valves BB, DD, HH, and JJ are open, releasing 76 pressure.
You move to valve EE.

== Minute 21 ==
Valves BB, DD, HH, and JJ are open, releasing 76 pressure.
You open valve EE.

== Minute 22 ==
Valves BB, DD, EE, HH, and JJ are open, releasing 79 pressure.
You move to valve DD.

== Minute 23 ==
Valves BB, DD, EE, HH, and JJ are open, releasing 79 pressure.
You move to valve CC.

== Minute 24 ==
Valves BB, DD, EE, HH, and JJ are open, releasing 79 pressure.
You open valve CC.

== Minute 25 ==
Valves BB, CC, DD, EE, HH, and JJ are open, releasing 81 pressure.

== Minute 26 ==
Valves BB, CC, DD, EE, HH, and JJ are open, releasing 81 pressure.

== Minute 27 ==
Valves BB, CC, DD, EE, HH, and JJ are open, releasing 81 pressure.

== Minute 28 ==
Valves BB, CC, DD, EE, HH, and JJ are open, releasing 81 pressure.

== Minute 29 ==
Valves BB, CC, DD, EE, HH, and JJ are open, releasing 81 pressure.

== Minute 30 ==
Valves BB, CC, DD, EE, HH, and JJ are open, releasing 81 pressure.
This approach lets you release the most pressure possible in 30 minutes with this valve layout, 1651.

Work out the steps to release the most pressure in 30 minutes. What is the most pressure you can release?

Your puzzle answer was 1641.

The first half of this puzzle is complete! It provides one gold star: *
*/
















