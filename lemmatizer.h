struct lemmanode
{
    char word[50];
    struct lemmanode *next;
    struct lemmanode *lemma;
};
typedef struct lemmanode NODE;

struct letter
{
    char ch;
    struct lemmanode *next;
    struct letter *down;
};
typedef struct letter FIRST;

struct lemmalist
{
    NODE *head;
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
        NODE *l = (NODE *)malloc(sizeof(NODE));
        l->next = l->lemma = NULL;
        strcpy(l->word, token);
        token = strtok_r(NULL, ",", &save);
        while (token != NULL)
        {
            NODE *node = (NODE *)malloc(sizeof(NODE));
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
                NODE *cur = pres->next;
                NODE *prev = NULL;
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