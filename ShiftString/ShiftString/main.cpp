
#include <iostream>
#include <string>

using namespace std;


void shift1(string &str, int n)
{
	const int len = str.length();
	string tmp = str;
	for (int i=0; i < str.length(); ++i)
	{
		int idx = i+len;
		idx = (idx-n) % len;
		tmp[ idx] = str[ i];
	}
	str = tmp;
}


void shift2(string &str, int n)
{
	if (str.empty())
		return;

	for (int i=0; i < n; ++i)
	{
		char c = str[ 0];
		for (int i=1; i < str.length(); ++i)
		{
			str[ i-1] = str[ i];
		}
		str[ str.length()-1] = c;
	}
}


void shift3(string &str, int n)
{
	const int len = str.length();

	int src = 0;
	int dst = len - n;
	int cnt = 0;
	
	int i=1;
	char tmp = str[ src];
	while (cnt < len)
	{
		char tmp2 = str[ dst];
		str[ dst] = tmp;
		tmp = tmp2;

		if (0 == dst)
		{
			src = i++;
			dst = src - n;
			dst += len;
			dst %= len;
			tmp = str[ src];
		}
		else
		{
			src = dst;
			dst -= n;
			if (dst < 0)
			{
				dst += len;
				//dst %= len;
			}
		}
		++cnt;
	}
}


void main()
{
	string str1 = "abcde";
	shift1(str1, 3);
	cout << str1.c_str() << endl;

	string str2 = "abcde";
	shift2(str2, 3);
	cout << str2.c_str() << endl;
	
	string str3 = "abcdefg";
	shift3(str3, 3);
	cout << str3.c_str() << endl;
	
}
