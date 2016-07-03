#ifndef VALUE_H
#define VALUE_H

/* values */
class Value
{
public:
    DataType *Type;                 /* the type of this value */
    
    Value();
};

class IntegerValue : public Value
{
public:
    unsigned long Integer;
    
    IntegerValue() : Integer(0) {};
};

class FloatValue : public Value
{
public:
    double FloatVal;
    
    FloatValue() : FloatValue(0.0) {};
};

class ComplexValue : public Value
{
public:
    double i;
    double j;
    
    ComplexValue() : i(0.0), j(0.0) {};
};

class ArrayValue : public Value 
{
public:
    
    int Size;                       /* -1 if the size is undefined */
    void *Data;                     /* the array data */

    ArrayValue() : Size(0), Data(NULL) {};
};

#endif /* VALUE_H */
