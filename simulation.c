#include "graph.h"
//array defining truth table of gates
int and_s[2][2] = {{0, 0}, {0, 1}};
int or_s[2][2] = {{0, 1}, {1, 1}};
int xor_s[2][2] = {{0, 1}, {1, 0}};
int not_s[2] = {1, 0};
int from_s[2] = {0, 1};
/********************************************************************************************************************************************/
//function to convert character to integer and integer to character
/*******************************************************************************************************************************************/
int CharToInt(char c)
{
	int toReturn;
	if (c == 'x')
	{
		toReturn = 2;
	}
	else
	{
		toReturn = c - '0';
	}
	return toReturn;
}

char IntToChar(int n)
{
	char c;
	if (n == 2)
	{
		c = 'x';
	}
	else
	{
		c = '0' + n;
	}
	return c;
}
//******************************************************************************************************************************************/
//funtion to to output and read vec file;
//******************************************************************************************************************************************/
int output(int gate, int in1, int in2)
{
	int out;
	switch (gate)
	{
	case 9:
		out = not_s[in1];
		break;
	case 2:
	case 3:
		out = and_s[in1][in2];
		break;
	case 4:
	case 5:
		out = or_s[in1][in2];
		break;
	case 6:
	case 7:
		out = xor_s[in1][in2];
		break;
	case 8:
	case 10:
		out = from_s[in1];
		break;
	default:
		printf("something went wrong\n");
	}
	return out;
}

int readvec(FILE *fvec, VECTOR *vec)
{
	int a = 0;
	char str[Mpi];
	while (!feof(fvec))
	{
		bzero(str, Mpi);
		fgets(str, Mpi, fvec);
		if (*str != '\0')
		{
			bzero(vec[a].vector, Mpi);
			strcpy(vec[a].vector, str);
			a++;
		}
	}
	return a;
}
///***********************************************************************************************************************************/
//simulation of correct ckt
//************************************************************************************************************************************/

int simulateCircuit_ff(NODE *graph, int max_node, char *input_pattern, char *c_out)
{
	int input_pattern_vector_count, graph_node_count;
	LIST *tempNode;
	int x = 0, y = 0, z = 0;
	int a = 0, i = 0;
	for (graph_node_count = 1; graph_node_count <= max_node; graph_node_count++)
	{
		if (graph[graph_node_count].Type == INPT)
		{
			graph[graph_node_count].Cval = CharToInt(input_pattern[a]);
			a++;
		}
		else
		{
			if (graph[graph_node_count].Fin != NULL)
			{
				tempNode = graph[graph_node_count].Fin;
				x = graph[tempNode->id].Cval;
				while (tempNode != NULL)
				{
					if (tempNode->next != NULL)
					{
						y = graph[tempNode->next->id].Cval;
						x = output(graph[graph_node_count].Type, x, y);
					}
					tempNode = tempNode->next;
				}
				if (graph[graph_node_count].Type == FROM || graph[graph_node_count].Type == NOT || graph[graph_node_count].Type == BUFF)
					graph[graph_node_count].Cval = output(graph[graph_node_count].Type, x, 1);
				if (graph[graph_node_count].Type == AND || graph[graph_node_count].Type == OR || graph[graph_node_count].Type == XOR)
					graph[graph_node_count].Cval = x;
				if (graph[graph_node_count].Type == NAND || graph[graph_node_count].Type == NOR || graph[graph_node_count].Type == XNOR)
					graph[graph_node_count].Cval = not_s[x];
				if (graph[graph_node_count].Po == 1)
				{
					c_out[i] = IntToChar(graph[graph_node_count].Cval);
					i++;
				}
			}
		}
	}
	return i;
}
//*************************************************************************************************************************************
//simulation of faulty circuit
//*************************************************************************************************************************************

int simulateCircuit_f(NODE *graph, int max_node, char *input_pattern, char *f_out, int node, int type_change)
{
	int input_pattern_vector_count, graph_node_count;
	LIST *tempNode;
	int x = 0, y = 0, z = 0;
	int a = 0, i = 0, type;
	for (graph_node_count = 1; graph_node_count <= max_node; graph_node_count++)
	{
		type = graph[graph_node_count].Type;
		if (graph_node_count == node)
		{
			graph[graph_node_count].Type = type_change;
			
		}

		if (graph[graph_node_count].Type == INPT)
		{
			graph[graph_node_count].Cval = CharToInt(input_pattern[a]);
			a++;
		}
		else
		{
			if (graph[graph_node_count].Fin != NULL)
			{
				tempNode = graph[graph_node_count].Fin;
				x = graph[tempNode->id].Cval;
				while (tempNode != NULL)
				{
					if (tempNode->next != NULL)
					{
						y = graph[tempNode->next->id].Cval;
						x = output(graph[graph_node_count].Type, x, y);
					}
					tempNode = tempNode->next;
				}
				if (graph[graph_node_count].Type == FROM || graph[graph_node_count].Type == NOT || graph[graph_node_count].Type == BUFF)
					graph[graph_node_count].Cval = output(graph[graph_node_count].Type, x, 1);
				//printf("node=%d val=%d\n",graph_node_count, graph[graph_node_count].Cval);
				if (graph[graph_node_count].Type == AND || graph[graph_node_count].Type == OR || graph[graph_node_count].Type == XOR)
					graph[graph_node_count].Cval = x;
				//printf("node=%d val=%d\n",graph_node_count, graph[graph_node_count].Cval);
				if (graph[graph_node_count].Type == NAND || graph[graph_node_count].Type == NOR || graph[graph_node_count].Type == XNOR)
					graph[graph_node_count].Cval = not_s[x];
				//printf("node=%d val=%d\n",graph_node_count, graph[graph_node_count].Cval);
				if (graph[graph_node_count].Po == 1)
				{
					f_out[i] = IntToChar(graph[graph_node_count].Cval);
					i++;
					//printf("%c", int_to_char(graph[graph_node_count].Cval));
				}
			}
		}
	}
	graph[node].Type = type;
	return i;
}

//*****************************************************************************************************************************
//function to simulate all the pattern and circuits in order to detect faults that are injected
//*****************************************************************************************************************************

void simulate(NODE *graph, int max, VECTOR *pattern, int total)
{
	FILE *find_error;
	char c_out[Mpo], f_out[Mpo];
	int cout = 0, fout = 0, type = 0, i, j, k, l, p;
	find_error = fopen("resultc17.txt", "w+");
	if (find_error == NULL)
	{
		printf("no file found::exiting:\n");
		exit(-1);
	}
	for (p = 0; p < total; p++) //passing single pattern at a time
	{
		fprintf(find_error, "%s", pattern[p].vector);
		//fprintf(find_error,"\n");
		bzero(c_out, strlen(c_out));
		cout = simulateCircuit_ff(graph, max, pattern[p].vector, c_out);
		c_out[cout] = '\0';

		FAULT *output[cout];

		for (l = 0; l < cout; l++)
		{
			output[l] = NULL;
		}
		for (i = 0; i <= max; i++) //checking all the faults for a pattern each time
		{
			type = graph[i].Type;
			if (graph[i].Type == 2 || graph[i].Type == 3 || graph[i].Type == 4 || graph[i].Type == 5 || graph[i].Type == 6 || graph[i].Type == 7)
			{
				for (j = 2; j <= 7; j++)
				{
					if (type == j)
						continue;
					else
					{
						bzero(f_out, strlen(f_out));
						fout = simulateCircuit_f(graph, max, pattern[p].vector, f_out, i, j);
						f_out[fout] = '\0';
						for (k = 0; k < cout; k++)
						{
							int count_s = 0;
							if (c_out[k] != f_out[k])
							{
								Insertoutput(&output[k], i,j);
								//printf("%c %c %d %d\n", c_out[k], f_out[k], i, j);
								//fprintf(find_error, "\t%d/%d", i, j);
							}
						}
					}
				}
			}
			if (graph[i].Type == 8 || graph[i].Type == 9)
			{
				if (type == 8)
					j = 9;
				else
					j = 8;
				bzero(f_out, strlen(f_out));
				fout = simulateCircuit_f(graph, max, pattern[p].vector, f_out, i, j);
				f_out[fout] = '\0';
				for (k = 0; k < cout; k++)
				{
					if (c_out[k] != f_out[k])
					{
						Insertoutput(&output[k], i,j);
						//printf("%c %c %d %d\n", c_out[k], f_out[k], i, j);
					}	//printf("%d\n", c_out[k]);
				}
			}
		}

		for (int l = 0; l < cout; l++)       //prints to the file 
		{
			fprintf(find_error,"OUT[%d]::",l);
			FAULT *temp = NULL;
			temp = output[l];
			//temp1 = output[l];
			while (temp != NULL)
			{
				//printf("%d\n%d\n", temp->id,temp->fault);
				fprintf(find_error,"\t%d/%d", temp->id,temp->fault);
				temp = temp->next;
			}
			fprintf(find_error,"\n");
			//printf("\n");
		}
		for (int l = 0; l < cout; l++)  //freeing the LIST name FAULT
		{
			FreeFault(&output[l]);
		}
	}
	fclose(find_error);  //close the file 
}