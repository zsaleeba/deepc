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
    const std::string &a_;
    const std::string &b_;
    std::vector<off_t> buf_;
    std::vector<DiffEdit> *ses_;
    
private:
    off_t   ses(off_t aoff, size_t alen, off_t boff, size_t blen);
    off_t   findMiddleSnake(off_t aoff, ssize_t alen, off_t boff, ssize_t blen, struct middle_snake *ms);
    void    setV(int k, bool reverse, off_t val) { off_t off = k <= 0 ? -k * 4 + (reverse?1:0) : k * 4 + ((reverse?1:0) - 2); if (off >= static_cast<off_t>(buf_.size())) buf_.resize(off+1); buf_[off] = val; }
    off_t   getV(int k, bool reverse)            { return buf_[k <= 0 ? -k * 4 + (reverse?1:0) : k * 4 + ((reverse?1:0) - 2)]; }
    off_t   getVForward(int k)                   { return getV(k, false); }
    off_t   getVReverse(int k)                   { return getV(k, true); }
    void    edit(DiffEdit::Op op, int off, int len);
    
public:
    Diff(const std::string &a, const std::string &b);
    
    ssize_t diff(std::vector<DiffEdit> *ses);
};

}  // namespace deepC

#endif /* DEEPC_DIFF_H */
