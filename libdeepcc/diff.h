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
    std::vector<size_t> &buf_;
    std::vector<DiffEdit> *ses_;
    
private:
    int    ses(off_t aoff, size_t alen, off_t boff, size_t blen);
    int    find_middle_snake(off_t aoff, size_t alen, off_t boff, size_t blen, struct middle_snake *ms);
    void   setv(int k, int r, size_t val) { buf_[k <= 0 ? -k * 4 + r : k * 4 + (r - 2)] = val; }
    size_t getv(int k, int r)             { return buf_[k <= 0 ? -k * 4 + r : k * 4 + (r - 2)]; }
    size_t getv_f(int k)                  { return getv(k, 0); }
    size_t getv_r(int k)                  { return getv(k, 1); }
    void   edit(DiffEdit::Op op, int off, int len);
    
public:
    Diff(const std::string &a, const std::string &b, std::vector<size_t> &buf);
    
    ssize_t diff(std::vector<DiffEdit> *ses);
};

/* consider alternate behavior for each NULL parameter
 */
}  // namespace deepC

#endif /* DEEPC_DIFF_H */
