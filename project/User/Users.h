#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sqlite_orm/sqlite_orm.h>
#include <stdexcept>
using namespace std;
using namespace sqlite_orm;
struct user {//user's struct
	string username;//user's name
	string password;//user's password
	int point;//user's point
	int token;//This variable is used to check whether the user has loggined in
};

struct Record {//Record's struct,this struct is used to storage Ranklist information,every Record has a username and apoint
	string username;
	int point;
};


class Users
{
public:
	Users();
	~Users();
	int login(string uname, string password);//The function to login
	void reg(string uname, string password1, string password2);//The function to reg
	void modifyPassword(string uname, string password, string password1, string password2);//The function to modify password
	void update(string uname, int point,int token);//The function to update user score after ever game
	vector<Record> getRankList();//The function to get ranklist
private:
	vector<Record> RankList;//The ranklist of the game
};
