#include <string>
#include "unistd.h"
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

static char outputDelim = ' ';
static char outputActionDelim = '\n';

static bool quiet = false;

static std::string input;
static char delimiter = ' ';

void readStdin(){
    char ch;
    while(read(STDIN_FILENO, &ch, 1) > 0){
        input += ch;
    }
}

int getNumCount(){
    int numberCount = 0;
    for(int i = 0; i < input.size(); i++){
        if(input[i] == delimiter || input[i + 1] == '\0'){
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

template<class item1, class item2>
void swap(item1* one, item2* two){
    int temp = *one;
    *one = *two;
    *two = temp;
}

double* calculateSortMin(double numbs[], size_t size){
    int i, j;
    for(i = 0; i < size - 1; i++)
    for(j = 0; j < size - i - 1; j++){
        if(numbs[j] > numbs[j+1]){
            swap<double, double>(&numbs[j], &numbs[j+1]);
        }
    }
    return numbs;
}
double* calculateSortMax(double numbs[], size_t size){
    int i, j;
    for(i = 0; i < size -1; i++){
        for(j = 0; j < size - i - 1; j++){
            if(numbs[j] < numbs[j+1]){
                swap<double, double>(&numbs[j], &numbs[j+1]);
            }
        }
    }
    return numbs;
}

double calculateMean(double numbs[], size_t size){
    return calculateSum(numbs, size) / size;
}

ModeReturn calculateMode(double numbs[], size_t size){
    std::map<double, int> counts;
    for(int i = 0; i < size; i++){
        double curr = numbs[i];
	try{
	    counts[curr]++;
	} catch(int e){
	    counts[curr] = 1;
	}
    }
    //i mean they could have a ton of numbers ¯\_(ツ)_/¯
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

void helpinfo(){
    //possibly not the best way to make a help menu, but this is the way i like to make a help menu
    std::cout << "USAGE: statcalc [options...] actions...\n\
Numbers must be passed in through stdin. Non numbers are ignored but must be connected to the number eg: 1m\n\
Actions:\n\
    mean\n\
    mode\n\
    min\n\
    max\n\
    sortmin\n\
    sortmax\n\
Options:\n\
    -q:		Do not print the leading 'action:'\n\
    -D arg:	The delimiter to use for the output between numbers\n\
    -A arg:	The delimiter to use for the output between actions\n\
    -d arg: 	The delimiter to use as separater between numbers (default is space)" << std::endl;
}

void parseOpts(int& argc, char**& argv){
    int c;
    while((c = getopt(argc, argv, "d:hqD:A:")) != -1){
        switch(c){
            case 'd':
                delimiter = *optarg;
                break;
	    case 'h':
		helpinfo();
		exit(0);
	    case 'q':
		quiet = true;
		break;
	    case 'D':
		outputDelim = *optarg;
		break;
	    case 'A':
		outputActionDelim = *optarg;
		break;
        }
    }
    argc -= optind - 1;
    argv += optind - 1;
}

int main(int argc, char** argv){
    parseOpts(argc, argv);
    //read stdin
    readStdin();

    int numberCount = getNumCount();
    if(numberCount == 0){
        std::cerr << "No numbers provided" << std::endl;
        return 1;
    }

    //convert numbers into doubles, and put into list
    double numbers[numberCount];
    int numberIndex = 0;
    std::string currNum;
    for(int i = 0; i < input.size(); i++){
        if(input[i] == delimiter || input[i + 1] == '\0'){
            numbers[numberIndex] = atof(currNum.c_str());
            numberIndex++;
            currNum = "";
        }
        else currNum += input[i];
    }

    for(int i = 1; i < argc; i++){
        if(!quiet) std::cout << argv[i] << ": ";
        std::string action = argv[i];
        if(action == "mean"){
            std::cout << calculateMean(numbers, numberCount) << outputActionDelim;
        }
        else if(action == "sum"){
            std::cout << calculateSum(numbers, numberCount) << outputActionDelim;
        }
        else if(action == "max"){
            std::cout << calculateMax(numbers, numberCount) << outputActionDelim;
        }
        else if(action == "mode"){
            ModeReturn mode = calculateMode(numbers, numberCount);
            std::cout << mode.value << " (" << mode.amount << ")" << outputActionDelim;
        }
        else if(action == "min"){
            std::cout << calculateMin(numbers, numberCount) << outputActionDelim;
        }
        else if(action == "sortmax"){
            calculateSortMax(numbers, numberCount);
            for(double i : numbers){
                std::cout << i << outputDelim;
            }
            std::cout << outputActionDelim;
        }
        else if(action == "sortmin"){
            calculateSortMin(numbers, numberCount);
            for(double i : numbers){
                std::cout << i << ' ';
            }
            std::cout << outputActionDelim;
        }
        else{
            for(int i = 0; i < action.size() + 2; i++){
                std::cerr << "\b";
            }
            std::cerr << "\033[31mError: " + std::string(argv[i]) + " is not a valid action\033[0m" << std::endl;
            return 3;
        }
    }
    return 0;
}
