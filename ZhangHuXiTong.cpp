#include "ZhangHuXiTong.h"

ZhangHuXiTong::ZhangHuXiTong() {
	infile.open("E:\\test\\yonghu.txt");
	if (!infile) cout << "没有打开文件！" << endl;

	string s;
	istringstream ss;
	user u;
	while (getline(infile,s))
	{
		ss = istringstream(s);
		ss >> u.username >> u.psd >> u.point;
		users.push_back(u);
	}

	infile.close();
}

ZhangHuXiTong::~ZhangHuXiTong() {};

void ZhangHuXiTong::showUsers() {
	for (int i = 0; i < users.size(); i++) {
		cout << users[i].username << endl;
	}
}

int ZhangHuXiTong::DengLu(string uname, string psd) {//101登陆成功，201用户名或密码错误
	int flag = 201;
	for (int i = 0; i < users.size(); i++) {
		if (users[i].username == uname && users[i].psd == psd) {
			flag = 101;
			break;
		}
	}

	return flag;
}

int ZhangHuXiTong::ZhuCe(string uname, string psd1, string psd2) {//102注册成功，202已被注册，203两次密码不同
	int flag = 102;
	for (int i = 0; i < users.size(); i++) {
		if (users[i].username == uname ) {
			flag = 202;
			break;
		}
	}

	if (flag == 102 && psd1 != psd2) {
		flag = 203;
	}
	else if (flag == 102 && psd1 == psd2){
		user u = { uname,psd1,0 };
		users.push_back(u);
		ofstream outfile;
		outfile.open("E:\\test\\yonghu.txt",ios::app);
		if (!outfile) cout << "没有打开文件！" << endl;

		outfile << endl << uname << " " << psd1 << " " << "0" ;
		outfile.close();
	}

	return flag;
}

int ZhangHuXiTong::XiuGaiMiMa(string uname, string psd, string psd1, string psd2) {
	int flag = 204;//用户名不存在
	for (int i = 0; i < users.size(); i++) {
		if (users[i].username == uname) {
			if (psd != users[i].psd) {
				flag = 205;//密码不正确
			}
			else if (psd1 != psd2) {
				flag = 203;//两次输入密码不同
			}
			else {
				users[i].psd = psd1;
				flag = 103;
			}
		}
	}
	if (flag == 103) {
		ofstream outfile;
		outfile.open("E:\\test\\yonghu.txt", ios::beg);
		if (!outfile) cout << "没有打开文件！" << endl;

		for (int i = 0; i < users.size(); i++) {
			outfile << users[i].username << " " << users[i].psd << " " << users[i].point << endl;
		}
		
		outfile.close();
	}
	return flag;
}