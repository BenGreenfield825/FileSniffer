#include <iostream>
#include "Params.h"
#include "Sniff.h"

using namespace std;

int main(int argc, char * argv[]) {
    banner();
    Params params(argc, argv);
    params.print();
    Sniff sniff(&params);
//    chdir(params.getPath());
//    sniff.oneDir();
    sniff.run(params.getPath());

    bye();
return 0;
}
