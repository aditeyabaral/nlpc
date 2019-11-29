#include "Final header.h"

int dbEndswith(char *str)
{
	char cmtext[]=".txt";
	int n=strlen(str);
	char t[20];
	str=str+n-4;
	strcpy(t,str);
	return strcmp(t,cmtext);
}

float df(float a,float b)
{
   if((a+b)==0.0)
     {
       return 0.0;
     }
    else if(a+b==1.0)
      return 1.0;
    else if(a+b>1.0)
      return 2.0;
}
    
void IDF(float*a,float*b,int n)
{
    for(int i=0;i<=n-1;i++)
    {
     a[i]=a[i]*((log(3.0/(df(a[i],b[i])+1)))+1);
    }
}
float findsim(char*sent,char*sent1)
{
  int count1, count2;
  float cosinesimilarity;
  char a[10000][25];
  char w[10000][25];
  count1=arrayofwords(sent,a);
  count2=arrayofwords(sent1,w);
  cosinesimilarity=cosinesim(a,w,count1,count2);
  return cosinesimilarity;
}
int arrayofwords(char*sent,char a[][25])
{
 char q[25];
 int m=0;
 int j=0;
 int i;
 int count=0;

 for(i=0;i<=strlen(sent);i++)
  {
    if(sent[i]!=' ' && i!=strlen(sent))
     {
       q[m]=sent[i];
       m+=1;
       
     }
    else
     {
       q[m]='\0';
       strcpy(a[j],q);
       count+=1;
       j+=1;
       m=0;
     }
   }
 return count;
}
float cosinesim(char a[][25],char w[][25],int count1,int count2)
{
    int i;
    int count3=0;
    int q=0;
    int flag;
    char e[10000][25];
    float final1[1000],final2[1000];
    for(i=0;i<=count1-1;i++)
     {
         flag=0;
         for(int s=0;s<=count3;s++)
            {
               if(!strcmp(a[i],e[s]))
                  { 
                    flag=1;
                  }
             }
          if(flag!=1)
            {
              strcpy(e[q],a[i]);
              q+=1;
              count3+=1;
            }
       }
     for(i=0;i<=count2-1;i++)
     {
         flag=0;
         for(int s=0;s<=count3;s++)
            {
               if(!strcmp(w[i],e[s]))
                  { 
                    flag=1;
                  }
             }
          if(flag!=1)
            {
              strcpy(e[q],w[i]);
              q+=1;
              count3+=1;
            }
       }
     int o=0,l=0;
     int sum1=0;
     int sum2=0;
     for(i=0;i<=count3-1;i++)
     {
        sum1=0;
        for(int j=0;j<=count1-1;j++)
            {
               if(!(strcmp(e[i],a[j])))
                   sum1+=1;
            }
      final1[o]=sum1;
      o+=1;
     }
     for(i=0;i<=count3-1;i++)
     { 
        sum2=0;
        for(int j=0;j<=count2-1;j++)
            {
               if(!(strcmp(e[i],w[j])))sum2+=1;
            }
      final2[l]=sum2;
      l+=1;
     }
     IDF(final1,final2,l);
     IDF(final2,final1,l);
    float csum=0,psum=0,qsum=0;
    float denominator;
    float result;
    for(i=0;i<=l-1;i++)
     {
       csum+=((final1[i]*final2[i])/pow(l,2));
     }
    for(int j=0;j<=l-1;j++)
     {
        psum+=(pow(final1[j],2)/(pow(l,2)));
        qsum+=(pow(final2[j],2)/(pow(l,2)));
      }
        denominator=sqrt(psum)*sqrt(qsum);
        result=(csum/denominator);
        return result;
}