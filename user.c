#include "graph.h"

int and[] = {3, 4, 5, 6, 7};
int nand[] = {2, 4, 5, 6, 7};
int or [] = {2, 3, 5, 6, 7};
int nor[] = {2, 3, 4, 6, 7};
int xor [] = {2, 3, 4, 5, 7};
int xnor[] = {2, 3, 4, 5, 6};
int buff[] = {9, 0};
int not[] = {8, 0};
int array[3000] = {0};
int output_o[150] = {0};
int output_n[150] = {0};
int or_gate[150] = {0};
int iteration = 0;
int pattern[Mnod] = {0};
char pattern_vec[Mnod][Mpt][Mlin];
int length = 0;
void gate_check(FILE *bench, int type_rep)
{
    fprintf(bench, "%s", " = ");
    if (type_rep == 2)
        fprintf(bench, "%s", "AND(");
    if (type_rep == 3)
        fprintf(bench, "%s", "NAND(");
    if (type_rep == 4)
        fprintf(bench, "%s", "OR(");
    if (type_rep == 5)
        fprintf(bench, "%s", "NOR(");
    if (type_rep == 6)
        fprintf(bench, "%s", "XOR(");
    if (type_rep == 7)
        fprintf(bench, "%s", "XNOR(");
    if (type_rep == 8)
        fprintf(bench, "%s", "BUFF(");
    if (type_rep == 9)
        fprintf(bench, "%s", "NOT(");
}

void exclusive_gate(FILE *bench, NODE *graph, int Max, int type, int *count, int node_no, LIST *temp)
{
    int i = 1;
    int x, y, id_previous;
    if (i = 1)
    {
        x = temp->id;
        y = temp->next->id;
        id_previous = *count + Max;
        fprintf(bench, "%d,%d", test_fin(graph, x), test_fin(graph, y));
        fprintf(bench, "%c\n", ')');
        i++;
    }
    temp = temp->next->next;
    while (temp != NULL)
    {
        *count = *count + 1;
        x = temp->id;
        fprintf(bench, "%d", *count + Max);
        gate_check(bench, type);
        fprintf(bench, "%d,%d", id_previous, test_fin(graph, x));
        fprintf(bench, "%c\n", ')');
        id_previous = *count + Max;
        temp = temp->next;
    }
    graph[node_no].Mark = id_previous;
   // printf("gate[%d]=%d id_previous=%d\n",node_no,graph[node_no].Mark,id_previous);
}

int test_fin(NODE *graph, int fin_id)
{
    int temp_l;
    if (graph[fin_id].Mark != 0)
        return graph[fin_id].Mark;
    else
    {
        if (graph[fin_id].Type == 10)
        {
            temp_l = graph[fin_id].Fin->id;
            if (graph[temp_l].Mark != 0)
                return graph[temp_l].Mark;
            else
                return graph[fin_id].Fin->id;
        }
        else
            return fin_id;
    }
}

void readisc_bench(NODE *graph, int Max, FILE *bench)
{
    int i, y, x, j, ndoe_no, type_local;
    LIST *temp;
    // bench = fopen(filename, "w");
    fprintf(bench, "%s\n", "# modified circuit");
    j = 0;
    for (i = 0; i <= Max; i++)
    {
        temp = NULL;
        if (graph[i].Type == 1)
        {
            fprintf(bench, "%s", "INPUT(");
            fprintf(bench, "%d", i);
            fprintf(bench, "%c\n", ')');
        }
        if (graph[i].Po == 1)
        {
            output_o[j] = i;
            j++;
        }
        if (graph[i].Type != 0)
        {
            if (graph[i].Type != 1 && graph[i].Type != 10)
            {
                fprintf(bench, "%d", i);
                type_local = graph[i].Type;
                gate_check(bench, type_local);
                temp = graph[i].Fin;
                x = graph[i].Nfi;
                y = 0;
                while (temp != NULL)
                {
                    y++;
                    ndoe_no = temp->id;
                    if (graph[ndoe_no].Type == 10)
                    {
                        fprintf(bench, "%d", graph[ndoe_no].Fin->id);
                    }
                    else
                        fprintf(bench, "%d", temp->id);
                    temp = temp->next;
                    if (y != x)
                        fprintf(bench, "%c", ',');
                }
                fprintf(bench, "%c\n", ')');
            }
        }
    }
}

int dublicate_bench(NODE *graph, int Max, int type, int id_change, FILE *bench)
{
    int *count, i, j, x, y, type_local, pass_id;
    LIST *temp;
    int count_S = 0;
    count = &count_S;
    j = 0;
    for (i = 0; i <= Max; i++)
    {
        temp = NULL;
        if(graph[i].Type==INPT && graph[i].Nfo==0)
        {
            output_n[j]=i;
            j++;
        }
        if (i == id_change)
        {
            *count = *count + 1;
            graph[i].Mark = *count + Max;
            fprintf(bench, "%d", (*count + Max));
            gate_check(bench, type);
            temp = graph[i].Fin;
            if (type == 6 || type == 7)
            {
                exclusive_gate(bench, graph, Max, type, count, id_change, temp);
            }
            else
            {
                x = graph[i].Nfi;
                y = 0;
                while (temp != NULL)
                {
                    y++;
                    pass_id = temp->id;
                    fprintf(bench, "%d", test_fin(graph, pass_id));
                    temp = temp->next;
                    if (y != x)
                        fprintf(bench, "%c", ',');
                }
                fprintf(bench, "%c\n", ')');
            }
             if (graph[i].Po == 1)
            {
                output_n[j] = *count + Max;
                j++;
            }
        }
        if (graph[i].Type != 0)
        {
            if (i != id_change && graph[i].Type != INPT && graph[i].Type != FROM)
            {
                *count = *count + 1;
                graph[i].Mark = *count + Max;

                if (graph[i].Po == 1)
                {
                    output_n[j] = *count + Max;
                    j++;
                }
                type_local = graph[i].Type;
                fprintf(bench, "%d", *count + Max);
                gate_check(bench, type_local);
                temp = graph[i].Fin;
                x = graph[i].Nfi;
                y = 0;
                while (temp != NULL)
                {
                    y++;
                    pass_id = temp->id;
                    fprintf(bench, "%d", test_fin(graph, pass_id));
                    temp = temp->next;
                    if (y != x)
                        fprintf(bench, "%c", ',');
                }
                fprintf(bench, "%c\n", ')');
            }
        }
    }
    //output of bench file/////////////////////////////////////////////////////////////////////////////
    i = 0;
    while (output_n[i] != 0)
    {
        *count = *count + 1;
        or_gate[i] = Max + *count;
        fprintf(bench, "%d", Max + *count);
        fprintf(bench, "%s", " = ");
        fprintf(bench, "%s", "XOR(");
        fprintf(bench, "%d,%d", output_o[i], output_n[i]);
        fprintf(bench, "%c\n", ')');
        i++;
        if (i == j)
        {
            *count = *count + 1;
            fprintf(bench, "%d", *count + Max);
            fprintf(bench, "%s", " = ");
            fprintf(bench, "%s", "OR(");
            for (int k = 0; k < i; k++)
            {
                fprintf(bench, "%d", or_gate[k]);
                if (k < i - 1)
                    fprintf(bench, "%c", ',');
            }
            fprintf(bench, "%c\n", ')');
        }
    }
    fprintf(bench, "%s", "OUTPUT(");
    fprintf(bench, "%d", Max + *count);
    fprintf(bench, "%c\n", ')');
    return (*count + Max);
}

int make_modified_bench(NODE *graph, int Max, char *filename)
{
    int type, i;
    for (i = 0; i <= Max; i++)
    {
        if (graph[i].Type == 2)
        {
            call_atlanta(graph, Max, i, and, filename);
        }
        else if (graph[i].Type == 3)
        {
            call_atlanta(graph, Max, i, nand, filename);
        }
        else if (graph[i].Type == 4)
        {
            call_atlanta(graph, Max, i, or, filename);
        }
        else if (graph[i].Type == 5)
        {
            call_atlanta(graph, Max, i, nor, filename);
        }
        else if (graph[i].Type == 6)
        {
            call_atlanta(graph, Max, i, xor, filename);
        }
        else if (graph[i].Type == 7)
        {
            call_atlanta(graph, Max, i, xnor, filename);
        }
        else if (graph[i].Type == 8)
        {
            call_atlanta(graph, Max, i, buff, filename);
        }
        else if (graph[i].Type == 9)
        {
            call_atlanta(graph, Max, i, not, filename);
        }
        else
        {
            ;
        }
    }
    return 0;
}

void call_atlanta(NODE *graph, int Max, int node_no, int *gate_array, char *filename)
{
    FILE *bench, *fault, *pattern_atalanta;
    int output, i, no_of_pattern;
    for (i = 0; i < 5; i++)
    {
        int type = *(gate_array + i);
        if (*(gate_array + i) == 0)
            break;
        bench = fopen(filename, "w+");
        readisc_bench(graph, Max, bench);
        output = dublicate_bench(graph, Max, type, node_no, bench);
        fclose(bench);
        fault = fopen("test_file.flt", "w+");
        fprintf(fault, "%d /0\n", output);
        fclose(fault);
        system("Atalanta/atalanta -D 20 -f test_file.flt test_file.bench");
        pattern_atalanta = fopen("test_file.test", "r");
        if (pattern_atalanta == NULL)
            printf("no file found\n");
        no_of_pattern = read_pattern_atalanta(pattern_atalanta);
        pattern[iteration] = no_of_pattern;
        iteration++;
        fclose(pattern_atalanta);
    }
}

int read_pattern_atalanta(FILE *pattern_atalanta)
{

    char line[Mlin], first[10], second[Mlin];
    int out, i;
    int pattern_count = 0;
    srand(time(NULL));
    do
    {
        bzero(first, strlen(first));
        bzero(second, strlen(second));
        bzero(line, strlen(line));
        fgets(line, Mlin, pattern_atalanta);
        sscanf(line, "%s %s %d", first, second, &out);
    } while (strcmp(second, "/0") != 0);

    bzero(line, strlen(line));
    fgets(line, Mlin, pattern_atalanta);

    while (!feof(pattern_atalanta))
    {
        bzero(first, strlen(first));
        bzero(second, strlen(second));
        sscanf(line, "%s %s %d", first, second, &out);
        length=strlen(second);
        for (i = 0; i < length; i++)
        {
            if (second[i] == 'x')
            {
                second[i] = rand() % 2 + '0';
            }
            pattern_vec[iteration][pattern_count][i] = second[i];
        }
        pattern_count++;
        bzero(line, Mlin);
        fgets(line, Mlin, pattern_atalanta);
    }
    return pattern_count;
}

int print_test_file(FILE* testfile)
{
    int group, test_set, loop, no, i;
    int random_pattern, count;
    srand(time(NULL));
    for (group = 1; group < 5; group++)
    {
        //fprintf(testfile, "*************GROUP=%d**********\n", group);
        for (test_set = 1; test_set <= 500; test_set++)
        {
            //fprintf(testfile, "************TEST_SET=%d**********\n", test_set);
            for (loop = 0; loop < iteration; loop++)
            {
                count = pattern[loop];
                for (no = 0; no < group; no++)
                {
                    if (count == 0)
                    {
                        break;
                    }
                    random_pattern = rand() % count;
                    for (i = 0; i < length; i++)
                    {
                        fprintf(testfile, "%c", pattern_vec[loop][random_pattern][i]);
                    }
                    fprintf(testfile, "\n");
                }
            }
        }
    }
    return 0;
}
