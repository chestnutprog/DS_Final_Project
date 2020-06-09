#include "Users.h"
auto storage = make_storage(
	"user.sqlite",
	make_table("users",
		make_column("username", &user::username, unique()),
		make_column("psd", &user::password),
		make_column("point", &user::point))
);

Users::Users() {
	storage.sync_schema();

	auto users = storage.get_all<user>(order_by(&user::point));
	int n = users.size();
	RankList = vector<Record>(n);
	for (int i = 0; i < n ; i++) {
		RankList[i].username = users[n - i - 1].username;
		RankList[i].point = users[n - i - 1].point;
	}
}

Users::~Users() {};

int Users::login(string uname, string password) {//101登陆成功，201用户名或密码错误
	auto users = storage.get_all<user>(where(c(&user::username) == uname));
	if (users.size() == 0) {
		return 204;//用户名不存在
	}
	else if (users[0].password != password) {
		return 205;//密码不正确
	}
	else {
		return 101;//登陆成功
	}
}

int Users::reg(string uname, string password1, string password2) {//102注册成功，202已被注册，203两次密码不同
	auto users = storage.get_all<user>(where(c(&user::username) == uname));
	if (users.size() != 0) {
		return 202;//用户名已经被注册
	}
	else if (password1 ！= password2) {
		return 203;//两次密码不同
	}else {
		user u = { uname,password1,0 };
		storage.insert(u);
		return 102;//注册成功
	}
}

int Users::modifyPassword(string uname, string password, string password1, string password2) {
	auto users = storage.get_all<user>(where(c(&user::username) == uname));
	if (users.size() == 0) {
		return 204;//用户名不存在
	}
	else if (users[0].password != password){
		return 205;//密码不正确
	}
	else if (password1 ！= password2) {
		return 203;//两次密码不同
	}
	else {
		user u =  { uname,password1,users[0].point };
		storage.replace(u);
		return 103;//修改密码成功
	}
	
}

vector<Record> Users::getRankList() {//排行榜按降序排列
	return RankList;
}

void Users::update(string uname,int point) {//更新数据，在每局游戏之后，这里我感觉我对排行榜的更新弄得不好 
	auto users = storage.get_all<user>(where(c(&user::username) == uname));
	if (users[0].point < point) {
		user u = { uname,users[0].password,point };
		storage.replace(u);

		auto users = storage.get_all<user>(order_by(&user::point));
		int n = users.size();
		for (int i = 0; i < n; i++) {
			RankList[i].username = users[n - i - 1].username;
			RankList[i].point = users[n - i - 1].point;
		}
	}
}
