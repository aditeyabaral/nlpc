#include "Final header.h"

float calcscore(char*s,char o[10000][25],int);
float ownfreq(char split[700],char splitwords[1000][25],int m);
void swap(float b[700][2],int p)
{
    int temp1=b[p][0];
    int temp2=b[p][1];
    b[p][0]=b[p+1][0];
    b[p][1]=b[p+1][1];
    b[p+1][0]=temp1;
    b[p+1][1]=temp2;
}
int in(char s[700],char m[1000][25],int h)
{
   for(int i=0;i<=h-1;i++)
   {
     if(!(strcmp(s,m[i])))
      {
        return 0;
      }
    }
   return 1;
}
void sort(float b[700][2],int h)
{
   for(int i=0;i<=h-1;i++)
   {
     for(int j=0;j<=h-2;j++)
     {
        if(b[j][1]<b[j+1][1])
        {
           swap(b,j);
         }
     }
   }
}
void parasum(char*s)
{
    char*e;
    int count=0;
    char a[700][700];
    int i=0;
    int j=0;
    int t=0;
    while(*s)
   {
      if(*s=='.')
      {
         a[i][j]='\0';
         i+=1;
         j=0;
      }
      else
      {
        a[i][j]=*s;
        j+=1;
      }
      s++;
   }
   char o[10000][25];
   int f=0,g=0,bb=-1;
   while(1)
   {
       e=(char*)malloc(100);
       while(1)
       {
         if(a[f][g]==' ' || a[f][g]==',' || a[f][g]=='!' || a[f][g]=='?' || a[f][g]==';' || a[f][g]==':'|| a[f][g]=='-'|| a[f][g]=='_' || a[f][g]=='(' || a[f][g]==')' || a[f][g]=='{' || a[f][g]=='}' || a[f][g]=='[' || a[f][g]==']' || a[f][g]=='\0') 
        {
           e[t]='\0';
           bb+=1;
           g+=1;
           strcpy(o[bb],e);
           free(e);
           e=(char*)malloc(100);
           t=0;
           if(a[f][g]=='\0')
           {
             g=0;
             break;
           }
         } 
         else
         {
            e[t]=a[f][g];
            g+=1;  
            t+=1;
         }
       }
       f+=1;
       if(f==i)
       {
         break;
       }
    }
 float b[700][2];
 float score;
 int x;
for(x=0;x<i;x++)
{
   score=calcscore(a[x],o,bb+1);
   b[x][0]=x;
   b[x][1]=score;
}
sort(b,i);

for(x=0;x<(i*0.2)-1;x++)
{
  printf("%s\n",a[(int)b[x][0]]);
}

}
float ownfreq(char split[700],char splitwords[1000][25],int m)
{
     int count=0;
     for(int i=0;i<m;i++)
     {
        if(!(strcmp(split,splitwords[i])))count+=1;
     }
     return count;
}
float calcscore(char s[700],char o[10000][25],int re)
{
   int y=0;
   int i=0;
   int m=0;
   int n=0;
   float totfrequency=0;;
   char u[1000][25];
   char splitwords[1000][25];
   float score=0;
   int F=0;
   while(1)
   {
       if(s[i]==' ' || s[i]=='\0'|| s[i]==' ' || s[i]==',' || s[i]=='!' || s[i]=='?' || s[i]==';' || s[i]==':'|| s[i]=='-'|| s[i]=='_' || s[i]=='(' || s[i]==')' || s[i]=='{' || s[i]=='}' || s[i]=='[' || s[i]==']' || s[i]=='\0')
       {
         splitwords[m][n]='\0';
         m+=1;
         n=0;
         if(s[i]=='\0')
             break;
       }
       else
       {
         splitwords[m][n]=s[i];
         n+=1;
       }
       i+=1;
   }
   for(int i=0;i<m;i++)
   {
      totfrequency=0;
      if((in(splitwords[i],u,F)))
       {
         for(int j=0;j<re;j++)
         {
           if(!(strcmp(splitwords[i],o[j])))
            {
              totfrequency+=1;
              strcpy(u[F],splitwords[i]);
              F+=1;
 
            }
           
          }
score+=((ownfreq(splitwords[i],splitwords,m))/totfrequency);
        }
   }

   return score;
}
/*
int main()
{
		char sent[20000]="Born to a Gujarati family in Vadnagar, Modi helped his father sell tea as a child and later ran his own stall.Born to a Gujarati family in Vadnagar, Modi helped his father sell tea as a child and later ran his own stall. He was introduced to the RSS at the age of eight, beginning a long association with the organisation. He left home after graduating from school, partly because of an arranged marriage which he rejected. Modi travelled around India for two years and visited a number of religious centres. He returned to Gujarat and moved to Ahmedabad in 1969 or 1970. In 1971 he became a full-time worker for the RSS. During the state of emergency imposed across the country in 1975, Modi was forced to go into hiding. The RSS assigned him to the BJP in 1985, and he held several positions within the party hierarchy until 2001, rising to the rank of General Secretary.Modi was appointed Chief Minister of Gujarat in 2001, due to Keshubhai Patel's failing health and poor public image following the earthquake in Bhuj. Modi was elected to the legislative assembly soon after. His administration has been considered complicit in the 2002 Gujarat riots,[a] or otherwise criticised for its handling of it; however, a Supreme Court-appointed Special Investigation Team (SIT) found no evidence to initiate prosecution proceedings against Modi personally.[b] His policies as chief minister, credited with encouraging economic growth, have received praise.[9] His administration has been criticised for failing to significantly improve health, poverty, and education indices in the state.Modi led the BJP in the 2014 general election, which gave the party a majority in the Lok Sabha, the first time a single party had achieved this since 1984. Modi himself was elected to parliament from Varanasi. Since taking office, Modi's administration has tried to raise foreign direct investment in the Indian economy, increased spending on infrastructure, and reduced spending on healthcare and social welfare programmes. Modi has attempted to improve efficiency in the bureaucracy, and centralised power by abolishing the planning commission and replacing it with the NITI Aayog. He has begun a high-profile sanitation campaign, and weakened or abolished environmental and labour laws.The economic policies of Modi's government focused on privatisation and liberalisation of the economy, based on a neoliberal framework.[189][190] Modi liberalised India's foreign direct investment policies, allowing more foreign investment in several industries, including in defence and the railways.[189][191][192] Other proposed reforms included making it harder for workers to form unions and easier for employers to hire and fire them;[190] some of these proposals were dropped after protests.[193] The reforms drew strong opposition from unions: on 2 September 2015, eleven of the country's largest unions went on strike, including one affiliated with the BJP.[190] The Bharatiya Mazdoor Sangh, a constituent of the Sangh Parivar, stated that the underlying motivation of labour reforms favored corporations over labourers.On 31 December 2014, Modi announced that the Planning Commission had been scrapped. It was replaced with a body called the National Institution for Transforming India, or NITI Aayog.[263][264] The Planning Commission was a legacy of the Indian Independence movement, although critics said that it was slowing economic growth.[265] The move had the effect of greatly centralising the power previously with the planning commission in the person of the prime minister.On 31 December 2014, Modi announced that the Planning Commission had been scrapped. It was replaced with a body called the National Institution for Transforming India, or NITI Aayog.[263][264] The Planning Commission was a legacy of the Indian Independence movement, although critics said that it was slowing economic growth.[265] The move had the effect of greatly centralising the power previously with the planning commission in the person of the prime minister.[189][197][263][264][265] It also reduced the extent of control individual states had over their financial allocation from the union government,[264][265] and unlike the planning commission, it does not have the power to allocate funds.[264] The planning commission had received heavy criticism in previous years for creating inefficiency in the government, and of not filling its role of improving social welfare: however, since the economic liberalisation of the 1990s, it had been the major government body responsible for measures related to social justice.On 27 February 2002, a train with several hundred passengers burned near Godhra, killing approximately 60 people.[e] The train carried a large number of Hindu pilgrims returning from Ayodhya after a religious ceremony at the site of the demolished Babri Masjid.[82][83] In making a public statement after the incident, Modi said that the attack had been terror attack planned by local Muslims.[4][82][84] The next day, the Vishwa Hindu Parishad called for a bandh across the state.[85][86] Riots began during the bandh, and anti-Muslim violence spread through Gujarat.[82][85][86] The government's decision to move the bodies of the train victims from Godhra to Ahmedabad further inflamed the violence.[82][87] The state government stated later that 790 Muslims and 254 Hindus were killed.[88] Independent sources put the death toll at over 2000.[82][89] Approximately 150,000 people were driven to refugee camps.[90] Numerous women and children were among the victims; the violence included mass rapes and mutilations of women.The economic policies of Modi's government focused on privatisation and liberalisation of the economy, based on a neoliberal framework.[189][190] Modi liberalised India's foreign direct investment policies, allowing more foreign investment in several industries, including in defence and the railways.[189][191][192] Other proposed reforms included making it harder for workers to form unions and easier for employers to hire and fire them;[190] some of these proposals were dropped after protests.[193] The reforms drew strong opposition from unions: on 2 September 2015, eleven of the country's largest unions went on strike, including one affiliated with the BJP.[190] The Bharatiya Mazdoor Sangh, a constituent of the Sangh Parivar, stated that the underlying motivation of labour reforms favored corporations over labourers.In September 2014, Modi introduced the Make in India initiative to encourage foreign companies to manufacture products in India, with the goal of turning the country into a global manufacturing hub.[189][195] Supporters of economic liberalisation supported the initiative, while critics argued it would allow foreign corporations to capture a greater share of the Indian market.[189] Modi's administration passed a land-reform bill that allowed it to acquire private agricultural land without conducting a social impact assessment, and without the consent of the farmers who owned it.[196] The bill was passed via an executive order after it faced opposition in parliament, but was eventually allowed to lapse.[197] Modi's government put in place the Goods and Services Tax, the biggest tax reform in the country since independence. It subsumed around 17 different taxes and became effective from 1 July 2017.On 25 June 2015, Modi launched a programme intended to develop 100 smart cities.[199] The smart Cities programme is expected to bring Information Technology companies an extra benefit of ?20 billion (US$280 million).[200] In June 2015, Modi launched the for All By 2022 project, which intends to eliminate slums in India by building about 20 million affordable homes for India's urban poor.In his first year as prime minister, Modi reduced the amount of money spent by the central government on healthcare.[159] The Modi government launched New Health Policy (NHP) in January 2015. The policy did not increase the government's spending on healthcare, instead emphasizing the role of private healthcare organisations. This represented a shift away from the policy of the previous Congress government, which had supported programmes to assist public health goals, including reducing child and maternal mortality rates.[217] The National Health Mission, which included public health programmes targeted at these indices received nearly 20%[218][219] less funds in 2015 than in the previous year. 15 national health programmes, including those aimed at controlling tobacco use and supporting healthcare for the elderly, were merged with the National Health Mission. In its budget for the second year after it took office, the Modi government reduced healthcare spending by 15%.[220] The healthcare budget for the following year rose by 19%. The budget was viewed positively by private insurance providers. Public health experts criticised its emphasis on the role of private healthcare providers, and suggested that it represented a shift away from public health facilities.[221] The healthcare budget rose by 11.5% in 2018; the change included an allocation of 2000 crore for a government-funded health insurance program, and a decrease in the budget of the National Health Mission.[222] Modi initially appointed Harsh Vardhan, a doctor and an advocate of tobacco control, as minister of health. However, Vardhan was replaced in November 2014 by Jagat Prakash Nadda.[223][217] The government introduced stricter packaging laws for tobacco which requires 85% of the packet size to be covered by pictorial warnings.[224] An article in the medical journal Lancet stated that the country might have taken a few steps back in public health under Modi.[217] A new National Health Policy was approved in 2017.Modi has generally emphasised his governments efforts at sanitation as a means of ensuring good health.[217] On 2 October 2014, Modi launched the Swachh Bharat Abhiyan (Clean India) campaign. The stated goals of the campaign included eliminating open defecation and manual scavenging.[227][228] The plan was to achieve these aims in five years.[228] As part of the programme, the Indian government began constructing millions of toilets in rural areas and encouraging people to use them.[229][230][231] The government also announced plans to build new sewage treatment plants.[232] The administration plans to construct 60 million toilets by 2019. The construction projects have faced allegations of corruption, and have faced severe difficulty in getting people to use the toilets constructed for them.[228][229][230] Since the launch of scheme, sanitation cover in the country has increased from 38.7% in October 2014 to 84.1% in May 2018. But people are not using them due to various reasons.[233] World Health Organization (WHO) has in its report stated that at least 180,000 diarrhoeal deaths were averted in rural India since the launch of the Swachhta Bharat Mission.";
        parasum(sent);
}
*/