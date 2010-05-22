/* sharestr.c */
#include <stdlib.h>
#include <string.h>

#include "deepc.h"


/* defines */
#define DUMMY_LENGTH 1024       /* it doesn't matter what this is */

/* shared string table. This is global because the purpose of this module
 * is to share strings across the whole program */
SymTable StringTable;
const char *StrEmpty = NULL;


/* symbol table entries can also have a payload attached. This payload 
 * includes the string */
typedef struct _SymTableEntryWithString SymTableEntryWithString;
struct _SymTableEntryWithString
{
    SymTableEntry Entry;        /* the main part of the entry */
    int StringLength;           /* the length of the shared string */
    char String[DUMMY_LENGTH];  /* the string text. we only allocate as much as we need */
};


/* initialise the shared string system */
void ShareStrInit()
{
    SymTableInit(&StringTable, INITIAL_STRING_TABLE_SIZE);
    StrEmpty = ShareStr("");
}


/* free all the shared strings */
void ShareStrClose()
{
    SymTableClose(&StringTable);
}


/* hash function for shared strings */
static unsigned long ShareStrHash(const char *Key, int Len)
{
    unsigned long FirstChars = 0;
    unsigned long LastChars = 0;
    int CopyLen = min((int)sizeof(unsigned long), Len);

    /* take the first four characters */    
    memcpy(&FirstChars, Key, CopyLen);
    
    /* and the last four characters */
    memcpy(&LastChars, Key+Len-CopyLen, CopyLen);
    
    /* and exor them together and with the identifier length */
    return FirstChars ^ (LastChars << 1) ^ Len;
}


/* check a hash table entry for an identifier */
static SymTableEntryWithString *ShareStrSearchIdentifier(SymTable *Tbl, unsigned long HashValue, const char *Key, int Len, int *AddAt)
{
    SymTableEntryWithString *ThisEntry;
    int HashIndex = HashValue % Tbl->Size;
    
    for (ThisEntry = (SymTableEntryWithString *)Tbl->HashTable[HashIndex]; ThisEntry != NULL; ThisEntry = (SymTableEntryWithString *)ThisEntry->Entry.Next)
    {
        if (ThisEntry->StringLength == Len && memcmp(ThisEntry->String, Key, Len) == 0)
            return ThisEntry;   /* found */
    }
    
    *AddAt = HashIndex;    /* didn't find it in the chain */
    return NULL;
}


/* set an identifier and return the identifier. share if possible */
const char *ShareStrSetIdentifier(SymTable *Tbl, const char *Ident, int IdentLen)
{
    int AddAt;
    unsigned long HashValue = ShareStrHash(Ident, IdentLen);
    SymTableEntryWithString *FoundEntry = ShareStrSearchIdentifier(Tbl, HashValue, Ident, IdentLen, &AddAt);
    
    if (FoundEntry != NULL)
        return FoundEntry->String;
    else
    {   
        /* add it to the table - we allocate the entry and just enough 
         * space for the string */
        SymTableEntryWithString *NewEntry = (SymTableEntryWithString *)CheckedAlloc(sizeof(SymTableEntry) + sizeof(int) + IdentLen+1);

        memcpy(&NewEntry->String[0], Ident, IdentLen);
        NewEntry->String[IdentLen] = '\0';
        NewEntry->StringLength = IdentLen;
        NewEntry->Entry.Key = HashValue;
        
        /* insert at the head of this hash list */
        NewEntry->Entry.Next = Tbl->HashTable[AddAt];
        Tbl->HashTable[AddAt] = (SymTableEntry *)NewEntry;

        /* if we're running out of space increase the size of the table */
        Tbl->Entries++;
        if (Tbl->Entries >= Tbl->Size)
            SymTableResize(Tbl);
        
        return &NewEntry->String[0];
    }
}


/* register a string in the shared string store */
const char *ShareStr2(const char *Str, int Len)
{
    return ShareStrSetIdentifier(&StringTable, Str, Len);
}


const char *ShareStr(const char *Str)
{
    return ShareStrSetIdentifier(&StringTable, Str, strlen((char *)Str));
}
