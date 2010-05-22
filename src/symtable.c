/* symtable.c */
#include <stdlib.h>

#include "deepc.h"


/* some good prime-number valued table sizes for efficient hashing. Each 
 * step is about a factor of four increase to minimise the number of times
 * we have to resize the table */
static int TableSize[] = { 17, 67, 257, 1031, 4099, 16411, 65537, 262147, 1048583, 4194319, 16777259, 67108879, 268435459, 1073741827, 2147483629 };


/* symbol table entries can also have a payload attached. This payload 
 * includes a Value */
typedef struct _SymTableEntryWithValue SymTableEntryWithValue;
struct _SymTableEntryWithValue
{
    SymTableEntry Entry;       /* the main part of the entry */
    Value *Val;                /* the value attached to this entry */
};

    
/* initialise a table */
void SymTableInit(SymTable *Tbl, int Size)
{
    /* pick a table size which is prime and bigger than the requested size. */
    /* Primes make great hash table sizes as they distribute the entries */
    /* more randomly */
    for (Tbl->SizeIndex = 0; Size > TableSize[Tbl->SizeIndex]; Tbl->SizeIndex++)
    {}
    
    Tbl->Entries = 0;
    Tbl->Size = TableSize[Tbl->SizeIndex];
    Tbl->HashTable = (SymTableEntry **)CheckedAlloc(Tbl->Size * sizeof(SymTableEntry *));
}


/* free the contents of a table. only the hash table is freed, not the values */
void SymTableClose(SymTable *Tbl)
{
    SymTableEntry *Entry;
    int Count;
    
    /* go through all the hash table rows */
    for (Count = 0; Count < Tbl->Size; Count++)
    {
        /* go through all the entries in this row */
        while (Tbl->HashTable[Count] != NULL)
        {
            Entry = Tbl->HashTable[Count];
            Tbl->HashTable[Count] = Entry->Next;
            free(Entry);
        }
    }
    
    free(Tbl->HashTable);
}


/*
 * NAME:        SymTableResize
 * ACTION:      Resize a hash table to the next larger size
 * PARAMETERS:  SymTable *Tbl - the table
 */
 
void SymTableResize(SymTable *Tbl)
{
    int OldSize;
    int NewSize;
    SymTableEntry **NewHashTable;
    SymTableEntry **TPos;
    SymTableEntry *RowEntry;
    int TCount;
    int HashValue;
    
    /* work out the next bigger table size */
    OldSize = Tbl->Size;
    Tbl->SizeIndex++;
    NewSize = TableSize[Tbl->SizeIndex];
    
    /* make a new table */
    NewHashTable = (SymTableEntry **)CheckedAlloc(NewSize * sizeof(SymTableEntry *));
    
    /* move all the entries across from the old table to the new one */
    for (TCount = 0, TPos = Tbl->HashTable; TCount < OldSize; TCount++, TPos++)
    {
        /* move all the entries in this table row */
        while (*TPos != NULL)
        {
            /* pop an entry from the head of the hash list */
            RowEntry = *TPos;
            *TPos = RowEntry->Next;
            
            /* rehash this entry into the new table */
            HashValue = RowEntry->Key % NewSize;   /* shared strings have unique addresses so we don't need to hash them */
            RowEntry->Next = NewHashTable[HashValue];
            NewHashTable[HashValue] = RowEntry;
        }
    }
    
    /* replace the old table with the new one */
    free(Tbl->HashTable);
    Tbl->HashTable = NewHashTable;
}


/* check a hash table entry for a key */
static SymTableEntryWithValue *SymTableSearch(SymTable *Tbl, const char *Key, int *AddAt)
{
    SymTableEntry *Entry;
    int HashValue = ((unsigned long)Key) % Tbl->Size;   /* shared strings have unique addresses so we don't need to hash them */
    
    for (Entry = Tbl->HashTable[HashValue]; Entry != NULL; Entry = Entry->Next)
    {
        if (Entry->Key == (unsigned long)Key)
            return (SymTableEntryWithValue *)Entry;   /* found */
    }
    
    *AddAt = HashValue;    /* didn't find it in the chain */
    return NULL;
}


/* set an identifier to a value. returns FALSE if it already exists. 
 * Key must be a shared string from TableStrRegister() */
int SymTableSet(SymTable *Tbl, const char *Key, Value *Val)
{
    int AddAt;
    SymTableEntryWithValue *FoundEntry;
    
    /* see if the entry already exists */
    FoundEntry = SymTableSearch(Tbl, Key, &AddAt);
    if (FoundEntry == NULL)
    {
        /* add it to the table */
        SymTableEntryWithValue *NewEntry = (SymTableEntryWithValue *)CheckedAlloc(sizeof(SymTableEntryWithValue));
        NewEntry->Entry.Key = (unsigned long)Key;
        NewEntry->Val = Val;
        NewEntry->Entry.Next = Tbl->HashTable[AddAt];
        Tbl->HashTable[AddAt] = (SymTableEntry *)NewEntry;

        /* if we're running out of space increase the size of the table */
        Tbl->Entries++;
        if (Tbl->Entries >= Tbl->Size)
            SymTableResize(Tbl);
        
        return TRUE;
    }

    return FALSE;
}

/* find a value in a table. returns FALSE if not found. 
 * Key must be a shared string from TableStrRegister() */
int SymTableGet(SymTable *Tbl, const char *Key, Value **Val)
{
    int AddAt;
    SymTableEntryWithValue *FoundEntry = SymTableSearch(Tbl, Key, &AddAt);
    if (FoundEntry == NULL)
        return FALSE;
    
    *Val = FoundEntry->Val;
    return TRUE;
}

/* remove an entry from the table */
Value *SymTableDelete(SymTable *Tbl, const char *Key)
{
    SymTableEntry **EntryPtr;
    int HashValue = ((unsigned long)Key) % Tbl->Size;   /* shared strings have unique addresses so we don't need to hash them */
    
    for (EntryPtr = &Tbl->HashTable[HashValue]; *EntryPtr != NULL; EntryPtr = &(*EntryPtr)->Next)
    {
        if ((*EntryPtr)->Key == (unsigned long)Key)
        {
            Value *Val = ((SymTableEntryWithValue *)(*EntryPtr))->Val;
            *EntryPtr = (*EntryPtr)->Next;
            return Val;
        }
    }

    return NULL;
}
