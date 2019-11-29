#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct node
{
    char word[20];
    struct node *next;
    struct node *lemma;
};
typedef struct node NODE;

struct letter
{
    char ch;
    struct node *next;
    struct letter *down;
};
typedef struct letter FIRST;

struct list
{
    NODE *head;
};
typedef struct list LIST;

struct graph
{
    FIRST *head;
};
typedef struct graph GRAPH;

void init(GRAPH*);
void create_graph(GRAPH*);
char *lemmatise(GRAPH*,char*);
void display(GRAPH*);

int main()
{
    GRAPH *graph = (GRAPH*)malloc(sizeof(GRAPH));
    init(graph);
    create_graph(graph);
    //display(graph);
    #if 1
    char s[20];
    printf("Enter word : ");
    scanf("%s", &s);
    char *lemma = lemmatise(graph,s);
    printf("Lemma : %s\n",lemma);
    #endif
}
void init(GRAPH *graph)
{
    FIRST *A = (FIRST*)malloc(sizeof(FIRST));
    A->ch = 'a';
    A->down = NULL;
    A->next = NULL;
    graph->head = A;

    FIRST *pres = graph->head;
    int ctr = 98;

    while(ctr<=122)
    {
        FIRST *node = (FIRST*)malloc(sizeof(FIRST));
        node->ch = (char)ctr;
        node->down = NULL;
        node->next = NULL;
        pres->down = node;
        pres = pres->down;
        ctr++;
    }
}
void create_graph(GRAPH *graph)
{
    FILE *fp = fopen("VerbForms.csv","r");
    char s[100], temp[100];
    char *token; char x; 
    while(fgets(s,100,fp)!=NULL)
    {
        strcpy(temp,s);
        token = strtok(temp,",");
        x = token[0];
        NODE *l =(NODE*)malloc(sizeof(NODE));
        l->next = l->lemma = NULL;
        strcpy(l->word,token);
        token = strtok(NULL,",");
        while(token!=NULL)
        {
            NODE *node = (NODE*)malloc(sizeof(NODE));
            strcpy(node->word,token);
            node->lemma = l;
            node->next = NULL;
            FIRST *pres = graph->head;
            while(pres->ch!=x)
                pres = pres->down;
            if (pres->next==NULL)
            {
                LIST *l = (LIST*)malloc(sizeof(LIST));
                l->head = node;
                pres->next = node;
            }
            else
            {
                NODE *cur = pres->next;
                NODE *prev = NULL;
                while(cur!=NULL)
                {
                    prev = cur;
                    cur = cur->next;
                }
                prev->next = node;
            }
            token = strtok(NULL,",");
        }
    }
    fclose(fp);
}
void display(GRAPH *graph)
{
    FIRST *node = graph->head;
    while(node!=NULL)
    {
        printf("%c->\n",node->ch);
        NODE *pres = node->next;
        while(pres!=NULL)
        {
            printf("%s->%s\n",pres->word,pres->lemma);
            pres = pres->next;
        }
        node = node->down;
    }
}
char *lemmatise(GRAPH *graph, char *s)
{
    int ctr = 0;
    FIRST *pres = graph->head;
    char x = s[0];
    while(pres->ch!=x)
    {
        ctr++;
        pres = pres->down;
    }
    NODE *cur = pres->next;
    while(cur!=NULL)
    {
        ctr++;
        if(strcmp(cur->word,s)==0)
            return cur->lemma->word;
        cur = cur->next;
    }
    return s;
}