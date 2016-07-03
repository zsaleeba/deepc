#ifndef SYMTABLE_H
#define SYMTABLE_H

#include <string>
#include <map>

class Value;

using namespace std;


class SymTable
{
    map<string, Value *> Table;

public:
    SymTable();
    ~SymTable();
    
    void Add(const string &Key, Value *Val);
    void Add(string Key, Value *Val);
    
    Value *Find(const string &Key);
    Value *Find(string Key);
    
    bool Delete(const string &Key);
    bool Delete(string Key);
};

#endif /* SYMTABLE_H */
