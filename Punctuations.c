#include "Final header.h"

void Punctuations(char *sent2, char *sent3)
{
	char punc[] = {'\'', '!', ']', '{', '#', '.', '<', '/', '(', '~', ',', '%', ';', '`', ':', '?', '+', '$', '^', '\\', '@', '*', '}', '=', '_', '\"', ')', '-', '|', '[', '&', '>'};
	int check;int ctr = 0;
	for (int i = 0; i< strlen(sent2); i++)
	{
		check = 0;
		for (int j = 0; j< 31; j++)
		{
			
			if ((char)sent2[i] == (char)punc[j])
			{
				check = 1;

				break;
			}
			
		}
		if (check==0)
		{
			sent3[ctr] = sent2[i];
			ctr++;
		}	
	}
	sent3[ctr]='\0';	
	return;
}