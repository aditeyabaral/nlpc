# NLPC
Natural Language Toolkit built using the C Programming Language.

# Running NLPC
You can use NLPC simply by including the main header file:

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "nlpc.h"
int main(){
    ...
    ...
}
```
Alternatively, you can include only the utilities desired

```C
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stemmer.h"
#include "preprocess.h"
int main(){
    ...
    ...
}
```

# Documentation 
## 1)  preprocess.h
    
```C 
char *lower(char *str)
```
		
Converts all characters of passed string to lowercase

Parameters: char *str: String which is to be converted to lowercase.

Returns char* with all characters in lower case

Usage:

```C
char *in = "Convert This String To LOWER case";
char *out = lower(in);
``` 


```C
char *upper(char *str)
```

Converts all characters of passed string to uppercase
		
Parameters: char *str: String which is to be converted to uppercase.
		
Returns char* with all characters in upper case.

Usage:

```C
char *in = "Convert This String To UPPER case";
char *out = upper(in);
```


```C
char *trim(char *str)
```

Removes spaces, newline character('\n') and tab space('\t') from either end of the passed string str.

Parameters: char *str: String from which extra space is to be removed.
		
Returns char* with extra spaces removed.

Usage:

```C
char *in = "\t\t\n Remove spaces and newline character from this string\t\t\n";
char *out = trim(in);
```


```C
char *removeStopwords(char *str)
```

Removes stopwords from any given string and returns the modified string
		
Parameters: char *str: String from which stopwords are to be removed.
		
Returns char* with stopwords removed.

Usage:
```C
```


```C   
char *removeSymbols(char *str)
```
Removes any kind of symbols from a string and returns modified string.
		
Parameters: char *str: String from which the symbols are to be removed.
	
Returns: char *str: String from which all kinds of symbols have been removed.

Usage:
```C
```


```C		
char *removeContractions(char *str)
```

Removes contractions( like aren't,can't,don't, etc..) from any given string and returns the modified string
		
Parameters: char *str: String from which contractions are to be removed.
		
Returns: char *: String from which contractions have been removed.	

Usage:
```C
```

```C
int levenshteinDistance(char *str1, char *str2)
```

Calculates the Levenshtein distance between two words, where Levenshtein distance is defined as the minimum number of single character changes(insertions,deletions,substitutions ) required to change one word into another.  
		  
Parameters: char *str1: First word, char *str2: Second word.
	
Returns: int which is the Levenshtein distance between char *str1 and char *str2

Usage:
```C
```


```C
char *char_ngram(char *s,int n)
```

Returns character n-grams with n continuous characters grouped together.
		
Parameters: char *s: string from which n-grams are to be formed

int n: Number of continuous characters to be grouped together.

Returns: char *: The n-grams seperated by newline

Usage:
```C
```

```C
char *word_ngram(char *s, int n)
```

Returns word n-grams with n continuous words grouped together.

Parameters: char *s: string from which word level n-grams are to be formed
   
int n: Number of continuous words to be grouped together

Returns char *: The word level n-grams seperated by newline.

Usage:
```C
```


```C
char *removeSentenceSeparators(char *corpus)
```

Removes sentence separators such as '.','!','?','\n' from the input text.  

Parameters: char *corpus: The text from which the sentence separators are to be removed.
	
Returns char *: Text with the sentence separators removed.

Usage:
```C
```

```C 
char *getSentenceSeparators(char *corpus)
```

Gives a list of sentence separators in the document.
		
Parameters: char *corpus: The text from which the sentence seperators are to be retrieved.

Returns char *: String containing the sentence separators in the document.

Usage:
```C
```


## 2) lemmatizer.h

```c
LEMMATIZER *lemmatizer();
```

Initialises the lemmatizer object

```c
char *lemmatize(LEMMATIZER *graph, char *str);
```

Lemmatizes a word and returns the resulting word4

Parameters: LEMMATIZER *graph: Lemmatizer object returned by lemmatizer() function.

char *str: String to be lemmatized.

Returns char *:Lemmatized string.

Usage:
```C
```

## 3) stemmer.h

```c 
  char *stem(char *str)
  ```
Implementation of the Porter2 Stemmer or the Snowball Stemmer.Returns the stemmed word.

Parameters: char *str: String to be stemmed/

Returns: char *: Stemmed string.

Usage:
```C
```

			
 ## 4) tfidf.h

```c 
double term_frequency(char *word, char *document)
```
Obtain the scaled term frequency of a word in a document

Parameters: char *word: the word whose frequency is to be found, char *document: The document from which the frequency of char *word is to be found

Returns double: The scaled frequency of the word in a document.

Usage:
```C
```

```c 
int numberOfDocuments(char *corpus);	
```
Find the number of documents in a corpus.Documents are separated by a \n.

Parameters: char *corpus: String with the documents separated by \n.

Returns int: number of documents in a corpus.

Usage:
```C
```

   ```c 
   double document_frequency(char *word, char *corpus)
   ``` 
   Obtain the document frequency of a word (the number of documents containing the word) in a corpus.Documents are separated by \n.

   Parameters: char *word: Word whose document frequency is to be found, char *corpus: string with all documents separated by newline \n.
	 
   Returns double document_frequency: The document frequency of the word.
  
  Usage:
```C
```  

   ```c
   double inverse_document_frequency(char *word, char *corpus, int n, bool use_idf, bool smooth_idf)
```

Obtain the inverse document frequency of a word in a corpus
Parameters: char *word: Word whose idf is to be found, char *corpus: String with all the documents separated by newline,
		      int n: The number of documents in the corpus, bool use_idf: Enable inverse-document-frequency reweighting, 
                      bool smooth_idf:Smooth idf weights by adding one to document frequencies, as if an extra document was seen containing every term in the collection exactly                       once. Prevents zero divisions.		

Returns double: the inverse document frequency of a word in a corpus

Usage:
```C
```


```c
TFIDF *tfidfVectorizer()
```
Initialises the tfidfVectorizer

Usage:
```C
```


 ```c 
 void tfidfFitVocabulary(TFIDF *vectorizer, char *corpus)
  ```
  Fit the vocabulary of a tfidf vectorizer to a corpus

Parameters: TFIDF *vectorizer: tfidfVectorizer object returned by tfidfVectorizer,
            char *corpus: The corpus to which the vocabulary of the tfidf vectorizer is to be fit.

Usage:
```C
```

```c 
double ** tfidfTransform(TFIDF *vectorizer, char *corpus, bool use_idf, bool smooth_idf)
```
Transform a corpus into a tfidf matrix.Vectorizer must be initially fit with a vocabulary
 
Parameters: TFIDF *vectorizer: tfidfVectorizer object returned by tfidfVectorizer
                       char *corpus: The corpus to which the vocabulary of the tfidf vectorizer is to be fit.		
		       bool use_idf: Enable inverse-document-frequency reweighting, 
                       bool smooth_idf:Smooth idf weights by adding one to document frequencies, as if an extra document was seen containing every term in the collection exactly once. Prevents zero divisions.
		
Returns double ** : The tfidf matrix.

Usage:
```C
```

```c 
double **tfidfFitTransform(TFIDF *vectorizer, char *corpus, bool use_idf, bool smooth_idf)
```
Fit a vectorizer with a vocabulary and then transform the vocabulary into a tfidf matrix.Equivalent to first fitting the vocabulary with tfidfFitVocabulary() followed by transforming the same corpus with tfidfTransform() 			

Parameters: TFIDF *vectorizer: tfidfVectorizer object returned by tfidfVectorizer
                       char *corpus: The corpus to which the vocabulary of the tfidf vectorizer is to be fit.		
		       bool use_idf: When TRUE, Enable inverse-document-frequency reweighting, 
                       bool smooth_idf:When TRUE, Smooth idf weights by adding one to document frequencies, as if an extra document was seen containing every term in the collection exactly once. Prevents zero divisions.

Returns double ** : The tfidf matrix.

Usage:
```C
```
						 
```c 
void destroyTfidfModel(TFIDF *vectorizer)
```
Destroy the tfidf vectorizer and free up memory

Usage:
```C
```

## 5)word2vec.h
```C
EMBEDDING *createModel();
```
Allocate memory for model object and initalize all variables 

Usage:

```C
```

```C
void extractEmbeddings(EMBEDDING *model)
```
Get final word embeddings from the trained model 

Parameters: EMBEDDING * model returned by the createModel() function.

Usage
```C
```

```C
void train(EMBEDDING *model, char *corpus, int C, int N, float alpha, int epochs, int random_state, bool save);
```
Create vocabulary from corpus, initialize hyperparameters and start the training loop 

Parameters:  EMBEDDING * model returned by the createModel() function, char * corpus: The corpus on which the word2vec model is to be trained, int C: The context width, int N: output dimension, float alpha: The learning rate, int epochs: The number of epochs, int random_state:,bool save: 

Usage:
```C
```
