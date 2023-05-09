#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <boost/tokenizer.hpp>

typedef long long ll;

using namespace std;

vector< pair<char, ll> > traces;
string::size_type sz = 0;   // alias of size_t

// parse single line
void parse_trace(std::string line) {
    boost::tokenizer<boost::char_separator<char>> tokens(line, boost::char_separator<char>(", \t"));
    char instruction;
    ll address;
    for(auto it = tokens.begin(); it != tokens.end(); it++) {
        if (it == tokens.begin()) {
            instruction = (*it)[0];
        } else {
            address = stoll("0x" + *it, &sz, 0);
        }
    }
    cout << instruction << ' ' << address << '\n';
    traces.push_back(make_pair(instruction, address));
}

// construct the traces vector from the input file
void construct_traces(std::ifstream &file) {
    string line;
    while (getline(file, line)) {
        parse_trace(line);
    }
    file.close();
}


int main (int argc, char *argv[]) {
    // if (argc != 7) {
    //     cerr << "required arguments: blocksize, l1_size, l1_ssoc, l2_size, l2_assoc, filename\n";
    //     return 1;
    // }
    // int BLOCKSIZE = argv[1];
    // int L1_Size = argv[2];
    // int L1_Assoc = argv[3];
    // int L2_Size = argv[4];
    // int L2_Assoc = argv[5];
    // string filename = argv[6];
    ll BLOCKSIZE = 64;
    ll L1_Size = 1024;
    ll L1_Assoc = 2;
    ll L2_Size = 65536;
    ll L2_Assoc = 8;
    string filename = "memory_trace_files/trace1.txt";
    ifstream file(filename);
    if (file.is_open()) {
        // do caching
        construct_traces(file);
    } else {
        cerr << "File could not be opened. Terminating...\n";
        return 1;
    }

    return 0;
}
