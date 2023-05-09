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

ll BLOCKSIZE,L1_Size,L2_Size,L1Sets,L2Sets,L1_Assoc,L2_Assoc; //the paramters of caches
ll L1time=1,L2time=20,Memorytime=200; //access times

//we represent the tag array by a vector of vector of pairs where the first entry is the dirty bit, we ignore the cache array as we dont care
//about values
vector<vector<pair<bool,ll>>> L1Tag;
vector<vector<pair<bool,ll>>> L2Tag;

ll L1ReadHit=0,L1ReadMiss=0,L2ReadHit=0,L2ReadMiss=0;
ll L1WriteHit=0,L1WriteMiss=0,L2WriteHit=0,L2WriteMiss=0;
ll L1update=0,L2update=0;
ll MemoryRead=0,MemoryWrite=0;

//function prototypes
ll MemoryBlock(ll byte);
ll L1set(ll blockindex);
ll L2set(ll blockindex);
ll L1tag(ll blockindex);
ll L2tag(ll blockindex);
void WriteMemory(ll blockindex);
void ReadMemorytoBoth(ll memoryblock);
void ReadMemoryToL2(ll memoryblock);
void WriteL2(ll memoryblock);
void ReadL2(ll memoryblock);
void AddL2(ll memoryblock)
void WriteL1(ll memoryblock);
void ReadL1(ll memoryblock);
void AddL1(ll memoryblock);


//cache functions start from here
ll MemoryBlock(ll byte)
{
    return byte/BLOCKSIZE;
}
ll L1set(ll blockindex)
{
    return blockindex%L1Sets;
}
ll L2set(ll blockindex)
{
    return blockindex%L2Sets;
}
ll L1tag(ll blockindex)
{
    return blockindex/L1Sets;
}
ll L2tag(ll blockindex)
{
    return blockindex/L2Sets;
}

void WriteMemory(ll memoryblock)
{
    MemoryWrite=MemoryWrite+1;
}
void ReadMemorytoBoth(ll memoryblock)
{
    MemoryRead=MemoryRead+1;
    AddL2(memoryblock);
    AddL1(memoryblock);
}

void ReadMemoryToL2(ll memoryblock)
{
    MemoryRead=MemoryRead+1;
    AddL2(memoryblock);
}

void WriteL2(ll memoryblock)
{
    ll index=L2set(memoryblock);
    ll tag=L2tag(memoryblock);
    bool Hit=false;
    for(ll j=0;j<(ll)L2Tag[index].size();j++)
    {
        if(L2Tag[index][j].second == tag)
        {
            Hit=true;
            L2Tag[index][j].first=true;
            break;
        }
    }
    if(Hit == true)
    {
        L2WriteHit=L2WriteHit+1;
    }
    else
    {
        L2WriteMiss=L2WriteMiss+1;
        ReadMemorytoL2(memoryblock);

        //so make sure that the dirty bit of memoryblock is turned on, because ReadMemorytoL2 calls AddL2 and AddL2 doesnt change any
        //block's dirty bit. Basically write on the block after bringing it in the L2 cache
        for(ll j=0;j<(ll)L2Tag[index].size();j++)
        {
            if(L2Tag[index][j].second == tag)
            {
                L2Tag[index][j].first=true;
                break;
            }
        }
    }
}

void ReadL2(ll memoryblock)
{
    ll index=L2set(memoryblock);
    ll tag=L2tag(memoryblock);
    bool Hit=false;
    for(ll j=0;j<(ll)L2Tag[index].size();j++) //L2Tag[index].size() can be atmost L2_Assoc
    {
        //comparing tag entries with tag of memoryblock
        if(L2Tag[index][j].second == tag)
        {
            Hit=true;
            break;
        }
    }
    if(Hit == true)
    {
        L2ReadHit=L2ReadHit+1;
        AddL1(memoryblock);
    }
    //so there was no tag of memoryblock in L1Tag,increment L1ReadMiss and go to L2
    L2ReadMiss=L2ReadMiss+1;
    ReadMemorytoBoth(memoryblock);   
}

void AddL2(ll memoryblock)
{
    L2update=L2update+1;
    //so we are sure that this block is not there in L2 cache
    ll index=L2set(memoryblock);
    ll tag=L2tag(memoryblock);
    if((ll) L2Tag[index].size() == L2_Assoc)
    {
        //so eviction needs to be done
        pair<bool,ll> evicted;
        vector<pair<bool,ll>> tempTag;
        //now using the LRU policy
        for(ll j=1;j<(ll)L2Tag[index].size();j++) tempTag.push_back(L2Tag[index][j]);
        tempTag.push_back({false,tag});
        evicted=L2Tag[index][0];
        L2Tag[index]=tempTag;

        if(evicted.first == true)
        {
            //dirty bit is on
            ll evicted_memoryblock=L2Sets*evicted.second+index;
            WriteMemory(evicted_memoryblock);
        }
    }

    else
    {
        //no eviction required
        L2Tag[index].push_back({false,tag});
    } 
}

void WriteL1(ll memoryblock)
{
    ll index=L1set(memoryblock);
    ll tag=L1tag(memoryblock);
    bool Hit=false;

    for(ll j=0;j<(ll)L1Tag[index].size();j++)
    {
        if(L1Tag[index][j].second == tag)
        {
            Hit=true;
            L1Tag[index][j].first=true;
            break;
        }
    }

    if(Hit == true)
    {
        L1WriteHit=L1WriteHit+1;
    }
    else
    {
        L1WriteMiss=L1WriteMiss+1;
        ReadL2(memoryblock);
        //now turn the dirty bit of memoryblock on,basically write on the block that is brough into L1
        for(ll j=0;j<(ll)L1Tag[index].size();j++)
        {
            if(L1Tag[index][j].second == tag)
            {
                L1Tag[index][j].first=true;
                break;
            }
        }
    }
}

void ReadL1(ll memoryblock)
{
    ll index=L1set(memoryblock);
    ll tag=L1tag(memoryblock);
    bool Hit=false;
    for(ll j=0;j<(ll)L1Tag[index].size();j++) //L1Tag[index].size() can be atmost L1_Assoc
    {
        //comparing tag entries with tag of memoryblock
        if(L1Tag[index][j].second == tag)
        {
            Hit=true;
            break;
        }
    }
    if(Hit)
    {
        L1ReadHit=L1ReadHit+1;
    }
    else
    {
        //so there was no tag of memoryblock in L1Tag,increment L1ReadMiss and go to L2
        L1ReadMiss=L1ReadMiss+1;
        ReadL2(memoryblock);
    }
}

void AddL1(ll memoryblock)
{
    L1update=L1update+1;
    //so we are sure that this block is not there in L1 cache
    ll index=L1set(memoryblock);
    ll tag=L1tag(memoryblock);
    if((ll) L1Tag[index].size() == L1_Assoc)
    {
        //so eviction needs to be done as all ways are filled
        pair<bool,ll> evicted;
        vector<pair<bool,ll>> tempTag;
        //now using the LRU policy
        for(ll j=1;j<(ll)L1Tag[index].size();j++) tempTag.push_back(L1Tag[index][j]);
        tempTag.push_back({false,tag});
        evicted=L1Tag[index][0];
        L1Tag[index]=tempTag;

        if(evicted.first == true)
        {
            //dirty bit is on
            ll evicted_memoryblock=L1Sets*(evicted.second)+index;
            WriteL2(evicted_memoryblock);
        }
    }

    else
    {
        //no eviction required
        L1Tag[index].push_back({false,tag});
    }
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
    L1Sets=(L1_Size)/(BLOCKSIZE*L1_Assoc);
    L2Sets=(L2_Size)/(BLOCKSIZE*L2_Assoc);
    L1Tag.resize(L1Sets);
    L2Tag.resize(L2Sets);

    for(ll i=0;i<(ll)traces.size();i++)
    {
        ll memoryblock=MemoryBlock(traces[i].second);
        if(traces[i].first == 'r') ReadL1(memoryblock);
        else WriteL1(memoryblock);
    }

    L1Tag.clear();
    L2Tag.clear();
    return 0;
}
