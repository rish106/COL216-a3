typedef long long ll;

#ifndef __CACHE_HPP__
#define __CACHE_HPP__

//cache functions start from here

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
void AddL2(ll memoryblock);
void EvictL2(ll memoryblock);
void WriteL1(ll memoryblock);
void ReadL1(ll memoryblock);
void AddL1(ll memoryblock);
void EvictL1(ll memoryblock);


#endif
