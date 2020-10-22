//Saroj Sapkota  856310025
//procedure to run project
//1. make file 
//2.Then run command ./project Arg1 Arg2 Arg3
//where Arg1= isc file , Arg2 =bench file (test_file.bench), Arg3= name of file to save pattern set(c17.txt)
//this will finally print result of fault detected on resultc17.txt file.



#include "graph.h"
/***************************************************************************************************************************
 Main Function(Serial and Parallel Fault Simulation)
****************************************************************************************************************************/
void main(int argc,char **argv)
{
FILE *fisc,*bench,*fault,*test;             //file pointers used for .isc file, .vec file, .faults file and resultfile
int Max,Max_b,Opt,Npi,Npo,Total,Tfs;              //maxnode id,option,tot no of PIs,tot no of Pos,Tot no of input patterns& faults in.vec in.faults

NODE graph[Mnod];                         //structure used to store the ckt information in .isc file 
static VECTOR vector[150000];                      //structure used to store the input vectors information in .vec file 

//FAULT stuck[Mft];                      //structure used to store the faults information in .faults file
int a,b,c,d;                             //random variables


//Read the .isc file and store the information in graph structure
fisc=fopen(argv[1],"r");                           //file pointer to open .isc file 
Max=0; 
Max=ReadIsc(fisc,graph);                 //read .isc file and return index of last node in graph formed
fclose(fisc);
//bench=fopen(argv[2],"w+");
char filename[20];
strcpy(filename,argv[2]);
make_modified_bench(graph,Max,filename);
//fclose(fres);                          //close file pointer for .isc file
PrintCircuit(graph,Max);                 //print all members of graph structure
bench=fopen(argv[3],"w");
print_test_file(bench);
fclose(bench);

//Read the .vec file and store the information in  vector structure
test=fopen(argv[3],"r");                                               //file pointer to open .faults file 
if(test==NULL){
printf("can't open file \n");
exit(-1);}
Total=readvec(test,vector);
fclose(test);                                                          //close file pointer for .faults file
printf("\nTot No of Pattern:%d \n",Total);

//Opt=0; 
//Opt=atoi(argv[3]);                          //getting the option from terminal for xval
//fres=fopen(argv[2],"w");                           //file pointer to open .out file for printing results

//Perform Logic Simulation for each Input vector and print the Pos .val in output file   
simulate(graph,Max,vector,Total);
//fclose(fres);                                                  //close file pointer for .out file

ClearCircuit(graph,Mnod);                                      //clear memeory for all members of graph
//for(a=0;a<Total;a++){ bzero(vector[a].piv,Mpi); }                //clear memeory for all members of vector
return;
}//end of main
/****************************************************************************************************************************/

