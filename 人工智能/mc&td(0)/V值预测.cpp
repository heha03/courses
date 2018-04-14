#include <bits/stdc++.h>
using namespace std;
///假定策略为：右下两方向以等概率走一步，若某方向会出界，则必定走另一方向
///走到右下角为止，假定每一步的reward都为1
const int maxn=4;
const double gamma=1.0;
double s[maxn][maxn];///每个状态的总value
int n[maxn][maxn];  ///每个状态出现次数
const int mx[]={1,0};
const int my[]={0,1};
struct Node{int x,y;};
vector<Node> getEpisodes(){///获得一个从起点（左上角）到终点（右下角）的序列
    Node start;
    start.x=start.y=0;
    vector<Node> ret;
    ret.push_back(start);
    Node nex=start;
    while(!(nex.x==3&&nex.y==3)){
        if(nex.y==3)nex.x++;///无法向右走，就必须向下
        else if(nex.x==3)nex.y++;///无法向下走，就必须向右
        else{///两边都可以走，就随机走一步
            int k=rand()%2;
            nex.x+=mx[k],nex.y+=my[k];
        }
        ret.push_back(nex);
    }
    return ret;
}
void mc(){
    ///初始化
    memset(s,0,sizeof(s));memset(n,0,sizeof(n));
    srand((unsigned)time(NULL));

    int sumn=0;
    do{
        vector<Node> e=getEpisodes();
        double G=0;
        for(int i=0;i<e.size();i++)G*=gamma,G+=1;
        for(int i=0;i<e.size();i++){
            Node ei=e[i];
            s[ei.x][ei.y]+=G;
            n[ei.x][ei.y]++;
            G-=1;
            G/=gamma;
            sumn++;
        }
        if(sumn%5==0){///输出中间部分结果
            printf("value: \n");
            for(int i=0;i<maxn;i++){
                for(int j=0;j<maxn;j++){
                    if(n[i][j]==0)printf("oo    ");
                    else printf("%.4f ",s[i][j]/n[i][j]);
                }puts("\n");
            }
        }
    }while(sumn<1e2);

    for(int i=0;i<maxn;i++){
        for(int j=0;j<maxn;j++){
            if(n[i][j]==0)printf("oo    ");///未访问过，输出无穷
            else printf("%.4f ",s[i][j]/n[i][j]);
        }
        puts("\n");
    }
}
//const double alpha=0.001;
const double alpha=0.1;
double v[maxn][maxn];
void td(){
    memset(v,0,sizeof(v));
    int sumn=0;
    do{
        vector<Node> e=getEpisodes();
        for(int i=0;i<e.size();i++){
            sumn++;
            Node ei=e[i];
            double r=1;double nexv=0;
            if(i+1<(int)e.size()){///不是最后一步，就用下一步的值赋给nexv
                Node nexei=e[i+1];
                nexv=v[nexei.x][nexei.y];
            }
            v[ei.x][ei.y]+=alpha*(r+gamma*nexv-v[ei.x][ei.y]);
        }

        if(sumn%100==0){///输出中间部分结果
            printf("value :\n");
            for(int i=0;i<maxn;i++){
                for(int j=0;j<maxn;j++)printf("%.4f ",v[i][j]);
                puts("\n");
            }
        }
    }while(sumn<1e4);

    puts("\n");
    for(int i=0;i<maxn;i++){
        for(int j=0;j<maxn;j++){
            printf("%.4f ",v[i][j]);
        }puts("\n");
    }
}
int main(){
    //td();
    td();
    return 0;
}
