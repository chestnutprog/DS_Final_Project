#include "ZhangHuXiTong.h"

ZhangHuXiTong::ZhangHuXiTong() {
	infile.open("E:\\test\\yonghu.txt");
	if (!infile) cout << "û�д��ļ���" << endl;

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

int ZhangHuXiTong::DengLu(string uname, string psd) {//101��½�ɹ���201�û������������
	int flag = 201;
	for (int i = 0; i < users.size(); i++) {
		if (users[i].username == uname && users[i].psd == psd) {
			flag = 101;
			break;
		}
	}

	return flag;
}

int ZhangHuXiTong::ZhuCe(string uname, string psd1, string psd2) {//102ע��ɹ���202�ѱ�ע�ᣬ203�������벻ͬ
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
		if (!outfile) cout << "û�д��ļ���" << endl;

		outfile << endl << uname << " " << psd1 << " " << "0" ;
		outfile.close();
	}

	return flag;
}

int ZhangHuXiTong::XiuGaiMiMa(string uname, string psd, string psd1, string psd2) {
	int flag = 204;//�û���������
	for (int i = 0; i < users.size(); i++) {
		if (users[i].username == uname) {
			if (psd != users[i].psd) {
				flag = 205;//���벻��ȷ
			}
			else if (psd1 != psd2) {
				flag = 203;//�����������벻ͬ
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
		if (!outfile) cout << "û�д��ļ���" << endl;

		for (int i = 0; i < users.size(); i++) {
			outfile << users[i].username << " " << users[i].psd << " " << users[i].point << endl;
		}
		
		outfile.close();
	}
	return flag;
}