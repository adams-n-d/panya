#include <matio.h>
#include <stdio.h>

//reads the .mat variable specified into an array
void readMatArr(mat_t *matfp, char *varName, double *array);

void readMatArr(mat_t *matfp, char *varName, double *array){

	matvar_t *mv = Mat_VarReadInfo(matfp, varName);
//	printf("matrix is %d by %d\n", sdMatrix->numRows(), sdMatrix->numCols());
	int m, n;
	m = mv->dims[0];
	n = mv->dims[1];


	//check size
	//need to rework -- mask is n-1 by n
//			 -- grads are n-1 by n-1
	//if ( m*n != len(array) ){
		//std::cout << "Warning: incorrect size" << std::endl;
	//}

	void *matDat = malloc(m*n*sizeof(double));
	int start[2]={0,0},stride[2]={1,1},edge[2]={m,n};
	int rdErr;
	if( (rdErr = Mat_VarReadData(matfp, mv, matDat, start, stride, edge)) != 0){
		//std::cerr << "Error: cannot read data; errnum " << rdErr << std::endl;
	}
	array = (char *) matDat;
}

