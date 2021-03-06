#ifndef DEEPC_SOURCEFILE_H
#define DEEPC_SOURCEFILE_H

#include <ctime>
#include <string>
#include <string_view>
#include <chrono>
#include <mutex>
#include <memory>
#include <vector>

#include "storable.h"
#include "deeptypes.h"


namespace deepC
{


//
// Represents a source file in the system. May contain the full source text
// or if needToRead_ is true we may need to read it first. The id_ is the
// database id if the source file has been stored in the database or 0 if 
// not.
//

class SourceFile : public Storable
{
protected:
    // Members.
    std::string      fileName_;      // Path to source file.
    TimePoint        modified_;      // When it was last modified.
    std::string_view sourceText_;    // The full source text.

    std::vector<std::string_view> lines_; // The source file split into lines.
    bool             haveLines_;     // Whether we've lazily created lines_ yet.

protected:
    // Constructors.
    explicit SourceFile(uint32_t id) : Storable(id), haveLines_(false) {}
    explicit SourceFile(const std::string &fileName) : fileName_(fileName), haveLines_(false) {}
    explicit SourceFile(const std::string &fileName, const TimePoint &modified) : fileName_(fileName), modified_(modified), haveLines_(false) {}
    virtual ~SourceFile() {}

    // Split the file into lines.
    const std::vector<std::string_view> &makeLines();
    
public:
    // Accessors.
    const std::string      &fileName()   const { return fileName_; }
    const TimePoint        &modified()   const { return modified_; }
    const std::string_view &sourceText() const { return sourceText_; }

    void setFileName(const std::string &fileName) { fileName_ = fileName; }
    void setModified(const TimePoint &modified)   { modified_ = modified; }
    void setSourceText(std::string_view &source)  { sourceText_ = source; }

    // Split the file into lines.
    const std::vector<std::string_view> &getLines() { if (haveLines_) return lines_; else return makeLines(); }

    // Which databases to use for the content and the key mapping.
    DbGroup contentDbGroup() const override { return Storable::DbGroup::SourceFiles; }
    DbGroup keyDbGroup() const override     { return Storable::DbGroup::SourceFileKeys; }
    
    // To store this type in the database.
    void serialiseContent(flatbuffers::FlatBufferBuilder &builder) const override;
    void serialiseKey(flatbuffers::FlatBufferBuilder &builder) const override;
    void unserialise(const fb::StoredObject &so) override;
};


//
// Represents a source file in the system. May contain the full source text
// or if needToRead_ is true we may need to read it first. The id_ is the
// database id if the source file has been stored in the database or 0 if 
// not.
//

class SourceFileOnFilesystem : public SourceFile
{
private:
    // Members.
    int      fd_;               // The file descriptor of the mmap()ed file.
    void    *mapData_;          // The memory mapped data.
    size_t   mapSize_;          // The size of the memory mapped area.

public:
    // Constructors.
    explicit SourceFileOnFilesystem(const std::string &fileName);
    virtual ~SourceFileOnFilesystem();
};


//
// Represents a source file in the system. May contain the full source text
// or if needToRead_ is true we may need to read it first. The id_ is the
// database id if the source file has been stored in the database or 0 if 
// not.
//

class SourceFileOnDatabase : public SourceFile
{
private:
    // Members.
    std::shared_ptr<ProgramDb> pdb_; // We keep this to make sure the database isn't closed prematurely.

public:
    // Constructors.
    explicit SourceFileOnDatabase(uint32_t id) : SourceFile(id) {}
    virtual ~SourceFileOnDatabase();
};


//
// An exception thrown when source file operations fail.
//

class SourceFileException : public std::exception
{
    std::string message_;

public:
    SourceFileException(const std::string &message) : message_(message) {}

    const char * what () const throw ()
    {
        return message_.c_str();
    }
};


} // namespace deepC

#endif // SOURCEFILE_H
