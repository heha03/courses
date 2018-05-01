//#define LOCAL
#include <bits/stdc++.h>
#define mem(a) memset(a,0,sizeof(a))
#define mem1(a) memset(a,-1,sizeof(a))
#define pb(x) push_back(x)
#define rep(i,n) for(int i=0;i<n;i++)
#define rep2(i,l,r) for(int i=l;i<=r;i++)
#define dec(i,r,l) for(int i=r;i>=l;i--)
#define sz(x) ((int)(x).size())
#define startidx n+1
const long double PI=acosl((long double)-1.0);
using namespace std;
const int MAXN=1024;
const int INF=1e9;
struct Edge{int from,to,dist,dir;Edge(int u,int v,int d,int r):from(u),to(v),dist(d),dir(r){}};
struct HeapNode{int u,d;
bool operator <(const HeapNode&rhs)const{return d>rhs.d;}};
int R[255],D[255];
//char RD[]="wesn";
const char* RD[4]={"west","east","south","north"};
void setRD()
{
    D['w']=0;D['e']=1;D['s']=2;D['n']=3;///0^1=1,1^1=0,use ^ to get the opposite direction
    R['s']=0;R['l']=1;///sidewalk is 0,lane is 1
}
const int maxr=1024;
map<string,int> rid;
string road[maxr];
struct Dijkstra
{
    int n,m;
    vector<Edge> edges;
    vector<int> G[MAXN];
    bool done[MAXN];
    int d[MAXN],p[MAXN];
    void init(int n)
    {
        this->n=n;
        edges.clear();
        rep2(i,0,n+2)G[i].clear();
    }
    void AddEdge(int from,int to,int dist,int dir)
    {
        edges.push_back((Edge){from,to,dist,dir});
        int m=edges.size();G[from].pb(m-1);
    }
    void dijkstra(int s)
    {
        mem(done);
        rep2(i,0,n)d[i]=INF;
        d[s]=0;
        priority_queue<HeapNode> Q;
        Q.push((HeapNode){s,0});
        while(!Q.empty())
        {
            HeapNode x=Q.top();Q.pop();
            int u=x.u;
            if(done[u])continue;
            done[u]=true;
            rep(i,sz(G[u]))
            {
                Edge&e=edges[G[u][i]];
                if(d[e.to]>d[u]+e.dist)
                {
                    d[e.to]=d[u]+e.dist;
                    p[e.to]=G[u][i];
                    Q.push((HeapNode){e.to,d[e.to]});
                }
            }
        }
    }
    void guide(int s,int t)
    {
        dijkstra(s);
        if(d[t]==INF){printf("not accessable\n");return;}
        vector<int> v;
        v.pb(t);
        while(edges[p[t]].from!=s)v.pb(edges[p[t]].from),t=edges[p[t]].from;
        v.pb(s);
        vector<int> g;
        dec(i,sz(v)-1,0)g.pb(v[i]);
//        rep(i,sz(g))printf("%d ",g[i]);puts("");
        rep2(i,1,sz(g)-1)
        {
            Edge&e=edges[p[g[i]]];
//            cout<<"from "<<road[e.from]<<" to "<<road[e.to]<<" in the "<<RD[e.dir]<<" "<<e.dist<<"meters away"<<endl;
            cout<<"from "<<road[e.from]<<" to "<<road[e.to]<<" in the "<<RD[e.dir]<<" "<<e.dist<<" meters away"<<endl;
        }
    }

    void notAt(int s,int t,int dist,char cdir)
    {
        int dir=D[cdir];
        vector<Edge> oldEdges=edges;
        int start=startidx;
        G[start].clear();
        ///find the same direction edge
        rep(i,sz(G[s]))
        {
            Edge&e=edges[G[s][i]];
            if(e.dir==dir){AddEdge(start,e.to,e.dist-dist,dir);break;}
        }
        ///find the opposite direction edge
        rep(i,sz(oldEdges))
        {
            Edge&e=edges[i];
            if(e.to==s&&e.dir==dir^1){AddEdge(start,e.to,dist,e.dir);break;}
        }
        guide(start,t);
        edges=oldEdges;
    }
}dj[2];
const int maxo=128;
int main() {
#ifdef LOCAL
    freopen("#3.txt", "r", stdin);
#endif
    int n,m;setRD();
    cout<<"input the number of points and edges"<<endl;
    scanf("%d%d",&n,&m);
    dj[0].init(n);dj[1].init(n);
    rid.clear();
    int tot=0;
    while(m--)
    {
        string u,v;int w;
        string dir,roadType;
        cin>>u>>v>>w>>dir>>roadType;
        if(!rid.count(u))rid[u]=tot,road[tot++]=u;
        if(!rid.count(v))rid[v]=tot,road[tot++]=v;
        dj[R[roadType[0]]].AddEdge(rid[u],rid[v],w,D[dir[0]]);
    }
    road[startidx]="your position";
    while(true)
    {
        cout<<"Where do you want to arrive?"<<endl;
        string ts;cin>>ts;
        if(!rid.count(ts)){cout<<"Sorry, this place doesn't exist"<<endl;continue;}
        int t=rid[ts];

        cout<<"How do you want to get there?"<<endl;
        string sRoadType;cin>>sRoadType;
        int roadType=R[sRoadType[0]];

        cout<<"Where are you?"<<endl;
        char order[maxo];
        scanf("%s",order);
        if(strcmp(order,"exit")==0)break;
        else if(isdigit(order[0]))///200 East to XXX
        {
            int dist;sscanf(order,"%d",&dist);
            string dir;string ss;
            cin>>dir>>ss>>ss;
            if(!rid.count(ss)){cout<<"Sorry, this place doesn't exist"<<endl;continue;}
            int s=rid[ss];
            dj[roadType].notAt(s,t,dist,dir[0]);
        }else///at XXX
        {
            string ss;cin>>ss;
            if(!rid.count(ss)){cout<<"Sorry, this place doesn't exist"<<endl;continue;}
            int s=rid[ss];
            dj[roadType].guide(s,t);
        }
    }
    return 0;
}

