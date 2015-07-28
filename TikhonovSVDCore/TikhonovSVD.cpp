#include "TikhonovSVDCore/TikhonovSVD.h"
#include <gsl/gsl_linalg.h>
#include <gsl/gsl_blas.h>
#include <math.h>
TikhonovSVD::TikhonovSVD(gsl_matrix* &A_matrix, gsl_vector* &b_vector){
    bVector = b_vector;
    matrixSize = A_matrix->size1;
    phivector.assign(matrixSize, 0.0);
    AMatrix = gsl_matrix_alloc(matrixSize, matrixSize);
    VMatrix = gsl_matrix_alloc(matrixSize, matrixSize);
    SVector = gsl_vector_alloc(matrixSize);
    workVector = gsl_vector_alloc(matrixSize);
    XTikhonovSVD = gsl_vector_alloc(matrixSize);
    gsl_vector_set_all(XTikhonovSVD, 0.0);
    utb = gsl_vector_alloc(matrixSize);
    utbs = gsl_vector_alloc(matrixSize);
    for (size_t i = 0; i< matrixSize; i++){
        for (size_t j = 0; j < matrixSize; j++){
            gsl_matrix_set(AMatrix, i, j, gsl_matrix_get(A_matrix, i, j));
        }
    }
}

TikhonovSVD::~TikhonovSVD(){
    gsl_matrix_free(AMatrix);
    gsl_matrix_free(VMatrix);
    gsl_vector_free(SVector);
    gsl_vector_free(workVector);
    gsl_vector_free(utb);
    gsl_vector_free(utbs);
    gsl_vector_free(XTikhonovSVD);
}

void TikhonovSVD::getXtikhonovSVD(double lambdavalue){
    doSVDdecomposition();
    for (size_t i = 0; i < matrixSize; i++){
        gsl_matrix_get_col(workVector,VMatrix, i);
        double phi = pow(gsl_vector_get(SVector, i), 2.0)/(pow(gsl_vector_get(SVector, i), 2.0) + pow(lambdavalue, 2.0));
        phivector[i] = phi;
        double scale = phi*gsl_vector_get(utbs, i);
        gsl_vector_scale(workVector, scale);
        gsl_vector_add(XTikhonovSVD, workVector);
    }
}

void TikhonovSVD::doSVDdecomposition(){
    gsl_linalg_SV_decomp(AMatrix, VMatrix, SVector, workVector);
    gsl_blas_dgemv(CblasTrans, 1.0, AMatrix, bVector, 0.0, utb);
    for (size_t i = 0; i < matrixSize; i++){
        gsl_vector_set(utbs, i, gsl_vector_get(utb,i)/gsl_vector_get(SVector, i));
    }
}

double TikhonovSVD::getGCVValue(gsl_matrix* &A_matrix, gsl_vector* &b_vector, gsl_vector* &x_vector){
    gsl_blas_dgemv(CblasNoTrans, 1.0, A_matrix, x_vector, 0.0, workVector);
    gsl_vector_sub(workVector, b_vector);
    double ro = pow(gsl_blas_dnrm2(workVector), 2.0);
    double sumphi{0.0};
    for(int i = 0; i< matrixSize; i++){
        sumphi += phivector[i];
    }
    double divisor = pow((matrixSize - sumphi), 2.0);
    return ro/divisor;
}
