struct lemmanode
{
    char word[50];
    struct lemmanode *next;
    struct lemmanode *lemma;
};
typedef struct lemmanode LEMMANODE;

struct letter
{
    char ch;
    struct lemmanode *next;
    struct letter *down;
};
typedef struct letter FIRST;

struct lemmalist
{
    LEMMANODE *head;
};
typedef struct lemmalist LIST;

struct graph
{
    FIRST *head;
};
typedef struct graph LEMMATIZER;

LEMMATIZER *lemmatizer()
{
    LEMMATIZER *graph = (LEMMATIZER *)malloc(sizeof(LEMMATIZER));
    FIRST *A = (FIRST *)malloc(sizeof(FIRST));
    A->ch = 'a';
    A->down = NULL;
    A->next = NULL;
    graph->head = A;

    FIRST *pres = graph->head;
    int ctr = 98;

    while (ctr <= 122)
    {
        FIRST *node = (FIRST *)malloc(sizeof(FIRST));
        node->ch = (char)ctr;
        node->down = NULL;
        node->next = NULL;
        pres->down = node;
        pres = pres->down;
        ctr++;
    }

    FILE *fp = fopen("VerbForms.csv", "r");
    char s[100], temp[100];
    char *token, *save;
    char x;
    while (fgets(s, 100, fp) != NULL)
    {
        strcpy(temp, s);
        token = strtok_r(temp, ",", &save);
        x = token[0];
        LEMMANODE *l = (LEMMANODE *)malloc(sizeof(LEMMANODE));
        l->next = l->lemma = NULL;
        strcpy(l->word, token);
        token = strtok_r(NULL, ",", &save);
        while (token != NULL)
        {
            LEMMANODE *node = (LEMMANODE *)malloc(sizeof(LEMMANODE));
            strcpy(node->word, token);
            node->lemma = l;
            node->next = NULL;
            FIRST *pres = graph->head;
            while (pres->ch != x)
                pres = pres->down;
            if (pres->next == NULL)
            {
                LIST *l = (LIST *)malloc(sizeof(LIST));
                l->head = node;
                pres->next = node;
            }
            else
            {
                LEMMANODE *cur = pres->next;
                LEMMANODE *prev = NULL;
                while (cur != NULL)
                {
                    prev = cur;
                    cur = cur->next;
                }
                prev->next = node;
            }
            token = strtok_r(NULL, ",", &save);
        }
    }
    fclose(fp);
    return graph;
}

char *lemmatize(LEMMATIZER *graph, char *str)
{
    int ctr = 0;
    FIRST *pres = graph->head;
    char x = str[0];
    while (pres->ch != x)
    {
        ctr++;
        pres = pres->down;
    }
    LEMMANODE *cur = pres->next;
    while (cur != NULL)
    {
        ctr++;
        if (strcmp(cur->word, str) == 0)
            return cur->lemma->word;
        cur = cur->next;
    }
    return str;
}