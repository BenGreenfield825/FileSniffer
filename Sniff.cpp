//
// Created by Ben on 2/19/2022.
//

#include "Sniff.h"

//not sure that I like how I set pathname here
Sniff::Sniff(Params *p): current(), pathname(p->getPath()), words(), params(p), outputFile(p->getOutputStreamName()){
    outFile.open(outputFile, ios::app); //open in append mode so we get the options output from Params
    istringstream ssWrapper(params->getSearchWord()); //wrap search words in string stream
    string tempWord;
    for(;;){
        ssWrapper >> tempWord;
        words.push_back(tempWord);
        if(ssWrapper.eof()) break;
    }
}

void Sniff::oneDir() {
    cout << "Path: " << pathname << endl;
    DIR * dir;  //make directory stream
    dir = opendir(pathname);    //open directory
    if(dir == nullptr) {    //dir should be nullptr if not able to open
        fatal("Cannot open directory stream!\n");
    }
    //get "rid of" the first two entries, . and ..
    dStruct = readdir(dir);
    dStruct = readdir(dir);
    for(;;) {
        dStruct = readdir(dir);
        if(errno > 0){
            cerr << "Error! Continuing!" << endl;
            continue;
        }
//        cout << "errno: " << strerror(errno) << errno << endl;
//        cout << "dstruct name test: " << dStruct->d_name << endl;
//        cout << "dstruct type test: " << dStruct->d_type << endl;
//        cout << "dstruct iNode test: " << dStruct->d_ino << endl;
        if(dStruct == nullptr) break;
        //the macros did not work with dStruct->d_type, had to use stat
        struct stat sb;
        stat(dStruct->d_name, &sb);
        //check to see if we are looking at a directory or regular file entry
//        if(S_ISDIR(dStruct->d_type) || S_ISREG(dStruct->d_type))
        if(S_ISDIR(sb.st_mode) || S_ISREG(sb.st_mode)) {
            //your instructions mention a verbatim switch, I assume you meant verbose?
            if(params->getVerbose()) {
                cout << dStruct->d_name << endl;
            }
            //if specifically a directory
            if(S_ISDIR(sb.st_mode)){
//                string temp;
//                temp = string(pathname) + '/' + string(dStruct->d_name);
//                travel(string(pathname) + '/' + string(dStruct->d_name));
//                travel(temp);
            }
            //if specifically a regular file
            if(S_ISREG(sb.st_mode)) {
                FileID *fileId;
                fileId = oneFile(dStruct->d_name, to_string(sb.st_mode), dStruct->d_ino);
                if(fileId->foundWords()){
                    susFiles.push_back(*fileId);
                }
            }
        }
    }
    closedir(dir);  //close dir stream
    cout << "Finished searching directory!" << endl;
}

FileID* Sniff::oneFile(string fName, string type, int iNode) {
    FileID* fileID;
    fileID = new FileID(fName, type, pathname, iNode);
    ifstream inputFile;
    inputFile.open(fName);
    if(!inputFile) cerr << "Cannot open file!" << endl; //todo: replace with fatal?
    string current;
    while(inputFile >> current) {
//        cout << "Current word: " << current << endl;
        current = stripNonAlpha(current);
        checkWordMatch(fileID, current);
    }
    inputFile.close();
    return fileID;
}

string Sniff::stripNonAlpha(string str) {
    for(int j=0; j < str.size(); j++) {
        if(!isalnum(str[j])) {
            //erase 1 character starting from current position
            str.erase(j, 1);
        }
    }
    return str;
}

string Sniff::makeStringLower(string str) {
    for(int j=0; j < str.size(); j++) {
        str[j] = tolower(str[j]);
    }
    return str;
}

void Sniff::checkWordMatch(FileID *fileId, string current) {
    //insertSearchWord will check for duplicates before adding to internal vector
    for(int y=0; y<words.size(); y++) {
        if(params->getCaseSensitive()){
            if(current == words[y]) {
                fileId->insertSearchWord(current);
            }
        }
        else{
            if(makeStringLower(current) == makeStringLower(words[y])) {
                fileId->insertSearchWord(current);
            }
        }
    }
}

void Sniff::run(char* startDir) {
    current = startDir;
    pathname = startDir;
    chdir(current);
    travel(current, current);
    chdir("..");
    printFoundFiles();
}

void Sniff::travel(const string& path, string name) {
    ///Currently, most of this is copy paste from oneDir(). I had to make a copy since I was running into
    ///so much trouble getting it to work. I plan on changing travel() to utilize oneDir() for the next program
    ///to avoid having as much repeated code

    char *tempPath = new char[path.size() + 1];
    std::copy(path.begin(), path.end(), tempPath);
    tempPath[path.size()] = '\0';
    char *tempCharArr = new char[name.size() + 1];
    copy(name.begin(), name.end(), tempCharArr);
    tempCharArr[name.size()] = '\0';

    cout << "Path: " << path << endl;
    outFile << "Path: " << path << endl;
    DIR * dir;  //make directory stream
    dir = opendir(tempCharArr);    //open directory
    if(dir == nullptr) {    //dir should be nullptr if not able to open
        fatal("Cannot open directory stream!\n");
    }
    struct stat sb{};
    //get "rid of" the first two entries, . and ..
    dStruct = readdir(dir);
    dStruct = readdir(dir);
    for(;;) {
        chdir(tempPath);    //need to chdir() so that stat can find the file we're referencing
        dStruct = readdir(dir);
        if(dStruct == nullptr) {
//            cout << "dStruct hit null. Break!" << endl;
            break;
        }
        cout << "Name of item found: " << dStruct->d_name << endl;
        if(errno > 0){
            cerr << "Errno code " << errno << ". Continuing!" << endl;
//            break;  //todo: change back to continue (maybe). Update: having break or continue makes it explode so uhh
        }

        stat(dStruct->d_name, &sb);
        //check to see if we are looking at a directory or regular file entry
//        if(S_ISDIR(dStruct->d_type) || S_ISREG(dStruct->d_type))
        if(S_ISDIR(sb.st_mode) || S_ISREG(sb.st_mode)) {
            //your instructions mention a verbatim switch, I assume you meant verbose?
            if(params->getVerbose()) {
                cout << dStruct->d_name << endl;
            }
            //if specifically a directory
            if(S_ISDIR(sb.st_mode)){
                name = dStruct->d_name;
                cout << "Found a sub directory: " << name << endl;
                travel(path + "/" + dStruct->d_name, name);
            }
            //if specifically a regular file
            if(S_ISREG(sb.st_mode)) {
                FileID *fileId;
                fileId = oneFile(dStruct->d_name, to_string(sb.st_mode), dStruct->d_ino);
                if(fileId->foundWords()){
                    susFiles.push_back(*fileId);
                }
            }
        }
    }
    closedir(dir);  //close dir stream
    cout << "Finished searching directory!" << endl;
    outFile << "Finished searching directory!" << endl;
}

void Sniff::printFoundFiles() {
    cout << "\nFiles that contained search words:";
    outFile << "\nFiles that contained search words:";
    for(int j=0; j<susFiles.size(); j++) {
        cout << endl;
        cout << "---------------------------------------------------------------" << endl;
        susFiles[j].print(cout);
        susFiles[j].printWords(cout);

        outFile << endl;
        outFile << "---------------------------------------------------------------" << endl;
        susFiles[j].print(outFile);
        susFiles[j].printWords(outFile);
    }
    outFile << endl << endl;    //make room at the bottom for next run
}

