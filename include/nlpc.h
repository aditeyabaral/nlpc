#include "preprocess.h"
#include "stemmer.h"
#include "lemmatizer.h"
#include "tfidf.h"
#include "summary.h"
#include "word2vec.h"

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

/*
MODEL CREATION AND INITIALIZATION FUNCTIONS
*/

/* Initialize the hyperparameters of the model
(context width, output dimension, learning rate and number of epochs) */
void initialiseModelParameters(EMBEDDING* model, int C, int N, float alpha, int epochs);
/* Allocate memory to store the cleaned model corpus */
void initialiseModelCorpus(EMBEDDING* model, char* corpus);
/* Allocate memory for hashtable */
void initialiseModelHashtable(EMBEDDING*);
/* Create X and Y matrices from the corpus by moving the sliding window of length 2C+1 over the corpus */
void createXandY(EMBEDDING* model, int random_state);
/* Allocate memory for model object and initalize all variables */
EMBEDDING* createModel();
/* Get final word embeddings from the trained model */
void extractEmbeddings(EMBEDDING* model);
/* Create vocabulary from corpus, initialize hyperparameters and start the training loop */
void train(EMBEDDING* model, char* corpus, int C, int N, float alpha, int epochs, int random_state, bool save);

/*
DISPLAY FUNCTIONS
*/

/* Display a 2D array 'a' having m rows and n columns  */
void displayArray(double** a, int m, int n);
/* Display the hash table that stores the vocabulary */
void displayHashtable(EMBEDDING*);
/* Display all the hyperparameter values and weights of the model */
void displayModel(EMBEDDING* model);

/*
FUNCTIONS TO LOAD AND STORE MODEL IN FILE 
*/

/* Write word and its embedding to a CSV file called model-embeddings.csv */
void writeEmbeddings(EMBEDDING* model);
/* Write all hyperparameter values to model-parameters.csv */
void writeParameters(EMBEDDING* model);
/* Write clean and original corpus along with the vocabulary to model-corpus.txt */
void writeCorpus(EMBEDDING* model);
/* Write weights and biases of the NN model to model-weights-wi.csv and model-biases-bi.csv where i = 1, 2 */
void writeWeightsBiases(EMBEDDING* model);
/* Call writeEmbeddings, writeParameters and writeCorpus */
void saveModel(EMBEDDING* model, bool write_all);
/* Reads a matrix from a csv file and returns it */
double** getMatrixFromFile(char* filename);
/* Stores the dimensions of the matrix stored in the csv file in m and n */
void getFileDimensions(char* filename, int *m, int *n);
/* Checks if a file exists in the current working directory */
bool checkFileExists(char* filename);
/* Writes a matrix of dimension m x n into a file */
void writeMatrixToFile(double** M, char* filename, int m, int n);
/* Load model embeddings, X, y, W1, W2, b1 and b2 from a file for further training */
EMBEDDING* loadModelForTraining(char* embedding_filename, char* X_filename, char* Y_filename, char* W1_filename, char* W2_filename, char* b1_filename, char* b2_filename);
/* Load vocabulary size and embedding dimension from file */
void getEmbeddingFileDimensions(EMBEDDING* model, char* filename);
/* Create hash table from text file containing the vocabulary */
void getEmbeddingParametersFromFile(EMBEDDING* model, char* filename);
/* Load model embeddings from file for use only*/
EMBEDDING* loadModelEmbeddings(char* embedding_filename);

/*
MEMORY MANAGEMENT FUNCTIONS
*/

/*Free storage held by an arbitrary 2D array of doubles of size m * n */
void free2D(double** M, int m, int n);
/*Free storage held by an arbitrary 2D array of ints of size m * n */
void free2D_int(int** M, int m, int n);
/* Free all storage allocated for model object */
void destroyModel(EMBEDDING* model);

/*
HASH TABLE FUNCTIONS 
*/

/* Get the hash value (index) of a given word */
int getHashvalue(char* word, int vocab_size);
/* Insert a node into the hash table */
void insert(NODE* node, EMBEDDING* model);
/* Create the hash table from the corpus */
void createHashtable(EMBEDDING*, char*);

/*
MATHEMATICAL FUNCTIONS AND MATRIX UTILITIES
*/

/* Calculate ReLU of a matrix */
double** relu(double** X, int m, int n);
/* Calculate softmax function of a matrix M over a given axis */
double** softmax(double** M, int m, int n, int axis);
/* Create matrix of size m x n and populate it with random values between 0 and 1 */
double** createArray(int m, int n, int random_state);
/* Create matrix of size m x n and populate it with 0 */
double** createZerosArray(int m, int n);
/* Create matrix of size m x n and populate it with 1 */
double** createOnesArray(int m, int n);
/* Return the cosine of the angle between vectors v1 and v2 */
double cosine_similarity(double** v1, double** v2, int N);
/* Return the cosine distance (1-cosine similarity) between vectors v1 and v2 */
double cosine_distance(double** v1, double** v2, int N);
/* Multiply matrices M1 and M2 */
double** multiply(double **M1, double **M2, int m1, int n1, int m2, int n2);
/* Add matrices M1 and M2 */
double** add(double **M1, double **M2, int m, int n);
/* Subtract M2 from M1 */
double** subtract(double **M1, double **M2, int m, int n);
/* Multiply each element of matrix M with scalar C */
double** multiply_scalar(double **M, double C, int m, int n);
/* Transpose of matrix A having original dimensions m x n */
double** transpose(double**A, int m, int n);
/* Create one hot representation of a word contained in the node based on its index in the hashtable */
int** createOneHot(NODE* node, EMBEDDING* model);
/* Broadcast vector b to the dimension of matrix WX and then add the result*/
double** broadcast_and_add(double** WX, double **b, int m1, int n1, int m2, int n2);
/* Get the matrix X from the matrix of context words */
double** getX(EMBEDDING* model, int m, char* s);
/* Get matrix Y from matrix of target words */
double** getY(EMBEDDING* model, int m, char* s);
/* The dot product of vectors v1 and v2, of length n */
double dot(double** v1, double** v2, int n);
/* L2 Norm or Euclidean norm of the matrix M of size m x n */
double norm(double** M, int m, int n);
/* Returns the cosine similarity between 2 words */
double similarity(EMBEDDING* model, char* word1, char* word2);
/* Calculate the cosine distance between the embeddings of 2 words */
double distance(EMBEDDING* model, char* word1, char* word2);

/*
NEURAL NETWORK FUNCTIONS
*/

/* Implements the binary cross entropy cost function*/
double cost(EMBEDDING* model);
/* A single forward pass through the neural network */
void forward_propagation(EMBEDDING* model);
/* A single backward pass through the neural network (by calculating the gradients) */
void back_propagation(EMBEDDING* model);
/* A single forward pass, followed by a backward pass and the parameter update step */
void gradientDescent(EMBEDDING* model, bool save);

/*
TEXT PREPROCESSING FUNCTIONS
*/

/* Remove all punctuations from the corpus */
char* remove_punctuations(char*);
/* Remove leading and trailing spaces from the corpus */
char* trim(char*);
/* Get the number of words in the vocabulary from the corpus */
int getVocabularySize(EMBEDDING*);

/*
WORD2VEC FUNCTIONALITIES
*/

/* Retrieve the embedding of a particular word */
double** getVector(EMBEDDING* model, char* word);
/* Get the word with the highest similarity to a given embedding */
char* getWord(EMBEDDING* model, double** vector);
/* Get the k most similar words to a given embedding. Returns a single string of words separated by \n */
char* mostSimilarByVector(EMBEDDING* model, double** vector, int k);
/* Get the k most similar words to a given word. Returns a single string of words separated by \n */
char* mostSimilarByWord(EMBEDDING* model, char* word, int k);