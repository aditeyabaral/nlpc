#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "nlpc.h"

void displayArray(double **a, int m, int n)
{
    for(int i = 0; i<m ;i++)
    {
        for(int j = 0; j<n; j++)
            printf("%lf ", a[i][j]);
        printf("\n");
    }
    printf("\n");
}

int main()
{
    char* s = (char*)malloc(sizeof(char)*INT_MAX);
    scanf("%[^\\0]s", s);
    printf("Input: %s\n\n", s);

    char* no_contract = removeContractions(s);
    printf("No contractions: %s\n\n", no_contract);

    int dist = levenshteinDistance("play", "stay");
    printf("Distance between play & stay: %d\n\n", dist);

    char* trimmed = trim("  hello world  ");
    printf("Trimmed '  hello world  ' is: '%s'\n\n", trimmed);

    char* no_punkt = removeSymbols(s);
    printf("No symbols: %s\n\n", no_punkt);

    char* no_stopw = removeStopwords(s);
    printf("No stopwords: %s\n\n", no_stopw);

    char* lwr = lower(s);
    printf("Lowercase: %s\n\n", lwr);

    char* uppr = upper(s);
    printf("Uppercase: %s\n\n", uppr);

    char* stemmed = stem("aching");
    printf("Stemmed version of 'aching' is: '%s'\n\n", stemmed);

    LEMMATIZER* lemma = lemmatizer();
    char* lemmatized = lemmatize(lemma, "aching");
    printf("Lemmatized version of 'aching' is: '%s'\n\n", lemmatized);

    TFIDF* vec = tfidfVectorizer();
    double**M = tfidfFitTransform(vec, s, true, true);
    displayArray(vec->matrix, vec->docs, vec->vocab_size);
}