#include <string>
#include "unistd.h"
#include "stdlib.h"
#include <iostream>
#include <map>

//ERROR CODES:
//0: success
//1: general error
//3: invalid action

struct ModeReturn {
    double value;
    unsigned long long int amount;
};

static std::string input;

void readStdin(){
    char ch;
    while(read(STDIN_FILENO, &ch, 1) > 0){
        input += ch;
    }
}

int getNumCount(){
    int numberCount = 1;
    for(int i = 0; i < input.size(); i++){
        if(input[i] == ' '){
            numberCount++;
        }
    }
    return numberCount;
}

double calculateSum(double numbs[], size_t size){
    double sum;
    for(int i = 0; i < size; i++){
        sum += numbs[i];
    }
    return sum;
}

double calculateMax(double numbs[], size_t size){
    double max = -99999999999;
    for(int i = 0; i < size; i++){
        if(numbs[i] > max) max = numbs[i];
    }
    return max;
}

double calculateMin(double numbs[], size_t size){
    double min = 99999999999;
    for(int i = 0; i < size; i++){
        if(numbs[i] < min) min = numbs[i];
    }
    return min;
}

double calculateMean(double numbs[], size_t size){
    return calculateSum(numbs, size) / size;
}

ModeReturn calculateMode(double numbs[], size_t size){
    std::map<double, int> counts;
    for(int i = 0; i < size; i++){
        double curr = numbs[i];
        if(counts.find(curr) != counts.end()){
            counts[curr]++;
        }
        else counts[curr] = 1;
    }
    unsigned long long int max = 0;
    double largest;
    for(auto item = counts.begin(); item != counts.end(); item++){
        if(item->second > max){
            max = item->second;
            largest = item->first;
        }
    }
    return {largest, max};
}

int main(int argc, char** argv){
    //read stdin
    readStdin();
    int numberCount = getNumCount();

    //convert numbers into doubles, and put into list
    double numbers[numberCount];
    int numberIndex = 0;
    std::string currNum;
    for(int i = 0; i < input.size(); i++){
        currNum += input[i];
        if(input[i] == ' '){
            numbers[numberIndex] = atof(currNum.c_str());
            numberIndex++;
            currNum = "";
        }
    }
    if(currNum != ""){
	    numbers[numberIndex] = atof(currNum.c_str());
	    numberIndex++;
	    currNum = "";
    }

    for(int i = 1; i < argc; i++){
        std::cout << argv[i] << ": ";
        std::string action = argv[i];
        if(action == "mean"){
            std::cout << calculateMean(numbers, numberCount) << std::endl;
        }
        else if(action == "sum"){
            std::cout << calculateSum(numbers, numberCount) << std::endl;
        }
        else if(action == "max"){
            std::cout << calculateMax(numbers, numberCount) << std::endl;
        }
        else if(action == "mode"){
            ModeReturn mode = calculateMode(numbers, numberCount);
            std::cout << mode.value << " (" << mode.amount << ")" << std::endl;
        }
        else if(action == "min"){
            std::cout << calculateMin(numbers, numberCount) << std::endl;
        }
        else{
            for(int i = 0; i < action.size() + 2; i++){
                std::cerr << "\b";
            }
            std::cerr << "\033[31mError: " + std::string(argv[i]) + " is not a valid action" << std::endl;
            return 3;
        }
    }

    return 0;
}
