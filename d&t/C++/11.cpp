#include <string>
#include <iostream>
#include <stdio.h>
using namespace std;

/*
char * str_rem(char *str1, char *str2)
{
	int l1, l2;
	l1 = strlen(str1);
	l2 = strlen(str2);
	for (int i = 0; i < l1; i++)
	{
		char* tmpc[l]
		tmpc = str1[i]
	}
}
*/
string str_rem(string str1, string str2)
{
	int l1, l2;
	l1 = str1.length();
	l2 = str2.length();
	string res[l1];
	int idx = 0;
	for (int i = 0; i <= l1-l2; i++)
	{
		string tmps = str1.substr(i,l2)
		if (tmps == str2):
		{
			i = i + l2 - 1;

		}
		else
		{
			res[idx] = str1[i]
			idx++;
		}
	}
	return res;
}

int main()
{
	stirng a = "abcdefasfawe";
	string b = "cd";
	string c;
	c = str_rem(a, b);
	cout << c << endl;
	return 0;
}