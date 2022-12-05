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

        std::stack<char> temp;
        for (int i = 0; i < howMany; i++){  
            temp.push(stacks[from].top()); stacks[from].pop();
        }  

        while (!temp.empty()){
            stacks[to].push(temp.top()); temp.pop();
        }


    }



    for (int i = 1; i < 10; i++ ){
        std::cout << stacks[i].top();
    }
    std::cout << std::endl;




}


/*
--- Part Two ---
As you watch the crane operator expertly rearrange the crates, you notice the process isn't following your prediction.

Some mud was covering the writing on the side of the crane, and you quickly wipe it away. The crane isn't a CrateMover 9000 - it's a CrateMover 9001.

The CrateMover 9001 is notable for many new and exciting features: air conditioning, leather seats, an extra cup holder, and the ability to pick up and move multiple crates at once.

Again considering the example above, the crates begin in the same configuration:

    [D]    
[N] [C]    
[Z] [M] [P]
 1   2   3 
Moving a single crate from stack 2 to stack 1 behaves the same as before:

[D]        
[N] [C]    
[Z] [M] [P]
 1   2   3 
However, the action of moving three crates from stack 1 to stack 3 means that those three moved crates stay in the same order, resulting in this new configuration:

        [D]
        [N]
    [C] [Z]
    [M] [P]
 1   2   3
Next, as both crates are moved from stack 2 to stack 1, they retain their order as well:

        [D]
        [N]
[C]     [Z]
[M]     [P]
 1   2   3
Finally, a single crate is still moved from stack 1 to stack 2, but now it's crate C that gets moved:

        [D]
        [N]
        [Z]
[M] [C] [P]
 1   2   3
In this example, the CrateMover 9001 has put the crates in a totally different order: MCD.

Before the rearrangement process finishes, update your simulation so that the Elves know where they should stand to be ready to unload the final supplies. After the rearrangement procedure completes, what crate ends up on top of each stack?

Your puzzle answer was QZFJRWHGS.

Both parts of this puzzle are complete! They provide two gold stars: **

At this point, you should return to your Advent calendar and try another puzzle.

If you still want to see it, you can get your puzzle input.

You can also [Share] this puzzle.
*/
