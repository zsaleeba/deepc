/* type.c */
#include <stdlib.h>

#include "deepc.h"

/* some basic types */
DataType *TypeUber;
DataType *TypeUnknown;
DataType *TypeVoid;
DataType *TypeBool;
DataType *TypeUChar;
DataType *TypeSChar;
DataType *TypeUShort;
DataType *TypeSShort;
DataType *TypeUInt;
DataType *TypeSInt;
DataType *TypeULong;
DataType *TypeSLong;
DataType *TypeFloat;
DataType *TypeDouble;
DataType *TypeComplex;
DataType *TypeImaginary;
DataType *TypeCharPtr;
DataType *TypeCharArray;
DataType *TypeVoidPtr;


/* add a new type to the set of types we know about */
DataType *DataTypeAdd(ParseState *Parser, DataType *ParentType, enum BasicType Basic, int TypeOpt, int ArraySize, const char *Identifier, int Sizeof)
{
    DataType *NewType = (DataType *)CheckedAlloc(sizeof(DataType));
    DataType **SiblingListPtr = (ParentType == NULL) ? &TypeUber->DerivedTypeList : &ParentType->DerivedTypeList;
    NewType->Basic = Basic;
    NewType->Sizeof = Sizeof;
    NewType->ArraySize = ArraySize;
    NewType->Identifier = Identifier;
    NewType->Members = NULL;
    NewType->FromType = ParentType;
    NewType->DerivedTypeList = NULL;
    NewType->Next = *SiblingListPtr;
    *SiblingListPtr = NewType;
    
    return NewType;
}


/* add a basic type */
DataType *DataTypeAddBasicType(enum BasicType Basic, int TypeOptions, int Sizeof)
{
    return DataTypeAdd(NULL, NULL, Basic, TypeOptions, 0, StrEmpty, Sizeof);
}


/* given a parent type, get a matching derived type and make one if necessary */
DataType *DataTypeGetMatching(ParseState *Parser, DataType *ParentType, enum BasicType Basic, int TypeOptions, int ArraySize, const char *Identifier)
{
    int Sizeof;
    DataType *ThisType;
    
    /* look for a matching type */
    if (ParentType == NULL)
        ThisType = TypeUber->DerivedTypeList;
    else
        ThisType = ParentType->DerivedTypeList;
    
    while (ThisType != NULL && (ThisType->Basic != Basic || ThisType->ArraySize != ArraySize || ThisType->Identifier != Identifier || ThisType->TypeOpt != TypeOptions))
        ThisType = ThisType->Next;
    
    if (ThisType != NULL)
        return ThisType; /* found it */
    
    /* make a new one */
    switch (Basic)
    {
        case BasicTypePointer:  Sizeof = sizeof(void *); break;
        case BasicTypeArray:    Sizeof = ArraySize * ParentType->Sizeof; break;
        case BasicTypeEnum:     Sizeof = sizeof(int); break;
        default:                Sizeof = 0; break;      /* structs and unions will get bigger when we add members to them */
    }

    return DataTypeAdd(Parser, ParentType, Basic, TypeOptions, ArraySize, Identifier, Sizeof);
}


/* initialise the type system */
void DataTypeInit()
{
    TypeUber = (DataType *)CheckedAlloc(sizeof(DataType));
    TypeUnknown = DataTypeAddBasicType(BasicTypeUnknown, TypeOptNone, 0);
    TypeVoid = DataTypeAddBasicType(BasicTypeVoid, TypeOptNone, 0);
    TypeBool = DataTypeAddBasicType(BasicTypeBool, TypeOptNone, sizeof(unsigned char));
    TypeUChar = DataTypeAddBasicType(BasicTypeChar, TypeOptUnsigned, sizeof(unsigned char));
    TypeSChar = DataTypeAddBasicType(BasicTypeChar, TypeOptSigned, sizeof(signed char));
    TypeUShort = DataTypeAddBasicType(BasicTypeInt, TypeOptUnsigned | TypeOptShort, sizeof(unsigned short));
    TypeSShort = DataTypeAddBasicType(BasicTypeInt, TypeOptSigned | TypeOptShort, sizeof(signed short));
    TypeUInt = DataTypeAddBasicType(BasicTypeInt, TypeOptUnsigned, sizeof(unsigned int));
    TypeSInt = DataTypeAddBasicType(BasicTypeInt, TypeOptSigned, sizeof(signed int));
    TypeULong = DataTypeAddBasicType(BasicTypeInt, TypeOptUnsigned | TypeOptLong, sizeof(unsigned long));
    TypeSLong = DataTypeAddBasicType(BasicTypeInt, TypeOptSigned | TypeOptLong, sizeof(signed long));
    TypeFloat = DataTypeAddBasicType(BasicTypeDouble, TypeOptShort, sizeof(float));
    TypeDouble = DataTypeAddBasicType(BasicTypeDouble, TypeOptNone, sizeof(double));
    TypeComplex = DataTypeAddBasicType(BasicTypeComplex, TypeOptNone, 2 * sizeof(double));
    TypeImaginary = DataTypeAddBasicType(BasicTypeImaginary, TypeOptNone, sizeof(double));
    TypeCharArray = DataTypeAdd(NULL, TypeUChar, BasicTypeArray, TypeOptNone, 0, StrEmpty, sizeof(unsigned char));
    TypeCharPtr = DataTypeAdd(NULL, TypeUChar, BasicTypePointer, TypeOptNone, 0, StrEmpty, sizeof(void *));
    TypeVoidPtr = DataTypeAdd(NULL, TypeVoid, BasicTypePointer, TypeOptNone, 0, StrEmpty, sizeof(void *));
}


/* deallocate heap-allocated types */
void DataTypeCleanupNode(DataType *Typ)
{
    DataType *SubType;
    DataType *NextSubType;
    
    /* clean up and free all the sub-nodes */
    for (SubType = Typ->DerivedTypeList; SubType != NULL; SubType = NextSubType)
    {
        NextSubType = SubType->Next;
        DataTypeCleanupNode(SubType);

#if 0
        /* if it's a struct or union deallocate all the member values */
        if (SubType->Members != NULL)
        {
            VariableTableCleanup(SubType->Members);
            HeapFreeMem(SubType->Members);
        }
#endif
    }

    /* free this node */
    free(Typ);
}


void DataTypeCleanup()
{
    DataTypeCleanupNode(TypeUber);
}


/*
 * NAME:        DataTypeAllocValue
 * ACTION:      Allocate a Value of a given data type
 * PARAMETERS:  DataType *Type - the type of the value
 * RETURNS:     Value * - the new Value
 */
 
Value *DataTypeAllocValue(DataType *Type)
{
    Value *NewVal = (Value *)CheckedAlloc(sizeof(Value));
    NewVal->Type = Type;
    
    return NewVal;
}


/*
 * NAME:        DataTypeGetTypedef
 * ACTION:      Find a typedef given its name
 * PARAMETERS:  const char *Identifier - the name of the type
 * RETURNS:     DataType * - the type or NULL if not found
 */
 
DataType *DataTypeGetTypedef(const char *Identifier)
{
    return NULL;
}


/*
 * NAME:        DataTypeCombine
 * ACTION:      Combine two data types. This is necessary when we have the
 *              part of the data type from before an identifier and the part
 *              from after an identifier and we need to make one complete
 *              type
 * PARAMETERS:  ParseState *Parser - the parser
 *              DataType *LeftType - the left part of the type
 *              DataType *RightType - the right part of the type
 * RETURNS:     DataType * - the new type
 */
 
DataType *DataTypeCombine(ParseState *Parser, DataType *LeftType, DataType *RightType)
{
    int NewTypeOpt;
    DataType *NewLeftType;
    
    if (RightType->FromType == NULL)
    {
        /* we're combining with a basic type */
        if (RightType->Basic != BasicTypeUnknown)
            ProgramFail(Parser, "not a valid data type");
        
        /* resolve the type options */
        NewTypeOpt = LeftType->TypeOpt | RightType->TypeOpt;
        if ((NewTypeOpt & TypeOptShort) != 0 && (NewTypeOpt & TypeOptLong) != 0)
            ProgramFail(Parser, "you can't have short longs!");
            
        if ((NewTypeOpt & TypeOptSigned) != 0 && (NewTypeOpt & TypeOptUnsigned) != 0)
            ProgramFail(Parser, "you can't have signed unsigneds!");
        
        /* make a new type with the combined type options */    
        return DataTypeGetMatching(Parser, LeftType->FromType, LeftType->Basic, NewTypeOpt, LeftType->ArraySize, LeftType->Identifier);
    }
    else
    {
        /* recursively take one level off Type2 and put it on Type1 */
        NewLeftType = DataTypeGetMatching(Parser, LeftType, RightType->Basic, RightType->TypeOpt, RightType->ArraySize, RightType->Identifier);
        return DataTypeCombine(Parser, NewLeftType, RightType->FromType);
    }
}


/*
 * NAME:        DataTypeFromTypeSpec
 * ACTION:      Given an enum TypeSpec like TypeSpecConst etc., make a
 *              data type we can put in a parse tree. This will have
 *              a basic type of BasicTypeUnknown since we don't know
 *              what type it refers to yet. We'll TypeCombine() it 
 *              with a real type later.
 * PARAMETERS:  int TypeSpec - contains an arithmetic OR of enum TypeSpec
 *                  options.
 * RETURNS:     DataType * - the resulting data type
 */
 
DataType *DataTypeFromTypeSpec(int TypeSpec)
{
    return DataTypeGetMatching(NULL, NULL, BasicTypeUnknown, TypeSpec, 0, NULL);
}
