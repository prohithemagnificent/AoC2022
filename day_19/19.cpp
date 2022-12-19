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




class RobotCollection{
public:
    int time = 0;
    int ore = 0;
    int clay = 0;
    int obs = 0;
    int geo = 0;

    int oreRobots = 1;
    int clayRobots = 0;
    int obsRobots = 0;
    int geoRobots = 0;

    int id;
    int ore_costInOre;
    int clay_costInOre;
    int obs_costInOre;
    int obs_costInClay;
    int geo_costInOre;
    int geo_costInObs;
    
    int maxCostInOre = 0;

    RobotCollection(int in_id, int in_ore_costInOre, int in_clay_costInOre, int in_obs_costInOre, int in_obs_costInClay, int in_geo_costInOre, int in_geo_costInObs){
         id = in_id;
         ore_costInOre = in_ore_costInOre;
         clay_costInOre = in_clay_costInOre;
         obs_costInOre = in_obs_costInOre;
         obs_costInClay = in_obs_costInClay;
         geo_costInOre = in_geo_costInOre;
         geo_costInObs = in_geo_costInObs;

         maxCostInOre = std::max({ore_costInOre, clay_costInOre, obs_costInOre, geo_costInOre});
    }


    void waitForResources(){
        ore += oreRobots;
        clay += clayRobots;
        obs += obsRobots;
        geo += geoRobots;
        time++;
    }

    // dont build robot if (1) not enough resources, (2) we already produce enough for any other type of robot in each minute
    bool ispossible_buildOreRobot(){        
        if (ore < ore_costInOre || oreRobots >= maxCostInOre){
            return false;
        }
        return true;
    }

    bool ispossible_buildClayRobot(){
        if (ore < clay_costInOre || clayRobots >= obs_costInClay){
            return false;
        }
        return true;
    }

    bool ispossible_buildObsRobot(){
        if (ore < obs_costInOre || clay < obs_costInClay || obsRobots >= geo_costInObs){
            return false;
        }
        return true;
    }

    bool ispossible_buildGeoRobot(){
        if (ore < geo_costInOre || obs < geo_costInObs){
            return false;
        }
        return true;
    }

    // build stuff: subtract resource, waitForResource(), add the new robot,
    // int ore_costInOre, int clay_costInOre, int obs_costInOre, int obs_costInClay, int geo_costInOre, int geo_costInObs
    void buildOreRobot(){
        ore -= ore_costInOre;
        waitForResources();
        oreRobots++;
    }

    void buildClayRobot(){
        ore -= clay_costInOre;
        waitForResources();
        clayRobots++;
    }

    void buildObsRobot(){
        ore -= obs_costInOre;
        clay -= obs_costInClay;
        waitForResources();
        obsRobots++;
    }

    void buildGeoRobot(){
        ore -= geo_costInOre;
        obs -= geo_costInObs;
        waitForResources();
        geoRobots++;
    }

    

    int calculateAvailableMaxGeo(){
        int timeRemaining = 24 - time; // t
        int maxFromCurrentRobots = geoRobots * timeRemaining;
        return geo + maxFromCurrentRobots;
    }

    bool isBetterThanCurrentBest(int currentbest){
        if (time < 10){
            return true;
        }
        if (currentbest > calculatePossibleMaxGeo()){
            return false;
        }
        return true;
    }

    int returnNOfOreAfterTimestep(){
        return ore + oreRobots;
    }

private:
    int calculatePossibleMaxGeo(){
        int timeRemaining = 25 - time; // t
        int maxFromCurrentRobots = geoRobots * timeRemaining;
        int maxFromFutureRobots = timeRemaining * (timeRemaining + 1) / 2;  // optimistic: we build 1 geo robot each minute from now on
        return geo + maxFromCurrentRobots + maxFromFutureRobots;
    }


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

    std::vector<int> id;
    std::vector<int> ore_costInOre;
    std::vector<int> clay_costInOre;
    std::vector<int> obs_costInOre;
    std::vector<int> obs_costInClay;
    std::vector<int> geo_costInOre;
    std::vector<int> geo_costInObs;
    


    std::regex re1 { R"([\D]+(\d+)[\D]+(\d+)[\D]+(\d+)[\D]+(\d+)[\D]+(\d+)[\D]+(\d+)[\D]+(\d+)[\D]+)" };

    for (auto line : v){
        std::smatch matchResults;
        
        std::regex_match(line, matchResults, re1);


        id.push_back(std::stoi(matchResults[1]));
        ore_costInOre.push_back(std::stoi(matchResults[2]));
        clay_costInOre.push_back(std::stoi(matchResults[3]));
        obs_costInOre.push_back(std::stoi(matchResults[4]));
        obs_costInClay.push_back(std::stoi(matchResults[5]));
        geo_costInOre.push_back(std::stoi(matchResults[6]));
        geo_costInObs.push_back(std::stoi(matchResults[7]));
    }


    // go through each blueprint
    for (int i = 0; i < id.size(); i++){
        RobotCollection c = RobotCollection(id[i], ore_costInOre[i], clay_costInOre[i], obs_costInOre[i], obs_costInClay[i], geo_costInOre[i], geo_costInObs[i]);
        c.waitForResources();  // first minute

        // queueueueue to store all possibilities
        std::deque<RobotCollection> possibilities;
        possibilities.push_back(c);

        int currentBestGeo = 0;

        // other 22 minutes  -> in the last minute, it is not worth to build anything
        for (int j = 0 + 2; j < 22 + 2; j++){  // + 2 becouse we start at minute 2
            while (possibilities.front().time == j - 1){
                RobotCollection current = possibilities.front(); possibilities.pop_front();

                // 5 possibilities: wait, build 1 of each robots
                RobotCollection wait = current;
                wait.waitForResources();

                if (wait.isBetterThanCurrentBest(currentBestGeo)){
                    possibilities.push_back(wait);
                    currentBestGeo = wait.calculateAvailableMaxGeo();
                }
                

                

                if (current.ispossible_buildOreRobot()){
                    RobotCollection ore = current;
                    ore.buildOreRobot();
                    
                    if (ore.isBetterThanCurrentBest(currentBestGeo)){
                        possibilities.push_back(ore);
                        currentBestGeo = ore.calculateAvailableMaxGeo();
                    }
                }
                if (current.ispossible_buildClayRobot()){
                    RobotCollection clay = current;
                    clay.buildClayRobot();

                    if (clay.isBetterThanCurrentBest(currentBestGeo)){
                        possibilities.push_back(clay);
                        currentBestGeo = clay.calculateAvailableMaxGeo();
                    }
                    
                }
                if (current.ispossible_buildObsRobot()){
                    RobotCollection obs = current;
                    obs.buildObsRobot();

                    if (obs.isBetterThanCurrentBest(currentBestGeo)){
                        possibilities.push_back(obs);
                        currentBestGeo = obs.calculateAvailableMaxGeo();
                    }

                    
                }
                if (current.ispossible_buildGeoRobot()){
                    RobotCollection geo = current;
                    geo.buildGeoRobot();

                    if (geo.isBetterThanCurrentBest(currentBestGeo)){
                        possibilities.push_back(geo);
                        currentBestGeo = geo.calculateAvailableMaxGeo();
                    }
                    possibilities.push_back(geo);
                }
                /*if (possibilities.size() % 100 == 0 && j <= 22){
                    std::cout << "possibilities.size(): " << possibilities.size() << std::endl;
                }*/

            }
            std::cout << "j: " << j << std::endl;
            std::cout << "possibilities.size(): " << possibilities.size() << std::endl;
        }

        // minute 24
        // go though queueueue, calculate last minute
    }




    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    std::cout << "Elapsed time (ms)= " << duration << std::endl;
}
