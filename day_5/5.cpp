#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string>
#include <stack>








int main(){

    std::vector<std::string> v;

    std::ifstream file("input_5.txt");
    if (file.is_open()){
        std::string line; 
        while (std::getline(file, line)){
            v.push_back(line);
        } 
    }

    std::vector< std::stack<char> > stacks;

    stacks.push_back(std::stack<char> ({'.'}));
    stacks.push_back(std::stack<char> ({'R','G','J','B','T','V','Z'}));
    stacks.push_back(std::stack<char> ({'J','R','V','L'}));
    stacks.push_back(std::stack<char> ({'S','Q','F'}));
    stacks.push_back(std::stack<char> ({'Z','H','N','L','F','V','Q','G'}));
    stacks.push_back(std::stack<char> ({'R','Q','T','J','C','S','M','W'}));
    stacks.push_back(std::stack<char> ({'S','W','T','C','H','F'}));
    stacks.push_back(std::stack<char> ({'D','Z','C','V','F','N','J'}));
    stacks.push_back(std::stack<char> ({'L','G','Z','D','W','R','F','Q'}));
    stacks.push_back(std::stack<char> ({'J','B','W','V','p'}));

    std::regex re { "^[^0-9]+([0-9]+)[^0-9]+([0-9])[^0-9]+([0-9])$" };


    for (const std::string& s : v){
        std::smatch matchResults;
        std::regex_match(s, matchResults, re);

        int howMany = std::stoi(matchResults[1]);
        int from = std::stoi(matchResults[2]);
        int to = std::stoi(matchResults[3]);

        for (int i = 0; i < howMany; i++){
            char whatToMove = stacks[from].top(); stacks[from].pop();
            stacks[to].push(whatToMove);
        }  
    }



    for (int i = 1; i < 10; i++ ){
        std::cout << stacks[i].top();
    }
    std::cout << std::endl;




}



/*
--- Day 5: Supply Stacks ---
The expedition can depart as soon as the final supplies have been unloaded from the ships. Supplies are stored in stacks of marked crates, but because the needed supplies are buried under many other crates, the crates need to be rearranged.

The ship has a giant cargo crane capable of moving crates between stacks. To ensure none of the crates get crushed or fall over, the crane operator will rearrange them in a series of carefully-planned steps. After the crates are rearranged, the desired crates will be at the top of each stack.

The Elves don't want to interrupt the crane operator during this delicate procedure, but they forgot to ask her which crate will end up where, and they want to be ready to unload them as soon as possible so they can embark.

They do, however, have a drawing of the starting stacks of crates and the rearrangement procedure (your puzzle input). For example:

    [D]    
[N] [C]    
[Z] [M] [P]
 1   2   3 

move 1 from 2 to 1
move 3 from 1 to 3
move 2 from 2 to 1
move 1 from 1 to 2
In this example, there are three stacks of crates. Stack 1 contains two crates: crate Z is on the bottom, and crate N is on top. Stack 2 contains three crates; from bottom to top, they are crates M, C, and D. Finally, stack 3 contains a single crate, P.

Then, the rearrangement procedure is given. In each step of the procedure, a quantity of crates is moved from one stack to a different stack. In the first step of the above rearrangement procedure, one crate is moved from stack 2 to stack 1, resulting in this configuration:

[D]        
[N] [C]    
[Z] [M] [P]
 1   2   3 
In the second step, three crates are moved from stack 1 to stack 3. Crates are moved one at a time, so the first crate to be moved (D) ends up below the second and third crates:

        [Z]
        [N]
    [C] [D]
    [M] [P]
 1   2   3
Then, both crates are moved from stack 2 to stack 1. Again, because crates are moved one at a time, crate C ends up below crate M:

        [Z]
        [N]
[M]     [D]
[C]     [P]
 1   2   3
Finally, one crate is moved from stack 1 to stack 2:

        [Z]
        [N]
        [D]
[C] [M] [P]
 1   2   3
The Elves just need to know which crate will end up on top of each stack; in this example, the top crates are C in stack 1, M in stack 2, and Z in stack 3, so you should combine these together and give the Elves the message CMZ.

After the rearrangement procedure completes, what crate ends up on top of each stack?

Your puzzle answer was ZSQVCCJLL.

The first half of this puzzle is complete! It provides one gold star: *
*/