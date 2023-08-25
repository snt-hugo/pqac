#ifndef SHAKE_H
#define SHAKE_H

/*
 * Internal functions for Falcon. This is not the API intended to be
 * used by applications; instead, this internal API provides all the
 * primitives on which wrappers build to provide external APIs.
 *
 * ==========================(LICENSE BEGIN)============================
 *
 * Copyright (c) 2017-2019  Falcon Project
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * ===========================(LICENSE END)=============================
 *
 * @author   Thomas Pornin <thomas.pornin@nccgroup.com>
 */

#include "new_arithm_poly.h"

typedef struct
{
  union
  {
    uint64_t A[25];
    uint8_t dbuf[200];
  } st;
  uint64_t dptr;
} shake256_context;

void shake256_init (shake256_context *sc);
void shake256_inject (shake256_context *sc, const uint8_t *in, size_t len);

void shake256_inject_polyq1 (shake256_context *sc, const polyq1_t polq1_val);
void shake256_inject_polyq2 (shake256_context *sc, const polyq2_t polq2_val);
void shake256_inject_polyQ (shake256_context *sc, const polyQ_t polQ_val);

void shake256_flip (shake256_context *sc);
void shake256_extract (shake256_context *sc, uint8_t *out, size_t len);

#endif /* SHAKE_H */
