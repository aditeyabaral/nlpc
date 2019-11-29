float JaccardianSimilarity(char*sent1,char*sent2)
{
  int count=0;
  char a[10000][25];
  char b[10000][25];
  int i=0;
  int r=0;
  int j=0;
  int u=0,y=0,v=0;
  while(sent1[i])
  {
    if(sent1[i]!=' ')
    {
      a[r][j]=sent1[i];
      j+=1;
      i+=1;
    }
    else
    {  
      r+=1;
      j=0;
      i+=1;
    }
  }
  while(sent2[u])
  {
    if(sent2[u]!=' ')
    {
      b[v][y]=sent2[u];
      y+=1;
      u+=1;
    }
    else
    {  
      v+=1;
      y=0;
      u+=1;
    }
  }
  char c[10000][25];
  int e=0;
  int p=0;
  int h=0;
  while(h<=r)
  {
    p=0;
    for(i=0;i<=v;i++)
    {
      if(!(strcmp(a[h],b[i])))
        p+=1;
    }
    if(p>0)
    {
      strcpy(c[e],a[h]);
      e+=1;
    }
    h+=1;
  }
  float w=e;
  float z=r+1-e;
  float x=v+1-e;
  float q=w/(x+z);
  return (q*100);
}