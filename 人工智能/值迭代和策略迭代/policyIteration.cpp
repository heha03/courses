#include <bits/stdc++.h>
#include <windows.h>
using namespace std;

const int MAX_N = 4;
const float INF = -1000000000.0f;
float V[MAX_N][MAX_N];      //ֵ����
float OLDV[MAX_N][MAX_N];    //����ɵ�valֵ
char S[MAX_N][MAX_N];      //״̬����ֱ�۴洢�˶�����
char OLDS[MAX_N][MAX_N];   //�ɵ�״̬����
char MAP[MAX_N][MAX_N];     //the map for the grid world
char A[4] = { 'n','e','s','w' };     //Action ,'��','��','��','��'
int dx[] = { -1,0,1,0 }, dy[]= { 0,1,0,-1 };  //�����ƶ�
int r = -1;                  //����ֵ
float disc = 1.0f;            //�ۿ�ֵ
float en = 0.00000001f;        //subС�������ʱ����


//  ��ȡ������ľ��
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

//��ʼ��
void AddEndPoint(int x,int y){MAP[x][y]=OLDS[x][y]=S[x][y]='E';}//��(x,y)λ������Ϊ�յ�
void AddObstcle(int x,int y){MAP[x][y]=OLDS[x][y]=S[x][y]='#';}//��(x,y)λ������Ϊ�ϰ�
void init() {
    for (int i = 0; i < MAX_N; i++)
        for (int j = 0; j < MAX_N; j++) {
            S[i][j] = 'n';                    //��ʼ��Ϊ������
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

void printVal() {//���ͼ�ϵ㵽���Ͻǵ���̾���
    puts("--------------distance----------------");
    for (int i = 0; i < MAX_N; i++) {
        for (int j = 0; j < MAX_N; j++) {
            if(V[i][j]>=-16)printf(" %.0f ", -1*V[i][j]+1e-10);
            else printf("oo ");//���ɴ����������
        }
        puts("\n");
    }
}

int IV[MAX_N][MAX_N];
void printPath(int x,int y) {//������Ͻǵ�x��y��һ�����·��
    {
    char outs[MAX_N][MAX_N];
    memcpy(outs,S,sizeof(S));//����S���������ֱ�Ӷ�S���и���
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
}

void PolicyEvaluation(){
    float sub;
    int cnt = 0;
    do{
        sub = 0.0f;
        for (int i = 0; i < MAX_N; i++) {
            for (int j = 0; j < MAX_N; j++)if(i+j>0){

                if (S[i][j] != '#') {           //�����ϰ��Ｐ�յ�
                    float val = V[i][j];
                    int k = getIndex(S[i][j]);       //����״̬�ó��ƶ��±�
                    int x = i + dx[k], y = j + dy[k];
                    if (x >= 0 && x < MAX_N && y >= 0 && y < MAX_N && S[x][y] != '#') {
//                        V[i][j] =max(V[i][j], r +  disc * OLDV[x][y]);
                        V[i][j] =r +  disc * OLDV[x][y];
                    }
                    sub = max(sub, fabs(val - V[i][j]));
                }
            }
       }

        //���µ�valֵ�������ɵ�������
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
                            ma = round;    //�ı�Valֵ
                            S[i][j] = A[k];    //�ı䵱ǰ״̬����Ѳ���
                        }
                    }
                }

                if (oldact != S[i][j])
                    stable = false;//���Է����˱仯��˵����ǰ���Ի��иĽ������
            }
        }
    }

    //��������״̬��������״̬
    for (int i = 0; i < MAX_N; i++) {
        for (int j = 0; j < MAX_N; j++) {
            OLDS[i][j] = S[i][j];
        }
    }

    if (stable) {
       SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
       printf("\n--------------------------STOP-----------------------\n");

       printVal();
       SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0xA); //����
       for(int i=0;i<MAX_N;i++)for(int j=0;j<MAX_N;j++)IV[i][j]=round(V[i][j]);
       for(int i=0;i<MAX_N;i++)for(int j=0;j<MAX_N;j++)if(MAP[i][j]=='E'&&IV[i][j]>=-16)
       printPath(i,j);
       SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 0x0f); //���ԭ״
       break;
     }
     else {
        PolicyEvaluation();    //�������Ե���
     }
   }
}


int main() {

    init();
    PolicyEvaluation();
    PolicyImprovement();
    return 0;
}
