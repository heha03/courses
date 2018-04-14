#include <bits/stdc++.h>
using namespace std;
///�ٶ�����Ϊ�������������Եȸ�����һ������ĳ�������磬��ض�����һ����
///�ߵ����½�Ϊֹ���ٶ�ÿһ����reward��Ϊ1
const int maxn=4;
const double gamma=1.0;
double s[maxn][maxn];///ÿ��״̬����value
int n[maxn][maxn];  ///ÿ��״̬���ִ���
const int mx[]={1,0};
const int my[]={0,1};
struct Node{int x,y;};
vector<Node> getEpisodes(){///���һ������㣨���Ͻǣ����յ㣨���½ǣ�������
    Node start;
    start.x=start.y=0;
    vector<Node> ret;
    ret.push_back(start);
    Node nex=start;
    while(!(nex.x==3&&nex.y==3)){
        if(nex.y==3)nex.x++;///�޷������ߣ��ͱ�������
        else if(nex.x==3)nex.y++;///�޷������ߣ��ͱ�������
        else{///���߶������ߣ��������һ��
            int k=rand()%2;
            nex.x+=mx[k],nex.y+=my[k];
        }
        ret.push_back(nex);
    }
    return ret;
}
void mc(){
    ///��ʼ��
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
        if(sumn%5==0){///����м䲿�ֽ��
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
            if(n[i][j]==0)printf("oo    ");///δ���ʹ����������
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
            if(i+1<(int)e.size()){///�������һ����������һ����ֵ����nexv
                Node nexei=e[i+1];
                nexv=v[nexei.x][nexei.y];
            }
            v[ei.x][ei.y]+=alpha*(r+gamma*nexv-v[ei.x][ei.y]);
        }

        if(sumn%100==0){///����м䲿�ֽ��
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
