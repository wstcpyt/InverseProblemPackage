#include<gmock/gmock.h>
#include "TikhonovSVDCore/TikhonovSVD.h"
#include <gsl/gsl_blas.h>
#define MATRIXSIZE 10
using namespace testing;

class TikhonovSVDTEST: public Test{
public:
    gsl_matrix* AMatrix;
    gsl_vector* bVector;
    TikhonovSVDTEST(){
        AMatrix = gsl_matrix_alloc(MATRIXSIZE, MATRIXSIZE);
        gsl_matrix_set_all(AMatrix, 1.0);
        bVector = gsl_vector_alloc(MATRIXSIZE);
        gsl_vector_set_all(bVector, 10.0);
    }
    ~TikhonovSVDTEST(){
        gsl_matrix_free(AMatrix);
        gsl_vector_free(bVector);
    }
};

TEST_F(TikhonovSVDTEST, TikhonovSVD_Constructor){
    TikhonovSVD tikhonovSVD(AMatrix, bVector);
    ASSERT_THAT(tikhonovSVD.XTikhonovSVD->size, MATRIXSIZE);
}

