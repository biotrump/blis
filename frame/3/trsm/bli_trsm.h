/*

   BLIS    
   An object-based framework for developing high-performance BLAS-like
   libraries.

   Copyright (C) 2014, The University of Texas at Austin

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions are
   met:
    - Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    - Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    - Neither the name of The University of Texas at Austin nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
   HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
   DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
   THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
   (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "bli_trsm_cntl.h"
#include "bli_trsm_blocksize.h"
#include "bli_trsm_query.h"
#include "bli_trsm_check.h"
#include "bli_trsm_entry.h"
#include "bli_trsm_front.h"
#include "bli_trsm_int.h"

#include "bli_gemmtrsm_ukernel.h"
#include "bli_trsm_ukernel.h"

#include "bli_trsm_blk_var1f.h"
#include "bli_trsm_blk_var1b.h"

#include "bli_trsm_blk_var2f.h"
#include "bli_trsm_blk_var2b.h"

#include "bli_trsm_blk_var3f.h"
#include "bli_trsm_blk_var3b.h"

#include "bli_trsm_ll_ker_var2.h"
#include "bli_trsm_lu_ker_var2.h"
#include "bli_trsm_rl_ker_var2.h"
#include "bli_trsm_ru_ker_var2.h"

#include "bli_gemmtrsm_l_ukr_ref.h"
#include "bli_gemmtrsm_u_ukr_ref.h"

#include "bli_trsm_l_ukr_ref.h"
#include "bli_trsm_u_ukr_ref.h"

#include "bli_trsm4m1.h"
#include "bli_trsm3m1.h"


//
// Prototype object-based interface.
//
void bli_trsm( side_t  side,
               obj_t*  alpha,
               obj_t*  a,
               obj_t*  b );


//
// Prototype BLAS-like interfaces with homogeneous-typed operands.
//
#undef  GENTPROT
#define GENTPROT( ctype, ch, opname ) \
\
void PASTEMAC(ch,opname)( \
                          side_t  side, \
                          uplo_t  uploa, \
                          trans_t transa, \
                          diag_t  diaga, \
                          dim_t   m, \
                          dim_t   n, \
                          ctype*  alpha, \
                          ctype*  a, inc_t rs_a, inc_t cs_a, \
                          ctype*  b, inc_t rs_b, inc_t cs_b  \
                        );

INSERT_GENTPROT_BASIC( trsm )


//
// Prototype BLAS-like interfaces with heterogeneous-typed operands.
//
#undef  GENTPROT2
#define GENTPROT2( ctype_a, ctype_b, cha, chb, opname ) \
\
void PASTEMAC2(cha,chb,opname)( \
                                side_t    side, \
                                uplo_t    uploa, \
                                trans_t   transa, \
                                diag_t    diaga, \
                                dim_t     m, \
                                dim_t     n, \
                                ctype_a*  alpha, \
                                ctype_a*  a, inc_t rs_a, inc_t cs_a, \
                                ctype_b*  b, inc_t rs_b, inc_t cs_b  \
                        );

INSERT_GENTPROT2_BASIC( trsm )

#ifdef BLIS_ENABLE_MIXED_DOMAIN_SUPPORT
INSERT_GENTPROT2_MIX_D( trsm )
#endif

#ifdef BLIS_ENABLE_MIXED_PRECISION_SUPPORT
INSERT_GENTPROT2_MIX_P( trsm )
#endif

