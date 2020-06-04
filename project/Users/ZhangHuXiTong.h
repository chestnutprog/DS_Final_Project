#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
struct user{
	string username;
	string psd;
	int point;
};
class ZhangHuXiTong
{
public:
	ZhangHuXiTong();
	~ZhangHuXiTong();
	int DengLu(string uname,string psd);
	int ZhuCe(string uname,string psd1,string psd2);
	int XiuGaiMiMa(string uname,string psd,string psd1,string psd2);
	void showUsers();
private:
	ifstream infile;
	vector<user> users;
};

