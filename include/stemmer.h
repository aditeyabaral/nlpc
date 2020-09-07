struct wInfo
{
	char *word;
	char *cv;
	int measure;
};
int strEndswith(char *str1, char *str2)
{
	size_t l1 = strlen(str1), l2 = strlen(str2);
	return (0 == strcmp(str2, str1 + (l1 - l2)));
}

void removeSuffix(char *p, char *s)
{
	size_t l1 = strlen(p), l2 = strlen(s);
	if (strEndswith(p, s))
		*(p + l1 - l2) = '\0';
}

struct wInfo wordMeasure(char *word)
{
	struct wInfo wordm;
	char cvword[50];
	wordm.cv = cvword;
	char temp[20];
	int i = 0, j;

	wordm.word = word;

	for (j = 0; (*word) != '\0'; word++, j++)
	{
		switch (*word)
		{
		case 'a':
		case 'e':
		case 'i':
		case 'o':
		case 'u':
		{
			*(wordm.cv + j) = 'v';
			break;
		}
		case 'y':
		{
			if (*(word - 1) == 'a' || *(word - 1) == 'e' || *(word - 1) == 'i' || *(word - 1) == 'o' || *(word - 1) == 'u')
				*(wordm.cv + j) = 'c';
			else
				*(wordm.cv + j) = 'v';
			break;
		}
		default:
		{
			*(wordm.cv + j) = 'c';
			break;
		}
		}
	}
	*(wordm.cv + j) = '\0';

	for (j = 0; *(wordm.cv + j) != '\0';)
	{
		if (*(wordm.cv + j) == 'c')
		{
			while (*(wordm.cv + j) == 'c' && *(wordm.cv + j) != '\0')
				j++;
			temp[i++] = 'c';
			//printf("%d",i);
			continue;
		}
		while (*(wordm.cv + j) == 'v' && *(wordm.cv + j) != '\0')
			j++;
		temp[i++] = 'v';
	}
	temp[i] = '\0';

	int m = 0;
	for (int k = 0; temp[k] != '\0'; k++)
	{
		if (temp[k] == 'v')
			m++;
		if (strEndswith(wordm.cv, "v"))
			wordm.measure = m - 1;
		else
			wordm.measure = m;
	}
	return wordm;
}

void rule_1(char *word)
{
	struct wInfo wordm;
	int i = 0, norule = 1;

	if (strEndswith(word, "sses"))
	{
		removeSuffix(word, "sses");
		strcat(word, "ss");
	}
	else if (strEndswith(word, "ies"))
	{
		removeSuffix(word, "ies");
		strcat(word, "i");
	}
	else if (strEndswith(word, "ss"))
		;
	else if (strEndswith(word, "s"))
		removeSuffix(word, "s");
	else if (strEndswith(word, "eed"))
	{
		removeSuffix(word, "eed");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ee");
		else
			strcat(word, "eed");
	}
	else if (strEndswith(word, "ed"))
	{
		removeSuffix(word, "ed");
		wordm = wordMeasure(word);
		while (*(wordm.cv + i) != '\0')
		{
			if (*(wordm.cv + i) == 'v')
			{
				if (strEndswith(word, "at") || strEndswith(word, "bl") || strEndswith(word, "iz"))
					strcat(word, "e");
				else if ((*(word + strlen(word) - 2) == *(word + strlen(word) - 1)) && (*(word + strlen(word) - 1) != 'l' && *(word + strlen(word) - 1) != 's' && *(word + strlen(word) - 1) != 'z'))
					*(word + strlen(word) - 1) = '\0';
				else if (wordm.measure == 1 && strEndswith(wordm.cv, "cvc") && (*(word + strlen(word) - 1) != 'w' && *(word + strlen(word) - 1) != 'x' && *(word + strlen(word) - 1) != 'y'))
					strcat(word, "e");
				norule = 0;
				break;
			}
			i++;
		}
		if (norule)
			strcat(word, "ed");
	}
	else if (strEndswith(word, "ing"))
	{
		removeSuffix(word, "ing");
		wordm = wordMeasure(word);
		while (*(wordm.cv + i) != '\0')
		{
			if (*(wordm.cv + i) == 'v')
			{
				if (strEndswith(word, "at") || strEndswith(word, "bl") || strEndswith(word, "iz"))
					strcat(word, "e");
				else if ((*(word + strlen(word) - 1)) == (*(word + strlen(word) - 2)) && (*(word + strlen(word) - 1) != 'l' && *(word + strlen(word) - 1) != 's' && *(word + strlen(word) - 1) != 'z'))
					*(word + strlen(word) - 1) = '\0';
				else if ((wordm.measure == 1 && strEndswith(wordm.cv, "cvc") && (*(word + strlen(word) - 1) != 'w' && *(word + strlen(word) - 1) != 'x' && *(word + strlen(word) - 1) != 'y')) || 0 == strcmp(word, "ag"))
					strcat(word, "e");
				norule = 0;
				break;
			}
			i++;
		}
		if (norule)
			strcat(word, "ing");
	}
	else if (strEndswith(word, "y"))
	{
		removeSuffix(word, "y");
		wordm = wordMeasure(word);
		while (*(wordm.cv + i) != '\0')
		{
			if (*(wordm.cv + i) == 'v')
			{
				strcat(word, "i");
				norule = 0;
				break;
			}
			i++;
		}
		if (norule)
			strcat(word, "y");
	}
}

void rule_2(char *word)
{
	struct wInfo wordm;

	if (strEndswith(word, "ational"))
	{
		removeSuffix(word, "ational");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ate");
		else
			strcat(word, "ational");
	}
	else if (strEndswith(word, "tional"))
	{
		removeSuffix(word, "tional");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "tion");
		else
			strcat(word, "tional");
	}
	else if (strEndswith(word, "enci"))
	{
		removeSuffix(word, "enci");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ence");
		else
			strcat(word, "enci");
	}
	else if (strEndswith(word, "anci"))
	{
		removeSuffix(word, "anci");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ance");
		else
			strcat(word, "anci");
	}
	else if (strEndswith(word, "izer"))
	{
		removeSuffix(word, "izer");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ize");
		else
			strcat(word, "izer");
	}
	else if (strEndswith(word, "abli"))
	{
		removeSuffix(word, "abli");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "able");
		else
			strcat(word, "abli");
	}
	else if (strEndswith(word, "alli"))
	{
		removeSuffix(word, "alli");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "al");
		else
			strcat(word, "alli");
	}
	else if (strEndswith(word, "entli"))
	{
		removeSuffix(word, "entli");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ent");
		else
			strcat(word, "entli");
	}
	else if (strEndswith(word, "eli"))
	{
		removeSuffix(word, "eli");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "e");
		else
			strcat(word, "eli");
	}
	else if (strEndswith(word, "ousli"))
	{
		removeSuffix(word, "ousli");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ous");
		else
			strcat(word, "ousli");
	}
	else if (strEndswith(word, "ization"))
	{
		removeSuffix(word, "ization");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ize");
		else
			strcat(word, "ization");
	}
	else if (strEndswith(word, "ation"))
	{
		removeSuffix(word, "ation");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ate");
		else
			strcat(word, "ation");
	}
	else if (strEndswith(word, "ator"))
	{
		removeSuffix(word, "ator");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ate");
		else
			strcat(word, "ator");
	}
	else if (strEndswith(word, "alism"))
	{
		removeSuffix(word, "alism");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "al");
		else
			strcat(word, "alism");
	}
	else if (strEndswith(word, "iveness"))
	{
		removeSuffix(word, "iveness");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ive");
		else
			strcat(word, "iveness");
	}
	else if (strEndswith(word, "fulness"))
	{
		removeSuffix(word, "fulness");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ful");
		else
			strcat(word, "fulness");
	}
	else if (strEndswith(word, "ousness"))
	{
		removeSuffix(word, "ousness");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ous");
		else
			strcat(word, "ousness");
	}
	else if (strEndswith(word, "aliti"))
	{
		removeSuffix(word, "aliti");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "al");
		else
			strcat(word, "aliti");
	}
	else if (strEndswith(word, "iviti"))
	{
		removeSuffix(word, "iviti");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ive");
		else
			strcat(word, "iviti");
	}
	else if (strEndswith(word, "biliti"))
	{
		removeSuffix(word, "biliti");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ble");
		else
			strcat(word, "biliti");
	}
}

void rule_3(char *word)
{
	struct wInfo wordm;

	if (strEndswith(word, "icate"))
	{
		removeSuffix(word, "icate");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ic");
		else
			strcat(word, "icate");
	}
	else if (strEndswith(word, "ative"))
	{
		removeSuffix(word, "ative");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			;
		else
			strcat(word, "ative");
	}
	else if (strEndswith(word, "alize"))
	{
		removeSuffix(word, "alize");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "al");
		else
			strcat(word, "alize");
	}
	else if (strEndswith(word, "iciti"))
	{
		removeSuffix(word, "iciti");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ic");
		else
			strcat(word, "iciti");
	}
	else if (strEndswith(word, "ical"))
	{
		removeSuffix(word, "ical");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			strcat(word, "ic");
		else
			strcat(word, "ical");
	}
	else if (strEndswith(word, "ful"))
	{
		removeSuffix(word, "ful");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			;
		else
			strcat(word, "ful");
	}
	else if (strEndswith(word, "ness"))
	{
		removeSuffix(word, "ness");
		wordm = wordMeasure(word);
		if (wordm.measure > 0)
			;
		else
			strcat(word, "ness");
	}
}

void rule_4(char *word)
{
	struct wInfo wordm;

	if (strEndswith(word, "al"))
	{
		removeSuffix(word, "al");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "al");
	}
	else if (strEndswith(word, "ance"))
	{
		removeSuffix(word, "ance");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "ance");
	}
	else if (strEndswith(word, "ence"))
	{
		removeSuffix(word, "ence");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "ence");
	}
	else if (strEndswith(word, "er"))
	{
		removeSuffix(word, "er");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "er");
	}
	else if (strEndswith(word, "ic"))
	{
		removeSuffix(word, "ic");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "ic");
	}
	else if (strEndswith(word, "able"))
	{
		removeSuffix(word, "able");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "able");
	}
	else if (strEndswith(word, "ible"))
	{
		removeSuffix(word, "ible");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "ible");
	}
	else if (strEndswith(word, "ant"))
	{
		removeSuffix(word, "ant");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "ant");
	}
	else if (strEndswith(word, "ement"))
	{
		removeSuffix(word, "ement");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "ement");
	}
	else if (strEndswith(word, "ment"))
	{
		removeSuffix(word, "ment");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "ment");
	}
	else if (strEndswith(word, "ent"))
	{
		removeSuffix(word, "ent");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "ent");
	}
	else if (strEndswith(word, "ion"))
	{
		removeSuffix(word, "ion");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1 && !(strEndswith(wordm.word, "t")) && !(strEndswith(wordm.word, "s")))
			strcat(word, "ion");
	}
	else if (strEndswith(word, "ou"))
	{
		removeSuffix(word, "ou");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "ou");
	}
	else if (strEndswith(word, "ism"))
	{
		removeSuffix(word, "ism");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "ism");
	}
	else if (strEndswith(word, "ate"))
	{
		removeSuffix(word, "ate");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "ate");
	}
	else if (strEndswith(word, "iti"))
	{
		removeSuffix(word, "iti");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "iti");
	}
	else if (strEndswith(word, "ous"))
	{
		removeSuffix(word, "ous");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "ous");
	}
	else if (strEndswith(word, "ive"))
	{
		removeSuffix(word, "ive");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "ive");
	}
	else if (strEndswith(word, "ize"))
	{
		removeSuffix(word, "ize");
		wordm = wordMeasure(word);
		if (wordm.measure <= 1)
			strcat(word, "ize");
	}
}

void rule_5(char *word)
{
	struct wInfo wordm;

	if (strEndswith(word, "e"))
	{
		removeSuffix(word, "e");
		wordm = wordMeasure(word);
		if (wordm.measure < 1 || (wordm.measure == 1 && strEndswith(wordm.cv, "cvc")) || 0 == strcmp(word, "ag"))
			strcat(word, "e");
	}
	else if (word[strlen(word) - 1] == word[strlen(word) - 2] && word[strlen(word) - 1] == 'l')
	{
		wordm = wordMeasure(word);
		if (wordm.measure > 1)
			word[strlen(word) - 1] = '\0';
	}
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