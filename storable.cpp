#include "storable.h"
#include "flatbuffers/flatbuffers.h"
#include "deeptypes.h"
#include "sourcefile.h"
#include "programdb.h"

namespace deepC
{

// Factory method to create an appropriately typed Storable from stored data.
std::shared_ptr<Storable> Storable::create(uint32_t id, const fb::StoredObject &so)
{
    std::shared_ptr<Storable> obj;
    
    // Create an object of the correct type.
    switch (so.obj_type())
    {
    case fb::StoredAny_SourceFile:
        obj = std::make_shared<SourceFileOnDatabase>(id);
        break;
        
    default:
        throw ProgramDbException(std::string("can't create object of invalid type ") + std::to_string(static_cast<int>(so.obj_type())));
        break;
    }
    
    // Fill out the fields from the stored data.
    obj->unserialise(so);
    
    return obj;
}

} // namespace deepC
