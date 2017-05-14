#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<math.h>
#include <cstdlib>
#include<stdint.h>
#include <float.h>
#include <unistd.h>
#include <time.h>  
#include <vector>  
#define getDist(a,b) (b < a ? dist[a][b] : dist[b][a])
using namespace std;
float **dist=0;
double epsilon=0.20;
double dziltab[]={20000,30000,60000,90000,100000,100001};
double roundstab[]={10000,6500,4000,2500,2000,2001};
double kartab[]={100,90,50,30,50,51};
double counter[]={2000,4000,7999,12000,15001,15002};  
int dzil=100000;
unsigned int kar=0;
unsigned int rounds=1000;
unsigned char* sab;
typedef struct{
	double x;
	double y;
	uint32_t label;
} city;
typedef struct
{
	unsigned int k;
	unsigned int h;
} tabs;
typedef struct
{
	unsigned int x;
	unsigned int y;
} edge;
typedef std::vector < edge > edges;
city* cities;
int* way;
unsigned long long int z=0;
void dis(int n)
{
	register double x,y;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<=i;j++)
		{
			if(j!=i)
			{
				x=(cities[i].x-cities[j].x);
				y=(cities[i].y-cities[j].y);
				dist[i][j]=(float) sqrt(x*x+y*y);
			}
			else
				dist[i][j]=0;
		}
	}
}
inline double fun(int a,int b)
{
	return (getDist(way[a],way[b])+getDist(way[a+1],way[b+1])-getDist(way[a],way[a+1])-getDist(way[b],way[b+1]));
}
void NEH(int n)
{
	for(int i=0;i<=n;i++)
		way[i]=0;
	double best=0;
	double newcand;
	double old;
	int bestpos;
	for(int i=1;i<n;i++)
	{
		old=best;
		best=old-getDist(way[i-1],way[i])+getDist(way[i-1],i)+getDist(way[i],i);
		bestpos=i;
		for(int j=i-1;j>0;j--)
		{
			newcand=old-getDist(way[j-1],way[j])+getDist(way[j-1],i)+getDist(way[j],i);
			if(newcand<best)
			{
				best=newcand;
				bestpos=j;
			}	
		}
		for(int z=i;z>=bestpos;z--)
			way[z+1]=way[z];	
		way[bestpos]=i;		
	}
}
void NN(int n)
{
	int change=0;	
	way[0]=change;
	double mindis;
	double curdis;
	bool checked[n];
	for(int i=0;i<n;i++)
		checked[i]=true;
	checked[change]=false;
	int li=0;
	int ci=change;
	for(int i=1;i<n;i++)
	{
		mindis=DBL_MAX;
		for(int j=0;j<n;j++)
		{
			if(checked[j]&&j!=ci)
			{
				curdis=getDist(ci,j);
				if(curdis<mindis)
				{
					mindis=curdis;
					li=j;
				}
			}
		}
		checked[li]=false;
		way[i]=li;
		ci=li;
	}
	way[n]=change;
}
void tsp(int n)
{
	if(n<=3)
		return;
	int a,b,wina,winb;
	edges boxOfGoodies;
	double actval;
	int sc=0;
	double dn=(double) n;
	for(;sc<5;sc++)
	{
		if(n<counter[sc])
			break;
	}
	if(sc==0)
	{
		dzil=(int) dziltab[0];
		rounds=(int) roundstab[0];
	}
	else
	{
		double dir=(dziltab[sc]-dziltab[sc-1])/(counter[sc]-counter[sc-1]);
		dzil=(int) (dir*dn+dziltab[sc-1]-counter[sc-1]*dir);
		dir=(roundstab[sc]-roundstab[sc-1])/(counter[sc]-counter[sc-1]);
		rounds=(int) (dir*dn+roundstab[sc-1]-counter[sc-1]*dir);
	}
	kar=kartab[sc];
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			sab[i*n+j]=0;
		}
	}
	unsigned int k=0;
	tabs tabu[n];
	for(int i=0;i<n;i++)
	{
		tabu[i].k=0;
		tabu[i].h=0;
	}
	edge e;
	int maxcount;
	int best[n+1];
	for(int i=0;i<n;i++)
		best[i]=way[i];
	double bestroad=0;
	for(int i=0;i<n;i++)
	{
		bestroad+=getDist(way[i],way[i+1]);
	}
    double fullroad=bestroad;
	double newmax;
	z=0;
	int negcount=dzil;
	float temp;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<n;j++)
		{
			sab[i*n+j]=1;
			sab[j*n+i]=1;
		}
	}
	while(k<rounds)
	{
		newmax=DBL_MAX;
		for( edges::iterator i = boxOfGoodies.begin(); i != boxOfGoodies.end(); i++ )
		{
    		e=*i;
    		sab[e.x*n+e.y]=1;
			sab[e.y*n+e.x]=1;
		}
		boxOfGoodies.clear();
		negcount=dzil;
		maxcount=negcount*2;
		for(int i=0;i<negcount;i++)
		{
			do
			{
				a=rand()%n;
				b=rand()%n;	
			}
			while(abs(a-b)%(n-1)<=1);
			if(sab[a*n+b])
			{
				temp=fun(a,b);
				temp=temp+fullroad;
				actval=temp;
				if((tabu[way[a]].k>k&&tabu[way[a+1]].k>k))
				{
					temp=temp*(1+epsilon*max(tabu[way[a]].h,tabu[way[a+1]].h));
				}
				if((tabu[way[b]].k>k&&tabu[way[b+1]].k>k))
				{
					temp=temp*(1+epsilon*max(tabu[way[b]].h,tabu[way[b+1]].h));
				}
				if(temp<fullroad)
				{
					if(negcount<maxcount)
						negcount++;
				}
				if(temp<newmax)
				{
					newmax=actval;
					wina=a;
					winb=b;
				}
				z++;
				e.x=a;
				e.y=b;
				boxOfGoodies.push_back(e);
				sab[a*n+b]=0;
				sab[b*n+a]=0;
			}				
		}
		tabu[way[wina]].k=k+kar;
		tabu[way[wina+1]].k=k+kar;	
		tabu[way[winb]].k=k+kar;
		tabu[way[winb+1]].k=k+kar;
		tabu[way[wina]].h++;	
		tabu[way[wina+1]].h++;
		tabu[way[winb]].h++;
		tabu[way[winb+1]].h++;
		if(winb<wina)
		swap(winb,wina);
		wina++;
		while(wina<winb)
		{
			swap(way[wina],way[winb]);
			wina++; winb--;
		}
		fullroad=newmax;
		if(fullroad<bestroad)
		{
			bestroad=fullroad;
			for(int i=0;i<=n;i++)
				best[i]=way[i];
			negcount=dzil;
		}
		k++;
		z++;
	}
	for(int i=0;i<n;i++)
		way[i]=best[i];	
}
int main()
{
	ios_base::sync_with_stdio(0);
	srand( time( NULL ) );
	int n;
	cin >> n;
	cities=new city[n];
	int k=0;
	double x,y;
	float l=0;
	way=new int[n+1];
	dist=new float*[n];
	sab=new unsigned char[n*n];
	for(int i=n-1;i>=0;i--)
	{
		dist[i]=new float[i+1];
	}
	for(int i=0;i<n;i++)
	{
		cin >> k >> x >> y;
		cities[i].x=x;
		cities[i].y=y;
		cities[i].label=k;
	}
	dis(n);
	z=0;
	NEH(n);
	tsp(n);
	l=0;
	for(int i=0;i<n;i++)
	{
		double x,y;
		x=(cities[way[i]].x-cities[way[i+1]].x);
		y=(cities[way[i]].y-cities[way[i+1]].y);
		l+=sqrt(x*x+y*y);
	}
	cout << l << "\n";
	for(int i=0;i<n+1;i++)
	{
		cerr << cities[way[i]].label << "\n";
	}
	for(int i=0;i<n;i++)
	{
		delete(dist[i]);
	}
	delete dist;
	delete cities;
	delete way;
	delete sab;
}
