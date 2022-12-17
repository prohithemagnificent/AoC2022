#include <fstream>
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <regex>
#include <deque>
#include <algorithm>
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
    

    // for part 2: 
    std::vector<int> allscores {0};


    for (auto& path : allpaths){
        if (path.size() <= 1){
            continue;
        }

        int minutes = 26;
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
        allscores.push_back(totalflow);
        
    }


/*
    // assume we never cross paths (????) -> NOPE 2241 toolow
    for (auto& path : allpaths){
        std::sort(path.begin(), path.end());
    }

    for (int i = 0; i < allpaths.size() - 1; i++){
        for (int j = i + 1; j < allpaths.size(); j++){
            std::vector<std::string> common;
            auto it_common_end = std::set_intersection(allpaths[i].begin(), allpaths[i].end(),
                                                       allpaths[j].begin(), allpaths[j].end(), 
                                                       std::back_inserter(common));
            if (std::distance(common.begin(), common.end()) == 1){
                // 2 paths are distinct, only AA is the common
                maxscore = std::max(maxscore, allscores[i] + allscores[j] );
            }
            
            
        }
        if (i % 25 == 0){
            auto currentelapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
            std::cout << "cycle i= " << i << ", elapsed time (ms): " << currentelapsed  << std::endl; 
        }
        
        
    }

*/
    
    // any combination of 2 paths -> NOPE 2373 toohigh, 2418 nemjó
    std::vector<std::vector<std::string>> allpaths_with_highenough;
    std::vector<int> allscores_highenough;

    for (int i = 0; i < allpaths.size(); i++){
        if (allscores[i] > 169){
            allpaths_with_highenough.push_back(allpaths[i]);
            allscores_highenough.push_back(allscores[i]);

        }
    }

    for (int i = 0; i < allpaths_with_highenough.size() - 1; i++){
        for (int j = i + 1; j < allpaths_with_highenough.size(); j++){
            std::vector<std::string> common;
            auto it_common_end = std::set_intersection(allpaths_with_highenough[i].begin(), allpaths_with_highenough[i].end(),
                                                       allpaths_with_highenough[j].begin(), allpaths_with_highenough[j].end(), 
                                                       std::back_inserter(common));
            //if (std::distance(common.begin(), common.end()) == 1){
                // 2 paths are distinct, only AA is the common
                maxscore = std::max(maxscore, allscores_highenough[i] + allscores_highenough[j] );
            //}
            
            
        }
        if (i % 25 == 0){
            auto currentelapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
            std::cout << "cycle i= " << i << ", elapsed time (ms): " << currentelapsed  << std::endl; 
        }
        
        
    }

    std::cout << maxscore << std::endl; // 2373 toohigh,    2241 toolow

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();

    std::cout << "Elapsed time (ms)= " << duration << std::endl;   


    int lol;
    
}

















