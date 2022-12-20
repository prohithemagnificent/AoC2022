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
#include <set>

struct perry{
    long long int index;
    long long int value;

    // constructor
    perry(long long int idx, long long int val){
        index = idx;
        value = val;
    }
};

void addNToArrayBetween2indices(std::vector<perry>& array, long long int n, long long int i_start, long long int i_end){
    // including i_start anmd i_end, add to array[i].index
    auto it = array.begin();
    while (it != array.end()){
        long long int idx = (*it).index;
        if (idx >= i_start && idx <= i_end){
            (*it).index += n;
        }
        it++;
    }
}



int main(){

    auto startTime = std::chrono::steady_clock::now();

    const long long int decription_key = 811589153;
    std::vector<long long int> v;
    std::ifstream file("input.txt");
    if (file.is_open()){
        std::string line; 
        while (std::getline(file, line)){
            v.push_back(decription_key * std::stoi(line));
        } 
    }

    //std::set<long long int> s {v.begin(), v.end()};  // set size not same as v size -> there are multiples of some elements

    const long long int N = v.size();
    // vector of indices and values
    std::vector<perry> array;
    for (long long int i = 0; i < N; i++){
        perry currP = perry(i, v[i]);
        array.push_back(currP);
    }
    
    for (long long int p2 = 0; p2 < 10; p2++){
        for (long long int i = 0; i < N; i++){
            long long int idx_from = array[i].index;
            long long int val = array[i].value;

            
            long long int newpos;

            if (val > 0){
                newpos = idx_from + val;
                while (newpos >= N){
                    newpos -= N;
                    newpos++;
                }
            }else{  // val < 0
                newpos = idx_from + val;
                while (newpos < 0){
                    newpos += N;
                    newpos--;
                }
            }
            long long int diff = newpos - idx_from;

            if (val == 0){
                // nothing
            }else if (diff > 0){
                
                addNToArrayBetween2indices(array, -1, idx_from + 1, idx_from + diff);
                array[i].index  += diff;
            }else{   // diff < 0
                
                addNToArrayBetween2indices(array, +1, idx_from + diff, idx_from - 1);
                array[i].index  += diff;
            }

        }
    }

    long long int res = 0;
    long long int idx_of_0 = -1;
    for (long long int i = 0; i < N; i++){
        if (array[i].value == 0){
            idx_of_0 = array[i].index;
            break;
        }
    }

    for (long long int i = 0; i < N; i++){
        if (array[i].index == (idx_of_0 + 1000) % N){
            res += array[i].value;
        }
        if (array[i].index == (idx_of_0 + 2000) % N){
            res += array[i].value;
        }
        if (array[i].index == (idx_of_0 + 3000) % N){
            res += array[i].value;
        }
    }



    std::cout << res << std::endl; // 570068847 toolow


    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - startTime).count();
    std::cout << "Elapsed time (ms)= " << duration << std::endl;
}

