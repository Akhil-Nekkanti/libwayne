#ifndef _MATVEC_H
#define _MATVEC_H
/*
** All these routines are careful to work correctly even if the result
** argument is the same memory space as another argument.
**
** Simplistic Sparse matrices are supported.  They take the same memory as a
** regular matrix (n x m), but in each row, the last element is flagged with
** the 8-byte hexadecimal value 0xDEADBEEFBABEFACE.  The zeroth element in
** the row is an integer (stored as a double) indicating the number of non-zero
** elements, say N, in that row.  Then there are N integers indicating the indices,
** then the actual N values.  So, for example, in a 10x10 matrix where
** the first row has only 3 non-zero elements in positions 3, 6, and 9,
** with values 1.3, 4.7, and -3.4, then the first row *represents*
**     0 0 0 1.3 0 0 4.7 0 0 -3.4
** and is physically stored like this:
**     3 3 6 9 1.3 4.7 -3.4 X X [0xDEADBEEFBABEFACE]
** where we don't care what's stored in a place marked X.  You would loop
** through the values like this:
**    assert(A[0] < physNumColumns-1);
**    for(i=1; i<A[0]; i++) { column=A[i]; value= A[column];}
** Note that with this representation, we can only use the sparse
** representation if EVERY row is more than half zeros.  In other
** words, if the matrix physically has M columns, we can only store
** at most (M-2)/2 non-zero elements.
*/

/* Returns boolean: whether it worked or not */
Boolean MatMakeSparse(int n, int m, double sparse[n][m], const double nonSparse[n][m]);
void MatMakeUnSparse(int n, int m, double nonSparse[n][m], const double sparse[n][m]);
Boolean MatIsSparse(int n, int m, const double A[n][m]);
Boolean MatSparseSanity(int n, int m, const double A[n][m]);

/* Pretty print a matrix.  If s is != NULL, print it.  */
void MatPut(int n, int m, const double yy[n][m]);
void VecPut(int n, const double y[n]);
void VecGet(int n, double y[n]);
void MatGet(int n, int m, double A[n][m]);

double *VecAssign(double *v, int n, ...);
double VecNorm1(int n, const double x[n]);
double VecNormEucl(int n, const double x[n]);
double MatNormEucl(int n, int m, const double A[n][m]);

/* Compute A = B*C. */
void MatMatMult(int n, int m, int p, double A[n][p],
    const double B[n][m], const double C[m][p]);

/* Compute the inverse of a matrix */
void MatTranspose(int n, int m, double AT[m][n], const double A[n][m]);

/* Compute the inverse of a matrix */
void MatInverse(int n, int m, double AI[m][n], const double A[n][m]);

/* For these, assert(n==m); */
void MatLUFact(int n, double L[n][n], double U[n][n], const double A[n][n]);
double *MatForwardSubst(int n, double y[n], const double L[n][n], const double b[n]);
double * MatBackSubst(int n, double x[n], const double U[n][n], const double b[n]);
/* Solve Ax = b for x */
double *MatSolve(int n, const double A[n][n], double x[n], const double b[n]);

/* Compute y = A*x. */
double *MatVecMult(int rows, int cols, double y[rows],
    const double A[rows][cols], const double x[cols]);
void    MatCopy(int n, int m, double dest[n][m], const double src[n][m]);

double VecDot(int dim, const double vec1[dim], const double vec2[dim]);
double VecLength(int dim, const double vec[dim]);
double *VecNormalize(int dim, double dest[dim], const double src[dim]);
double *VecAdd (int dim, double sum[dim],  const double v1[dim], const double v2[dim]);
/* VecDiff: diff = v1 - v2 */
double *VecDiff(int dim, double diff[dim], const double v1[dim], const double v2[dim]);
#define VecSub VecDiff
double *VecScalMul(int dim, double prod[dim], double k, const double v[dim]);
double *VecSetZero(int dim, double v[dim]);
double *VecCopy(int dim, double dest[dim], double const src[dim]);

/* Warning: do not nest calls to these macros, they all use the same counter variable! */
static int _MatVec_i=0;
static int _MatVec_f=0.0;
#define VECCOPY(n,d,s) for(_MatVec_i=0;_MatVec_i<(n); ++_MatVec_i) (d)[_MatVec_i]=(s)[_MatVec_i]
#define VECDIFF(n,d,u,v) for(_MatVec_i=0;_MatVec_i<(n); ++_MatVec_i) (d)[_MatVec_i]=(u)[_MatVec_i]-(v)[_MatVec_i]
#define VECADD(n,d,u,v) for(_MatVec_i=0;_MatVec_i<(n); ++_MatVec_i) (d)[_MatVec_i]=(u)[_MatVec_i]+(v)[_MatVec_i]


/*
** Compute the matrix exponential e^A using the Taylor expansion until the
** maxNorm of current term is less than eps.
*/
void MatExpMat(int n, double result[n][n], const double A[n][n], double eps);

/*
* Linear eq soln by Gauss Jordan method.
* Input: A(n,n)
*        B(n,m) containing m right-hand sides
* On output, A := inverse(A), B := soln's.
*/
void MatGaussJordan(int n, double A[n][n], int m, double B[n][m]);
#endif // _MATVEC_H
