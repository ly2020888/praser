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
	set <string> end_char;//终结符集合
	set <string> not_end_char;//非终结符集合
	vector<pair<string, string>> rules;//文法规则
	map <string,string> FIRST;
	map <string, string> FOLLOW;
	map <string, string> SELECT;
	string start;
	string unique(string s);//一个字符串去重函数
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
		cout << "找不到源文件testA！";
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
	}//非终结符

	src >> n;
	end_char.clear();
	for (int i = 0; i < n; i++) {
		src >> temp;
		end_char.insert(temp);
		FIRST[temp] = temp;//终结符的FIRST 就是他自己
	}//终结符

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
	}//非终结符
	src >> start;
	FOLLOW[start] = "#";
	end_char.insert("~");//ε天生就是个终结符
	getFirst();
	getFollow();
}
void praser::output() {
	cout << "非终结符：";
	for (set <string> ::iterator it = not_end_char.begin(); it != not_end_char.end(); it++) {
		cout << *it<<" ";
	}
	cout << endl;
	cout << "终结符：";
	for (set <string> ::iterator it = end_char.begin(); it != end_char.end(); it++) {
		cout << *it << " ";
	}
	cout << endl;
	cout << "产生式集合：";
	for (vector<pair<string,string>>::iterator it = rules.begin(); it != rules.end(); it++) {
		cout << it->first << " -> "<<it->second<<" "<<endl;
	}
	cout << endl;
	cout << "开始符号："<<start;
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
	}//产生串的FIRST
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
	}//产生某个字符串中的字符 例如（T'）分别产生“（ ” ”T‘“ “）” 因为非终结符的都是字符串所以这步也很重要
}
string praser::g3(string s) {
	for (int i = 0; i < s.size(); i++) {
		if (s[i] == ' ') {
			return s.substr(0, i);;
		}
	}//与上面的函数功能一样，只是不删除被取出的子串
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
			if (end_char.count(ss)) {//如果拿出的是终结符
				continue;
			}
			else if (not_end_char.count(ss)) {//如果拿出的非终结符
				if (temp == "") {//非终结符后为空
					FOLLOW[ss] += FOLLOW[it->first];
					FOLLOW[ss] = unique(FOLLOW[ss]);
					break;//处理完毕后没有东西了，直接跳出
				}
				if ( end_char.count(g3(temp)) ) {//如果当前待处理的子串 的第一个字符是终结符
					FOLLOW[ss] += g2(temp);//FOLLOW集加上这个终结符
				}
				else {// 如果拿出的是非终结符 ，那就要判断非终结符能否最终推导到epislon
					if (havEpsilon(FIRST[g3(temp)])) {
						FOLLOW[ss] += eraseEpsilon(FIRST[g3(temp)]);
						FOLLOW[ss] += FOLLOW[it->first];
					}
					else FOLLOW[ss] = FIRST[g3(temp)];//非终结符后的字串的FIRST集合并且去掉ε
				}
			}
			FOLLOW[ss] = unique(FOLLOW[ss]);
		}
	}
}