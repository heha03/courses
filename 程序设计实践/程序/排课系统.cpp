//#define LOCAL
#include <bits/stdc++.h>
#define mem(a) memset(a,0,sizeof(a))
#define mem1(a) memset(a,-1,sizeof(a))
#define pb(x) push_back(x)
#define rep(i,n) for(int i=0;i<n;i++)
#define rep2(i,l,r) for(int i=l;i<=r;i++)
#define dec(i,r,l) for(int i=r;i>=l;i--)
#define sz(x) ((int)(x).size())
const long double PI=acosl((long double)-1.0);
using namespace std;
typedef pair<int,int> pii;
const int maxRoom=1024;
const int maxRoomType=3;
const int roomCapacity[maxRoomType]={120,240,360};
int rn;
string rName[maxRoom];
map<string,int> rid;
vector<int> room[maxRoomType];
const int maxCourse=1024;
int cn;
string cName[maxCourse];
map<string,int> cid;
const int maxTeacher=1024;
int tn;
string tName[maxTeacher];
map<string,int> tid;
const int maxGrade=1024;
int gn;
string gName[maxGrade];
map<string,int> gid;
int leastR[maxGrade];
void getPoint(){
    ///setRoom
    cout<<"input the number of the rooms and each one's name and type"<<endl;
    scanf("%d",&rn);
    rep(i,rn)
    {
        int type;
        cin>>rName[i]>>type;rid[rName[i]]=i;
        room[type].pb(i);
    }
    dec(i,maxRoomType-2,0)
        rep(j,sz(room[i+1]))room[i].pb(room[i+1][j]);

    ///setCourse
    cout<<"input the number of the course and each one's name"<<endl;
    scanf("%d",&cn);
    rep(i,cn)cin>>cName[i],cid[cName[i]]=i;
    ///setTeacher
    cout<<"input the number of the teacher and each one's name"<<endl;
    scanf("%d",&tn);
    rep(i,tn)cin>>tName[i],tid[tName[i]]=i;
///    ///setGrade
///    cout<<"input the number of the grade and each one's name and the number of students in it"<<endl;
///    scanf("%d",&gn);
///    rep(i,gn)
///    {
///        cin>>gName[i],gid[gName[i]]=i;
///        int cntStudents;cin>>cntStudents;
///        int j=0;
///        while(roomCapacity[j]<cntStudents)j++;
///        leastR[i]=j;
///    }
}
vector<int> ct[maxCourse];
vector<pii> cg;
const int maxcg=1024*2;
bool isDouble[maxcg];
void teachCourse()
{
    ///for each course ci the name of teachers who can teach it
    cout<<"input the teachers who are due to teach each course"<<endl;
    rep(i,cn)
    {
        int ctSize;scanf("%d",&ctSize);
        while(ctSize--)
        {
            string teacher;cin>>teacher;ct[i].pb(tid[teacher]);
        }
    }
}
struct Topo
{
    const static int maxn=1024;
    int n,m;
    string cname[maxn];
    int cTime[maxn];
    map<string,int> M;
    int inDegree[maxn];
    vector<int> edges[maxn];
    vector<int> plan[4];
    void init()
    {
        M.clear();mem(inDegree);
        rep2(i,0,n)edges[i].clear();
        rep(i,4)plan[i].clear();
    }
    void buildCG(int g0)
    {
        rep(pi,4)
        {
            rep(i,sz(plan[pi]))
            {
                int course=cid[cname[plan[pi][i]]];
                cg.pb(pii(course,g0+pi));
                if(cTime[plan[pi][i]]==2)isDouble[sz(cg)-1]=true;
            }
        }
    }
    bool ableArrange(int g0)
    {
        cout<<"input the number of courses and the number of the first_second relationship"<<endl;
        cin>>n>>m;
        cout<<"input each name and period"<<endl;
        init();
        rep(i,n)
        {
            cin>>cname[i]>>cTime[i];
            M[cname[i]]=i;
        }
        cout<<"input the first-learned course and the next-learned course"<<endl;
        while(m--)
        {
            string su,sv;
            cin>>su>>sv;
            edges[M[su]].pb(M[sv]);
            inDegree[M[sv]]++;
        }

        queue<int> now,nex;
        rep(i,n)if(inDegree[i]==0)now.push(i);
        rep(pi,4)
        {
            while(!now.empty())
            {
                int x=now.front();now.pop();
                plan[pi].pb(x);
                rep(i,sz(edges[x]))
                {
                    int v=edges[x][i];
                    inDegree[v]--;
                    if(inDegree[v]==0)nex.push(v);
                }
            }
            while(!nex.empty())now.push(nex.front()),nex.pop();
        }

        int cntPlanned=0;
        rep(pi,4)cntPlanned+=sz(plan[pi]);
        if(cntPlanned!=n)return false;

        buildCG(g0);
        return true;
    }
}to;
bool arrangeGrade()
{
    cout<<"input the number of major in the college"<<endl;
    int mn;cin>>mn;
    bool canArrange=true;
    gn=0;
    while(mn--)
    {
        cout<<"input its major name,then follows the number of students of each grade"<<endl;
        string majorName;
        cin>>majorName;
        rep(i,4)
        {
            gName[gn]=majorName+"201"+(char)('4'+i);
            int cntStudents;cin>>cntStudents;
            int j=0;
            while(roomCapacity[j]<cntStudents)j++;
            leastR[gn]=j;
            gn++;
        }
        if(!to.ableArrange(gn-4)){canArrange=false;cout<<"the plan of "<<majorName<<" has some problem"<<endl;}
    }

    rep(i,gn)gid[gName[i]]=i;
    return canArrange;
}
//void buildEdge()
//{
//    ///for each course ci the grade of students who need to choose it
//    cout<<"input the grades who are due to learn each course and the period"<<endl;
//    rep(i,cn)
//    {
//        int cgSize;scanf("%d",&cgSize);
//        while(cgSize--)
//        {
//            string grade;cin>>grade;cg.pb(pii(i,gid[grade]));
//            int classTime;cin>>classTime;if(classTime==2)isDouble[sz(cg)-1]=true;
//        }
//    }
//}
const int maxWeekDay=5;
const int maxLesson=5;
struct Node{int tea,roo,gcid;};
vector<Node> table[maxWeekDay][maxLesson];
set<int> teaBusy[maxWeekDay][maxLesson];
bool teacherFree(int teacher,int day,int les){return !teaBusy[day][les].count(teacher);}
void addTeacher(int teacher,int day,int les){teaBusy[day][les].insert(teacher);}
void cancleTeacher(int teacher,int day,int les){teaBusy[day][les].erase(teacher);}

set<int> graBusy[maxWeekDay][maxLesson];
bool gradeFree(int grade,int day,int les){return !graBusy[day][les].count(grade);}
void addGrade(int grade,int day,int les){graBusy[day][les].insert(grade);}
void cancleGrade(int grade,int day,int les){graBusy[day][les].erase(grade);}

set<int> rooBusy[maxWeekDay][maxLesson];
int FindRoom(int least,int day,int les)
{
    rep(i,sz(room[least]))if(!rooBusy[day][les].count(room[least][i]))return room[least][i];
    return -1;
}
void addRoom(int roomAdd,int day,int les){rooBusy[day][les].insert(roomAdd);}
void cancleRoom(int roomAdd,int day,int les){rooBusy[day][les].erase(roomAdd);}
void getConstraint()
{
    ///some classroom/teacher/grade and major is busy in some time,you can add it before dfs
    cout<<"does some teacher have no time?"<<endl;
    string teacherName;
    while(cin>>teacherName&&teacherName!="no")
    {
        cout<<"input the number of such time"<<endl;
        int cntTime;cin>>cntTime;
        while(cntTime--)
        {
            int day,les;cin>>day>>les;addTeacher(tid[teacherName],day,les);
        }
    }

    cout<<"does some classroom have no time?"<<endl;
    string roomName;
    while(cin>>roomName&&roomName!="no")
    {
        cout<<"input the number of such time"<<endl;
        int cntTime;cin>>cntTime;
        while(cntTime--)
        {
            int day,les;cin>>day>>les;addRoom(rid[roomName],day,les);
        }
    }

    cout<<"does some major in some grade have no time?"<<endl;
    string majorGrade;
    while(cin>>majorGrade&&majorGrade!="no")
    {
        cout<<"input the number of such time"<<endl;
        int cntTime;cin>>cntTime;
        while(cntTime--)
        {
            int day,les;cin>>day>>les;addGrade(gid[majorGrade],day,les);
        }
    }
}
void outputNode(const Node& rhs)
{
    cout<<tName[rhs.tea]<<'-'<<gName[cg[rhs.gcid].second]<<'-'<<rName[rhs.roo]<<'-'<<cName[cg[rhs.gcid].first]<<endl;
}
void outputDay(int day)
{
    printf("The %dth day:\n",day);
    rep(i,maxLesson)
    {
        printf("the %dth lesson:\n",i);
        rep(j,sz(table[day][i]))outputNode(table[day][i][j]);
    }
}

bool dfs(int cgi)
{
    if(cgi==sz(cg))return true;
    pii t=cg[cgi];
    int course=t.first;int grade=t.second;
    if(!isDouble[cgi]){
    rep(les,maxLesson)rep(day,maxWeekDay)if(gradeFree(grade,day,les))
    {
        addGrade(grade,day,les);
        ///find a room
        int least=leastR[grade];
        int chooseRoom=FindRoom(least,day,les);
        if(chooseRoom==-1){cancleGrade(grade,day,les);return false;}
        addRoom(chooseRoom,day,les);

        rep(i,sz(ct[course]))
        {
            int teacher=ct[course][i];
            if(teacherFree(teacher,day,les))
            {
                addTeacher(teacher,day,les);

                if(dfs(cgi+1))
                {
                    table[day][les].pb(((Node){teacher,chooseRoom,cgi}));
                    return true;
                }

                cancleTeacher(teacher,day,les);
            }
        }

        cancleRoom(chooseRoom,day,les);
        cancleGrade(grade,day,les);
    }return false;
    }
    else
    {
        rep(i,sz(ct[course]))
        {
            int teacher=ct[course][i];
            dec(day1,maxWeekDay-1,2)dec(day0,day1-2,0)
            {
                dec(les1,maxLesson-1,0)if(teacherFree(teacher,day1,les1)&&gradeFree(grade,day1,les1)&&FindRoom(leastR[grade],day1,les1)!=-1){
                int room1=FindRoom(leastR[grade],day1,les1);
                addGrade(grade,day1,les1);addTeacher(teacher,day1,les1);addRoom(room1,day1,les1);

                dec(les0,maxLesson-1,0)if(teacherFree(teacher,day0,les0)&&gradeFree(grade,day0,les0)&&FindRoom(leastR[grade],day1,les1)!=-1)
                {
                    int room0=FindRoom(leastR[grade],day0,les0);
                    addGrade(grade,day0,les0);addTeacher(teacher,day0,les0);addRoom(room0,day0,les0);

                    if(dfs(cgi+1))
                    {
                        table[day0][les0].pb(((Node){teacher,room0,cgi}));
                        table[day1][les1].pb(((Node){teacher,room1,cgi}));
                        return true;
                    }

                    cancleGrade(grade,day0,les0);cancleTeacher(teacher,day0,les0);cancleRoom(room0,day0,les0);
                }

                    cancleGrade(grade,day1,les1);cancleTeacher(teacher,day1,les1);cancleRoom(room1,day1,les1);
                }
            }
        }return false;
    }
}
int main() {
#ifdef LOCAL
    freopen("#5.txt", "r", stdin);
#endif
    getPoint();teachCourse();
    if(!arrangeGrade())return 0;
    getConstraint();
    if(dfs(0))
    rep(day,maxWeekDay)outputDay(day);
    else printf("such table doesn't exist.\n");
//    getConstraint();
    return 0;
}

