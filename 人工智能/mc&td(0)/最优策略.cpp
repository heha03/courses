#include <bits/stdc++.h>
using namespace std;
///假定策略为：右下两方向以等概率走一步
///从左上角开始，走到右下角为止，假定每一步的reward都为-1
const int maxn=4;
const int maxaction=2;
const double gamma=1.0;
double q[maxn][maxn][maxaction];///每个状态的总value
int n[maxn][maxn][maxaction];  ///每个状态出现次数
double f[maxn][maxn][maxaction];///每个状态下向右下两个方向的概率（策略）
const int mx[]={1,0};
const int my[]={0,1};
struct Node{int x,y;};
bool obstcale[maxn][maxn];///纪录某位置是否有障碍
bool inGrid(Node tn){return tn.x>=0&&tn.y>=0&&tn.x<maxn&&tn.y<maxn&&!obstcale[tn.x][tn.y];}///当前位置是否合法
struct QNode{Node qn;int k;};
int getK(Node tn){///取得某个位置的策略，因为f[i][j][0]+f[i][j][1]=1,所以用随机数(0,1)比f[i][j][0]大就用action1，否则用action0
    double drand=(double)rand()/RAND_MAX;
    if(drand>=f[tn.x][tn.y][0])return 1;
    return 0;
}
vector<QNode> getEpisodes(){///获得一个从起点（左上角）到终点（右下角）的序列
    Node start;
    start.x=start.y=0;
    vector<QNode> ret;
//    ret.push_back(start);
    Node nex=start;
    while(!(nex.x==3&&nex.y==3)){
        int k=getK(nex);
        ret.push_back((QNode){nex,k});
        Node tnex=nex;
        nex.x+=mx[k],nex.y+=my[k];
        if(!inGrid(nex)){
            nex=ret[ret.size()-2].qn;
        }
    }
    return ret;
}
void output_policy(){///输出最终策略
    char outs[maxn][maxn];
    for(int i=0;i<maxn;i++)for(int j=0;j<maxn;j++)
        if(obstcale[i][j])outs[i][j]='#';else outs[i][j]='.';

    ///获取并记录每个状态的最优action
    int best[maxn][maxn];
    for(int i=0;i<maxn;i++){
        for(int j=0;j<maxn;j++){
            int maxk=0;double maxp=f[i][j][maxk];
            for(int k=0;k<maxaction;k++)if(f[i][j][k]>maxp)
                maxp=f[i][j][k],maxk=k;
            best[i][j]=maxk;
        }
    }

    ///保存最优路线上经过的点
    bool needout[maxn][maxn];
    memset(needout,0,sizeof(needout));
    Node nex;nex.x=nex.y=0;
    while(!(nex.x==3&&nex.y==3)){
        needout[nex.x][nex.y]=true;
        outs[nex.x][nex.y]= best[nex.x][nex.y]==0? 'd':'r';
        int k=best[nex.x][nex.y];
        nex.x+=mx[k],nex.y+=my[k];
    }

    outs[maxn-1][maxn-1]='e';
    for(int i=0;i<maxn;i++){
        for(int j=0;j<maxn;j++){
            if(needout[i][j]){///若是最优路线上的点
                int maxk=best[i][j];
                if(maxk==0)printf(" ↓ ");else printf("→ ");
            }
            else{
                 printf(" %c ",outs[i][j]);
            }
        }puts("\n");
    }
}
void mc(){
    ///初始化
    memset(q,0,sizeof(q));memset(n,0,sizeof(n));
    ///初始化每个状态下的走每个action的概率都相同
    for(int i=0;i<maxn;i++)for(int j=0;j<maxn;j++)for(int k=0;k<maxaction;k++)
        f[i][j][k]=1/(double)maxaction;
    memset(obstcale,0,sizeof(obstcale));
    ///设置障碍
    obstcale[3][1]=true;obstcale[3][2]=true;

    int cnt=0;
    do{
        vector<QNode> e=getEpisodes();
        double epsilon=1/(double)(++cnt);
        double G=0;
        for(int i=0;i<e.size();i++)G*=gamma,G+=-1;
        for(int i=0;i<e.size();i++){
            ///update n&&q
            int x=e[i].qn.x,y=e[i].qn.y,k=e[i].k;
            q[x][y][k]+=G;
            n[x][y][k]++;
            G-=-1;
            G/=gamma;

            ///update policy
            int maxk=0;double maxp=f[x][y][maxk];
            for(int tk=0;tk<maxaction;tk++)if(f[x][y][tk]>maxp)
                maxk=tk,maxp=f[x][y][tk];
            if(maxk!=k&&
               q[x][y][k]*n[x][y][maxk]>q[x][y][maxk]*n[x][y][k]){
                for(int tk=0;tk<maxaction;tk++)
                    if(tk==k) f[x][y][tk]=1-epsilon+epsilon/maxaction;
                    else f[x][y][tk]=epsilon/maxaction;
            }
        }
       }
    }while(cnt<1e4);
    ///print the policy
    output_policy();
}
const double alpha=0.1;
double v[maxn][maxn][maxaction];///保存sarsa中每个q(s,a)
void sa(){
    ///初始化
    memset(v,0,sizeof(v));
    for(int i=0;i<maxn;i++)for(int j=0;j<maxn;j++)for(int k=0;k<maxaction;k++)
        f[i][j][k]=1/(double)maxaction;
    memset(obstcale,0,sizeof(obstcale));
    ///设置障碍
    obstcale[3][1]=true;obstcale[3][2]=true;

    int cnt=0;
    do{
        vector<QNode> e=getEpisodes();
        double epsilon=1/(double)(++cnt);
        for(int i=0;i<e.size();i++){
            QNode ei=e[i];
            int x=ei.qn.x,y=ei.qn.y,k=ei.k;
            double r=-1;double nexv=0;
            if(i+1<(int)e.size()){
                QNode nexei=e[i+1];
                nexv=v[nexei.qn.x][nexei.qn.y][nexei.k];
            }
            v[x][y][k]+=alpha*(r+gamma*nexv-v[x][y][k]);

            ///update policy
            int maxk=0;double maxp=f[x][y][maxk];
            for(int tk=0;tk<maxaction;tk++)if(f[x][y][tk]>maxp)
                maxk=tk,maxp=f[x][y][tk];
            if(maxk!=k&&
               v[x][y][k]>v[x][y][maxk]){
                for(int tk=0;tk<maxaction;tk++)
                    if(tk==k) f[x][y][tk]=1-epsilon+epsilon/maxaction;
                    else f[x][y][tk]=epsilon/maxaction;
            }
        }
    }while(cnt<1e4);

   ///print the policy
    output_policy();
}
int main(){
    srand((unsigned)time(NULL));
    sa();
//    mc();
    return 0;
}
