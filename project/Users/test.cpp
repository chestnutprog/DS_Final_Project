#include "Users.h"

void menu() {
	cout << endl;
	cout << "1.登陆" << endl;
	cout << "2.注册" << endl;
	cout << "3.修改密码" << endl;
	cout << "4.展示排行榜" << endl;
	cout << "0.退出" << endl;
	cout << "请输入编号进行操作！" << endl;
}

int main() {
	Users* z = new Users();
	int k = -1;
	string uname;
	string psd, psd1, psd2;
	int flag = 0;
	while (k != 0) {
		menu();
		cin >> k;
		switch (k) {
		case 1:
			cout << endl;
			cout << "输入用户名：" << endl;
			cin >> uname;
			cout << "输入密码：" << endl;
			cin >> psd;
			flag = z->login(uname, psd);
			break;
		case 2:
			cout << endl;
			cout << "请输入要注册的用户名：" << endl;
			cin >> uname;
			cout << "请输入密码：" << endl;
			cin >> psd1;
			cout << "请再次输入密码：" << endl;
			cin >> psd2;
			flag = z->reg(uname, psd1, psd2);
			break;
		case 3:
			cout << endl;
			cout << "请输入用户名：" << endl;
			cin >> uname;
			cout << "请输入原密码：" << endl;
			cin >> psd;
			cout << "请输入修改后的密码：" << endl;
			cin >> psd1;
			cout << "请再次输入密码：" << endl;
			cin >> psd2;
			flag = z->modifyPassword(uname, psd, psd1, psd2);
			break;
		case 0:
			system("exit");
			break;
		case 4:
			cout << endl;
			z->sortUsers();
			z->showUsers();
			break;
		default:
			cout << "请输入正确的编号来进行操作！" << endl;
			break;
		}

		switch (flag) {
		case 101:
			//进入用户操作界面
			cout << "登录成功！" << endl;
			k = 0;
			break;
		case 102:
			cout << "注册成功！" << endl;
			break;
		case 103:
			cout << "修改密码成功！" << endl;
			break;
		case 201:
			cout << "请输入正确的用户名以及密码！" << endl;
			break;
		case 202:
			cout << "该用户名已经被注册！" << endl;
			break;
		case 203:
			cout << "两次输入密码不相同！" << endl;
			break;
		case 204:
			cout << "该用户名不存在！" << endl;
			break;
		case 205:
			cout << "密码错误" << endl;
		}
	}
}
}
