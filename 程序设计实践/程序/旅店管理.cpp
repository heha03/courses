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
const char divide='-';
class Date
{
public:
    Date(int year = 1900, int month = 1, int day = 1):_year(year), _month(month), _day(day)
    {
        if (!isInvalidDate(_year, _month, _day))
        {
            _year = 1900;
            _month = 1;
            _day = 1;
        }
    }
    Date operator+(int counts)
    {
        Date tmp(*this);
        tmp._day += counts;
        ToCorrect(tmp);
        return tmp;
    }
    bool operator>(const Date &d)const
    {
        return (_year > d._year
            || (_year == d._year && _month > d._month)
            || (_year == d._year && _month == d._month && _day > d._day));
    }
    bool operator==(const Date &d)const
    {
        return ((_year == d._year) && (_month == d._month) && (_day == d._day));
    }
    bool operator<(const Date &d)const
    {
        return !((*this>d)||(*this==d));
    }
public:
    bool IsLeapYear(int year)
    {
        if(year % 400 || (year % 4 && year % 100))
            return true;
        return false;
    }
    int YearsOfMonth(int year, int month)
    {
        int day;
        int days[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        day = days[month];
        if (month == 2)
            day += IsLeapYear(year);
        return day;
    }

    Date ToCorrect(Date &d)
    {
        if (d._day>YearsOfMonth(d._year, d._month))
        {
            while (d._day > YearsOfMonth(d._year, d._month))
            {
                 d._day -= YearsOfMonth(d._year,d._month);

                if (d._month == 12)
                {
                    d._year++;
                    d._month = 1;
                }
                else
                {
                    ++d._month;
                }
            }
        }
        else
        {
            while (d._day <= 0)
            {
                if (d._month == 1)
                {
                    d._year--;
                    d._month = 12;
                }
                else
                {
                    --d._month;
                }
                d._day += YearsOfMonth(d._year, d._month);
            }
        }
        return d;
    }


    bool isInvalidDate(int year, int month, int day)
    {
        if ((year < 1) || (month<0 || month>12) || (day<0 || day>YearsOfMonth(year,month)))
            return false;
        return true;
    }
    void Display()
    {
        cout << _year << divide << _month << divide << _day << endl;
    }
    friend istream& operator>>(istream& is, Date &d);
    friend ostream& operator<<(ostream& os, const Date &d);
private:
    int _year;
    int _month;
    int _day;
};
istream& operator>>(istream& is, Date& d)
{
//    cout << "请输入一个日期" << endl;
    is >> d._year >> d._month >> d._day;
    return is;
}

ostream& operator<<(ostream& os, const Date &d)
{
    cout << d._year << divide <<d. _month << divide<< d._day << endl;
    return os;
}

struct Live{///maintain the roomInfo that is reserved
    Date d;int roomType;int level;int idx;
};
const int maxt=4;
const int maxn=1024;
const int maxl=3;
const char* typeName[maxt]={"  single room","  double room","standard room","deluxe stuite"};
const int price[maxt][maxl]={{3,2,1},{6,5,4},{9,8,7},{12,11,10}};
vector<int> rid[maxt][maxl];
bitset<maxn> reserve[maxt][maxl];
vector<Live> v;
struct Record{string customerInfo;Date dates;int type,level,period,roomId;};
vector<Record> r;
///set the room id for each type and level
void setRoom()
{
    int tot=0;
    rep(i,maxt)rep(j,maxl)rep(k,4)rid[i][j].pb(tot++);
}
int main() {
#ifdef LOCAL
    freopen("#.txt", "r", stdin);
#endif
    setRoom();
    char order[10];
    int income=0;
    cout<<"input the order :"<<endl;
    cout<<"query or update or live"<<endl;
    while(scanf("%s",order)==1)
    {
        if(order[0]=='q')///query
        {
            cout<<"In this month, we have made "<<income<<" yuan"<<endl;
            rep(i,sz(r))
            {
                Record ri=r[i];
                if(ri.level==-1)///level==-1 means this reservation is canceled
                    cout<<ri.dates<<" "<<" a reservation is canceled"<<endl;
                else{
                    cout<<ri.dates<<" "<<typeName[ri.type]<<" level "<<ri.level<<" ";
                    printf("%.05d room",ri.roomId);
                    cout<<" is reserved till "<<ri.dates+ri.period<<" by "<<ri.customerInfo<<endl;
                }
            }
        }else if(order[0]=='u')///update the month
        {
            income=0;r.clear();
        }
        else/// live
        {
            cout<<"Welcome!"<<endl;
            cout<<"What day is it today?"<<endl;
            Date now;
            cin>>now;
            ///release all the occupied resources
            vector<Live> tem=v;
            v.clear();
            rep(i,sz(tem))
            {
                Live ti=tem[i];
                if(ti.d<now)reserve[ti.roomType][ti.level].reset(ti.idx);
                else v.pb(ti);
            }

            cout<<"which type of room do you need:"<<endl;
            rep(i,maxt)cout<<i<<": "<<typeName[i]<<" ";
            cout<<endl;
//            cout<<"0: single room 1: double room 2: standard room 3: deluxe stuite"<<endl;
            int type;
            cin>>type;

            cout<<"which level of room do you need:"<<endl;
            cout<<"0: best 1: better 2: last -1: cancel"<<endl;
            int level;
            while(scanf("%d",&level)==1&&level!=-1)///level =-1 means cancel
            {
                if(reserve[type][level].count()<sz(rid[type][level]))///not all the rooms have been reserved
                {
                    cout<<"such room will cost you "<<price[type][level]<<" yuan"<<endl;
                    cout<<"yes if you are sure"<<endl;
                    scanf("%s",order);
                    if(strcmp(order,"yes")==0)break;
                }else cout<<"sorry, there's no such spare room"<<endl;
                cout<<"which level of room do you need:"<<endl;
                cout<<"0: best 1: better 2: last -1: cancel"<<endl;
            }

            if(level==-1)///customer cancels the reservation
            {
                r.pb(((Record){"",now,type,level,0,-1}));
            }else
            {
                cout<<"how many days do you want to live:"<<endl;
                int period;cin>>period;
                cout<<"perfect your information"<<endl;
                string customerInfo;
//                getline(cin,customerInfo);
                getchar();///read in the '\n'
                getline(cin,customerInfo);
                Live newOne;
                newOne.d=now+period;newOne.roomType=type;newOne.level=level;
                ///find the room id which has not been reserved
                rep(i,sz(rid[type][level]))if(!reserve[type][level][i])
                {reserve[type][level][i].flip(),newOne.idx=i;break;}
                v.pb(newOne);
                cout<<"reservation succeeds!\nyour room id is "<<rid[type][level][newOne.idx]<<endl;

                income+=price[type][level]*period;
                r.pb(((Record){customerInfo,now,type,level,period,rid[type][level][newOne.idx]}));
            }
        }
        cout<<"input the order :"<<endl;
        cout<<"query or update or live"<<endl;
    }
    return 0;
}

