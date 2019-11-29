#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
struct node
{
    float count;
    char word[20];
    struct node *link;
};
typedef struct node NODE;

struct vector
{
    NODE *head;
};
typedef struct vector VECTOR;

struct graphnode
{
    char word[20];
    struct graphnode *next;
    struct graphnode *lemma;
};
typedef struct graphnode GNODE;

struct letter
{
    char ch;
    struct graphnode *next;
    struct letter *down;
};
typedef struct letter FIRST;

struct list
{
    GNODE *head;
};
typedef struct list LIST;

struct graph
{
    FIRST *head;
};
typedef struct graph GRAPH;

void init(GRAPH*);
void create_graph(GRAPH*);
char *lemmatise(char*);
void display_graph(GRAPH*);
int count(char*,char*);
VECTOR* vectorise(char*);
void insert(VECTOR*,char*,int);
int count(char*,char*);
VECTOR* bow(VECTOR*,char*);
void tfidf(VECTOR*,char*,char*);
float sim(VECTOR*,VECTOR*);
float idf(float);
int docfreq(char*,char*);
void display_vector(VECTOR*);
int dbEndswith(char *str);

GRAPH *graph;

int main()
{

    graph = (GRAPH*)malloc(sizeof(GRAPH));
    init(graph);
    create_graph(graph);

    struct dirent *de;   
    DIR *dr = opendir("Database/"); 
    if (dr == NULL)   
    { 
        printf("Could not open current directory" ); 
        return 0; 
    } 
	char database[500][100]={};
	char tempfilename[100];
	int c=0;
    while ((de = readdir(dr)) != NULL) 
    {
		if(dbEndswith(de->d_name)==0 && strlen(de->d_name)>4)
		{
			strcpy(database[c],de->d_name);
			c++;
		}
	}
	closedir(dr);


    char s1[10000];
    char s2[10000];
    char ch;
    int pos = 0;
    scanf("%[^\0]s",s1);
    char s[20000]=""; int flag = 0;
    char p[] = "!\"#$%%&\'()*+,-./:;<=>?@[\\]^_`{|}~";

    for(int i = 0; i<strlen(s1);i++)
    {
        flag = 0;
        for(int j=0;j<strlen(p);j++)
        {
            if(p[j]==s1[i])
            {
                flag = 1;
                break;
            }
        }
        if (flag==0)
        {
            s[pos] = tolower((char)s1[i]);
            pos++;
        }
    }
    strcpy(s1,s);
    //printf("\n%s",s1);
	pos = 0;
    FILE *fp,*Write;
	strcpy(s2,"");
	char distemp[50];
    float similarity = 0.0;
    char allw[10000];
	for (int i = 0;i<500;i++)
	{
		if(strcmp(database[i],"")==0) continue;
		strcpy(distemp,database[i]);
		strcpy(tempfilename,"Database/");
		strcat(tempfilename,database[i]);
		strcpy(database[i],tempfilename);
		fp = fopen(database[i],"r");
		//fscanf(fp,"%[^\0]s",s2);
        pos = 0;
        while((ch=fgetc(fp))!=EOF)
        {
            s2[pos] = ch;
            pos++;
        }
        s2[pos]='\0';

		//fflush(fp);
        strcpy(allw,s1);
        strcat(allw," ");
        strcat(allw,s2);

        VECTOR *all = vectorise(allw);
        VECTOR *v1 = bow(all,s1);
        VECTOR *v2 = bow(all,s2);
        tfidf(v1,s1,s2);
        tfidf(v2,s1,s2);
		similarity = sim(v1,v2)*100;

		if(similarity>=60.0)
			printf("\n%s %lf%% PLAGIARISED.",distemp,similarity);
		else
			printf("\n%s %lf%% NOT PLAGIARISED.",distemp,similarity);
		strcpy(s2,"");
        strcpy(allw,"");
		similarity = 0.0;
        #if 0
        all->head = NULL;
        v1->head = NULL;
        v2->head = NULL;
        #endif

        #if 1
        free(all);
        free(v1);
        free(v2);
        #endif
		fclose(fp);
	}
    free(graph);
}

float sim (VECTOR *v1, VECTOR *v2)
{
    NODE *n1 = (NODE*)malloc(sizeof(NODE));
    NODE *n2 = (NODE*)malloc(sizeof(NODE));
    n1 = v1->head;
    n2 = v2->head;
    float sum = 0.0;
    float mag1 = 0.0, mag2 = 0.0;
    while(n1!=NULL)
    {
        sum+=(n1->count)*(n2->count);
        mag1+=(n1->count)*(n1->count);
        mag2+=(n2->count)*(n2->count);
        n1 = n1->link;
        n2 = n2->link;
    }
    mag1 = sqrt(mag1);
    mag2 = sqrt(mag2);
    return (sum/(mag1*mag2));
}


void display_vector(VECTOR* v)
{
    NODE *pres = v->head;
    while(pres!=NULL)
    {
        printf("%s %f\n", pres->word,pres->count);
        pres = pres->link;
    }
}

VECTOR *vectorise(char *s)
{
    VECTOR *v = (VECTOR*)malloc(sizeof(VECTOR));
    v->head = NULL;
    int ctr = 0;
    char temp[10000];
    char temp2[10000];
    strcpy(temp,s);
    char *token = strtok(temp," ");
    char *l;
    while(token!=NULL)
    {
        strcpy(temp2,token);
        l = lemmatise(temp2);
        ctr = count(token,s);
        insert(v,l,ctr);
        token = strtok(NULL," ");
    }
    return v;
}

void insert(VECTOR *v, char *word, int ctr)
{
    NODE *node = (NODE*)malloc(sizeof(NODE));
    strcpy(node->word,word);
    node->count = ctr;
    node->link = NULL;
    if(v->head==NULL)
        v->head = node;
    else
    {
        NODE* pres = v->head;
        NODE* prev = NULL;
        int flag = 0;
        while(pres!=NULL)
        {
            if(!strcmp(pres->word,word))
                flag = 1;
            prev = pres;
            pres = pres->link;
        }
        if (flag==0)
            prev->link = node;
    }
}

int count(char *toSearch,char *str)
{
    int i, j, found, count;
    int stringLen, searchLen;

    stringLen = strlen(str);      // length of string
    searchLen = strlen(toSearch); // length of word to be searched

    count = 0;

    for(i=0; i <= stringLen-searchLen; i++)
    {
        /* Match word with string */
        found = 1;
        for(j=0; j<searchLen; j++)
        {
            if(str[i + j] != toSearch[j])
            {
                found = 0;
                break;
            }
        }

        if(found == 1)
        {
            count++;
        }
    }

    return count;
}

VECTOR* bow(VECTOR *all,char *s)
{
    VECTOR *v = (VECTOR*)malloc(sizeof(VECTOR));
    v->head = NULL;
    NODE* pres = all->head;
    while(pres!=NULL)
    {
        insert(v,pres->word,count(pres->word,s));
        pres = pres->link;
    }
    return v;
}

void tfidf(VECTOR *v, char*s1, char*s2)
{
    NODE* pres = v->head;
    float tf = 0;float df= 0;
    while(pres!=NULL)
    {
        tf = pres->count;
        df = docfreq(pres->word,s1)+docfreq(pres->word,s2);
        pres->count = tf*idf(df);
        pres = pres->link;
    }
}

int docfreq(char*word, char*s1)
{
    char temp1[20000]= "";
    strcpy(temp1,s1);
    char *token1 = strtok(temp1," ");
    while(token1!=NULL)
    {
        if(!strcmp(word,token1))
            return 1;
        token1 = strtok(NULL," ");
    }
    return 0;
}

float idf(float df)
{ 
    return (1+log(3.0/(1.0+df)));
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
        GNODE *l =(GNODE*)malloc(sizeof(GNODE));
        l->next = l->lemma = NULL;
        strcpy(l->word,token);
        token = strtok(NULL,",");
        while(token!=NULL)
        {
            GNODE *node = (GNODE*)malloc(sizeof(GNODE));
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
                GNODE *cur = pres->next;
                GNODE *prev = NULL;
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
void displaygraph(GRAPH *graph)
{
    FIRST *node = graph->head;
    while(node!=NULL)
    {
        printf("%c->\n",node->ch);
        GNODE *pres = node->next;
        while(pres!=NULL)
        {
            printf("%s->%s\n",pres->word,pres->lemma);
            pres = pres->next;
        }
        node =node->down;
    }
}
char *lemmatise(char *s)
{
    int ctr = 0;
    FIRST *pres = graph->head;
    char x = s[0];
    while(pres->ch!=x)
    {
        ctr++;
        pres = pres->down;
    }
    GNODE *cur = pres->next;
    while(cur!=NULL)
    {
        ctr++;
        if(strcmp(cur->word,s)==0)
            return cur->lemma->word;
        cur = cur->next;
    }
    return s;
}
int dbEndswith(char *str)
{
	char cmtext[]=".txt";
	int n=strlen(str);
	char t[20];
	str=str+n-4;
	strcpy(t,str);
	return strcmp(t,cmtext);
}