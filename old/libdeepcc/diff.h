#ifndef DEEPC_DIFF_H
#define DEEPC_DIFF_H

#include <unistd.h>
#include <vector>
#include <string>

namespace deepC
{

/* diff - compute a shortest edit script (SES) given two sequences
 */

class DiffEdit
{
public:
    enum class Op {
        MATCH = 1,
        DELETE,
        INSERT
    };

private:
    Op op_;
    off_t off_; // Offset into s1 if MATCH or DELETE but s2 if INSERT.
    size_t len_;

public:
    DiffEdit(Op op, off_t off, size_t len) : op_(op), off_(off), len_(len) {}
    Op     getOp() const         { return op_; }
    off_t  getOffset() const     { return off_; }
    size_t getLength() const     { return len_; }
    void   addLength(size_t add) { len_ += add; }
};


class Diff
{
private:
    enum offsetType
    {
        OFFSET_FORWARD_POSITIVE = 0,
        OFFSET_FORWARD_NEGATIVE = 1,
        OFFSET_REVERSE_POSITIVE = 2,
        OFFSET_REVERSE_NEGATIVE = 3,
    };
    
private:
    const std::string &strA_;
    const std::string &strB_;
    std::vector<off_t> offsetBuf_[4];
    std::vector<DiffEdit> *edits_;
    
private:
    off_t   findShortestEditSequence(off_t aoff, size_t alen, off_t boff, size_t blen);
    off_t   findMiddleSnake(off_t aoff, ssize_t alen, off_t boff, ssize_t blen, struct middle_snake *ms);
    
    off_t  &offBuf(int k, bool reverse) 
    { 
        int ot = reverse ? 2 : 0;
        if (k < 0)
        {
            ot++;
            k = -k;
        }

        std::vector<off_t> &v = offsetBuf_[ot]; 
        if (k >= static_cast<int>(v.size()))
        {
            v.resize(k+1); 
        }
        
        return v.at(k);
    }

    off_t  &offFwd(int k) { return offBuf(k, false); }
    off_t  &offRev(int k) { return offBuf(k, true); }

    void    edit(DiffEdit::Op op, int off, int len);
    
public:
    Diff(const std::string &a, const std::string &b);
    
    ssize_t diff(std::vector<DiffEdit> *findShortestEditSequence);
};

}  // namespace deepC

#endif /* DEEPC_DIFF_H */
