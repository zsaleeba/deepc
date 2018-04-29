#ifndef STORABLE_H
#define STORABLE_H

#include <chrono>

#include "flatbuffers/flatbuffers.h"
#include "deeptypes.h"
#include "storedobject_generated.h"


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
    
    // Which database to store this thing in.
    enum class DbGroup
    {
        SourceFiles,
        SourceFileKeys
    };
    
protected:
    // Id number used to look up the item in the program database.
    Id id_;

public:
    explicit Storable() : id_(0) {}
    explicit Storable(uint32_t id) : id_(id) {}
    virtual ~Storable() {}

    // Accessors
    Id   id() const   { return id_; }
    void setId(Id id) { id_ = id; }

    // Which databases to use for the content and the key.
    virtual DbGroup contentDbGroup() const = 0;
    virtual DbGroup keyDbGroup() const = 0;
    
    // To serialise this type.
    virtual void serialiseContent(flatbuffers::FlatBufferBuilder &builder) const = 0;
    virtual void serialiseKey(flatbuffers::FlatBufferBuilder &builder) const = 0;

    // To unserialise this type.
    virtual void unserialise(const fb::StoredObject &so) = 0;
    
    // Factory method to create an appropriately typed Storable from stored data.
    static std::shared_ptr<Storable> create(uint32_t id, const fb::StoredObject &so);
};


} // namespace deepC

#endif // STORABLE_H
