#include "ZhangHuXiTong.h"

void menu() {
	cout << endl;
	cout << "1.��½" << endl;
	cout << "2.ע��" << endl;
	cout << "3.�޸�����" << endl;
	cout << "0.�˳�" << endl;
	cout << "�������Ž��в�����" << endl;
}

int main() {
	ZhangHuXiTong *z = new ZhangHuXiTong();
	z->showUsers();
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
			cout << "�����û�����" << endl;
			cin >> uname;
			cout << "�������룺" << endl;
			cin >> psd;
			flag = z->DengLu(uname, psd);
			break;
		case 2:
			cout << endl;
			cout << "������Ҫע����û�����" << endl;
			cin >> uname;
			cout << "���������룺" << endl;
			cin >> psd1;
			cout << "���ٴ��������룺" << endl;
			cin >> psd2;
			flag = z->ZhuCe(uname, psd1, psd2);
			break;
		case 3:
			cout << endl;
			cout << "�������û�����" << endl;
			cin >> uname;
			cout << "������ԭ���룺" << endl;
			cin >> psd;
			cout << "�������޸ĺ�����룺" << endl;
			cin >> psd1;
			cout << "���ٴ��������룺" << endl;
			cin >> psd2;
			flag = z->XiuGaiMiMa(uname, psd, psd1, psd2);
			break;
		case '0':
			system("exit");
			break;
		default:
			cout << "��������ȷ�ı�������в�����" << endl;
			break;
		}

		switch (flag) {
		case 101:
			//�����û���������
			cout << "��¼�ɹ���" << endl;
			k = 0;
			break;
		case 102:
			cout << "ע��ɹ���" << endl;
			break;
		case 103:
			cout << "�޸�����ɹ���" << endl;
			break;
		case 201:
			cout << "��������ȷ���û����Լ����룡" << endl;
			break;
		case 202:
			cout << "���û����Ѿ���ע�ᣡ" << endl;
			break;
		case 203:
			cout << "�����������벻��ͬ��" << endl;
			break;
		case 204:
			cout << "���û��������ڣ�" << endl;
			break;
		case 205:
			cout << "�������" << endl;
		}
	}
}