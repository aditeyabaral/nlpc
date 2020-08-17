/*
Trims any blank spaces before and after the string and returns the modified string
Usage: trim(string)
Dependencies : stdlib.h, stdio.h, string.h
*/
char *trim(char *str)
{
    int len = strlen(str);
    char *s = (char *)malloc(sizeof(char) * len);
    int pos1 = 0, pos2 = len - 1, ctr = 0;
    while (str[pos1] == ' ')
        pos1++;
    while (str[pos2] == ' ')
        pos2--;
    while (pos1 <= pos2)
    {
        s[ctr] = str[pos1];
        pos1++;
        ctr++;
    }
    s[pos1] = '\0';
    return s;
}