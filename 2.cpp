
#include <iostream>
#include <fstream>
#include <string>




/*
A for Rock, B for Paper, and C for Scissorsű
X for Rock, Y for Paper, and Z for Scissors   -> ME

1 for Rock, 2 for Paper, and 3 for Scissors)
plus 0 if you lost, 3 if the round was a draw, and 6 if you won



part2:
X means you need to lose, Y means you need to end the round in a draw, and Z means you need to win
*/


int pointsOfCLASH(const char theirs, const char mine){
    // return e + f   e: points for win/lose/draw,  f: points for what you went with
    if (mine == 'X'){
        if (theirs == 'A'){
            return 3 + 1;
        }else if(theirs == 'B'){
            return 0 + 1;
        }else{  // C
            return 6 + 1;   
        }
    }else if(mine == 'Y'){
        if (theirs == 'A'){
            return 6 + 2;
        }else if(theirs == 'B'){
            return 3 + 2;
        }else{  // C
            return 0 + 2;   
        }
    }else{ // Z
        if (theirs == 'A'){
            return 0 + 3;
        }else if(theirs == 'B'){
            return 6 + 3;
        }else{  // C
            return 3 + 3;   
        }
    }
    return -1; // BRAH
}


/*
A for Rock, B for Paper, and C for Scissorsű


1 for Rock, 2 for Paper, and 3 for Scissors)
*/

int pointsOfClash_part2(const char theirs, const char mine){
    // return e + f   e: points for win/lose/draw,  f: points for what you went with
    if (mine == 'X'){    // lose
        if (theirs == 'A'){
            return 0 + 3;
        }else if(theirs == 'B'){
            return 0 + 1;
        }else{  // C
            return 0 + 2;   
        }
    }else if(mine == 'Y'){  // draw
        if (theirs == 'A'){
            return 3 + 1;
        }else if(theirs == 'B'){
            return 3 + 2;
        }else{  // C
            return 3 + 3;   
        }
    }else{ // Z   -> win
        if (theirs == 'A'){
            return 6 + 2;
        }else if(theirs == 'B'){
            return 6 + 3;
        }else{  // C
            return 6 + 1;   
        }
    }
    return -1; // BRAH
}


int main(){
    int score = 0;
    int score_PT2 = 0;

    std::ifstream file("input_2.txt");
    if (file.is_open()){
        std::string line;
        int currentCal = 0;

        while (std::getline(file, line)){
            score += pointsOfCLASH(line[0], line[2]);
            score_PT2 += pointsOfClash_part2(line[0], line[2]);
        }
    }
    std::cout << score << std::endl << score_PT2 << std::endl;
}



/*
--- Day 2: Rock Paper Scissors ---
The Elves begin to set up camp on the beach. To decide whose tent gets to be closest to the snack storage, a giant Rock Paper Scissors tournament is already in progress.

Rock Paper Scissors is a game between two players. Each game contains many rounds; in each round, the players each simultaneously choose one of Rock, Paper, or Scissors using a hand shape. Then, a winner for that round is selected: Rock defeats Scissors, Scissors defeats Paper, and Paper defeats Rock. If both players choose the same shape, the round instead ends in a draw.

Appreciative of your help yesterday, one Elf gives you an encrypted strategy guide (your puzzle input) that they say will be sure to help you win. "The first column is what your opponent is going to play: A for Rock, B for Paper, and C for Scissors. The second column--" Suddenly, the Elf is called away to help with someone's tent.

The second column, you reason, must be what you should play in response: X for Rock, Y for Paper, and Z for Scissors. Winning every time would be suspicious, so the responses must have been carefully chosen.

The winner of the whole tournament is the player with the highest score. Your total score is the sum of your scores for each round. The score for a single round is the score for the shape you selected (1 for Rock, 2 for Paper, and 3 for Scissors) plus the score for the outcome of the round (0 if you lost, 3 if the round was a draw, and 6 if you won).

Since you can't be sure if the Elf is trying to help you or trick you, you should calculate the score you would get if you were to follow the strategy guide.

For example, suppose you were given the following strategy guide:

A Y
B X
C Z
This strategy guide predicts and recommends the following:

In the first round, your opponent will choose Rock (A), and you should choose Paper (Y). This ends in a win for you with a score of 8 (2 because you chose Paper + 6 because you won).
In the second round, your opponent will choose Paper (B), and you should choose Rock (X). This ends in a loss for you with a score of 1 (1 + 0).
The third round is a draw with both players choosing Scissors, giving you a score of 3 + 3 = 6.
In this example, if you were to follow the strategy guide, you would get a total score of 15 (8 + 1 + 6).

What would your total score be if everything goes exactly according to your strategy guide?

Your puzzle answer was 11603.

--- Part Two ---
The Elf finishes helping with the tent and sneaks back over to you. "Anyway, the second column says how the round needs to end: X means you need to lose, Y means you need to end the round in a draw, and Z means you need to win. Good luck!"

The total score is still calculated in the same way, but now you need to figure out what shape to choose so the round ends as indicated. The example above now goes like this:

In the first round, your opponent will choose Rock (A), and you need the round to end in a draw (Y), so you also choose Rock. This gives you a score of 1 + 3 = 4.
In the second round, your opponent will choose Paper (B), and you choose Rock so you lose (X) with a score of 1 + 0 = 1.
In the third round, you will defeat your opponent's Scissors with Rock for a score of 1 + 6 = 7.
Now that you're correctly decrypting the ultra top secret strategy guide, you would get a total score of 12.

Following the Elf's instructions for the second column, what would your total score be if everything goes exactly according to your strategy guide?

Your puzzle answer was 12725.

Both parts of this puzzle are complete! They provide two gold stars: **

At this point, you should return to your Advent calendar and try another puzzle.

If you still want to see it, you can get your puzzle input.

You can also [Share] this puzzle.
*/