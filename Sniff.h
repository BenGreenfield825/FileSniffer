//
// Created by Ben on 2/19/2022.
//

#ifndef SNIFFER_SNIFF_H
#define SNIFFER_SNIFF_H
#include "tools.hpp"
#include "Params.h"
#include "FileID.h"
#include <vector>
#include <dirent.h>
#include <sys/stat.h>

class Sniff {
private:
    Params *params;
    char *outputFile = nullptr;
    ofstream outFile;
    char* current; //initialized from params
    char* pathname; //initially same as directory from params
    vector<string> words;
    vector<FileID> susFiles;
    //I think you do the struct like this
    dirent* dStruct;
//    struct dirent * readdir(DIR* dirp) {}

public:
    Sniff(Params *p);
    void oneDir();
    FileID* oneFile(string fName, string type, int iNode);
    string stripNonAlpha(string str);
    string makeStringLower(string str);
    void checkWordMatch(FileID *fileId, string current);
    void run(char* startDir);
    void travel(const string& path, string name);
    void printFoundFiles();
    ~Sniff() {susFiles.clear(); outFile.close();}
};


#endif //SNIFFER_SNIFF_H
