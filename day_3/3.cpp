
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <string>





int main(){
    int ret = 0;

    std::ifstream file("input_3.txt");
    if (file.is_open()){
        std::string line;
        
        std::unordered_map<char, bool> m;

        while (std::getline(file, line)){
            //std::cout << line << "   ||";
            m.clear();
            for (size_t i = 0; i < line.size()/2; i++){
                m[line[i]] = true;
            }
            for (size_t i = line.size() / 2; i < line.size(); i++){
                if (m.find(line[i]) != m.end() && m[line[i]]){

                    /*
                    Lowercase item types a through z have priorities 1 through 26.
                    Uppercase item types A through Z have priorities 27 through 52.
                    ASCI: 65:A   90:Z  (capital)
                          97:a   122:z  (smol)
                    */
                    int nasci = line[i];
                    int tobeadded = 0;
                    if (nasci < 95){  // capital letter
                        tobeadded = nasci - 'A' + 27;
                    }else{   // smol letter
                        tobeadded = nasci - 'a' + 1;
                    }
                    ret += tobeadded;
                    
                    //std::cout << "double letter: " << line[i] << " and we added " << tobeadded << std::endl;
                    break;
                }  // if
            }   // for           
        }
    }

    std::cout << ret << std::endl;
}










