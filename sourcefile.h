#ifndef DEEPC_SOURCEFILE_H
#define DEEPC_SOURCEFILE_H

#include <ctime>
#include <string>
#include <string_view>
#include <chrono>
#include <mutex>
#include <memory>

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

protected:
    // Constructors.
    explicit SourceFile(const std::string &fileName) : fileName_(fileName) {}
    explicit SourceFile(const std::string &fileName, const TimePoint &modified) : fileName_(fileName), modified_(modified) {}
    virtual ~SourceFile() {}
    
public:
    // Accessors.
    const std::string      &fileName()   const { return fileName_; }
    const TimePoint        &modified()   const { return modified_; }
    const std::string_view &sourceText() const { return sourceText_; }

    void setFileName(const std::string &fileName) { fileName_ = fileName; }
    void setModified(const TimePoint &modified)   { modified_ = modified; }
    void setSourceText(std::string_view &source)  { sourceText_ = source; }
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
    
    // To store this type in the database.
    void store(ProgramDb &pdb) override;
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
    explicit SourceFileOnDatabase(std::shared_ptr<ProgramDb> pdb, const std::string &fileName);
    virtual ~SourceFileOnDatabase();
    
    // To store this type in the database.
    void store(ProgramDb &pdb) override;
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
