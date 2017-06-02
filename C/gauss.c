#include <math.h>
#include <stdlib.h>
#include <stdio.h>
int main(void){
  int solveTriSup(int n, double** A, double* b, double *x, double tol);
  int gauss( int n, double ** A, double * v, double tol);
  void prodMatVec(int n, double ** A, double * x, double * y);
  double norma2(int n, double *z);
  int n; /*Matrix size*/
  int i, j; /*Iterator pointer / counters*/
  int resgauss; /*Gauss Result*/
  int restrisup;/*solveTriSup Result*/
	
  /*A * x = B*/
  double **a, **r, *b,*v, *x,*y;/*Main matrix A, vector B independent, vector x solution*/
  double tol; /*Tolerancy, value that represents 0 and every value under it is deduced as 0*/
  printf("Type matrix n dimension: \n");
  scanf("%d",&n);
  a = (double **)malloc(n*sizeof(double*));
  r = (double **)malloc(n*sizeof(double*));
  b = (double *)malloc(n*sizeof(double*));
  v = (double *)malloc(n*sizeof(double*));
  x = (double *) malloc(n*sizeof(double*));
  y = (double *) malloc(n*sizeof(double*));
  if((a==NULL||b==NULL)){
    printf("Not enough memory space to run the program.");
    return 1;
  }
  if((r==NULL||v==NULL)){
    printf("Not enough memory space to run the program.");
    return 2;
  }
  if((x==NULL||y==NULL)){
    printf("Not enough memory space to run the program.");
    return 3;
  }
  for(i=0; i<n; i++){
    a[i]=(double*)malloc(n*sizeof(double));
    r[i]=(double*)malloc(n*sizeof(double));
    if(a[i]==NULL || r[i]==NULL){
	printf("Not enough memory space to run the program.");
        return 4;
    }
  }
  printf("\n Type %d x %d A elements: \n",n,n);
  for(i = 0; i < n; i++){
    printf("\n");
    y[i]=0;
    a[i][0]=0.;
    r[i][0]=0.;
    for(j = 0; j < n; j++){
      r[i][j]=0.;
      scanf("%le",&a[i][j]);
      r[i][j]+=a[i][j];
      printf("%e",a[i][j]);
    }
  }
  printf("Type vector B elements: %d \n",n);
  for(i=0; i<n;i++){
    v[i]=0.;
    printf("Type b %d 0: \n",i);
    scanf("%le",&b[i]);
    v[i]+=b[i];
  }
  printf("Type the k tolerancy: where 10 ^-k\n");
  scanf("%le",&tol);
  tol=1./(pow(10,tol));
 
  resgauss = gauss(n,r,v,tol);
  
  if(resgauss==1){
      printf("\n  The equation system can't be solved by Gauss method. \n");
      return 1;
  }
  restrisup=solveTriSup(n, r, v, x, tol);
  if(restrisup==1){
      printf("\n  The equation system can't be solved using solTriSup method. \n");
      return 1;
  }
  
  prodMatVec(n, a, x,y);
  /*After having the solution we substract each term of the independent vector, should
  *output a vector of 0.
  */
  for(i=0;i<n;i++){
    y[i]-=b[i];
  }
  printf("\n Rule: %e",norma2(n, y));
 
  /*Freeing space
  */
  for(i=0; i<n; i++){
    free(a[i]);
  }
  free(a);
  free(b);
  free(x);
  return 0;
}
/*Method that given a Vector Y (should be full of 0), checks if 
*it actually is full of absolute zeros
*/
double norma2(int n, double *y){
  int i;
  double solution=0.;
  for(i=0;i<n;i++){
      solution = solution + pow(y[i],2);
  }
  solution = sqrt(solution);
  return solution;
}

/*Method that multiplies A * x and stores its solution in 
* the vector Y
*/

void prodMatVec(int n, double** a, double *x, double *y){
  int i;  
  int j;
  printf("\n A: \n");
  for(i=0;i<n;i++){
      printf("\n");
      for(j=0;j<n;j++){
	  printf(" %e ",a[i][j]);
      }
  }
  for(i=0;i<n;i++){
      printf("\n");
      y[i]=0.;
      printf("X: %e",x[i]);
      for(j=0;j<n;j++){
	  y[i]+=(a[i][j]*x[j]);
      }
      printf("     Product: %e ",y[i]);
  }
}
/*Method that gets the X factor in the following equation A * X - B = 0
* using an algorithm (Substitution backwards) that requires an A matrix
* full of Zeros "0" below the main diagonal of the matrix.
* Xi = 1 / Uji * (Yi - nΣj=i+1 * Uij * Xj) (i=n, n-1 ... 1)
*
*@param n: size of the square matrix (nxn)
*@param A: main source matrix
*@param b: independent vector
*@param x: empty vector where will be stored the solution
*@param tol: tolerancy that indicates the limit for values between 0 and tol
*/
int solveTriSup(int n, double** A, double* b, double *x, double tol){
  int i;
  int j;
  for(i=n-1; i>=0; i--){
    x[i]=b[i];
    for(j=n-1; j>i; j--){
      x[i]-=A[i][j]*x[j];
    }
    if(fabs(A[i][i])<tol){
      return 1; 
    }
    x[i]=x[i]/A[i][i];
    printf("%e \n",x[i]);
  }
  return 0;
}
/*GAUSS Pivot method that makes a 0 in every position of the 
*array under the main diagonal. Matrix form: A * x = v
*
*@param n: size of the square matrix (nxn)
*@param A: main source matrix
*@param v: vector independent
*@param tol: tolerancy that indicates the limit for values between 0 and tol
*/
int gauss( int n, double ** A, double * v, double tol){
  double aux=0;
  int auxIdx;
  double* auxPivot;
  int i;
  int j;
  int o;
  for(i=0; i< n-1; i++){
      auxIdx=i;
      for(j=i+1; j<n; j++){
	       if(fabs(A[j][i])>fabs(A[auxIdx][i])){
	          auxIdx=j;
	       }
      }
      auxPivot=A[auxIdx];
      A[auxIdx]=A[i];
      A[i]=auxPivot;
      printf("\n Swap: \n	");
      for(j=0;j<n;j++){
	       printf("\n");
	       for(o=0;o<n;o++){
	          printf("%e  ", A[j][o]);
	       }
      }
      aux  = A[i][i];
      if(fabs(aux)<tol){
	       return 1;
      }
      for(o=i+1;o<n;o++){
	       aux = A[o][i]/aux;
	       for(j=0;j<n;j++){
	          A[o][j]= (A[o][j])-(aux*A[i][j]);
	       }
	       v[o] = (v[o])-(aux*v[i]);
      }
  }
  printf("\n Final \n");
  for(i=0;i<n;i++){
      for(j=0;j<n;j++){
	       printf("%e  ",A[i][j]);
      }
      printf("\n");
  }
return 0;
}
