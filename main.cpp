#include<iostream>
#include<fstream>
#include<vector>
#include<cctype>
#include<regex>
#include<sstream>
#include<algorithm>
using namespace std;
struct Obj{
	float x;
	float y;
	vector<string> words;
	Obj (string s){
		cout<<s<<endl;
		string p1 = ", \"addr\" : \"";
		int pos0 = s.find(p1);
		cout<<pos0<<endl;
		cout<<s.substr(pos0);
		int pos1 = pos0+p1.size();
		int pos2 = s.find("\"",pos1);
		string addr = s.substr(pos1,pos2-pos1);
		cout<<addr<<endl;
		exit(0);
		/*
		string p2 = ", \"latlang\" : [ ";
		int pos1 = s.find(p1)+p1.size();
		int pos2 = s.find(pos1,"\"",pos1);
		string addr = s.substr(pos1,pos2-pos1);
		cout<<addr<<endl;
		string p1 = ", \"addr\" : \"";
		int pos1 = s.find(p1)+p1.size();
		int pos2 = s.find(pos1,"\"",pos1);
		string addr = s.substr(pos1,pos2-pos1);
		cout<<addr<<endl;
		stringstream ss(string(m[0])+" "+string(m[3]));
		string word;
		while(getline(ss,word)){
			if(isdigit(word[0]))
				continue;
			word.erase(word.find_last_not_of(",")+1);
			transform(word.begin(),word.begin(),word.begin(),::tolower);
			this->words.push_back(word);
		}
		sort(this->words.begin(),this->words.end());
		unique(this->words.begin(),this->words.end());
		*/
	}
	friend ostream& operator<<(ostream& os,const Obj& o){
		os<<"["<<o.x<<"\t"<<o.y<<"]\t";
		copy(o.words.begin(),o.words.end(),ostream_iterator<string>(os," "));
		os<<endl;
		return os;
	}
};
vector<Obj> objs;
void build_index(string fname){
	ifstream ifs(fname);
	string line;
	while(getline(ifs,line)){
		objs.push_back(Obj(line));
	}
	copy(objs.begin(),objs.end(),ostream_iterator<Obj>(cout,""));
}
int main(){
	build_index("minidata");
}