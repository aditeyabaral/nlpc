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

static int hash(char a) {
    return a - 'a';
}

LEMMATIZER *lemmatizer()
{
    LEMMATIZER *graph = (LEMMATIZER *)malloc(sizeof(LEMMATIZER));
    graph->head = (FIRST *)calloc(sizeof(FIRST), 26);
    for (int i = 0; i < 26; ++i) {
        graph->head[i].ch = i+'a';
        graph->head[i].next = NULL;
    }

    FILE *fp = fopen("include/VerbForms.csv", "r");
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
            FIRST *pres = &graph->head[hash(x)];
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
    char x = str[0];
    FIRST *pres = &graph->head[hash(x)];
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
