#ifndef DEEPC_SOURCEFILE_H
#define DEEPC_SOURCEFILE_H

#include <ctime>
#include <string>


namespace deepC
{


class SourceFile
{
public:
    // Time that a source file was modified.
    class TimeDate
    {
    private:
        time_t   seconds_;
        uint32_t nanoseconds_;

    public:
        explicit TimeDate(time_t seconds, uint32_t nanoseconds) : seconds_(seconds), nanoseconds_(nanoseconds) {}
        explicit TimeDate(const TimeDate &td) : seconds_(td.seconds_), nanoseconds_(td.nanoseconds_) {}

        time_t seconds() const       { return seconds_; }
        uint32_t nanoseconds() const { return nanoseconds_; }
    };

private:
    // Members.
    std::string fileName_;
    TimeDate    modified_;
    std::string sourceText_;
    bool        needToRead_;

private:
    // Read the source text from the file.
    void readSourceText();

public:
    SourceFile(const std::string &fileName);
    SourceFile(const std::string &fileName, const std::string &sourceText, const TimeDate &modified);

    const std::string &fileName() const { return fileName_; }
    const std::string &sourceText()     { if (needToRead_) readSourceText(); return sourceText_; }
    const TimeDate    &modified() const { return modified_; }
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
