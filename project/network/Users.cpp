#include <bits/stdc++.h>
#include "Users.h"
using namespace std;

Users::Users()
{
	infile.open("Users.txt");
	if (!infile)
		cout << "没有打开文件！" << endl;

	string s;
	istringstream ss;
	user u;
	while (getline(infile, s))
	{
		ss = istringstream(s);
		ss >> u.username >> u.password >> u.point;
		users.push_back(u);
	}

	infile.close();
}

Users::~Users(){};

void Users::save()
{
	ofstream outfile;
	outfile.open("Users.txt"); //, ios::beg);
	if (!outfile)
		cout << "没有打开文件！" << endl;

	for (int i = 0; i < users.size(); i++)
	{
		outfile << users[i].username << " " << users[i].password << " " << users[i].point << endl;
	}

	outfile.close();
}

string Users::showUsers()
{
	stringstream ss;
	for (int i = 0; i < users.size(); i++)
	{
		ss << users[i].username << " " << users[i].point << endl;
	}
	return ss.str();
}

int Users::login(string uname, string password)
{ //101登陆成功，201用户名或密码错误
	int flag = 201;
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].username == uname && users[i].password == password)
		{
			flag = 101;
			break;
		}
	}

	return flag;
}

int Users::reg(string uname, string password1, string password2)
{ //102注册成功，202已被注册，203两次密码不同
	int flag = 102;
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].username == uname)
		{
			flag = 202;
			break;
		}
	}

	if (flag == 102 && password1 != password2)
	{
		flag = 203;
	}
	else if (flag == 102 && password1 == password2)
	{
		user u = {uname, password1, 0};
		users.push_back(u);
		save();
	}

	return flag;
}

int Users::modifyPassword(string uname, string password, string password1, string password2)
{
	int flag = 204; //用户名不存在
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].username == uname)
		{
			if (password != users[i].password)
			{
				flag = 205; //密码不正确
			}
			else if (password1 != password2)
			{
				flag = 203; //两次输入密码不同
			}
			else
			{
				users[i].password = password1;
				flag = 103;
			}
		}
	}
	if (flag == 103)
	{
		save();
	}
	return flag;
}

int Users::updateScore(string uname, int newScore)
{
	int flag = 204; //用户名不存在
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].username == uname)
		{
			users[i].point = newScore;
			flag = 103;
		}
	}
	if (flag == 103)
	{
		save();
	}
	return flag;
}
user Users::getUser(string uname)
{
	for (int i = 0; i < users.size(); i++)
	{
		if (users[i].username == uname)
		{
			return users[i];
		}
	}
	throw invalid_argument("There isn't such user in system.");
}