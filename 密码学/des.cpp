#include <bits/stdc++.h>
using namespace std;
const char sixteen[]="0123456789ABCDEF";
//输出bitset的十六进制表示
void outputBy16(bitset<64> b)
{
    int v[16];
    for(int i=0;i<16;i++)
    {
        int tv=0;
        for(int j=0;j<4;j++){
            tv*=2;
            if(b[i*4+3-j])tv++;
        }
        v[15-i]=tv;
    }
    for(int i=0;i<16;i++)cout<<sixteen[v[i]];
    cout<<endl;
}
//ip置换
const int ip[] = {
58, 50, 42, 34, 26, 18, 10, 2,
60, 52, 44, 36, 28, 20, 12, 4,
62, 54, 46, 38, 30, 22, 14, 6,
64, 56, 48, 40, 32, 24, 16, 8,
57, 49, 41, 33, 25, 17,  9, 1,
59, 51, 43, 35, 27, 19, 11, 3,
61, 53, 45, 37, 29, 21, 13, 5,
63, 55, 47, 39, 31, 23, 15, 7,
};
//ip逆置换
const int ip_1[] = {
40, 8, 48, 16, 56, 24, 64, 32,
39, 7, 47, 15, 55, 23, 63, 31,
38, 6, 46, 14, 54, 22, 62, 30,
37, 5, 45, 13, 53, 21, 61, 29,
36, 4, 44, 12, 52, 20, 60, 28,
35, 3, 43, 11, 51, 19, 59, 27,
34, 2, 42, 10, 50, 18, 58, 26,
33, 1, 41,  9, 49, 17, 57, 25,
};

//密钥置换表，64位密钥变成56位
const int pc1[] = {
57, 49, 41, 33, 25, 17, 9,
1, 58, 50, 42, 34, 26, 18,
10,  2, 59, 51, 43, 35, 27,
19, 11,  3, 60, 52, 44, 36,
63, 55, 47, 39, 31, 23, 15,
7, 62, 54, 46, 38, 30, 22,
14,  6, 61, 53, 45, 37, 29,
21, 13,  5, 28, 20, 12,  4,
};

//压缩置换，将56位密钥变48位子密钥
const int pc2[] = {
14, 17, 11, 24,  1,  5,
3, 28, 15,  6, 21, 10,
23, 19, 12,  4, 26,  8,
16,  7, 27, 20, 13,  2,
41, 52, 31, 37, 47, 55,
30, 40, 51, 45, 33, 48,
44, 49, 39, 56, 34, 53,
46, 42, 50, 36, 29, 32,
};

//每轮左移的位数
const int shiftBits[] = {1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1};

//S盒，每个S盒是4x16的置换表，6位 -> 4位
const int sbox[8][4][16] = {
    {
        {14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
        {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
        {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
        {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}
    },
    {
        {15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
        {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
        {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
        {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}
    },
    {
        {10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
        {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
        {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
        {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}
    },
    {
        {7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
        {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
        {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
        {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}
    },
    {
        {2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
        {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
        {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
        {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}
    },
    {
        {12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
        {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
        {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
        {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}
    },
    {
        {4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
        {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
        {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
        {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}
    },
    {
        {13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
        {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
        {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
        {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}
    }
};

//P置换
const int p[] = {
16,  7, 20, 21,
29, 12, 28, 17,
1, 15, 23, 26,
5, 18, 31, 10,
2,  8, 24, 14,
32, 27,  3,  9,
19, 13, 30,  6,
22, 11,  4, 25,
};


//密码函数f，使用子密钥加密右半部分
 //扩展置换表，将32位扩展至48位
const int E[] = {
32,  1,  2,  3,  4,  5,
4,  5,  6,  7,  8,  9,
8,  9, 10, 11, 12, 13,
12, 13, 14, 15, 16, 17,
16, 17, 18, 19, 20, 21,
20, 21, 22, 23, 24, 25,
24, 25, 26, 27, 28, 29,
28, 29, 30, 31, 32,  1,
};
bitset<32> f(bitset<32> righs, bitset<48> subk)
{
    //第一步：扩展右半部分
    bitset<48> expandr;
    for(int i=0;i<48;++i)expandr[47-i]=righs[32-E[i]];
    //第二步：异或
    expandr=expandr^subk;
    //第三步：使用S盒进行映射
    bitset<32> ret;
    int x=0;
    for(int i=0;i<48;i=i+6)
    {
        int row=expandr[47-i]*2+expandr[47-i-5];
        int col=0;
        for(int j=0;j<4;j++)col+=expandr[47-i-4+j]*(1<<j);
        int num=sbox[i/6][row][col];
        for(int j=0;j<4;j++)
        {
            if(num&1)ret[31-x-3+j]=1;else ret[31-x-3+j]=0;
            num/=2;
        }
        x+=4;
    }
    //第四步：P置换
    bitset<32> tmp=ret;
    for(int i=0;i<32;i++)ret[31-i]=tmp[32-p[i]];
    return ret;
}

//对28位密钥(L或R)进行循环左移
bitset<28> leftShift(bitset<28> k, int shift)
{
    bitset<28> temp=k;
    for(int i=27;i>=0;i--)k[i]=temp[(i-shift+28)%28];
    return k;
}

 //生成16个48位的子密钥
 bitset<48> subKey[16];
void generateKeys(bitset<64> pkey)
{
    bitset<56> key;
    bitset<48> tkey;
    //去掉奇偶标记位，将64位密钥变成56位
    for (int i=0;i<56;i++)key[55-i]=pkey[64-pc1[i]];
    //生成子密钥
    for(int i=0;i<16;i++)
    {
         //拆成前28位与后28位
        bitset<28> lefts,righs;
        for(int j=28;j<56;j++)lefts[j-28]=key[j];
        for(int j=0;j<28;j++)righs[j]=key[j];
        //左移
        lefts=leftShift(lefts,shiftBits[i]);
        righs=leftShift(righs,shiftBits[i]);
        //置换，由56位得到48位子密钥
        for(int j=28;j<56;j++)key[j]=lefts[j-28];
        for(int j=0;j<28;j++)key[j]=righs[j];
        for(int j=0;j<48;j++)tkey[47-j]=key[56-pc2[j]];
        subKey[i]=tkey;
    }
}
//DES加密
bitset<64> encrypt(bitset<64> plain)
{
    //第一步：IP置换
    {
        bitset<64> temp=plain;
        for(int i=0;i<64;i++)plain[63-i]=temp[64-ip[i]];
    }

    //第二步：获取左半部分和右半部分
    bitset<32> lefts,righs;
    for(int i=32;i<64;i++)lefts[i-32]=plain[i];
    for(int i=0;i<32;i++)righs[i]=plain[i];

    //第三步：16轮迭代
    for(int i=0;i<16;i++)
    {
        bitset<32> temp=lefts^f(righs,subKey[i]);
        swap(lefts,righs);
        righs=temp;
    }

    //第四步：合并L16和R16为 R16L16
    bitset<64> cipher;
    for(int i=0;i<32;i++)cipher[i]=lefts[i];
    for(int i=32;i<64;i++)cipher[i]=righs[i-32];

    //第五步：IP逆置换
    {
        bitset<64> temp=cipher;
        for(int i=0;i<64;i++)cipher[63-i]=temp[64-ip_1[i]];
    }
    return cipher;
}
string getRandomKey()
{
    string ret="";
    for(int i=0;i<64;i++)
    {
        int getRan=rand();
        if(getRan&1)ret+='1';else ret+='0';
    }return ret;
}
//测试我的DES实现的正确性
void testTrue()
{
    string s = "0000000100100011010001010110011110001001101010111100110111101111";
    string k = "0001001000110100010101100111100010011010101111001101111011110000";
    bitset<64> plain=bitset<64>(s);
    bitset<64> key=bitset<64>(k);

    generateKeys(key);

    bitset<64> cipher=encrypt(plain);
    cout<<"the right answer is "<<"85E813540F0AB405"<<endl;
    cout<<"my design answer is ";
    outputBy16(cipher);
}
void test1(string s,string k)
{
    bitset<64> plain=bitset<64>(s);
    bitset<64> key=bitset<64>(k);
    //生成16个子密钥
    generateKeys(key);
    //密文写入 a.txt
    bitset<64> cipher=encrypt(plain);
    cout<<"the ciphertext1 is ";
    outputBy16(cipher);
    cout<<"after changing one position"<<endl;
    plain[3].flip();
    bitset<64> cipher2=encrypt(plain);
    cout<<"the ciphertext2 is ";
    outputBy16(cipher2);
    bitset<64> dif=cipher2^cipher;
    cout<<"there are "<<dif.count()<<"positions differ"<<endl;
}
vector<bitset<64> > test_encrypt(bitset<64> plain)
{
    //第一步：初始置换IP
    {
        bitset<64> temp=plain;
        for(int i=0;i<64;i++)plain[63-i]=temp[64-ip[i]];
    }

    //第二步：获取 Li 和 Ri
    bitset<32> lefts,righs;
    for(int i=32;i<64;i++)lefts[i-32]=plain[i];
    for(int i=0;i<32;i++)righs[i]=plain[i];

    //第三步：共16轮迭代
    vector<bitset<64> >v;
    for(int i=0;i<16;i++)
    {
        bitset<32> temp=lefts^f(righs,subKey[i]);
        swap(lefts,righs);
        righs=temp;

        bitset<64> cipher;
        for(int i=0;i<32;i++)cipher[i]=lefts[i];
        for(int i=32;i<64;i++)cipher[i]=righs[i-32];
        v.push_back(cipher);
    }

    //第四步：合并L16和R16，注意合并为 R16L16
    bitset<64> cipher;
    for(int i=0;i<32;i++)cipher[i]=lefts[i];
    for(int i=32;i<64;i++)cipher[i]=righs[i-32];

    //第五步：结尾置换IP-1
    {
        bitset<64> temp=cipher;
        for(int i=0;i<64;i++)cipher[63-i]=temp[64-ip_1[i]];
    }
    return v;
}
vector<int> test2(string s,string k,int pos,bool outRes)
{
    bitset<64> plain=bitset<64>(s);
    bitset<64> key=bitset<64>(k);

    generateKeys(key);
    vector<bitset<64> >cipher=test_encrypt(plain);
    plain[pos].flip();
    vector<bitset<64> >cipher2=test_encrypt(plain);
    vector<int> eij;
    for(int i=0;i<16;i++)
    {
        bitset<64> dif=cipher2[i]^cipher[i];
//       cout<<"there are "<<dif.count()<<"positions differ"<<endl;
        eij.push_back(dif.count());
    }

    if(outRes)for(int i=0;i<16;i++)if(fabs(eij[i]-32)<=1){cout<<"the min i that satisfy when flipping the "<<pos<<" position is "<<i<<endl;break;}
    return eij;
}
void test3(string s,string k)
{
    vector<int> e[64];
    for(int j=0;j<64;j++)e[j]=test2(s,k,j,false);
    double avg[16];
    for(int i=0;i<16;i++){
        avg[i]=0;
        for(int j=0;j<64;j++)avg[i]+=e[j][i];
        avg[i]/=64;
    }

    for(int i=0;i<16;i++)if(fabs(avg[i]-32)<=1){cout<<"the min i that is closed to 32 in average situation is "<<i<<endl;break;}
}
int main() {
    testTrue();
    srand((unsigned)time(NULL));
    string s=getRandomKey(),k=getRandomKey();
    cout<<"In this run :"<<endl;
    cout<<"the plaintext is "<<s<<endl;
    cout<<"the key       is "<<k<<endl;
    test1(s,k);
    test2(s,k,3,true);
    test3(s,k);
    return 0;
}
