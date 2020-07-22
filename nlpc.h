#include "stemmer.h"
#include "lemmatizer.h"
/*
Finds the minimum of two numbers, a and b
*/
int min(int a, int b);

/*
Converts a string to uppercase
Usage: upper(string)
*/
char *upper(char *str);

/*
Converts a string to lowercase
Usage: lower(string)
*/
char *lower(char *str);

/*
Removes contractions from any given string and returns the modified string
Usage : remove_contractions(string)
*/
char *removeContractions(char *str);

/*
Trims any blank spaces before and after the string and returns the modified string
Usage: trim(string)
*/
char *trim(char *str);

/*
Finds the minimum edit distance (Levenshtein's Distance) between two strings
Usage: levenshteinDistance(string1, string2)
*/
int levenshteinDistance(char *word1, char *word2);

/*
Removes any kind of symbols from a string and returns the modified string
Usage: removeSymbols(string)
*/
char *removeSymbols(char *str);

/*
Removes stopwords from any given string and returns the modified string
Usage : removeStopwords(<character array>)
*/
char *removeStopwords(char *str);

/*
Implementation of the Porter2 Stemmer or the Snowball Stemmer. Returns the stemmed word.
Usage : stem(string)
*/
char *stem(char *str);

/*
Lemmatizes a word and returns the resulting word.
Usage :
LEMMATIZER* lemmatizerObject = lemmatizer();
lemmatize(lemmatizerObject, string);
*/
char *lemmatize(LEMMATIZER *graph, char *str);

int min(int a, int b)
{
    return a < b ? a : b;
}

char *lower(char *str)
{
    int len = strlen(str);
    char *result = (char *)malloc(sizeof(char) * len);
    for (int i = 0; i < len; i++)
    {
        if (str[i] >= 65 && str[i] <= 90)
            result[i] = str[i] + 32;
        else
            result[i] = str[i];
    }
    return result;
}

char *upper(char *str)
{
    int len = strlen(str);
    char *result = (char *)malloc(sizeof(char) * len);
    for (int i = 0; i < len; i++)
    {
        if (str[i] >= 97 && str[i] <= 122)
            result[i] = str[i] - 32;
        else
            result[i] = str[i];
    }
    return result;
}

char *removeContractions(char *str)
{
    int len = strlen(str);
    char *s = (char *)malloc(sizeof(char) * len);
    strcpy(s, "");
    char *temp = (char *)malloc(sizeof(char) * 2 * len);
    strcpy(temp, str);
    char key[124][15] = {"ain't", "aren't", "can't", "can't've", "'cause", "could've", "couldn't", "couldn't've", "didn't", "doesn't",
                         "don't", "hadn't", "hadn't've", "hasn't", "haven't", "he'd", "he'd've", "he'll", "he'll've", "he's", "how'd", "how'd'y", "how'll",
                         "how's", "I'd", "I'd've", "I'll", "I'll've", "I'm", "I've", "i'd", "i'd've", "i'll", "i'll've", "i'm", "i've", "isn't", "it'd",
                         "it'd've", "it'll", "it'll've", "it's", "let's", "ma'am", "mayn't", "might've", "mightn't", "mightn't've", "must've", "mustn't",
                         "mustn't've", "needn't", "needn't've", "o'clock", "oughtn't", "oughtn't've", "shan't", "sha'n't", "shan't've", "she'd", "she'd've",
                         "she'll", "she'll've", "she's", "should've", "shouldn't", "shouldn't've", "so've", "so's", "this's", "that'd", "that'd've",
                         "that's", "there'd", "there'd've", "there's", "they'd", "they'd've", "they'll", "they'll've", "they're", "they've", "to've",
                         "wasn't", "we'd", "we'd've", "we'll", "we'll've", "we're", "we've", "weren't", "what'll", "what'll've", "what're", "what's", "what've",
                         "when's", "when've", "where'd", "where's", "where've", "who'll", "who'll've", "who's", "who've", "why's", "why've", "will've", "won't",
                         "won't've", "would've", "wouldn't", "wouldn't've", "y'all", "y'all'd", "y'all'd've", "y'all're", "y'all've", "you'd", "you'd've", "you'll",
                         "you'll've", "you're", "you've"};
    char value[124][20] = {"is not", "are not", "cannot", "cannot have", "because", "could have", "could not", "could not have", "did not",
                           "does not", "do not", "had not", "had not have", "has not", "have not", "he would", "he would have", "he will", "he he will have",
                           "he is", "how did", "how do you", "how will", "how is", "I would", "I would have", "I will", "I will have", "I am", "I have",
                           "i would", "i would have", "i will", "i will have", "i am", "i have", "is not", "it would", "it would have", "it will", "it will have",
                           "it is", "let us", "madam", "may not", "might have", "might not", "might not have", "must have", "must not", "must not have", "need not",
                           "need not have", "of the clock", "ought not", "ought not have", "shall not", "shall not", "shall not have", "she would", "she would have",
                           "she will", "she will have", "she is", "should have", "should not", "should not have", "so have", "so as", "this is", "that would",
                           "that would have", "that is", "there would", "there would have", "there is", "they would", "they would have", "they will", "they will have",
                           "they are", "they have", "to have", "was not", "we would", "we would have", "we will", "we will have", "we are", "we have", "were not",
                           "what will", "what will have", "what are", "what is", "what have", "when is", "when have", "where did", "where is", "where have", "who will",
                           "who will have", "who is", "who have", "why is", "why have", "will have", "will not", "will not have", "would have", "would not",
                           "would not have", "you all", "you all would", "you all would have", "you all are", "you all have", "you would", "you would have", "you will",
                           "you will have", "you are", "you have"};
    int check, pos;
    char *token, *save;
    token = strtok_r(temp, " ", &save);
    while (token != NULL)
    {
        check = -1;
        for (int i = 0; i <= 123; i++)
        {
            if (!strcmp(token, key[i]))
            {
                check = i;
                break;
            }
        }
        if (check == -1)
            strcat(s, token);
        else

            strcat(s, value[check]);
        strcat(s, " ");
        token = strtok_r(NULL, " ", &save);
    }
    free(temp);
    s = trim(s);
    return s;
}

char *removeSymbols(char *str)
{
    char punc[] = {'\n', '\'', '!', ']', '{', '#', '.', '<', '/', '(', '~', ',', '%', ';', '`', ':', '?', '+', '$', '^', '\\', '@', '*', '}', '=', '_', '\"', ')', '-', '|', '[', '&', '>'};
    int check, ctr = 0, len = strlen(str);
    char *s = (char *)malloc(sizeof(char) * len);
    for (int i = 0; i < len; i++)
    {
        check = 0;
        for (int j = 0; j < 33; j++)
        {
            if ((char)str[i] == (char)punc[j])
            {
                check = 1;
                break;
            }
        }
        if (!check)
        {
            s[ctr] = str[i];
            ctr++;
        }
    }
    s[ctr] = '\0';
    return s;
}

char *trim(char *str)
{
    int len = strlen(str);
    char *s = (char *)malloc(sizeof(char) * len);
    int pos1 = 0, pos2 = len - 1, ctr = 0;
    while (str[pos1] == ' ')
        pos1++;
    while (str[pos2] == ' ')
        pos2--;
    while (pos1 <= pos2)
    {
        s[ctr] = str[pos1];
        pos1++;
        ctr++;
    }
    s[pos1] = '\0';
    return s;
}

int levenshteinDistance(char *str1, char *str2)
{
    int n1 = strlen(str1);
    int n2 = strlen(str2);
    if (n1 == 0 && n2 != 0)
        return n2;
    else if (n2 == 0 && n1 != 0)
        return n1;
    else if (n2 == 0 && n1 == 0)
        return 0;
    else
    {
        int dp[n1 + 1][n2 + 1];
        memset(dp, 0, n1 * n2);
        for (int i = 0; i <= n1; i++)
        {
            for (int j = 0; j <= n2; j++)
            {
                if (i == 0)
                    dp[i][j] = j;
                else if (j == 0)
                    dp[i][j] = i;
                else if (str1[i - 1] == str2[j - 1])
                    dp[i][j] = dp[i - 1][j - 1];
                else
                    dp[i][j] = 1 + min(min(dp[i - 1][j], dp[i][j - 1]), dp[i - 1][j - 1]);
            }
        }
        return dp[n1][n2];
    }
}

char *removeStopwords(char *str)
{
    char stopw[179][20] = {"she\'s", "them", "were", "into", "weren\'t", "before", "had", "from",
                           "how", "doesn\'t", "on", "again", "to", "we", "did", "mustn\'t", "all", "own", "as", "shan\'t",
                           "so", "now", "have", "you\'ll", "been", "such", "having", "yourself", "while", "he", "more",
                           "hadn", "hers", "out", "most", "above", "ain", "up", "against", "of", "has", "and", "you\'ve",
                           "not", "my", "should\'ve", "am", "s", "doing", "him", "aren\'t", "only", "off", "you", "o",
                           "down", "that\'ll", "isn", "m", "by", "me", "needn", "himself", "in", "some", "ourselves",
                           "the", "then", "same", "wasn\'t", "who", "weren", "over", "re", "hasn", "can", "was", "y",
                           "needn\'t", "yours", "very", "after", "be", "below", "between", "does", "just", "ours", "when",
                           "won\'t", "it\'s", "it", "didn\'t", "i", "don\'t", "their", "with", "you\'d", "further",
                           "herself", "haven\'t", "yourselves", "our", "than", "or", "aren", "should", "whom", "once",
                           "don", "other", "themselves", "why", "itself", "at", "shouldn\'t", "hadn\'t", "few", "there",
                           "any", "because", "is", "her", "mightn", "too", "an", "your", "that", "she", "where", "couldn",
                           "couldn\'t", "both", "mightn\'t", "until", "you\'re", "shouldn", "isn\'t", "for", "doesn", "are",
                           "a", "wasn", "hasn\'t", "this", "they", "if", "no", "d", "wouldn\'t", "through", "his", "what",
                           "shan", "but", "each", "havent\'t", "about", "theirs", "being", "ma", "do", "ve", "haven", "under",
                           "mustn", "which", "myself", "won", "during", "its", "will", "nor", "didn", "wouldn", "these", "ll", "those", "here"};
    int len = strlen(str);
    char *s = (char *)malloc(sizeof(char) * len);
    char *temp = (char *)malloc(sizeof(char) * len);
    char word[50];
    strcpy(s, "");
    strcpy(temp, str);
    int check;
    char *token, *save;
    token = strtok_r(temp, " ", &save);
    while (token != NULL)
    {
        strcpy(word, lower(token));
        check = 0;
        for (int i = 0; i <= 178; i++)
        {
            if (!strcmp(word, stopw[i]))
            {
                check = 1;
                break;
            }
        }
        if (check != 1)
        {
            strcat(s, token);
            strcat(s, " ");
        }
        token = strtok_r(NULL, " ", &save);
    }
    s = trim(s);
    return s;
}

char *stem(char *str)
{
    int len = strlen(str);
    char *word = (char *)malloc(sizeof(char) * len);
    strcpy(word, str);
    rule_1(word);
    rule_2(word);
    rule_3(word);
    rule_4(word);
    rule_5(word);
    return word;
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
    NODE *cur = pres->next;
    while (cur != NULL)
    {
        ctr++;
        if (strcmp(cur->word, str) == 0)
            return cur->lemma->word;
        cur = cur->next;
    }
    return str;
}