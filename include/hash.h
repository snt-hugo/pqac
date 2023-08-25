#ifndef HASH_H
#define HASH_H

#include "new_arithm_poly.h"
#include "shake.h"

/* ctx have to be flipped. output a fmpz_t integer z such that 0 <= z < q2 */
/* FDH is for Full Domain Hash
   ctx have to be flipped. output a a R poly mod q2.
   (i.e a fmpz_mod_poly_t polynomial P of degree < d ( =DIM_R )) */

/* FDH is for Full Domain Hash
   ctx have to be flipped. output a a NTT poly mod q2.
   WARNING : the FDH to R_poly and to NTT_poly aren't compatible
   (i.e NTT(FDS_unsigned_chars_to_R_poly(x)) ISN'T
   FDS_unsigned_chars_to_NTT_poly(x)) */
int FDH_polyq2 (polyq2_t polq2_res, shake256_context *shake_context);
int FDH_challenges (challenge_t chall_res, shake256_context *shake_context);

#endif /* HASH_H */
