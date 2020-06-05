#include "Users.h"

Users::Users() {
	infile.open("Users.txt");
	if (!infile) cout << "û�д��ļ���" << endl;

	string s;
	istringstream ss;
	user u;
	while (getline(infile,s))
	{
		ss = istringstream(s);
		ss >> u.username >> u.password >> u.point;
		users.push_back(u);
	}

	infile.close();
}

Users::~Users() {};

void Users::save(){
	ofstream outfile;
	outfile.open("Users.txt");//, ios::beg);
	if (!outfile) cout << "û�д��ļ���" << endl;

	for (int i = 0; i < users.size(); i++) {
		outfile << users[i].username << " " << users[i].password << " " << users[i].point << endl;
	}
	
	outfile.close();
}

void Users::showUsers() {
	for (int i = 0; i < users.size(); i++) {
		cout << users[i].username << endl;
	}
}

int Users::login(string uname, string password) {//101��½�ɹ���201�û������������
	int flag = 201;
	for (int i = 0; i < users.size(); i++) {
		if (users[i].username == uname && users[i].password == password) {
			flag = 101;
			break;
		}
	}

	return flag;
}

int Users::reg(string uname, string password1, string password2) {//102ע��ɹ���202�ѱ�ע�ᣬ203�������벻ͬ
	int flag = 102;
	for (int i = 0; i < users.size(); i++) {
		if (users[i].username == uname ) {
			flag = 202;
			break;
		}
	}

	if (flag == 102 && password1 != password2) {
		flag = 203;
	}
	else if (flag == 102 && password1 == password2){
		user u = { uname,password1,0 };
		users.push_back(u);
		save();
	}

	return flag;
}

int Users::modifyPassword(string uname, string password, string password1, string password2) {
	int flag = 204;//�û���������
	for (int i = 0; i < users.size(); i++) {
		if (users[i].username == uname) {
			if (password != users[i].password) {
				flag = 205;//���벻��ȷ
			}
			else if (password1 != password2) {
				flag = 203;//�����������벻ͬ
			}
			else {
				users[i].password = password1;
				flag = 103;
			}
		}
	}
	if (flag == 103) {
		save();
	}
	return flag;
}