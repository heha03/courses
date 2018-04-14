#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

const int MAX_N = 4;
const float INF = -9999999.0f;
float V[MAX_N][MAX_N];      //值矩阵
float OLDV[MAX_N][MAX_N];    //保存旧的val值
char S[MAX_N][MAX_N];      //状态矩阵，直观存储运动方向
char A[4] = { 'n','e','s','w' };     //Action ,'↑','→','↓','←'
int dx[] = { -1,0,1,0 }, dy[] = { 0,1,0,-1 };  //四向移动
int r = -1;                  //奖励值
float disc = 1.0f;            //折扣值
float en = 0.0000001f;        //sub小于这个数时收敛
void AddEndPoint(int x,int y){S[x][y]='E';}//将(x,y)位置设置为终点
void AddObstcle(int x,int y){S[x][y]='#';}//将(x,y)位置设置为障碍
 //初始化
void init() {///S is the map,# is the 障碍,E is the end point
    AddObstcle(0,1);AddObstcle(0,2);AddObstcle(0,3);
    AddObstcle(1,1);AddObstcle(1,3);
    AddEndPoint(1,2);
//    S[0][1]=S[0][2]=S[0][3]='#';
//    S[2][1]=S[2][2]=S[2][3]='#';
//    S[1][1]=S[1][3]='#';
//    S[1][2]='E';
//    S[3][1] = 'E';                       //E目标点
//    S[2][2] = '#';
//    S[1][2] = 'E';

    for(int i=0;i<MAX_N;i++)for(int j=0;j<MAX_N;j++){
        if(i==0&&j==0)OLDV[i][j]=0;//初始化起点的value function值为0
        else OLDV[i][j]=INF;
    }
}

void printVal() {//输出中间的value值
    puts("----------------value-----------------");
    for (int i = 0; i < MAX_N; i++) {
        for (int j = 0; j < MAX_N; j++) {
            printf("%.4f ", V[i][j]);
        }
        puts("\n");
    }
}
int IV[MAX_N][MAX_N];
void output_path(int x,int y){//输出起点到s[x][y]的最短路径
    char outs[MAX_N][MAX_N];
    memcpy(outs,S,sizeof(S));
    for(int i=0;i<MAX_N;i++)for(int j=0;j<MAX_N;j++)
        if(S[i][j]!='#'&&!(x==i&&y==j))outs[i][j]='.';
    while(!(x==0&&y==0)){
        int nx,ny;
        int k=4;
        while (k--) {
            nx = x + dx[k], ny = y + dy[k];
            if (nx >= 0 && nx < MAX_N && ny >= 0 && ny < MAX_N && IV[nx][ny]==IV[x][y]+1)
            {outs[nx][ny]=A[k];x=nx,y=ny;break;}
        }
    }


    for (int i = 0; i < MAX_N; i++) {
        for (int j = 0; j < MAX_N; j++){
            char ch;
            switch (outs[i][j]) {
              case 's':
                  printf(" ↑ ");
                  break;
              case 'w':
                  printf("→ ");
                  break;
              case 'n':
                  printf(" ↓ ");
                  break;
              case 'e':
                  printf("← ");
                  break;
              default:
                  printf(" %c ", outs[i][j]);
                  break;
            }
        }
        puts("\n");
    }puts("\n");
}

void ValueIteration() {
    float sub = 0;
    int cnt = 0;
    do {
        sub = 0;
        for (int i = 0; i < MAX_N; i++) {
            for (int j = 0; j < MAX_N; j++) if(!(i==0&&j==0)){
                if (S[i][j] != '#') {           //不是障碍物及终点
                    float val = V[i][j];
                    int k = 4;
                    float ma = INF;
                    while (k--) {
                        int x = i + dx[k], y = j + dy[k];
                        if (x >= 0 && x < MAX_N && y >= 0 && y < MAX_N && S[x][y] != '#') {
                            float round = r + disc * OLDV[x][y];
                            if (ma < round) {
                                 ma = round;    //改变Val值
                            }
                        }
                    }
                    V[i][j] = ma;                        //V[i][j]取得周围环境的最大值

                    sub = max(sub, fabs(val - V[i][j]));
                }else V[i][j]=INF;
            }
        }

        //把新的val值拷贝到旧的数组中
        memcpy(OLDV,V,sizeof(V));
        printf("cnt :   %d\n", ++cnt);
        printVal();

    } while (sub >= en);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xA); //亮绿
    puts("---------------distance---------------");
    for (int i = 0; i < MAX_N; i++) {
        for (int j = 0; j < MAX_N; j++) {
            if(V[i][j]==INF)printf("oo ");
            else printf(" %.0f ",-1* V[i][j]+1e-10);
        }
        puts("\n");
    }

    for(int i=0;i<MAX_N;i++)for(int j=0;j<MAX_N;j++)IV[i][j]=round(V[i][j]);
    //打印策略
    for(int i=0;i<MAX_N;i++)for(int j=0;j<MAX_N;j++)if(S[i][j]=='E'&&V[i][j]>=-16){//当前的E必须相对起点可达
        output_path(i,j);
    }

     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f); //变回原状
}

int main() {
    init();
    ValueIteration();
    return 0;
}
