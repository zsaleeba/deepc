#ifndef STORABLE_H
#define STORABLE_H

#include <chrono>

#include "deeptypes.h"


namespace deepC
{


// Forward declarations.
class ProgramDb;


//
// Classes which inherit Storable can be stored in the program database.
// They're allocated an id number which is used to reference the item
// in the database.
//

class Storable
{
public:
    // Id type used for all objects stored in the database.
    typedef uint32_t Id;
    
protected:
    // Id number used to look up the item in the program database.
    Id id_;

public:
    explicit Storable() : id_(0) {}
    virtual ~Storable() {}

    // Accessors
    Id   id() const   { return id_; }
    void setId(Id id) { id_ = id; }
    
    // To store this type in the database.
    virtual void store(ProgramDb &pdb) = 0;
};


} // namespace deepC

#endif // STORABLE_H
