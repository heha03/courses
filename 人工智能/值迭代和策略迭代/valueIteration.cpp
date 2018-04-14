#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

const int MAX_N = 4;
const float INF = -9999999.0f;
float V[MAX_N][MAX_N];      //ֵ����
float OLDV[MAX_N][MAX_N];    //����ɵ�valֵ
char S[MAX_N][MAX_N];      //״̬����ֱ�۴洢�˶�����
char A[4] = { 'n','e','s','w' };     //Action ,'��','��','��','��'
int dx[] = { -1,0,1,0 }, dy[] = { 0,1,0,-1 };  //�����ƶ�
int r = -1;                  //����ֵ
float disc = 1.0f;            //�ۿ�ֵ
float en = 0.0000001f;        //subС�������ʱ����
void AddEndPoint(int x,int y){S[x][y]='E';}//��(x,y)λ������Ϊ�յ�
void AddObstcle(int x,int y){S[x][y]='#';}//��(x,y)λ������Ϊ�ϰ�
 //��ʼ��
void init() {///S is the map,# is the �ϰ�,E is the end point
    AddObstcle(0,1);AddObstcle(0,2);AddObstcle(0,3);
    AddObstcle(1,1);AddObstcle(1,3);
    AddEndPoint(1,2);
//    S[0][1]=S[0][2]=S[0][3]='#';
//    S[2][1]=S[2][2]=S[2][3]='#';
//    S[1][1]=S[1][3]='#';
//    S[1][2]='E';
//    S[3][1] = 'E';                       //EĿ���
//    S[2][2] = '#';
//    S[1][2] = 'E';

    for(int i=0;i<MAX_N;i++)for(int j=0;j<MAX_N;j++){
        if(i==0&&j==0)OLDV[i][j]=0;//��ʼ������value functionֵΪ0
        else OLDV[i][j]=INF;
    }
}

void printVal() {//����м��valueֵ
    puts("----------------value-----------------");
    for (int i = 0; i < MAX_N; i++) {
        for (int j = 0; j < MAX_N; j++) {
            printf("%.4f ", V[i][j]);
        }
        puts("\n");
    }
}
int IV[MAX_N][MAX_N];
void output_path(int x,int y){//�����㵽s[x][y]�����·��
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
                  printf(" �� ");
                  break;
              case 'w':
                  printf("�� ");
                  break;
              case 'n':
                  printf(" �� ");
                  break;
              case 'e':
                  printf("�� ");
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
                if (S[i][j] != '#') {           //�����ϰ��Ｐ�յ�
                    float val = V[i][j];
                    int k = 4;
                    float ma = INF;
                    while (k--) {
                        int x = i + dx[k], y = j + dy[k];
                        if (x >= 0 && x < MAX_N && y >= 0 && y < MAX_N && S[x][y] != '#') {
                            float round = r + disc * OLDV[x][y];
                            if (ma < round) {
                                 ma = round;    //�ı�Valֵ
                            }
                        }
                    }
                    V[i][j] = ma;                        //V[i][j]ȡ����Χ���������ֵ

                    sub = max(sub, fabs(val - V[i][j]));
                }else V[i][j]=INF;
            }
        }

        //���µ�valֵ�������ɵ�������
        memcpy(OLDV,V,sizeof(V));
        printf("cnt :   %d\n", ++cnt);
        printVal();

    } while (sub >= en);

    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xA); //����
    puts("---------------distance---------------");
    for (int i = 0; i < MAX_N; i++) {
        for (int j = 0; j < MAX_N; j++) {
            if(V[i][j]==INF)printf("oo ");
            else printf(" %.0f ",-1* V[i][j]+1e-10);
        }
        puts("\n");
    }

    for(int i=0;i<MAX_N;i++)for(int j=0;j<MAX_N;j++)IV[i][j]=round(V[i][j]);
    //��ӡ����
    for(int i=0;i<MAX_N;i++)for(int j=0;j<MAX_N;j++)if(S[i][j]=='E'&&V[i][j]>=-16){//��ǰ��E����������ɴ�
        output_path(i,j);
    }

     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f); //���ԭ״
}

int main() {
    init();
    ValueIteration();
    return 0;
}
