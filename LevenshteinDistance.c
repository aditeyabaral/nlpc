int LevenshteinDistance(char *s1, char*s2)
{
	int len1 = strlen(s1), len2= strlen(s2);
	int ld = fabs(len1-len2);
	for(int i = 0; i< strlen(s2);i++)
	{
		if (s1[i]!=s2[i])
			ld++;
	}
	return ld;
}
