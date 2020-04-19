int LevenshteinDistance(char *word1, char *word2)
{
    int n1 = strlen(word1);
    int n2 = strlen(word2);
    if (n1==0 && n2!=0)
        return n2;
    else if (n2==0 && n1!=0)
        return n1;
    else if (n2==0 && n1==0)
        return 0;
    else
    {
        int dp[n1+1][n2+1];
        memset(dp,0,n1*n2);
        for(int i=0;i<=n1;i++)
        {
            for(int j=0;j<=n2;j++)
            {
                if(i==0)
                    dp[i][j]=j;
                else if(j==0)
                    dp[i][j]=i;
                else if(word1[i-1]==word2[j-1])
                    dp[i][j]=dp[i-1][j-1];
                else
                    dp[i][j]=1+min(min(dp[i-1][j],dp[i][j-1]),dp[i-1][j-1]);
            }
        }
        return dp[n1][n2];
    }
}

int min(int a,int b)
{
    return a<b?a:b;
}

