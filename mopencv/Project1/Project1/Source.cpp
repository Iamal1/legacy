#include <string>
#include <iostream>
#include <stdio.h>
using namespace std;

string str_rem(string str1, string str2)
{
	int l1, l2;
	l1 = str1.length();
	l2 = str2.length();
	string res="";
	for (int i = 0; i <= l1 - l2; i++)
	{
		string tmps = str1.substr(i, l2);
		if (tmps == str2)
			i = i + l2 - 1;
		else
			res += str1[i];
	}
	return res;
}
int main()
{
	string a = "abcfhgbcrbctbcbc";
	string b = "bc";
	string c;
	c = str_rem(a, b);
	cout << c << endl;
	system("pause");
	return 0;
}