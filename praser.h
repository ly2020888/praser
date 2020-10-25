#pragma once
#include <string>
#include <map>
#include <set>
#include <utility>
#include <vector>
#include <iostream>
#include <fstream>
using namespace std;
class praser {
private:
	set <string> end_char;//�ս������
	set <string> not_end_char;//���ս������
	vector<pair<string, string>> rules;//�ķ�����
	map <string,string> FIRST;
	map <string, string> FOLLOW;
	map <string, string> SELECT;
	string start;
	string unique(string s);//һ���ַ���ȥ�غ���
	void getFirst();
	void getFollow();
	void getSelct(string c);
	string eraseEpsilon(string s);
	string g1(string);
	string g2(string &);
	string g3(string);
	bool havEpsilon(string s);
public:
	void readFile(string path);
	void output();
};

string praser::unique(string s) {
	set<char> a;
	for (int i = 0; i < s.size(); i++) {
		a.insert(s[i]);
	}
	string t = "";
	for (set<char>::iterator i = a.begin(); i !=a.end(); i++) {
		t += *i;
	}
	return t;
}
void praser::readFile(string path) {
	ifstream src(path);
	if (!src.is_open()) {
		cout << "�Ҳ���Դ�ļ�testA��";
		return;
	}
	int n,m;
	string temp;

	src >> n;
	not_end_char.clear();
	for (int i = 0; i < n; i++) {
		src >> temp;
		not_end_char.insert(temp);
		pair<string, string> p1;
		p1.first = temp;
		p1.second = "";
		FIRST.insert(p1);
		FOLLOW.insert(p1);
	}//���ս��

	src >> n;
	end_char.clear();
	for (int i = 0; i < n; i++) {
		src >> temp;
		end_char.insert(temp);
		FIRST[temp] = temp;//�ս����FIRST �������Լ�
	}//�ս��

	src >> n;
	rules.clear();
	for (int i = 0; i < n; i++) {
		src >> temp;
		src >> m;
		string temp2 = "";
		for (int j = 0; j < m; j++) {
			string c;
			src >> c;
			temp2 += (c+" ");
		}
		pair<string, string> p1;
		p1.first = temp;
		p1.second = temp2;
		rules.push_back(p1);
	}//���ս��
	src >> start;
	FOLLOW[start] = "#";
	end_char.insert("~");//���������Ǹ��ս��
	getFirst();
	getFollow();
}
void praser::output() {
	cout << "���ս����";
	for (set <string> ::iterator it = not_end_char.begin(); it != not_end_char.end(); it++) {
		cout << *it<<" ";
	}
	cout << endl;
	cout << "�ս����";
	for (set <string> ::iterator it = end_char.begin(); it != end_char.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	cout << "����ʽ���ϣ�";
	for (vector<pair<string,string>>::iterator it = rules.begin(); it != rules.end(); it++) {
		cout << it->first << " -> "<<it->second<<" "<<endl;
	}
	cout << endl;
	cout << "��ʼ���ţ�"<<start;
}
string praser::g1(string s) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ' ') {
			string t = s.substr(0, i);
			if (end_char.count(t)) {
				return t;
			}
			else {
				return FIRST[t];
			}
		}
	}//��������FIRST
}

void praser::getFirst() {
		for (vector<pair<string, string>>::iterator it = rules.begin(); it != rules.end(); it++) {
			FIRST[it->first] += unique(g1(it->second));
		}
}
string praser::g2(string &s) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ' ') {
			string t = s.substr(0, i);
			s.erase(0, i+1);
			return t;
		}
	}//����ĳ���ַ����е��ַ� ���磨T'���ֱ�������� �� ��T���� ������ ��Ϊ���ս���Ķ����ַ��������ⲽҲ����Ҫ
}
string praser::g3(string s) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ' ') {
			return s.substr(0, i);;
		}
	}//������ĺ�������һ����ֻ�ǲ�ɾ����ȡ�����Ӵ�
}
string praser::eraseEpsilon(string s) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == '~') {
			s.erase(i, 1);
			return s;
		}
	}
	return s;
}
bool praser::havEpsilon(string s) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == '~') {
			return true;
		}
	}
	return false;
}

void praser::getFollow() {
	for (vector<pair<string, string>>::iterator it = rules.begin(); it != rules.end(); it++) {
		string temp = it->second;
		while (temp != "") {
			string ss = g2(temp);
			if (end_char.count(ss)) {//����ó������ս��
				continue;
			}
			else if (not_end_char.count(ss)) {//����ó��ķ��ս��
				if (temp == "") {//���ս����Ϊ��
					FOLLOW[ss] += FOLLOW[it->first];
					FOLLOW[ss] = unique(FOLLOW[ss]);
					break;//������Ϻ�û�ж����ˣ�ֱ������
				}
				if ( end_char.count(g3(temp)) ) {//�����ǰ��������Ӵ� �ĵ�һ���ַ����ս��
					FOLLOW[ss] += g2(temp);//FOLLOW����������ս��
				}
				else {// ����ó����Ƿ��ս�� ���Ǿ�Ҫ�жϷ��ս���ܷ������Ƶ���epislon
					if (havEpsilon(FIRST[g3(temp)])) {
						FOLLOW[ss] += eraseEpsilon(FIRST[g3(temp)]);
						FOLLOW[ss] += FOLLOW[it->first];
					}
					else FOLLOW[ss] = FIRST[g3(temp)];//���ս������ִ���FIRST���ϲ���ȥ����
				}
			}
			FOLLOW[ss] = unique(FOLLOW[ss]);
		}
	}
}