#include "symtable.h"


SymTable::SymTable()
{
}

    
SymTable::~SymTable()
{
}


void SymTable::Add(const string &Key, Value *Val)
{
    //Table.insert(pair<string, Value *>(Key, Val));
    Table[Key] = Val;
}


void SymTable::Add(string Key, Value *Val)
{
    //Table.insert(pair<string, Value *>(Key, Val));
    Table[Key] = Val;
}


Value *SymTable::Find(const string &Key)
{
    map<string, Value *>::iterator Pos = Table.find(Key);
        
    if (Pos == Table.end())
        return NULL;
    else
        return Pos->second;
}


Value *SymTable::Find(string Key)
{
    map<string, Value *>::iterator Pos = Table.find(Key);
        
    if (Pos == Table.end())
        return NULL;
    else
        return Pos->second;
}

    
bool SymTable::Delete(const string &Key)
{
    map<string, Value *>::iterator Pos = Table.find(Key);
        
    if (Pos == Table.end())
        return false;
    
    Table.erase(Pos);
    return true;
}


bool SymTable::Delete(string Key)
{
    map<string, Value *>::iterator Pos = Table.find(Key);
        
    if (Pos == Table.end())
        return false;
    
    Table.erase(Pos);
    return true;
}
