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


    std::regex re_num { R"(([a-z]+): ([\d]+))" };
    std::regex re_links { R"(([a-z]+): ([a-z]+) (.) ([a-z]+))" };

    std::unordered_map<std::string, long long int> umap;

    while (!v.empty()){
        std::cout << "v.size(): " << v.size() << std::endl;
        std::vector<long long int> to_erase;
        for (long long int i = 0; i < v.size(); i++){
            std::smatch matchResults;
            std::string line = v[i];
        
            if (std::regex_match(line, matchResults, re_num)){
                std::string currentstr = matchResults[1];
                long long int currentnum = std::stoi(matchResults[2]);

                umap[currentstr] = currentnum;

                to_erase.push_back(i);
                //v.erase(v.begin() + i);


            }else{
                std::regex_match(line, matchResults, re_links);

                std::string currentstr = matchResults[1];
                std::string lefthand = matchResults[2];
                std::string operation = matchResults[3];
                std::string righthand = matchResults[4];

                if (umap.find(lefthand) != umap.end() && umap.find(righthand) != umap.end()){
                    // if we have a value for both operands
                    long long int result = 0;

                    if (operation == "*"){
                        result = umap[lefthand] * umap[righthand];
                    }else if (operation == "/"){
                        result = umap[lefthand] / umap[righthand];
                    }else if (operation == "+"){
                        result = umap[lefthand] + umap[righthand];
                    }else{ // (operation == "/"){
                        result = umap[lefthand] - umap[righthand];
                    }

                    umap[currentstr] = result;
                    
                    to_erase.push_back(i);
                }else{
                    // we cant do this yet  -> it stays
                }
            }


            
        } // for i

        // erase starting from the back
        if (to_erase.size() >= 1){
            for (long long int j = to_erase.size() - 1; j >=0; j--){
                v.erase(v.begin() + to_erase[j]);
            }
        }
        
    }


    std::cout << "monkey root shouts: " << umap["root"] << std::endl;

    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    std::cout << "Elapsed time (ms)= " << duration << std::endl;

}


/*
--- Day 21: Monkey Math ---
The monkeys are back! You're worried they're going to try to steal your stuff again, but it seems like they're just holding their ground and making various monkey noises at you.

Eventually, one of the elephants realizes you don't speak monkey and comes over to interpret. As it turns out, they overheard you talking about trying to find the grove; they can show you a shortcut if you answer their riddle.

Each monkey is given a job: either to yell a specific number or to yell the result of a math operation. All of the number-yelling monkeys know their number from the start; however, the math operation monkeys need to wait for two other monkeys to yell a number, and those two other monkeys might also be waiting on other monkeys.

Your job is to work out the number the monkey named root will yell before the monkeys figure it out themselves.

For example:

root: pppw + sjmn
dbpl: 5
cczh: sllz + lgvd
zczc: 2
ptdq: humn - dvpt
dvpt: 3
lfqf: 4
humn: 5
ljgn: 2
sjmn: drzm * dbpl
sllz: 4
pppw: cczh / lfqf
lgvd: ljgn * ptdq
drzm: hmdt - zczc
hmdt: 32
Each line contains the name of a monkey, a colon, and then the job of that monkey:

A lone number means the monkey's job is simply to yell that number.
A job like aaaa + bbbb means the monkey waits for monkeys aaaa and bbbb to yell each of their numbers; the monkey then yells the sum of those two numbers.
aaaa - bbbb means the monkey yells aaaa's number minus bbbb's number.
Job aaaa * bbbb will yell aaaa's number multiplied by bbbb's number.
Job aaaa / bbbb will yell aaaa's number divided by bbbb's number.
So, in the above example, monkey drzm has to wait for monkeys hmdt and zczc to yell their numbers. Fortunately, both hmdt and zczc have jobs that involve simply yelling a single number, so they do this immediately: 32 and 2. Monkey drzm can then yell its number by finding 32 minus 2: 30.

Then, monkey sjmn has one of its numbers (30, from monkey drzm), and already has its other number, 5, from dbpl. This allows it to yell its own number by finding 30 multiplied by 5: 150.

This process continues until root yells a number: 152.

However, your actual situation involves considerably more monkeys. What number will the monkey named root yell?

Your puzzle answer was 85616733059734.
*/