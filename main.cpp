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
#include<set>
#include<cstdlib>
#define NODE 1
#define OBJ 2
#define M 10
using namespace std;
struct Obj{
	float x;
	float y;
	vector<string> words;
	Obj (string s){
        smatch m;
		string ptn = "\"addr\" : \"([^\"]+)\".*\"latlng\" : \\[ (\\S+), (\\S+) \\], \"name\" : \"([^\"]*)\"";
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
            words.push_back(word);
        }
        sort(words.begin(),words.end());
        words.resize(distance(words.begin(),unique(words.begin(),words.end())));
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
    vector<int> obj_ids;
    vector<PRNode*> childs;
    PRNode(vector<int>& ids,string pre){
        pre = pre;
        b = 90;
        u = 0;
        l = 180;
        r = 0;
        float x_mean = 0;
        float y_mean = 0;
        for(int i=0;i<ids.size();i++){
            int obj_id = ids[i];
            float x = objs[obj_id].x;
            float y = objs[obj_id].y;
            if(x<l)
                l = x;
            if(x>r)
                r = x;
            if(y<b)
                b = y;
            if(y>u)
                u = y;
            x_mean += x;
            y_mean += y;
        }
        x_mean/=ids.size();
        y_mean/=ids.size();
        if(ids.size()<=M){
            obj_ids = ids;
            is_leaf = true;
        }else{
            set<string> all_words;
            int charmap[256];
            vector<vector<int>> vvi;
            for(int i=0;i<256;i++){
                vvi.push_back(vector<int>());
            }
            memset(charmap, 0, sizeof(charmap));
            for(int i=0;i<ids.size();i++){
                for(int j=0;i<objs[i].words.size();j++){
                    string w = objs[i].words[j];
                    int index = (unsigned int)((unsigned char)w[0]);
                    vvi[index].push_back(i);
                }
            }
            for(int i=0;i<256;i++){
                if(vvi[i].size()>0){
                    /*(
                     vector<int> ll;
                     vector<int> ul;
                     vector<int> lr;
                     vector<int> ur;
                     */
                    vector<vector<int>> vvi_inner;
                    for(int t=0;t<4;t++){
                        vvi_inner.push_back(vector<int>());
                    }
                    for(int j=0;j<vvi.size();j++){
                        int n_id = vvi[i][j];
                        float n_x = objs[n_id].x;
                        float n_y = objs[n_id].y;
                        int pos = 2*(n_x<x_mean)+n_y>y_mean;
                        /*
                         if(pos==0){
                         ul.push_back(n_id);
                         }else if(pos==1){
                         ur.push_back(n_id);
                         }else if(pos==2){
                         ll.push_back(n_id);
                         }else{
                         lr.push_back(n_id);
                         }
                         */
                        vvi_inner[pos].push_back(n_id);
                    }
                    string new_pre = pre;
                    new_pre.append(1,(char)((unsigned char)i));
                    for(int f=0;f<4;f++){
                        if(vvi_inner[i].size()>0){
                            PRNode* new_node = new PRNode(vvi_inner[i],new_pre);
                            childs.push_back(new_node);
                        }
                        
                    }
                }
                
            }
        }
    }
};
class Qe{
public:
    int type;//pn 1 po 2
    PRNode* pn;
    int po;//index
    float dis;
    Qe(int type,PRNode* pn,int po,float dis){
        type = type;
        pn = pn;
        po = po;
        dis = dis;
    }
    bool operator< (const Qe& b) const{
        return this->dis<b.dis;
    }
};
PRNode* root;
void build_index(string fname){
    ifstream ifs(fname);
    string line;
    while(getline(ifs,line)){
        objs.push_back(Obj(line));
    }
    copy(objs.begin(),objs.end(),ostream_iterator<Obj>(cout,""));
    auto obj_size = objs.size();
    vector<int> init_obj_ids;
    for(int i=0;i<obj_size;i++){
        init_obj_ids.push_back(i);
    }
    root = new PRNode(init_obj_ids,"");
}
inline float ldis(float x1,float y1,float x2,float y2){
    return sqrt((x1-y1)*(x1-y1)+(x2-y2)*(x2-y2));
}
float dis(PRNode *n,float x,float y){
    int pos = 3*(y<n->u+y<n->l)+(x>n->r+x>n->l);
    switch (pos) {
        case 0:
            return ldis(x,y,n->l,n->u);
        case 1:
            return y-n->u;
        case 2:
            return ldis(x,y,n->r,n->u);
        case 3:
            return n->l-x;
        case 4:
            return 0;
        case 5:
            return x-n->r;
        case 6:
            return ldis(x,y,n->l,n->b);
        case 7:
            return n->b-y;
        case 8:
            return ldis(x, y, n->r, n->b);
        default:
            cerr<<"hehe"<<endl;
            exit(-1);
    }
}
vector<int> search(int n,string qp,float x,float y,vector<string> words){
    vector<int> res;
    priority_queue<Qe> q;
    q.push(Qe(NODE,root,0,0));
    while(res.size()<n&&!q.empty()){
        Qe e = q.top();
        q.pop();
        if(e.type==OBJ){
            res.push_back(e.po);
        }else if(e.pn->is_leaf){
            for(int id:e.pn->obj_ids){
                q.push(Qe(OBJ,nullptr,id,ldis(x,y,objs[id].x,objs[id].y)));
            }
        }else{//inner node
            for(PRNode* p:e.pn->childs){
                string tree_pre = p->pre;
                if(tree_pre.substr(qp.size())==qp||qp.substr(tree_pre.length())==tree_pre){
                    q.push(Qe(NODE,p,0,dis(p,x,y)));
                }
            }
        }
    }
    return res;
}
int main(){
    try{
        string f = "/Users/zhuo.zhang/Projects/minimap/minidata";
        f = "/Users/zhuo.zhang/Projects/minimap/sample_data";
        build_index(f);
        vector<string> vs;
        vector<int> res = search(2,"p",40.5,-74.0,vs);
        for(int i: res){
            cout<<i<<endl;
        }
    }catch(const exception& e){
        cout<<e.what();
    }
}
