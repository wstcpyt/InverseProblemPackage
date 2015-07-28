#ifndef INVERSEPROBLEM_TIKHONOVSVD_H
#define INVERSEPROBLEM_TIKHONOVSVD_H
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_vector.h>
#include <vector>
using namespace std;
class TikhonovSVD{
    gsl_vector* bVector;
    size_t matrixSize;
    gsl_matrix* AMatrix;
    gsl_matrix* VMatrix;
    gsl_vector* SVector;
    gsl_vector* workVector;
    gsl_vector* utb;
    gsl_vector* utbs;
    vector<double> phivector;
public:
    gsl_vector* XTikhonovSVD;
    TikhonovSVD(gsl_matrix* &A_matrix, gsl_vector* &b_vector);
    ~TikhonovSVD();
    void getXtikhonovSVD(double lambdavalue);
    double getGCVValue(gsl_matrix* &A_matrix, gsl_vector* &b_vector, gsl_vector* &x_vector);
private:
    void doSVDdecomposition();
};
#endif //INVERSEPROBLEM_TIKHONOVSVD_H
