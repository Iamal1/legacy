
#include<iostream>
#include <stdio.h>
#include <cmath>
#include <cstdlib>
using namespace std;

int main()
{
///
    double a= 200;
    char b = char(a);
    a=(int)b;
    cout<<a<<endl;

    ///
    a=200;
    unsigned char d=( unsigned char)a;
    int c = (int) d;
    //printf("%x\n",&b);
    printf("%c\n",d);
    printf("%d\n",c);
    static double k;k+=rand ()/1./RAND_MAX;
    int l=k;l%=512;
    cout << l << endl;
    return 0;

}
