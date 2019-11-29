#include "Final header.h"

void Stopwords(char *sent1,char *sent2)
{
	char word[20], temp[20];
	char stopw[179][20]={"she\'s", "them", "were", "into", "weren\'t", "before", "had", "from", 
	"how", "doesn\'t", "on", "again", "to", "we", "did", "mustn\'t", "all", "own", "as", "shan\'t",
	"so", "now", "have", "you\'ll", "been", "such", "having", "yourself", "while", "he", "more",
	"hadn", "hers", "out", "most", "above", "ain", "up", "against", "of", "has", "and", "you\'ve",
	"not", "my", "should\'ve", "am", "s", "doing", "him", "aren\'t", "only", "off", "you", "o",
	"down", "that\'ll", "isn", "m", "by", "me", "needn", "himself", "in", "some", "ourselves",
	"the", "then", "same", "wasn\'t", "who", "weren", "over", "re", "hasn", "can", "was", "y",
	"needn\'t", "yours", "very", "after", "be", "below", "between", "does", "just", "ours","when",
	"won\'t", "it\'s", "it", "didn\'t", "i", "don\'t", "their", "with", "you\'d", "further",
	"herself", "haven\'t", "yourselves", "our", "than", "or", "aren", "should", "whom", "once",
	"don", "other", "themselves", "why", "itself", "at", "shouldn\'t", "hadn\'t", "few", "there",
	"any", "because", "is", "her", "mightn", "too", "an", "your", "that", "she", "where", "couldn",
	"couldn\'t", "both", "mightn\'t", "until", "you\'re", "shouldn", "isn\'t", "for", "doesn", "are",
	"a", "wasn", "hasn\'t", "this", "they", "if", "no", "d", "wouldn\'t", "through", "his", "what",
	"shan", "but", "each", "havent\'t", "about", "theirs", "being", "ma", "do", "ve", "haven", "under",
	"mustn", "which", "myself", "won", "during", "its", "will", "nor", "didn", "wouldn", "these", "ll", "those", "here"};
	int check;
	char* token=strtok(sent1," ");
	while(token!=NULL)
	{
		strcpy(word, token);
		strcpy(temp,word);
		strlwr(temp);
		check = 0;
		for(int i=0;i<=178;i++)
		{
			if(strcmp(temp,stopw[i])==0)
			{
				check = 1;
				break;
			}
		}
		if (check!=1)
			{
				strcat(word," ");
				strcat(sent2,word);
			}
		token = strtok(NULL, " ");
	}
	return;
}