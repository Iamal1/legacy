#include<stdio.h>
#include<string.h>
#include<math.h>
#include<algorithm>
#include <ctime>
#include <iostream>
#include <fstream>
#include<vector>
#include<set>
using namespace std;
#define PR1 -1e-9
#define PR 1e-9
struct TPoint
{
    double x,y,z;
    TPoint(){}
    TPoint(double _x,double _y,double _z):x(_x),y(_y),z(_z){}
    inline bool operator == (const TPoint &v) const { return x==v.x && y==v.y && z==v.z; }
    inline bool operator != (const TPoint& v) const { return x!=v.x || y!=v.y || z!=v.z; }
    inline bool operator <  (const TPoint& v) const
    {
        if (x<v.x) return true;
        else if (x>v.x) return false;
        else if (y<v.y) return true;
        else if (y>v.y) return false;
        else return (z<v.z);
    }
    TPoint operator-(const TPoint p) {return TPoint(x-p.x,y-p.y,z-p.z);}
    TPoint operator*(const TPoint p) {return TPoint(y*p.z-z*p.y,z*p.x-x*p.z,x*p.y-y*p.x);}//叉积
    double operator^(const TPoint p) {return x*p.x+y*p.y+z*p.z;}//点积
};

struct fac//
{
    int a,b,c;//凸包一个面上的三个点的编号
    bool ok;//该面是否是最终凸包中的面
};
TPoint xmult(TPoint u,TPoint v)
{
    return TPoint(u.y*v.z-v.y*u.z,u.z*v.x-u.x*v.z,u.x*v.y-u.y*v.x);
}
double dmult(TPoint u,TPoint v)
{
    return u.x*v.x+u.y*v.y+u.z*v.z;
}
TPoint subt(TPoint u,TPoint v)
{
    return TPoint(u.x-v.x,u.y-v.y,u.z-v.z);
}
double vlen(TPoint u)
{
    return sqrt(u.x*u.x+u.y*u.y+u.z*u.z);
}
TPoint pvec(TPoint a,TPoint b,TPoint c)
{
    return xmult(subt(a,b),subt(b,c));
}
double Dis(TPoint a,TPoint b,TPoint c,TPoint d)
{
    return fabs(dmult(pvec(a,b,c),subt(d,a)))/vlen(pvec(a,b,c));
}
void printp(TPoint a)
{
    cout << a.x <<" "<< a.y  <<" "<< a.z<< endl;
}

struct T3dhull
{
    int n;//初始点数
    int trianglecnt;//凸包上三角形数
    TPoint *ply;//初始点
    vector <fac> tri;//所有三角形
    set <TPoint> hply;//凸包点
    vector <fac> htri;//凸包三角形
    int **vis;//点i到点j是属于哪个面
    T3dhull(int no)
    {
        n = no;
        ply = new TPoint [no];
        vis = new int*[no];
        for(int i=0;i!=no;i++)
            vis[i]=new int[no];
        //初始化是没问题的
        for(int i = 0; i<n; i++)
        {

            for(int j = 0; j<n;j++)
            {
               vis[i][j] = 0;
            }
        }
    }
    ~T3dhull()
    {
        delete [] ply;
        for(int i = 0; i != n; i++)
        {
            delete [] vis[i];
        }
        delete* vis;
    }
    double dist(TPoint a){return sqrt(a.x*a.x+a.y*a.y+a.z*a.z);}//两点长度
    double area(TPoint a,TPoint b,TPoint c){return dist((b-a)*(c-a));}//三角形面积*2
    double volume(TPoint a,TPoint b,TPoint c,TPoint d){return (b-a)*(c-a)^(d-a);}//四面体有向体积*6
    double ptoplane(TPoint &p,fac &f)//正：点在面同向
    {
         TPoint m=ply[f.b]-ply[f.a],n=ply[f.c]-ply[f.a],t=p-ply[f.a];
         return (m*n)^t;
    }
    double ptoplane1(TPoint &p,fac &f)//正：点在面同向
    {
         TPoint m=ply[f.b]-ply[f.a],n=ply[f.c]-ply[f.a],t=p-ply[f.a];
         return vlen((m*n)*t)/vlen(m*n);
    }
    void deal(int p,int a,int b)
    {
        int f=vis[a][b];//与当前面(cnt)共边(ab)的那个面
        //cout << p << " "<<a << " "<<b << endl;
        //cout << "f: "<<f << endl;
        if (f==0) {cout << p << " "<<a << " "<<b << endl;cout << "f: "<<f << endl;
        if (tri[f].ok) cout << "ok" << endl;
        else cout << "no" << endl;
        //sss();
        }//sss();}
        fac add;
        if(tri[f].ok)
        {
            if((ptoplane(ply[p],tri[f]))>1e-8) {
                /*
                    if(ptoplane(ply[p],tri[f])<1e-8)
                    {
                        cout << "deal:"<<endl;
                        cout << ptoplane(ply[p],tri[f]) <<endl;
                        cout <<p <<": " <<ply[p].x <<" "<< ply[p].y  <<" "<< ply[p].z<< endl;
                        cout <<tri[f].a <<": " <<  ply[tri[f].a].x <<" "<< ply[tri[f].a].y  <<" "<< ply[tri[f].a].z<< endl;
                        cout <<tri[f].b<<": " <<  ply[tri[f].b].x <<" "<< ply[tri[f].b].y  <<" "<< ply[tri[f].b].z<< endl;
                        cout <<tri[f].c <<": " <<  ply[tri[f].c].x <<" "<< ply[tri[f].c].y  <<" "<< ply[tri[f].c].z<< endl;
                    }*/
                    dfs(p,f);}//如果p点能看到该面f，则继续深度探索f的3条边，以便更新新的凸包面
            else//否则因为p点只看到cnt面，看不到f面，则p点和a、b点组成一个三角形。
            {
                if(ptoplane1(ply[p],tri[f])<1e-8)
                    {
                        cout << "deal:"<<endl;
                        cout << ptoplane1(ply[p],tri[f]) <<endl;
                        cout <<p <<": " <<ply[p].x <<" "<< ply[p].y  <<" "<< ply[p].z<< endl;
                        cout <<tri[f].a <<": " <<  ply[tri[f].a].x <<" "<< ply[tri[f].a].y  <<" "<< ply[tri[f].a].z<< endl;
                        cout <<tri[f].b<<": " <<  ply[tri[f].b].x <<" "<< ply[tri[f].b].y  <<" "<< ply[tri[f].b].z<< endl;
                        cout <<tri[f].c <<": " <<  ply[tri[f].c].x <<" "<< ply[tri[f].c].y  <<" "<< ply[tri[f].c].z<< endl;
                    }
                else{
                 add.a=b,add.b=a,add.c=p,add.ok=1;
                 vis[p][b]=vis[a][p]=vis[b][a]=trianglecnt;
                 //tri[trianglecnt++]=add;
                 tri.push_back(add);trianglecnt++;
                }
            }
        }
    }
    void dfs(int p,int cnt)//维护凸包，如果点p在凸包外更新凸包
    {

        tri[cnt].ok=0;//当前面需要删除，因为它在更大的凸包里面
//下面把边反过来(先b,后a)，以便在deal()中判断与当前面(cnt)共边(ab)的那个面。即判断与当头面(cnt)相邻的3个面(它们与当前面的共边是反向的，如下图中(1)的法线朝外(即逆时针)的面130和312,它们共边13，但一个方向是13,另一个方向是31)

        deal(p,tri[cnt].b,tri[cnt].a);
        deal(p,tri[cnt].c,tri[cnt].b);
        deal(p,tri[cnt].a,tri[cnt].c);
    }
    bool same(int s,int e)//判断两个面是否为同一面
    {
        TPoint a=ply[tri[s].a],b=ply[tri[s].b],c=ply[tri[s].c];
        return fabs(volume(a,b,c,ply[tri[e].a]))<PR
            &&fabs(volume(a,b,c,ply[tri[e].b]))<PR
            &&fabs(volume(a,b,c,ply[tri[e].c]))<PR;
    }
    void construct()//构建凸包
    {
        int i,j;
        trianglecnt=0;
        if(n<4) return ;
        bool tmp=true;
        for(i=1;i<n;i++)//前两点不共点
        {
            if((dist(ply[0]-ply[i]))>PR)
            {
                swap(ply[1],ply[i]); tmp=false; break;
            }
        }
        if(tmp) return;
        tmp=true;
        for(i=2;i<n;i++)//前三点不共线
        {
            if((dist((ply[0]-ply[1])*(ply[1]-ply[i])))>PR)
            {
                swap(ply[2],ply[i]); tmp=false; break;
            }
        }
        if(tmp) return ;
        tmp=true;
        for(i=3;i<n;i++)//前四点不共面
        {
            if(fabs((ply[0]-ply[1])*(ply[1]-ply[2])^(ply[0]-ply[i]))>PR)
            {
                swap(ply[3],ply[i]); tmp=false; break;
            }
        }
        if(tmp) return ;
        fac add;
        cout << "00"<< endl;
        for(i=0;i<4;i++)//构建初始四面体(4个点为ply[0],ply[1],ply[2],ply[3])
        {
            add.a=(i+1)%4,add.b=(i+2)%4,add.c=(i+3)%4,add.ok=1;
            //cout << add.a<<add.b<<add.c <<endl;
            if((ptoplane(ply[i],add))>PR) {swap(add.b,add.c);}//保证逆时针，即法向量朝外，这样新点才可看到。
            vis[add.a][add.b]=vis[add.b][add.c]=vis[add.c][add.a]=trianglecnt;//逆向的有向边保存
            //tri[trianglecnt++]=add;
            //cout << add.a<<add.b<<add.c <<endl;
            tri.push_back(add);trianglecnt++;
            //sss();cout << "."<< endl;
        }
        cout << "000"<< endl;
        //sss();

        for(i=4;i<n;i++)//构建更新凸包
        {

            for(j=0;j<trianglecnt;j++)//对每个点判断是否在当前3维凸包内或外(i表示当前点,j表示当前面)
            {
                if(tri[j].ok&&(ptoplane(ply[i],tri[j]))>1e-6)//对当前凸包面进行判断，看是否点能否看到这个面
                {
                    if(ptoplane(ply[i],tri[j])<1e-8)
                    {
                        cout << "consting" <<endl;
                        cout << ptoplane(ply[i],tri[j]) <<endl;
                        cout <<i <<": " <<ply[i].x <<" "<< ply[i].y  <<" "<< ply[i].z<< endl;
                        cout <<  ply[tri[j].a].x <<" "<< ply[tri[j].a].y  <<" "<< ply[tri[j].a].z<< endl;
                        cout <<  ply[tri[j].b].x <<" "<< ply[tri[j].b].y  <<" "<< ply[tri[j].b].z<< endl;
                        cout <<  ply[tri[j].c].x <<" "<< ply[tri[j].c].y  <<" "<< ply[tri[j].c].z<< endl;
                    }
                    dfs(i,j); break;//点能看到当前面，更新凸包的面(递归，可能不止更新一个面)。当前点更新完成后break跳出循环
                }
            }
        }
        cout << "save" << endl;
        int cnt=trianglecnt;//保存最外层的凸包
        for(i=0;i<cnt;i++)
        {
            if(tri[i].ok)
            {
                htri.push_back(tri[i]);
                hply.insert(ply[tri[i].a]);
                hply.insert(ply[tri[i].b]);
                hply.insert(ply[tri[i].c]);
            }

        }
    }
    bool isin(TPoint p)
	{
		bool flag = true;
		for (int i = 0; i<htri.size(); i++)
		{
			fac f = tri[i];
			if (ptoplane(p, f) > PR)
			{
				flag = false;
				break;
			}

		}
		return flag;
	}
    void sss()
    {

        for(int i = 0; i<n; i++)
        {
            cout << i<<":";
        for(int j = 0; j<n;j++)
            {
               //if (vis[i][j] > trianglecnt)
                cout <<vis[i][j] << " ";
            }
            cout << endl;
        }
        for(int i = 0; i<n; i++)
        { cout <<  ply[i].x <<" "<< ply[i].y  <<" "<< ply[i].z<< endl;
        }

        for(int i = 0; i<trianglecnt; i++)
        cout <<i<<": " <<tri[i].a << " "<<tri[i].b << " "<< tri[i].c << endl;/*
        for(int i = 0; i<htri.size(); i++)
        cout << htri[i].a << " "<<htri[i].b << " "<< htri[i].c << endl;
        */
    }
};

TPoint testa[100000];

int main()
{
    srand(time(NULL));
    double now,_min;
    int n;
    scanf("%d",&n);
    T3dhull hull(n);
    if(hull.n==0) return 0;
    int i,j,q;

    ifstream in("out1.txt");
    if (in.is_open())
    {
        for(i=0;i<hull.n;i++)
    {
         in>>testa[i].x >>testa[i].y>>testa[i].z;
    }
         in.close();
    }
/*
    for(i=0;i<n;i++)
    {
        testa[i].x = rand()%(1000);
        testa[i].y = rand()%(1000);
        testa[i].z = rand()%(1000);
    }
    /*
    ofstream out("out.txt");
    if (out.is_open())
    {
        for(i=0;i<n;i++)
    {
         out << testa[i].x <<" "<< testa[i].y  <<" "<< testa[i].z<< endl;
    }
         out.close();
    }
*/
    for(i=0;i<hull.n;i++)
    {
        hull.ply[i].x = testa[i].x ;
        hull.ply[i].y = testa[i].y;
        hull.ply[i].z = testa[i].z;
    }
    cout << "toc" <<endl;

    hull.construct();
    //hull.sss();
    cout << hull.hply.size() <<endl;



    return 0;
}
