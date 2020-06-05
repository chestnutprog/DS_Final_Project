#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
using namespace std;
struct user{
	string username;
	string password;
	int point;
};
class Users
{
public:
	Users();
	~Users();
	void save();
	int login(string uname,string password);
	int reg(string uname,string password1,string password2);
	int modifyPassword(string uname,string password,string password1,string password2);
	void sortUsers();
	void showUsers();
private:
	ifstream infile;
	vector<user> users;
};

