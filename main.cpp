//
//  main.cpp
//  minimap_proj
//
//  Created by Zhuo Zhang on 6/22/14.
//  Copyright (c) 2014 lispc_thu. All rights reserved.
//

#include<iostream>
#include<fstream>
#include<vector>
#include<cctype>
#include<regex>
#include<sstream>
#include<algorithm>
#include<queue>
#include<cmath>
using namespace std;
struct Obj{
	float x;
	float y;
	vector<string> words;
	Obj (string s){
        smatch m;
		string ptn = ", \"addr\" : \"([^\"]+)\".*\"latlng\" : \\[ (\\S+), (\\S+) \\], \"name\" : \"([^\"]+)\"";
        if(!regex_search(s,m,regex(ptn))){
            cout<<"no match"<<endl;
            exit(-1);
        }
        /*
        for(int i=0;i<m.size();i++){
            cout<<i<<" "<<m[i]<<endl;
        }*/
        x = stof(m[2]);
        y = stof(m[3]);
        stringstream ss(string(m[1]).substr(string(m[4]).length()+2)+" "+string(m[4]));
        string word;
        while(ss>>word){
            //if(isdigit(word[0]))
            //    continue;
            word.erase(word.find_last_not_of(",")+1);
            transform(word.begin(),word.end(),word.begin(),::tolower);
            this->words.push_back(word);
        }
        sort(this->words.begin(),this->words.end());
        this->words.resize(distance(this->words.begin(),unique(this->words.begin(),this->words.end())));
    }
    friend ostream& operator<<(ostream& os,const Obj& o){        os<<"["<<o.x<<"\t"<<o.y<<"]\t";
        copy(o.words.begin(),o.words.end(),ostream_iterator<string>(os," "));
        os<<endl;
        return os;
    }
};
vector<Obj> objs;
struct PRNode
{
    string pre;
    float b;
    float u;
    float l;
    float r;
    bool is_leaf;
};
void build_index(string fname){
    ifstream ifs(fname);
    string line;
    while(getline(ifs,line)){
        objs.push_back(Obj(line));
    }
    //copy(objs.begin(),objs.end(),ostream_iterator<Obj>(cout,""));

}

struct Qe{
    int type;//pnode 1 po 2
    PRNode* pnode;
    Obj* po;
    float dis;
};
bool operator<(const Qe& a,const Qe& b){
    return a.dis<b.dis;
}
inline float ldis(float x1,float y1,float x2,float y2){
    return sqrt((x1-y1)*(x1-y1)+(x2-y2)*(x2-y2));
}
float dis(PRNode n,float x,float y){
    int pos = 3*(y>n.u+y>n.l)+(x>n.r+x>n.l);
    switch (pos) {
        case 0:
            return ldis(x,y,n.l,n.u);
        case 1:
            return y-n.u;
        case 2:
            return ldis(x,y,n.r,n.u);
        case 3:
            return n.l-x;
        case 4:
            return 0;
        case 5:
            return x-n.r;
        case 6:
            return ldis(x,y,n.l,n.b);
        case 7:
            return n.b-y;
        case 8:
            return ldis(x, y, n.r, n.b);
        default:
            cerr<<"hehe"<<endl;
            exit(-1);
    }
}
void search(int n,string p,float x,float y,vector<string> words){
    queue<Qe> q;
    
    
}
int main(){
    try{
        build_index("/Users/zhuo.zhang/Projects/minimap/minidata");
    }catch(const exception& e){
        cout<<e.what();
    }
}
