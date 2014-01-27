#include <matio.h>
#include <stdio.h>


#include "mat2arr.hpp"
//#include "Teuchos_RCPNode.hpp"
//#include "Teuchos_SerialDenseMatrix.hpp"
//#include "Teuchos_SerialDenseVector.hpp"
//#include "Teuchos_SerialDenseSolver.hpp"
//#include "Teuchos_RCP.hpp"
//#include "Teuchos_Version.hpp"


int main(void){
	typedef int Ordinal;
	typedef double Scalar;

	mat_t *mat;

	mat = Mat_Open("phases.mat", MAT_ACC_RDONLY);

/*	while( NULL != (matvar = Mat_VarReadNext(mat)) ){
		Mat_VarPrint(matvar,1);
		Mat_VarFree(matvar);
	}
*/
	char varMask[] = "p_mask";
	char varPGradx[] = "phase_gradx";
	char varPGrady[] = "phase_grady";

	matvar_t *pupil_mask = Mat_VarReadInfo(mat, varMask);

	printf("pupil matrix is %d by %d\n", pupil_mask->dims[0], pupil_mask->dims[1]);

	int m, n;
	m = pupil_mask->dims[0];
	n = pupil_mask->dims[1];
	printf("matrix is %d elements\n", m*n);

	void *pupil_dat = malloc(m*n*sizeof(double));
	printf("Alloc'ed %d spaces\n", m*n);

	int start[2]={0,0},stride[2]={1,1},edge[2]={m,n};

	int err = Mat_VarReadData(mat, pupil_mask, pupil_dat, start, stride, edge);
// Alternative:
//	int err = Mat_VarReadDataLinear(mat, pupil_mask, pupil_dat, 0, 1, m*n);
	printf("error: %d\n", err);

	double *mat_array;
	readMatArr(mat, varPGradx, mat_array);


	int i;
	double *elem = (double *) pupil_dat;
	for(i=0; i < m*n; ++i){
		if(elem[i] > (double)0) printf("%f\t%d\t", elem[i], i);
	}

	//read matvar data into serial dense matrix
//	readMatvar(mat, varMask, sx);



//	mat_ft ver = Mat_GetVersion(mat);
//	printf("version is %d\n", ver);
//	printf("others are %d, %d\n", MAT_FT_MAT4, MAT_FT_MAT5);

	Mat_Close(mat);

	return 0;

}
