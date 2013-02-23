/*! \file
    \ingroup CCLAMBDA
    \brief Enter brief description of file here 
*/
#include <cstdio>
#include <libdpd/dpd.h>
#include "MOInfo.h"
#include "Params.h"
#define EXTERN
#include "globals.h"

namespace psi { namespace cclambda {

/** The RHF/ROHF contractions can be improved here **/

void cc2_fmiL2(int L_irr)
{
  int h, m;
  dpdbuf4 Lijab, LIJAB, LIjAb;
  dpdbuf4 newLijab, newLIJAB, newLIjAb;
  dpdfile2 fij, fIJ, F;
  dpdbuf4 X, X1, X2;
  dpdbuf4 L2, newL2;

  /* RHS -= P(ij)*Limab*Fjm */
  if(params.ref == 0) { /** RHF **/

    dpd_buf4_init(&X, PSIF_CC_TMP0, L_irr, 0, 5, 0, 5, 0, "X(Ij,Ab)");

    dpd_buf4_init(&L2, PSIF_CC_LAMBDA, L_irr, 0, 5, 0, 5, 0, "LIjAb");
    dpd_file2_init(&F, PSIF_CC_OEI, 0, 0, 0, "fIJ");
    dpd_contract244(&F, &L2, &X, 1, 0, 0, -1.0, 0);
    dpd_file2_close(&F);
    dpd_buf4_close(&L2);

    dpd_buf4_sort_axpy(&X, PSIF_CC_LAMBDA, qpsr, 0, 5, "New LIjAb", 1);
    dpd_buf4_init(&newL2, PSIF_CC_LAMBDA, L_irr, 0, 5, 0, 5, 0, "New LIjAb");
    dpd_buf4_axpy(&X, &newL2, 1);
    dpd_buf4_close(&newL2);

    dpd_buf4_close(&X);
  }
  else if(params.ref == 1) { /** RHF/ROHF **/

    dpd_file2_init(&fIJ, PSIF_CC_OEI, 0, 0, 0, "fIJ");
    dpd_file2_init(&fij, PSIF_CC_OEI, 0, 0, 0, "fij");

    dpd_buf4_init(&LIJAB, PSIF_CC_LAMBDA, L_irr, 0, 7, 2, 7, 0, "LIJAB");
    dpd_buf4_init(&X1, PSIF_CC_TMP1, L_irr, 0, 7, 0, 7, 0, "X(0,7) 1");
    dpd_contract424(&LIJAB, &fIJ, &X1, 1, 1, 1, -1.0, 0.0);
    dpd_buf4_init(&X2, PSIF_CC_TMP1, L_irr, 0, 7, 0, 7, 0, "X(0,7) 2");
    dpd_contract244(&fIJ, &LIJAB, &X2, 1, 0, 0, -1.0, 0.0);
    dpd_buf4_close(&LIJAB);
    dpd_buf4_axpy(&X1, &X2, 1.0);
    dpd_buf4_close(&X1);
    dpd_buf4_init(&newLIJAB, PSIF_CC_LAMBDA, L_irr, 0, 7, 2, 7, 0, "New LIJAB");
    dpd_buf4_axpy(&X2, &newLIJAB, 1.0);
    dpd_buf4_close(&X2);
    dpd_buf4_close(&newLIJAB);

    dpd_buf4_init(&Lijab, PSIF_CC_LAMBDA, L_irr, 0, 7, 2, 7, 0, "Lijab");
    dpd_buf4_init(&X1, PSIF_CC_TMP1, L_irr, 0, 7, 0, 7, 0, "X(0,7) 1");
    dpd_contract424(&Lijab, &fij, &X1, 1, 1, 1, -1.0, 0.0);
    dpd_buf4_init(&X2, PSIF_CC_TMP1, L_irr, 0, 7, 0, 7, 0, "X(0,7) 2");
    dpd_contract244(&fij, &Lijab, &X2, 1, 0, 0, -1.0, 0.0);
    dpd_buf4_close(&Lijab);
    dpd_buf4_axpy(&X1, &X2, 1.0);
    dpd_buf4_close(&X1);
    dpd_buf4_init(&newLijab, PSIF_CC_LAMBDA, L_irr, 0, 7, 2, 7, 0, "New Lijab");
    dpd_buf4_axpy(&X2, &newLijab, 1.0);
    dpd_buf4_close(&X2);
    dpd_buf4_close(&newLijab);

    dpd_buf4_init(&LIjAb, PSIF_CC_LAMBDA, L_irr, 0, 5, 0, 5, 0, "LIjAb");
    dpd_buf4_init(&newLIjAb, PSIF_CC_LAMBDA, L_irr, 0, 5, 0, 5, 0, "New LIjAb");
    dpd_contract424(&LIjAb, &fij, &newLIjAb, 1, 1, 1, -1.0, 1.0);
    dpd_contract244(&fIJ, &LIjAb, &newLIjAb, 1, 0, 0, -1.0, 1.0);
    dpd_buf4_close(&LIjAb);
    dpd_buf4_close(&newLIjAb);

    dpd_file2_close(&fij);
    dpd_file2_close(&fIJ);
  }
  else if(params.ref == 2) { /** UHF **/

    dpd_file2_init(&fIJ, PSIF_CC_OEI, 0, 0, 0, "fIJ");
    dpd_file2_init(&fij, PSIF_CC_OEI, 0, 2, 2, "fij");
    dpd_file2_copy(&fIJ, PSIF_CC_OEI, "fIJ diag");
    dpd_file2_copy(&fij, PSIF_CC_OEI, "fij diag");
    dpd_file2_close(&fIJ);
    dpd_file2_close(&fij);

    dpd_file2_init(&fIJ, PSIF_CC_OEI, 0, 0, 0, "fIJ diag");
    dpd_file2_init(&fij, PSIF_CC_OEI, 0, 2, 2, "fij diag");

    dpd_file2_mat_init(&fIJ);
    dpd_file2_mat_rd(&fIJ);
    dpd_file2_mat_init(&fij);
    dpd_file2_mat_rd(&fij);

    for(h=0; h < moinfo.nirreps; h++) {
      for(m=0; m < fIJ.params->rowtot[h]; m++) 
	fIJ.matrix[h][m][m] = 0;

      for(m=0; m < fij.params->rowtot[h]; m++) 
	fij.matrix[h][m][m] = 0;
    }

    dpd_file2_mat_wrt(&fIJ);
    dpd_file2_mat_close(&fIJ);
    dpd_file2_mat_wrt(&fij);
    dpd_file2_mat_close(&fij);

    dpd_file2_close(&fIJ);
    dpd_file2_close(&fij);

    dpd_file2_init(&fIJ, PSIF_CC_OEI, 0, 0, 0, "fIJ diag");
    dpd_file2_init(&fij, PSIF_CC_OEI, 0, 2, 2, "fij diag");

    /** X(IJ,AB) = F(I,M) L(MJ,AB) **/
    dpd_buf4_init(&X, PSIF_CC_TMP1, L_irr, 0, 7, 0, 7, 0, "X(IJ,AB) B");
    dpd_buf4_init(&LIJAB, PSIF_CC_LAMBDA, L_irr, 0, 7, 2, 7, 0, "LIJAB");
    dpd_contract244(&fIJ, &LIJAB, &X, 1, 0, 0, -1, 0);
    dpd_buf4_close(&LIJAB);
    /** X(IJ,AB) --> X'(JI,AB) **/
    dpd_buf4_sort(&X, PSIF_CC_TMP1, qprs, 0, 7, "X'(JI,AB)");
    dpd_buf4_close(&X);

    /** X(IJ,AB) = X(IJ,AB) - X'(JI,AB) **/
    dpd_buf4_init(&X1, PSIF_CC_TMP1, L_irr, 0, 7, 0, 7, 0, "X(IJ,AB) B");
    dpd_buf4_init(&X2, PSIF_CC_TMP1, L_irr, 0, 7, 0, 7, 0, "X'(JI,AB)");
    dpd_buf4_axpy(&X2, &X1, -1.0);
    dpd_buf4_close(&X2);
    /** L(IJ,AB) <--- X(IJ,AB) **/
    dpd_buf4_init(&newLIJAB, PSIF_CC_LAMBDA, L_irr, 0, 7, 2, 7, 0, "New LIJAB");
    dpd_buf4_axpy(&X1, &newLIJAB, 1.0);
    dpd_buf4_close(&X1);
    dpd_buf4_close(&newLIJAB);


    /** X(ij,ab) = F(i,m) L(mj,ab) **/
    dpd_buf4_init(&X, PSIF_CC_TMP1, L_irr, 10, 17, 10, 17, 0, "X(ij,ab) B");
    dpd_buf4_init(&LIJAB, PSIF_CC_LAMBDA, L_irr, 10, 17, 12, 17, 0, "Lijab");
    dpd_contract244(&fij, &LIJAB, &X, 1, 0, 0, -1, 0);
    dpd_buf4_close(&LIJAB);
    /** X(ij,ab) --> X'(ji,ab) **/
    dpd_buf4_sort(&X, PSIF_CC_TMP1, qprs, 10, 17, "X'(ji,ab)");
    dpd_buf4_close(&X);

    /** X(ij,ab) = X(ij,ab) - X'(ji,ab) **/
    dpd_buf4_init(&X1, PSIF_CC_TMP1, L_irr, 10, 17, 10, 17, 0, "X(ij,ab) B");
    dpd_buf4_init(&X2, PSIF_CC_TMP1, L_irr, 10, 17, 10, 17, 0, "X'(ji,ab)");
    dpd_buf4_axpy(&X2, &X1, -1.0);
    dpd_buf4_close(&X2);
    /** L(ij,ab) <--- X(ij,ab) **/
    dpd_buf4_init(&newLIJAB, PSIF_CC_LAMBDA, L_irr, 10, 17, 12, 17, 0, "New Lijab");
    dpd_buf4_axpy(&X1, &newLIJAB, 1.0);
    dpd_buf4_close(&X1);
    dpd_buf4_close(&newLIJAB);

    /** L(Ij,Ab) <-- L(Im,Ab) F(j,m) - F(I,M) L(Mj,Ab) **/
    dpd_buf4_init(&LIjAb, PSIF_CC_LAMBDA, L_irr, 22, 28, 22, 28, 0, "LIjAb");
    dpd_buf4_init(&newLIjAb, PSIF_CC_LAMBDA, L_irr, 22, 28, 22, 28, 0, "New LIjAb");
    dpd_contract424(&LIjAb, &fij, &newLIjAb, 1, 1, 1, -1, 1);
    dpd_contract244(&fIJ, &LIjAb, &newLIjAb, 1, 0, 0, -1, 1);
    dpd_buf4_close(&LIjAb);
    dpd_buf4_close(&newLIjAb);

    dpd_file2_close(&fij);
    dpd_file2_close(&fIJ);
  }
}

}} // namespace psi::cclambda
