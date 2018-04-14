#include <bits/stdc++.h>
using namespace std;
///�ٶ�����Ϊ�������������Եȸ�����һ��
///�����Ͻǿ�ʼ���ߵ����½�Ϊֹ���ٶ�ÿһ����reward��Ϊ-1
const int maxn=4;
const int maxaction=2;
const double gamma=1.0;
double q[maxn][maxn][maxaction];///ÿ��״̬����value
int n[maxn][maxn][maxaction];  ///ÿ��״̬���ִ���
double f[maxn][maxn][maxaction];///ÿ��״̬����������������ĸ��ʣ����ԣ�
const int mx[]={1,0};
const int my[]={0,1};
struct Node{int x,y;};
bool obstcale[maxn][maxn];///��¼ĳλ���Ƿ����ϰ�
bool inGrid(Node tn){return tn.x>=0&&tn.y>=0&&tn.x<maxn&&tn.y<maxn&&!obstcale[tn.x][tn.y];}///��ǰλ���Ƿ�Ϸ�
struct QNode{Node qn;int k;};
int getK(Node tn){///ȡ��ĳ��λ�õĲ��ԣ���Ϊf[i][j][0]+f[i][j][1]=1,�����������(0,1)��f[i][j][0]�����action1��������action0
    double drand=(double)rand()/RAND_MAX;
    if(drand>=f[tn.x][tn.y][0])return 1;
    return 0;
}
vector<QNode> getEpisodes(){///���һ������㣨���Ͻǣ����յ㣨���½ǣ�������
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
void output_policy(){///������ղ���
    char outs[maxn][maxn];
    for(int i=0;i<maxn;i++)for(int j=0;j<maxn;j++)
        if(obstcale[i][j])outs[i][j]='#';else outs[i][j]='.';

    ///��ȡ����¼ÿ��״̬������action
    int best[maxn][maxn];
    for(int i=0;i<maxn;i++){
        for(int j=0;j<maxn;j++){
            int maxk=0;double maxp=f[i][j][maxk];
            for(int k=0;k<maxaction;k++)if(f[i][j][k]>maxp)
                maxp=f[i][j][k],maxk=k;
            best[i][j]=maxk;
        }
    }

    ///��������·���Ͼ����ĵ�
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
            if(needout[i][j]){///��������·���ϵĵ�
                int maxk=best[i][j];
                if(maxk==0)printf(" �� ");else printf("�� ");
            }
            else{
                 printf(" %c ",outs[i][j]);
            }
        }puts("\n");
    }
}
void mc(){
    ///��ʼ��
    memset(q,0,sizeof(q));memset(n,0,sizeof(n));
    ///��ʼ��ÿ��״̬�µ���ÿ��action�ĸ��ʶ���ͬ
    for(int i=0;i<maxn;i++)for(int j=0;j<maxn;j++)for(int k=0;k<maxaction;k++)
        f[i][j][k]=1/(double)maxaction;
    memset(obstcale,0,sizeof(obstcale));
    ///�����ϰ�
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
double v[maxn][maxn][maxaction];///����sarsa��ÿ��q(s,a)
void sa(){
    ///��ʼ��
    memset(v,0,sizeof(v));
    for(int i=0;i<maxn;i++)for(int j=0;j<maxn;j++)for(int k=0;k<maxaction;k++)
        f[i][j][k]=1/(double)maxaction;
    memset(obstcale,0,sizeof(obstcale));
    ///�����ϰ�
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
