#include<iostream>
#include<stdlib.h>
#include<stdio.h>
#include<cmath>
#include <cstdlib>
#include<stdint.h>
#include <float.h>
#include <unistd.h>
#include <ctime>
#include <vector>  
#include <random> 
#define getDist(a,b) (b < a ? dist[a][b] : dist[b][a])
#ifndef M_E
#define M_E 2.71828182845904523536
#endif
using namespace std;
float **dist=0;
double T=100;
double red=0.000001;
double DZ=32;
int getBack=4000;
unsigned char* sab;
typedef struct{
	double x;
	double y;
	int label;
} city;
city* cities;
int* way;
unsigned long long int z=0;
unsigned long long int  sec=180;
time_t begintime;
double max1;
void dis(int n)
{
	register double x,y;
	max1=-1;
	for(int i=0;i<n;i++)
	{
		for(int j=0;j<=i;j++)
		{
			if(j!=i)
			{
				x=(cities[i].x-cities[j].x);
				y=(cities[i].y-cities[j].y);
				dist[i][j]=(float) sqrt(x*x+y*y);
				if(dist[i][j]>max1)
				{
					max1=dist[i][j];
				}
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
void tsp(int n)
{
	int a,b;
	double temp;
	double rad;
	int best[n+1];
	for(int i=0;i<n;i++)
		best[i]=way[i];
	double bestroad=0;
	int getBackCount=0;
	time_t czas;
	time_t bettertime;
	time( & bettertime);	
	std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<double> disto( 0, 1);	
    std::uniform_int_distribution<> dist2( 1, n-1);
    DZ=64;
    if(n>1500)
    	DZ+=(n/2000)*64;
    T=max1/DZ;
    double minT=0.001;
	for(int i=0;i<n;i++)
	{
		bestroad+=getDist(way[i],way[i+1]);
	}
    double fullroad=bestroad;
	double p;
	red=0.0000001;
	if(n>3000)
	{
		minT=0.00001;
		red=0.0000001;
		T=5;
		if(n>=8000)
		{
			T=2;
			red=0.0000005;
			minT=0.000001;
		}
		if(n>12000)
		{
			T=1;
		}
	}
	if(T>500)
	{
		red=0.000001;
	}
	if(T>10000)
	{
		T=10000;
	}
	double BT=T;
	bool test=true;
	//int lowcount=0;
	unsigned long long int lz=0;
	double dif=(1-red);
		//cout << temp << " " << rad << " " << p  << " " << bestroad << " " << T << "\n";
	for(int i=0;i<1;i++)
	{
	while(T>minT&&z<30000000&&test)
	{
		
		do
		{
			a=dist2(mt);
			b=dist2(mt);
		}
		while(abs(a-b)%(n-1)<=1);
		//cout << a << " " << b <<"\n";
		temp=fun(a,b);
		rad=disto(mt);
		p=pow(M_E,(temp/T));
		p=(double) 1/(1+p);
		if(p>rad)
		{
			//cout << temp << " " << rad << " " << p  << " " << bestroad << " " << T << "\n";
			if(b<a)
			swap(b,a);
			a++;
			while(a<b)
			{
				swap(way[a],way[b]);
				a++; b--;
			}
			fullroad+=temp;
			if(fullroad<bestroad)
			{
				lz=z;	
				//cout << temp << " " << rad << " " << p  << " " << bestroad << " " << T << "\n";
				bestroad=fullroad;
				for(int i=0;i<=n;i++)
					best[i]=way[i];
			}
			
		}
		z++;	
		T=dif*T;
		time( & czas );			
		if((czas-begintime)>=sec)
		{
			test=false;
		}
		if(BT>5)
			if(T<0.5)
					if((z-lz)>=100000)
						test=false;
	}
	for(int i=0;i<n;i++)
		way[i]=best[i];	
	fullroad=bestroad;
	T=BT;
	}
}
int main()
{
	ios_base::sync_with_stdio(0);
	time_t czas;
    time( & czas );
    begintime=czas;
	int n;
	cin >> n;
	cities=new city[n];
	int k=0;
	double x,y;
	float l=0;
	way=new int[n+1];
	dist=new float*[n];
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
	cin >> sec;
	//cout << sec << "\n";
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
	time( & czas );
	cout << (czas-begintime) << "\n";
	for(int i=0;i<n+1;i++)
	{
		//cerr << cities[way[i]].label << "\n";
	}
	for(int i=0;i<n;i++)
	{
		delete(dist[i]);
	}
	delete dist;
	delete cities;
	delete way;
}
