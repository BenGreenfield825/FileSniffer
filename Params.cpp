//
// Created by Ben on 2/7/2022.
//
#include "Params.h"

struct option longOpts[] = {
        {"verbose", no_argument, nullptr},
        {"case", no_argument, nullptr, 'c'},
        {"output", required_argument, nullptr, 'o'},
        {"directory", required_argument, nullptr, 'd'},
        {"usage", optional_argument, nullptr, 'u'},
        {nullptr, no_argument, nullptr, 0}
};
const char* optString = "co:d:u";

Params::Params(int argc, char * argv[]) {
    int opt;    //was not letting me use a char of any sort
    int optx;
    int *codeP = nullptr;
    for(;;) {
        opt = getopt_long(argc, argv, optString, longOpts, codeP);  //did not like argv being constant, so removed it
        if(opt == -1) break;
        switch(opt){
            case 'c': {
                caseSensitive = true;
                break;
            }
            case 'o': {
                outputFile = optarg;
                outFile.open(outputFile, ios::app);   //open file
                break;
            }
            case 'd': {
                pathName = optarg;
//                cout << "Path name " << pathName << endl;
                break;
            }
            case 'u': {
                usage();
                break;
            }
            case '?': { //I am not sure if it is better to use just '?' case or just default case to avoid repetition
                usage();
                break;
            }
            default: {
                usage();
                break;
            }
        }
        //handle rest of arguments
        for(optx = optind; optx < argc; ++optx) {
            //this assumes that the search word is at the end which could lead to errors
            if(optx == argc-1){
                searchWord = string(argv[optx]);
            }
        }
    }
    if(searchWord.empty()) {
        fatal("No search word!");
    }
}

void Params::print() {
    cout << "Start at: " << pathName << endl;
    cout << "Output file name: " << outputFile << endl;
    cout << "Verbose: " << (verbose ? "yes" : "no") << endl;
    cout << "Case sensitive: " << (caseSensitive ? "yes" : "no") << endl;
    cout << "Search word: " << searchWord << endl;
    cout << "---------------------------------------------------------------" << endl;

    fbanner(outFile);
    outFile << "Start at: " << pathName << endl;
    outFile << "Output file name: " << outputFile << endl;
    outFile << "Verbose: " << (verbose ? "yes" : "no") << endl;
    outFile << "Case sensitive: " << (caseSensitive ? "yes" : "no") << endl;
    outFile << "Search word: " << searchWord << endl;
    outFile << "---------------------------------------------------------------" << endl;
}

void Params::usage() {
    cout << "Usage: " << endl;
    cout << "Options: [-codu]" << endl;
    cout << "[--verbose]" << endl;
    cout << "[--case|-c] - Mark as case sensitive" << endl;
    cout << "[--output|-o] - Output file name" << endl;
    cout << "[--directory|-d] - Output directory" << endl;
    cout << "[--usage|-u] - Lists legal switches" << endl;
}

Params::~Params() {
    outFile.close();
}
