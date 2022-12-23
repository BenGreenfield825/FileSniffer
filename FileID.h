//
// Created by Ben on 2/19/2022.
//

#ifndef SNIFFER_FILEID_H
#define SNIFFER_FILEID_H
#include "tools.hpp"
#include <vector>

using namespace std;

class FileID {
private:
    string filename;
    int iNodeNum;
    string pathname; //append filename to end
    vector<string> searchWords;
public:
    FileID(string fName, string type, string pName, int iNode);
    void print( ostream& out );
    void printWords(ostream& out);
    bool foundWords() {return !searchWords.empty();} //empty returns true if size=0
    void insertSearchWord(string word);
};


#endif //SNIFFER_FILEID_H
