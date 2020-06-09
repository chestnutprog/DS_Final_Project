#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sqlite_orm/sqlite_orm.h>
using namespace std;
using namespace sqlite_orm;
struct user {
	string username;
	string password;
	int point;
};

struct Record {
	string username;
	int point;
};


class Users
{
public:
	Users();
	~Users();
	int login(string uname, string password);//登陆
	int reg(string uname, string password1, string password2);//注册
	int modifyPassword(string uname, string password, string password1, string password2);//修改密码
	void update(string uname, int point);//得到游戏之后的玩家姓名以及玩家分数
	vector<Record> getRankList();//得到排行耪
private:
	vector<Record> RankList;
};
