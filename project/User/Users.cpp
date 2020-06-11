#include "Users.h"

/**This is the database of the game.
*The information is stored in the user.sqlite
*The name of the table is called users
*And there are four column in this table ,they are username, psd, point, token.
*/
auto storage = make_storage(
	"user.sqlite",
	make_table("users",
		make_column("username", &user::username, unique()),
		make_column("psd", &user::password),
		make_column("point", &user::point),
		make_column("token", &user::token))
	);


/**This is the constructor
*Calling the constructor will automatically turn on sync
*Calling the constructor will automatically get the ranklist
*/
Users::Users() {
	storage.sync_schema();//turn on sync

	auto users = storage.get_all<user>(order_by(&user::point));//Get ranklist in ascending order 
	int n = users.size();
	RankList = vector<Record>(n);
	for (int i = 0; i < n ; i++) {//Change ascending order to descending order
		RankList[i].username = users[n - i - 1].username;
		RankList[i].point = users[n - i - 1].point;
	}
}

Users::~Users() {};

/**This is the function to login
*We will get the username and password
*If the username dosen't exist or the password is wrong will throw runtime error    
*If log in successfully will return a token which used to determine if the user is online
*/
int Users::login(string uname, string password) {
	auto users = storage.get_all<user>(where(c(&user::username) == uname));
	if (users.size() == 0) {//The database doesn't exist this user
		throw runtime_error("用户名不存在");
	}
	else if (users[0].password != password) {//The input password is wrong
		throw runtime_error("密码错误");
	}
	else {
		int token;
		token = rand();//Get a token by rand()
		users[0].token = token;
		return token;
	}
}

/**This is the function to reg
*We will get the username and two passwords
*If the username has already been registered or the two entered password are different will throw runtime error 
*/
void Users::reg(string uname, string password1, string password2) {
	auto users = storage.get_all<user>(where(c(&user::username) == uname));
	if (users.size() != 0) {//The username has already been registered
		throw runtime_error("用户名已经被注册");
	}
	else if (password1 != password2) {//The two entered password are different
		throw runtime_error("两次输入密码不相同");
	}else {
		user u = { uname,password1,0 ,0};//Initialize user information
		storage.insert(u);//Insert this user to the database
	}
}


/**This is the function to modify password
*We will get the username, original password and two upcoming passwords
*If the username dosen't exist or the password is wrong will throw runtime error 
*and if two entered password are different will also throw runtime error
*/
void Users::modifyPassword(string uname, string password, string password1, string password2) {
	auto users = storage.get_all<user>(where(c(&user::username) == uname));
	if (users.size() == 0) {//The database doesn't exist this user
		throw runtime_error("用户名不存在");
	}
	else if (users[0].password != password){//The input password is wrong
		throw runtime_error("密码不正确");
	}
	else if (password1 != password2) {//The two entered password are different
		throw runtime_error("两次输入密码不相同");
	}
	else {
		user u =  { uname,password1,users[0].point ,users[0].token};//modify the user's password
		storage.replace(u);//Update the database,use u to replace the original user information
	}
	
}

/**
*This is the function to return ranklist
*/
vector<Record> Users::getRankList() {
	return RankList;
}

/**This is the function to update the user score 
*We will get the user name, the score to be modified and a token to determine whether the user is online
*If the username dosen't exist or the user is not online will throw runtime error
*If the username is exist and he is online we can modify his score by the point and we can reorder the ranklist
*/
void Users::update(string uname,int point,int token) {
	auto users = storage.get_all<user>(where(c(&user::username) == uname));
	if (users.size() == 0) {//The database doesn't exist this user
		throw runtime_error("无法修改！该记录出错,该用户名不存在！");
	}
	else if (token != users[0].token) {//The user is not online ,can't modify his score
		throw runtime_error("无法修改！无法修改他人的分数！");
	}
	else if (users[0].point < point) { //If this user has broken his record, modify his information in the database
		user u = { uname,users[0].password,point ,token};//modify the point
		storage.replace(u); // Update the database, use u to replace the original user information

		auto users = storage.get_all<user>(order_by(&user::point));
		int n = users.size();
		for (int i = 0; i < n; i++) {//Reorder the ranklist
			RankList[i].username = users[n - i - 1].username;
			RankList[i].point = users[n - i - 1].point;
		}
	}
}
