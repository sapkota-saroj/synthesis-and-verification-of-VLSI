#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <strings.h>
#include <math.h>
#include <time.h>
#include <complex.h>
#include <assert.h>
#include <limits.h>

typedef struct LIST_type
{
    int id;                 //id for current element
    struct LIST_type *next; //pointer to next id element( if there is no element, then it will be NULL)
} LIST;

void InsertList(LIST **l, int x)
{
    LIST *p, *tl;

    if ((p = (LIST *)malloc(sizeof(LIST))) == NULL)
    {
        printf("LIST: Out of memory\n");
        exit(1);
    }
    else
    {
        p->id = x;
        p->next = NULL;
        if (*l == NULL)
        {
            *l = p;
            return;
        }
        tl = NULL;
        tl = *l;
        while (tl != NULL)
        {
            // if (tl->id == x)
            // {
            //     break;
            // }
            if (tl->next == NULL)
            {
                tl->next = p;
                break;
            }
            tl = tl->next;
        }
    }
    return;
}
void PrintList(LIST *l)
{
    LIST *temp = NULL;

    temp = l;
    while (temp != NULL)
    {
        printf("%d ", temp->id);
        temp = temp->next;
    }
    printf("\n");
    return;
}
void FreeList(LIST **l)
{
    LIST *temp = NULL;

    if (*l == NULL)
    {
        return;
    }
    temp = (*l);
    while ((*l) != NULL)
    {
        temp = temp->next;
        free(*l);
        (*l) = temp;
    }
    (*l) = NULL;
    return;
}
int main()
{
    int cout = 4;
    LIST *count[cout];
    LIST *output[cout];
    for (int k = 0; k < cout; k++)
    {
        count[k] = NULL;
        output[k] = NULL;
    }
    for (int l = 2; l < 10; l++)
    {
        for (int i = 2; i < 7; i++)
        {
            if (l == i)
                continue;
            else
            {
                for (int j = 0; j < cout; j++)
                {
                    
                        //printf("hello1=%d\n",i);
                        InsertList(&count[j], i);
                        //printf("hello2\n");
                        InsertList(&output[j], l);
                        //printf("hello3\n");
                   
                }
            }
        }
    }
    for (int k = 0; k < cout; k++)
    {
        LIST* temp=NULL;
        temp=count[k];
        while(temp!=NULL)
        {
            printf("%d ",temp->id);
            temp=temp->next;
        }
        PrintList(output[k]);
    }
    for (int k = 0; k < cout; k++)
    {
        FreeList(&count[k]);
    }
    return 0;
}
