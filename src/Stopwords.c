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