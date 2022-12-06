#include <iostream>
#include <fstream>
#include <deque>
#include <string>
#include <unordered_map>
#include <algorithm>



int main(){

    std::string s;
    std::ifstream file("input_6.txt");
    if (file.is_open()){
        std::string line; 
        while (std::getline(file, line)){
            s = line;
        } 
    }


   

    std::deque<char> q;

    int ret = -1;
    for (size_t i = 0; i < s.size(); i++){
        char c = s[i];
        if (std::find(q.begin(), q.end(), c) != q.end()){
            // if already in queue: pop from left until we pop this char
            while (q.front() != c){
                q.pop_front();
            }
            q.pop_front(); // pop this c from begin
            q.push_back(c); // add to end

        }else{
            q.push_back(c);
        }
        if (q.size() == 14){
            ret = i;
            break;
        }

    }
    // + 1 because 1 based indexing
    std::cout << ret + 1 << std::endl;  // 1356
}