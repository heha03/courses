//#define LOCAL
#include <bits/stdc++.h>
#define mem(a) memset(a,0,sizeof(a))
#define mem1(a) memset(a,-1,sizeof(a))
#define pb(x) push_back(x)
#define rep(i,n) for(int i=0;i<n;i++)
#define rep2(i,l,r) for(int i=l;i<=r;i++)
#define dec(i,r,l) for(int i=r;i>=l;i--)
#define sz(x) ((int)(x).size())
const double PI=acos(-1.0);
const double e=2.7182818284590452353602874713527;
using namespace std;
const double eps=1e-12;
int dcmp(double x){
    if(fabs(x)<=eps)return 0;return x>0? 1:-1;
}
const int maxn=1024;
int levelIn[255],levlOut[255];
void setLevel()
{
    levelIn['#']=-1;levlOut['#']=-1;
    levelIn['^']= 6;levlOut['^']= 5;
    levelIn['*']= 4;levlOut['*']= 3;
    levelIn['%']= 4;levlOut['%']= 3;
    levelIn['/']= 4;levlOut['/']= 3;
    levelIn['+']= 2;levlOut['+']= 1;
    levelIn['-']= 2;levlOut['-']= 1;
    levelIn['(']= 0;levlOut['(']= 7;
    levelIn[')']= 7;levlOut[')']= 0;

    levelIn['t']= 0;levlOut['t']= 7;
    levelIn['s']= 0;levlOut['s']= 7;
    levelIn['c']= 0;levlOut['c']= 7;
    levelIn['T']= 0;levlOut['T']= 7;
    levelIn['S']= 0;levlOut['S']= 7;
    levelIn['C']= 0;levlOut['C']= 7;
    levelIn['l']= 0;levlOut['l']= 7;
}
char in[maxn];
bool isNum(char c){return isdigit(c)||c=='.';}
bool oneOperator(char c){return c=='t'||c=='s'||c=='c'||c=='T'||c=='S'||c=='C'||c=='l';}
map<string,char> M;
void setMap()
{
    M["tan"]='t';M["atan"]='T';
    M["sin"]='s';M["asin"]='S';
    M["cos"]='c';M["acos"]='C';
    M["log"]='l';
}
void solveIn()
{
    in[0]='(';///(-1-3=>0-1-3
    int len=strlen(in);
    int i=1;
    string s="(";
    while(i<len)
    {
        if(in[i]=='-'&&in[i-1]=='(')s+='0',s+=in[i++];
        else s+=in[i++];///must use english (
    }
    memcpy(in,s.c_str(),sz(s));
    in[sz(s)]='\0';
//    cout<<s<<endl;cout<<in<<endl;
}
int main() {
#ifdef LOCAL
    freopen("#1.txt", "r", stdin);
#endif
    setLevel();
    setMap();
    while(scanf("%s",in+1)==1)
    {
        solveIn();
        int len=strlen(in);
        in[len++]='#';
        stack<double> opd;stack<char> opc;
        opc.push('#');
        int i=1;
        while(!(in[i]=='#'&&opc.top()=='#'))
        {
            char c=in[i];printf("i: %d ini: %c\n",i,in[i]);
            if(c=='p')opd.push(PI),i++;
            else if(c=='e')opd.push(e),i++;
            else if(isNum(c))
            {
                double num= isdigit(c)? c-'0':0;
                i++;
                while(isdigit(in[i]))num=num*10+in[i++]-'0';
                if(in[i]=='.')i++;
                double ten=0.1;
                while(isdigit(in[i]))num+=(in[i++]-'0')*ten,ten/=10;
                opd.push(num);
            }else
            {
                int ti;
                if(isalpha(c))
                {
                    ti=i;string ml="";
                    while(isalpha(in[ti]))ml+=in[ti++];
                    c=M[ml];printf("%c\n",c);
                }
                while(true)
                {
                    char cTop=opc.top();
                    if(cTop=='#'&&c=='#')break;
                    if(levelIn[cTop]==levlOut[c]){
                        i++;opc.pop();
                        while(oneOperator(opc.top()))
                        {
                            char cTop=opc.top();opc.pop();
                            double dTop=opd.top();opd.pop();
                            switch(cTop)
                            {
                                case 't':dTop=tan(dTop);break;
                                case 's':dTop=sin(dTop);break;
                                case 'c':dTop=cos(dTop);break;
                                case 'T':dTop=atan(dTop);break;
                                case 'S':dTop=asin(dTop);break;
                                case 'C':dTop=acos(dTop);break;
                                case 'l':if(dTop<=0){printf("the parameter of the function must be positive.\n");return 0;}
                                dTop=log(dTop);break;
                            }
                            opd.push(dTop);
                        }
                        break;
                    }
                    else if(levelIn[cTop]<levlOut[c]){
                        if(!isalpha(c))i++,opc.push(c);else i=ti,opc.push(c);
                        break;
                    }
                    else
                    {
                        opc.pop();
                        double num2=opd.top();opd.pop();
                        double num1=opd.top();opd.pop();
                        switch(cTop)
                        {
                            case '+':opd.push(num1+num2);break;
                            case '-':opd.push(num1-num2);break;
                            case '*':opd.push(num1*num2);break;
                            case '^':opd.push(pow(num1,num2));break;
                            case '%':
                                if(dcmp(num2)==0){printf("divisor can't be zero!\n");return 0;}
                                opd.push(num1-(int)(num1/num2)*num2);break;
                            case '/':
                                if(dcmp(num2)==0){printf("divisor can't be zero!\n");return 0;}
                                opd.push(num1/num2);break;
                        }
                    }
                }
            }
        }
        printf("%.5f\n",opd.top());
    }
    return 0;
}
