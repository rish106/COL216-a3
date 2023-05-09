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

ll BLOCKSIZE,L1_Size,L2_Size,L1_Assoc,L2_Assoc; //the paramters of caches
ll L1time=1,L2time=20,Memorytime=200; //access times

//we represent the caches by a vector of vectors and tags by a vector of vector of pairs where the first entry is the dirty bit
vector<vector<ll>> L1Cache;
vector<vector<pair<bool,ll>>> L1Tag;
vector<vector<ll>> L2Cache;
vector<vector<pair<bool,ll>>> L2Tag;

ll L1ReadHit=0,L1ReadMiss=0,L2ReadHit=0,L2ReadMiss=0;
ll L1WriteHit=0,L1WriteMiss=0,L2WriteHit=0,L2WriteMiss=0;
ll MemoryRead=0,MemoryWrite=0;

//function prototypes
ll MemoryBlock(ll byte);
ll L1set(ll blockindex);
ll L2set(ll blockindex);
ll L1tag(ll blockindex);
ll L2tag(ll blockindex);
void MemoryAccess(ll blockindex);
void WriteL2(ll memoryblock);
void ReadL2(ll memoryblock);
void addL2(ll memoryblock)
void WriteL1(ll memoryblock);
void ReadL1(ll memoryblock);
void addL1(ll memoryblock)


//cache functions start from here
ll MemoryBlock(ll byte)
{
    return byte/BLOCKSIZE;
}
ll L1set(ll blockindex)
{
    return blockindex%L1_Size;
}
ll L2set(ll blockindex)
{
    return blockindex%L2_Size;
}
ll L1tag(ll blockindex)
{
    return blockindex/L1_Size;
}
ll L2tag(ll blockindex)
{
    return blockindex/L2_Size;
}

void WriteMemory(ll memoryblock)
{

}
void ReadMemory(ll memoryblock)
{
    MemoryRead=MemoryRead+1;
    AddL2(memoryblock);
    AddL1(memoryblock);
}

void WriteL2(ll memoryblock)
{

}

void ReadL2(ll memoryblock)
{
    ll index=L2set(memoryblock);
    ll tag=L2tag(memoryblock);
    bool Hit=false;
    for(ll j=0;j<(int)L2Tag[index].size();j++) //L2Tag[index].size() can be atmost L2_Assoc
    {
        //comparing tag entries with tag of memoryblock
        if(L2Tag[index][j]==tag)
        {
            Hit=true;
            break;
        }
    }
    if(Hit)
    {
        L2ReadHit=L2ReadHit+1;
        return;
    }
    //so there was no tag of memoryblock in L1Tag,increment L1ReadMiss and go to L2
    L2ReadMiss=L2ReadMiss+1;
    ReadMemory(memoryblock);   
}

void AddL2(ll memoryblock)
{
    //so we are sure that this block is not there in L1 cache
    ll index=L2set(memoryblock);
    bool Evict=false;
    
}

void WriteL1(ll memoryblock)
{

}

void ReadL1(ll memoryblock)
{
    ll index=L1set(memoryblock);
    ll tag=L1tag(memoryblock);
    bool Hit=false;
    for(ll j=0;j<(int)L1Tag[index].size();j++) //L1Tag[index].size() can be atmost L1_Assoc
    {
        //comparing tag entries with tag of memoryblock
        if(L1Tag[index][j]==tag)
        {
            Hit=true;
            break;
        }
    }
    if(Hit)
    {
        L1ReadHit=L1ReadHit+1;
        return;
    }
    //so there was no tag of memoryblock in L1Tag,increment L1ReadMiss and go to L2
    L1ReadMiss=L1ReadMiss+1;
    ReadL2(memoryblock);
}

void AddL1(ll memoryblock)
{

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
    BLOCKSIZE = 64;
    L1_Size = 1024;
    L1_Assoc = 2;
    L2_Size = 65536;
    L2_Assoc = 8;
    string filename = "memory_trace_files/trace1.txt";
    ifstream file(filename);
    if (file.is_open()) {
        // do caching
        construct_traces(file);
    } else {
        cerr << "File could not be opened. Terminating...\n";
        return 1;
    }

    //code starts from here
    L1Cache.resize(L1Size),L1Tag.resize(L1Size);
    L2Cache.resize(L2Size); L2Tag.resize(L2Size);

    for(ll i=0;i<(ll)traces.size();i++)
    {
        ll memoryblock=MemoryBlock(traces[i].second);
        if(traces[i].first == 'r') ReadL1(memoryblock);
        else WriteL1(memoryblock);
    }

    return 0;
}
