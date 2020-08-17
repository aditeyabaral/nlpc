#include "preprocess.h"
#include "stemmer.h"
#include "lemmatizer.h"
#include "tfidf.h"

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
All \n are replaced with a single space
Usage: removeSymbols(string)
*/
char *removeSymbols(char *str);

/*
Removes stopwords from any given string and returns the modified string
Usage: removeStopwords(<character array>)
*/
char *removeStopwords(char *str);

/*
Implementation of the Porter2 Stemmer or the Snowball Stemmer
Returns the stemmed word
Usage: stem(string)
*/
char *stem(char *str);

/* 
Initialises the lemmatizer object
Usage: LEMMATIZER* lemmatizerObject = lemmatizer();
*/
LEMMATIZER *lemmatizer();

/*
Lemmatizes a word and returns the resulting word
Usage: lemmatize(lemmatizerObject, string);
*/
char *lemmatize(LEMMATIZER *graph, char *str);

/*
Obtain the scaled term frequency of a word in a document
Usage: term_frequency(word, document)
*/
double term_frequency(char *word, char *corpus);

/*
Find the number of documents in a corpus
Documents are separated by a \n
Usage: numberOfDocuments(corpus);
*/
int numberOfDocuments(char *corpus);

/* Obtain the document frequency of a word (the number of documents containing the word) in a corpus
Documents are separated by \n
Usage: document_frequency(word, corpus);
*/
double document_frequency(char *word, char *corpus);

/*
Obtain the inverse document frequency of a word in a corpus
Usage: inverse_document_frequency(word, corpus, number_of_documents , use_idf, smooth_idf);
*/
double inverse_document_frequency(char *word, char *corpus, int n, bool use_idf, bool smooth_idf);

/*
Initialises the tfidfVectorizer
Usage: TFIDF* vectorizer = tfidfVectorizer();
*/
TFIDF *tfidfVectorizer();

/*
Fit the vocabulary of a tfidf vectorizer to a corpus
Usage: tfidfFitVocabulary(vectorizer, corpus);
*/
void tfidfFitVocabulary(TFIDF *vectorizer, char *corpus);

/*
Transform a corpus into a tfidf matrix
Vectorizer must be initially fit with a vocabulary
Usage: tfidfTransform(vectorizer, corpus, se_idf, smooth_idf);
*/
double **tfidfTransform(TFIDF *vectorizer, char *corpus, bool use_idf, bool smooth_idf);

/*
Fit a vectorizer with a vocabulary and then transform the vocabulary into a tfidf matrix
Equivalent to first fitting the vocabulary with tfidfFitVocabulary() followed by transforming the same corpus with tfidfTransform() 
Usage: tfidfFitTransform(vectorizer, corpus, use_idf, smooth_idf);
*/
double **tfidfFitTransform(TFIDF *vectorizer, char *corpus, bool use_idf, bool smooth_idf);

/* 
Destroy the tfidf vectorizer and free up memory
Usage: destroyTfidfModel(vectorizer);
*/
void destroyTfidfModel(TFIDF *vectorizer);

