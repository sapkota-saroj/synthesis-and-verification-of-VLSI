#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
void TopDownMerge(int A[], int iBegin, int iMiddle, int iEnd, int B[]);
void TopDownMergeSort(int A[], int B[], int n);
void TopDownSplitMerge(int B[], int iBegin, int iEnd, int A[]);
void print_array(int* arr,int data);

void TopDownMergeSort(int A[], int B[], int n)
{
    int k;
    // one time copy of A[] to B[]
     for(k = 0; k < n; k++)
        B[k] = A[k];
    TopDownSplitMerge(B, 0, n, A);   // sort data from B[] into A[]
}

// Sort the given run of array A[] using array B[] as a source.
// iBegin is inclusive; iEnd is exclusive (A[iEnd] is not in the set).
void TopDownSplitMerge(int B[], int iBegin, int iEnd, int A[])
{
    int iMiddle=0;
    if(iEnd - iBegin <= 1)                       // if run size == 1
        return;                                 //   consider it sorted
    // split the run longer than 1 item into halves
    iMiddle = (iEnd + iBegin) / 2;              // iMiddle = mid point
    // recursively sort both runs from array A[] into B[]
    TopDownSplitMerge(A, iBegin,  iMiddle, B);  // sort the left  run
    TopDownSplitMerge(A, iMiddle,    iEnd, B);  // sort the right run
    // merge the resulting runs from array B[] into A[]
    TopDownMerge(B, iBegin, iMiddle, iEnd, A);
}

//  Left source half is A[ iBegin:iMiddle-1].
// Right source half is A[iMiddle:iEnd-1   ].
// Result is            B[ iBegin:iEnd-1   ].
void TopDownMerge(int A[], int iBegin, int iMiddle, int iEnd, int B[])
{
    int i,j,k;
    i = iBegin, j = iMiddle;
 
    // While there are elements in the left or right runs...
    for (k = iBegin; k < iEnd; k++) {
        // If left run head exists and is <= existing right run head.
        if (i < iMiddle && (j >= iEnd || A[i] <= A[j])) {
            B[k] = A[i];
            i = i + 1;
        } else {
            B[k] = A[j];
            j = j + 1;
        }
    }
}
void print_array(int* arr,int data)
{
    int i;
    for(i=0;i<data;i++)
    printf("%d \t",arr[i]);
    printf("\n");
}
int main()
{
    int data,i;
    srand(time(NULL));
    printf("enter no of data::\n");
    scanf("%d",&data);
    int arr[data];
    for(i=0;i<data;i++)
    {
        arr[i]=rand()%100;
    }
    printf("unsorted:::::\n");
    print_array(arr,data);
    int temp[data];
    TopDownMergeSort(arr,temp,data);
    printf("sorted:::::\n");
    print_array(arr,data);
    return 0;
}

