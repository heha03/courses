#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

const int MAX_N = 4;
const float INF = -1000000000.0f;
float V[MAX_N][MAX_N];      //值矩阵
float OLDV[MAX_N][MAX_N];    //保存旧的val值
char S[MAX_N][MAX_N];      //状态矩阵，直观存储运动方向
char OLDS[MAX_N][MAX_N];   //旧的状态矩阵
char MAP[MAX_N][MAX_N];     //the map for the grid world
char A[4] = { 'n','e','s','w' };     //Action ,'↑','→','↓','←'
int dx[] = { -1,0,1,0 }, dy[]= { 0,1,0,-1 };  //四向移动
int r = -1;                  //奖励值
float disc = 1.0f;            //折扣值
float en = 0.00000001f;        //sub小于这个数时收敛


//  获取输出流的句柄
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

//初始化
void AddEndPoint(int x,int y){MAP[x][y]=OLDS[x][y]=S[x][y]='E';}//将(x,y)位置设置为终点
void AddObstcle(int x,int y){MAP[x][y]=OLDS[x][y]=S[x][y]='#';}//将(x,y)位置设置为障碍
void init() {
    for (int i = 0; i < MAX_N; i++)
        for (int j = 0; j < MAX_N; j++) {
            S[i][j] = 'n';                    //初始化为向右走
            OLDS[i][j] = 'n';
        }

        AddEndPoint(3,3);
        AddObstcle(1,1);
        AddObstcle(1,2);AddObstcle(1,3);
        AddObstcle(3,2);

      for(int i=0;i<MAX_N;i++)for(int j=0;j<MAX_N;j++)if(i+j>0){
        V[i][j]=OLDV[i][j]=INF;
      }
}

int getIndex(char ch) {
    switch (ch) {
    case 'n':
        return 0;
    case 'e':
        return 1;
    case 's':
        return 2;
    case 'w':
        return 3;
    default:
        return -1;
    }
}

void printVal() {//输出图上点到左上角的最短距离
    puts("--------------distance----------------");
    for (int i = 0; i < MAX_N; i++) {
        for (int j = 0; j < MAX_N; j++) {
            if(V[i][j]>=-16)printf(" %.0f ", -1*V[i][j]+1e-10);
            else printf("oo ");//不可达则输出无穷
        }
        puts("\n");
    }
}

int IV[MAX_N][MAX_N];
void printPath(int x,int y) {//输出左上角到x行y列一条最短路径
    {
    char outs[MAX_N][MAX_N];
    memcpy(outs,S,sizeof(S));//拷贝S数组而不是直接对S进行更改
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
}

void PolicyEvaluation(){
    float sub;
    int cnt = 0;
    do{
        sub = 0.0f;
        for (int i = 0; i < MAX_N; i++) {
            for (int j = 0; j < MAX_N; j++)if(i+j>0){

                if (S[i][j] != '#') {           //不是障碍物及终点
                    float val = V[i][j];
                    int k = getIndex(S[i][j]);       //根据状态得出移动下标
                    int x = i + dx[k], y = j + dy[k];
                    if (x >= 0 && x < MAX_N && y >= 0 && y < MAX_N && S[x][y] != '#') {
//                        V[i][j] =max(V[i][j], r +  disc * OLDV[x][y]);
                        V[i][j] =r +  disc * OLDV[x][y];
                    }
                    sub = max(sub, fabs(val - V[i][j]));
                }
            }
       }

        //把新的val值拷贝到旧的数组中
        memcpy(OLDV,V,sizeof(V));

        printf("cnt :   %d\n", ++cnt);
        printVal();

    } while (sub > en);
}

void PolicyImprovement() {

  while (true) {

    bool stable = true;
    for (int i = 0; i < MAX_N; i++) {
        for (int j = 0; j < MAX_N; j++) {
            if (OLDS[i][j] != '#' && i+j>0) {
                char oldact = OLDS[i][j];
                int k = 4;
                float ma = INF;
                while (k--) {
                    int x = i + dx[k], y = j + dy[k];
                    if (x >= 0 && x < MAX_N && y >= 0 && y < MAX_N && OLDS[x][y] != '#') {
                        float round = r + disc * OLDV[x][y];
                        if (round - ma > en) {
                            ma = round;    //改变Val值
                            S[i][j] = A[k];    //改变当前状态的最佳策略
                        }
                    }
                }

                if (oldact != S[i][j])
                    stable = false;//策略发生了变化，说明当前策略还有改进的余地
            }
        }
    }

    //将所有新状态拷贝至旧状态
    for (int i = 0; i < MAX_N; i++) {
        for (int j = 0; j < MAX_N; j++) {
            OLDS[i][j] = S[i][j];
        }
    }

    if (stable) {
       SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
       printf("\n--------------------------STOP-----------------------\n");

       printVal();
       SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xA); //亮绿
       for(int i=0;i<MAX_N;i++)for(int j=0;j<MAX_N;j++)IV[i][j]=round(V[i][j]);
       for(int i=0;i<MAX_N;i++)for(int j=0;j<MAX_N;j++)if(MAP[i][j]=='E'&&IV[i][j]>=-16)
       printPath(i,j);
       SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f); //变回原状
       break;
     }
     else {
        PolicyEvaluation();    //继续策略迭代
     }
   }
}


int main() {

    init();
    PolicyEvaluation();
    PolicyImprovement();
    return 0;
}
