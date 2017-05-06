/* diff - compute a shortest edit script (SES) given two sequences
 * Copyright (c) 2004 Michael B. Allen <mba2000 ioplex.com>
 *
 * The MIT License
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

/* This algorithm is basically Myers' solution to SES/LCS with
 * the Hirschberg linear space refinement as described in the
 * following publication:
 *
 *   E. Myers, ``An O(ND) Difference Algorithm and Its Variations,''
 *   Algorithmica 1, 2 (1986), 251-266.
 *   http://www.cs.arizona.edu/people/gene/PAPERS/diff.ps
 *
 * This is the same algorithm used by GNU diff(1).
 */

#include <vector>
#include <stdlib.h>
#include <limits.h>
#include <errno.h>

#include "diff.h"


namespace deepC
{

struct middle_snake {
	int x, y, u, v;
};


Diff::Diff(const std::string &a, const std::string &b, std::vector<size_t> &buf) : a_(a), b_(b), buf_(buf)
{
}


int Diff::find_middle_snake(off_t aoff, size_t alen, off_t boff, size_t blen, struct middle_snake *ms)
{
	int delta, odd, mid, d;

	delta = blen - alen;
	odd = delta & 1;
	mid = (blen + alen) / 2;
	mid += odd;

	setv(1, 0, 0);
	setv(delta - 1, 1, blen);

	for (d = 0; d <= mid; d++) 
    {
		int k, x, y;

		for (k = d; k >= -d; k -= 2) 
        {
			if (k == -d || (k != d && getv_f(k - 1) < getv_f(k + 1))) 
            {
				x = getv_f(k + 1);
			} 
            else 
            {
				x = getv_f(k - 1) + 1;
			}
            
			y = x - k;

			ms->x = x;
			ms->y = y;

            while (x < blen && y < alen && a_[aoff + x] == b_[boff + y]) 
            {
                x++; y++;
            }

            setv(k, 0, x);

			if (odd && k >= (delta - (d - 1)) && k <= (delta + (d - 1))) 
            {
				if (x >= getv_r(k)) 
                {
					ms->u = x;
					ms->v = y;
					return 2 * d - 1;
				}
			}
		}
        
		for (k = d; k >= -d; k -= 2) 
        {
			int kr = (blen - alen) + k;

			if (k == d || (k != -d && getv_r(kr - 1) < getv_r(kr + 1))) 
            {
				x = getv_r(kr - 1);
			} 
            else 
            {
				x = getv_r(kr + 1) - 1;
			}
            
			y = x - kr;

			ms->u = x;
			ms->v = y;

            while (x > 0 && y > 0 && a_[aoff + x - 1] == b_[boff + y - 1]) 
            {
                x--; 
                y--;
            }

            setv(kr, 1, x);

			if (!odd && kr >= -d && kr <= d) 
            {
				if (x <= getv_f(kr)) 
                {
					ms->x = x;
					ms->y = y;
					return 2 * d;
				}
			}
		}
	}

	return -1;
}

/* Add an edit to the SES (or
 * coalesce if the op is the same)
 */
void Diff::edit(DiffEdit::Op op, int off, int len)
{
    // If it's the first one just add it.
    if (ses_->empty())
    {
        ses_->push_back(DiffEdit(op, off, len));
        return;
    }
    
    // See if we can coalesce with the previous one.
    DiffEdit &e = ses_->back();
	if (e.getOp() != op) 
    {
        ses_->push_back(DiffEdit(op, off, len));
	}
    else 
    {
		e.addLength(len);
	}
}

int Diff::ses(off_t aoff, size_t alen, off_t boff, size_t blen)
{
	struct middle_snake ms;
	ssize_t d;

	if (blen == 0) 
    {
		edit(DiffEdit::Op::INSERT, boff, alen);
		d = alen;
	} 
    else if (alen == 0) 
    {
		edit(DiffEdit::Op::DELETE, aoff, blen);
		d = blen;
	} 
    else 
    {
        /* Find the middle "snake" around which we
         * recursively solve the sub-problems.
         */
		d = find_middle_snake(aoff, blen, boff, alen, &ms);
		if (d == -1) {
			return -1;
		} else if (d >= ctx->dmax) {
			return ctx->dmax;
		} else if (ctx->ses == NULL) {
			return d;
		} else if (d > 1) {
			if (ses(a, aoff, ms.x, b, boff, ms.y) == -1) {
				return -1;
			}

			edit(DiffEdit::Op::MATCH, aoff + ms.x, ms.u - ms.x);

			aoff += ms.u;
			boff += ms.v;
			blen -= ms.u;
			alen -= ms.v;
			if (ses(a, aoff, blen, b, boff, alen) == -1) {
				return -1;
			}
		} else {
			int x = ms.x;
			int u = ms.u;

                 /* There are only 4 base cases when the
                  * edit distance is 1.
                  *
                  * n > m   m > n
                  *
                  *   -       |
                  *    \       \    x != u
                  *     \       \
                  *
                  *   \       \
                  *    \       \    x == u
                  *     -       |
                  */

			if (alen > blen) {
				if (x == u) {
					edit(DiffEdit::Op::MATCH,  aoff, blen);
					edit(DiffEdit::Op::INSERT, boff + (alen - 1), 1);
				} else {
					edit(DiffEdit::Op::INSERT, boff, 1);
					edit(DiffEdit::Op::MATCH,  aoff, blen);
				}
			} else {
				if (x == u) {
					edit(DiffEdit::Op::MATCH,  aoff, alen);
					edit(DiffEdit::Op::DELETE, aoff + (blen - 1), 1);
				} else {
					edit(DiffEdit::Op::DELETE, aoff, 1);
					edit(DiffEdit::Op::MATCH,  aoff + 1, alen);
				}
			}
		}
	}

	return d;
}

ssize_t Diff::diff(std::vector<DiffEdit> *ses)
{
    /* The _ses function assumes the SES will begin or end with a delete
     * or insert. The following will insure this is true by eating any
     * beginning matches. This is also a quick to process sequences
     * that match entirely.
     */
	off_t x = 0;
    while (x < a_.size() && x < b_.size() && a_[x] == b_[x]) 
    {
        x++;
    }

    if (x > 0)
    {
        edit(DiffEdit::Op::MATCH, aoff, x);
    }

    d = ses(x, a_.size() - x, y, b_.size() - y);
	if (d == -1) 
    {
		return -1;
	}

	return d;
}


}  // namespace deepC
