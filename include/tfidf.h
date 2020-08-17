#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>

typedef struct tfidfObject
{
  char *vocab;
  int docs;
  int vocab_size;
  double **matrix;
  bool use_idf;
  bool smooth_idf;
} TFIDF;

TFIDF *tfidfVectorizer()
{
  TFIDF *vectorizer = (TFIDF *)malloc(sizeof(TFIDF));
  vectorizer->docs = 0;
  vectorizer->vocab_size = 0;
  vectorizer->smooth_idf = true;
  vectorizer->use_idf = true;
  vectorizer->vocab = NULL;
  vectorizer->matrix = NULL;
  return vectorizer;
}

void tfidfFitVocabulary(TFIDF *vectorizer, char *corpus)
{
  if (vectorizer->vocab != NULL)
    free(vectorizer->vocab);

  char *trimmed = trim(corpus);
  char *token1, *save1, *token2, *save2, *token3, *save3;
  char *vocab = (char *)malloc(sizeof(char) * INT_MAX);
  char *temp = (char *)malloc(sizeof(char) * INT_MAX);

  token1 = strtok_r(trimmed, "\n", &save1);
  strcpy(vocab, "");
  int vocab_size = 0;
  bool flag;

  while (token1 != NULL)
  {
    token2 = strtok_r(token1, " ", &save2);
    while (token2 != NULL)
    {
      strcpy(temp, vocab);
      token3 = strtok_r(temp, " ", &save3);
      flag = false;
      while (token3 != NULL)
      {
        if (!strcmp(token2, token3))
        {
          flag = true;
          break;
        }
        token3 = strtok_r(NULL, " ", &save3);
      }
      if (!flag)
      {
        strcat(vocab, token2);
        strcat(vocab, " ");
        vocab_size++;
      }
      token2 = strtok_r(NULL, " ", &save2);
    }
    token1 = strtok_r(NULL, "\n", &save1);
  }
  vectorizer->vocab = trim(vocab);
  vectorizer->vocab_size = vocab_size;
  free(vocab);
  free(temp);
  free(trimmed);
}

int numberOfDocuments(char *corpus)
{
  int n = 0;
  int len = strlen(corpus);
  char temp[len];
  strcpy(temp, corpus);

  char *token, *save;
  token = strtok_r(temp, "\n", &save);
  while (token != NULL)
  {
    n++;
    token = strtok_r(NULL, "\n", &save);
  }
  return n;
}

double term_frequency(char *word, char *document)
{
  int len = strlen(document);
  char temp[len];
  strcpy(temp, document);

  char *token, *save;
  int num_words = 0, frequency = 0;
  token = strtok_r(temp, " ", &save);

  while (token != NULL)
  {
    if (!strcmp(token, word))
      frequency++;
    num_words++;
    token = strtok_r(NULL, " ", &save);
  }
  return (float)frequency / (float)num_words;
}

double document_frequency(char *word, char *corpus)
{
  int len = strlen(corpus);
  char temp[len];
  strcpy(temp, corpus);

  char *token1, *save1, *token2, *save2, *token3, *save3;
  bool flag1, flag2;
  int frequency = 0;

  token1 = strtok_r(temp, "\n", &save1);
  while (token1 != NULL)
  {
    token2 = strtok_r(token1, " ", &save2);
    while (token2 != NULL)
    {
      if (!strcmp(word, token2))
      {
        frequency++;
        break;
      }
      token2 = strtok_r(NULL, " ", &save2);
    }
    token1 = strtok_r(NULL, "\n", &save1);
  }
  return (float)frequency;
}

double inverse_document_frequency(char *word, char *corpus, int n, bool use_idf, bool smooth_idf)
{
  if (use_idf == false)
    return 1.0;
  double df = document_frequency(word, corpus);
  double result = log((n + 1.0) / (1.0 + df));
  if (smooth_idf)
    result += 1.0;
  return result;
}

double **tfidfTransform(TFIDF *vectorizer, char *corpus, bool use_idf, bool smooth_idf)
{
  if (vectorizer->matrix != NULL)
  {
    for (int i = 0; i < vectorizer->docs; ++i)
      free(vectorizer->matrix[i]);
    free(vectorizer->matrix);
  }

  vectorizer->use_idf = use_idf;
  vectorizer->smooth_idf = smooth_idf;

  char *trimmed = trim(corpus);
  int m = numberOfDocuments(corpus);
  vectorizer->docs = m;
  int n = vectorizer->vocab_size;

  double **M = (double **)malloc(sizeof(double *) * m);
  for (int i = 0; i < m; i++)
  {
    M[i] = (double *)malloc(sizeof(double) * n);
    for (int j = 0; j < n; j++)
      M[i][j] = 0;
  }

  int corpus_length = strlen(corpus), vocab_length = strlen(vectorizer->vocab);
  char *document = (char *)malloc(sizeof(char) * INT_MAX);
  char word[50], temp_vocab[vocab_length], temp_corpus[corpus_length];
  char *token1, *save1, *token2, *save2;
  int row = 0, col = 0;

  strcpy(temp_corpus, corpus);
  token1 = strtok_r(temp_corpus, "\n", &save1);
  while (token1 != NULL)
  {
    strcpy(document, token1);
    strcpy(temp_vocab, vectorizer->vocab);
    token2 = strtok_r(temp_vocab, " ", &save2);
    col = 0;
    while (token2 != NULL)
    {
      strcpy(word, token2);
      double val1 = term_frequency(word, document);
      double val2 = inverse_document_frequency(word, corpus, m, use_idf, smooth_idf);
      M[row][col] = val1 * val2;
      col++;
      token2 = strtok_r(NULL, " ", &save2);
      strcpy(word, "");
    }
    row++;
    strcpy(document, "");
    strcpy(temp_vocab, "");
    strcpy(word, "");
    token1 = strtok_r(NULL, "\n", &save1);
  }
  vectorizer->matrix = M;
  free(document);
  free(trimmed);
  return M;
}

double **tfidfFitTransform(TFIDF *vectorizer, char *corpus, bool use_idf, bool smooth_idf)
{
  tfidfFitVocabulary(vectorizer, corpus);
  return tfidfTransform(vectorizer, corpus, use_idf, smooth_idf);
}

void destroyTfidfModel(TFIDF *vectorizer)
{
  if (vectorizer == NULL)
    return;

  if (vectorizer->vocab != NULL)
    free(vectorizer->vocab);

  for (int i = 0; i < vectorizer->docs; ++i)
    free(vectorizer->matrix[i]);
  free(vectorizer->matrix);

  free(vectorizer);
}