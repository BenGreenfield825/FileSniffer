//
// Created by Ben on 2/7/2022.
//

#ifndef SNIFFER_PARAMS_H
#define SNIFFER_PARAMS_H

#include <fstream>
#include <iostream>
#include <getopt.h>
#include "tools.hpp"

using namespace std;

class Params {
private:
    char *pathName = nullptr; //use char pointers to hold string
    char *outputFile = nullptr;
    ofstream outFile;
    bool verbose = false;
    bool caseSensitive = false;
    string searchWord;
public:
    Params(int argc, char * argv[]);
    void print();
    void usage();
    string getSearchWord() {return searchWord;}
    char* getPath() {return pathName;}
    bool getVerbose() {return verbose;}
    bool getCaseSensitive() {return caseSensitive;}
    char* getOutputStreamName() {return outputFile;}
    ~Params();
};


#endif //SNIFFER_PARAMS_H
