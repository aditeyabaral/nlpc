//Returns n-grams for characters as well as words in a character array separated by newline
//Usage : char_ngram(<character array>,<integer n>) or word_ngram(<character array>,<integer n>)
//Dependencies : stdlib.h, stdio.h, string.h

char* char_ngram(char* s, int n)
{
    char *ngram = (char*)calloc(100000,sizeof(char));
    char temp[n+1];
    for(int i=0;i<strlen(s);i++)
    {
        strcpy(temp,"");
        for(int j=0;j<n;j++)
            temp[j] = s[i+j];
        strcat(ngram,temp);
        ngram[strlen(ngram)] = '\n';
    }
    return ngram;
}
char* word_ngram(char*s, int n)
{
    char *ngram = (char*)calloc(100000,sizeof(char));
    char *words = (char*)calloc(100000,sizeof(char));
    int beg = 0;
    s[strlen(s)] = ' ';
    for(int i=0;i<strlen(s);i++)
    {
        if (s[i]==' ')
        {
            for(int j = 0;j<10000;j++)
                words[j] = '\0';
            int pos = 0;
            for(int j=beg;j<i;j++)
                words[pos++] = s[j];
            int ctr = 0;
            beg = i+1;
            words[pos++] = ' ';
            for(int j = i+1;j<strlen(s);j++)
            {
                if(ctr==(n-1))
                    break;
                if (s[j]==' ')
                    ctr++;
                words[pos++] = s[j];
            }
            strcat(ngram,words);
            ngram[strlen(ngram)] = '\n';
        }
    }
    s[strlen(s)] = '\0';
    return ngram;
}