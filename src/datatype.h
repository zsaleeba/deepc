#ifndef DATATYPE_H
#define DATATYPE_H

/* data types */
enum BasicType
{
    BasicTypeUnknown,               /* no type has been given */
    BasicTypeVoid,                  /* a type with no data */
    BasicTypeBool,                  /* a boolean value */
    BasicTypeChar,                  /* a character */
    BasicTypeInt,                   /* integer */
    BasicTypeDouble,                /* double precision floating point */
    BasicTypeComplex,               /* a complex double precision floating point */
    BasicTypeImaginary,             /* an imaginary double precision floating point */
    BasicTypeFunction,              /* a function */
    BasicTypePointer,               /* a pointer */
    BasicTypeArray,                 /* an array of a sub-type */
    BasicTypeStruct,                /* aggregate type */
    BasicTypeUnion,                 /* merged type */
    BasicTypeEnum                   /* enumerated integer type */
};

class DataType
{
    enum BasicType Basic;           /* what kind of type this is */
    int TypeOpt;                    /* options for this type from enum TypeOptEnum */
    int Sizeof;                     /* the memory size of this type */
    int ArraySize;                  /* the size of an array type */
    string Identifier;              /* the name of a struct or union */
    DataType *FromType;             /* the type we're derived from (or NULL) */
    //DataType *DerivedTypeList;      /* first in a list of types derived from this one */
    //DataType *Next;                 /* next item in the derived type list */
    //SymTable *Members;              /* members of a struct or union */
    
public:
    DataType();
    
    // make a value of this type
    virtual Value *CreateValue();
};

#endif /* DATATYPE_H */
