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
#define M 4
using namespace std;
int pr_id;
struct Obj{
	double x;
	double y;
	vector<string> words;
	Obj (string s){
        smatch m;
		string ptn = "\"addr\" : \"([^\"]+)\".*\"latlng\" : \\[ (\\S+), (\\S+) \\], \"name\" : \"([^\"]*)\"";
        if(!regex_search(s,m,regex(ptn))){
            cout<<"no match"<<endl;
            cout<<s<<endl;
            exit(-1);
        }
        /*
         for(int i=0;i<m.size();i++){
         cout<<i<<" "<<m[i]<<endl;
         }*/
        y = stof(m[2]);
        x = stof(m[3]);
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
    int node_id;
    string pre;
    double b;
    double u;
    double l;
    double r;
    bool is_leaf;
    vector<int> obj_ids;
    vector<PRNode*> childs;
    PRNode(vector<int>& ids,string pre){
        node_id = pr_id++;
        //cout<<"build node "<<node_id<<endl;
        /*
        cout<<"build with "<<pre<<" and:"<<endl;
        for(int i=0;i<ids.size();i++){
            int id = ids[i];
            cout<<id<<" ";
            copy(objs[id].words.begin(),objs[id].words.end(),ostream_iterator<string>(cout," "));
            cout<<endl;
        }
        cout<<endl;
         */
        this->pre = pre;
        b = 90;
        u = -90;
        l = 180;
        r = -180;
        for(int i=0;i<ids.size();i++){
            int obj_id = ids[i];
            double x = objs[obj_id].x;
            double y = objs[obj_id].y;
            if(x<l)
                l = x;
            if(x>r)
                r = x;
            if(y<b)
                b = y;
            if(y>u)
                u = y;
        }
        if(ids.size()<=M){
            obj_ids = ids;
            is_leaf = true;
        }else{
            is_leaf = false;
            set<string> all_words;
            int charmap[256];
            vector<vector<int>> vvi;
            for(int i=0;i<256;i++){
                vvi.push_back(vector<int>());
            }
            memset(charmap, 0, sizeof(charmap));
            for(int i=0;i<ids.size();i++){
                int id = ids[i];
                unsigned long word_len = objs[id].words.size();
                for(int j=0;j<word_len;j++){
                    string w = objs[id].words[j];
                    int index = (unsigned int)((unsigned char)w[pre.length()]);
                    if(vvi[index].size()==0||vvi[index].back()!=id){
                        vvi[index].push_back(id);
                    }
                }
            }
            for(int i=0;i<256;i++){
                if(vvi[i].size()>0){
                    vector<vector<int>> vvi_inner;
                    for(int t=0;t<4;t++){
                        vvi_inner.push_back(vector<int>());
                    }
                    double x_mean = 0;
                    double y_mean = 0;
                    for(int index_text_node=0;index_text_node<vvi[i].size();index_text_node++){
                        int obj_id = vvi[i][index_text_node];
                        double x = objs[obj_id].x;
                        double y = objs[obj_id].y;
                        x_mean += x;
                        y_mean += y;

                    }
                    x_mean/=vvi[i].size();
                    y_mean/=vvi[i].size();
                    for(int j=0;j<vvi[i].size();j++){
                        int n_id = vvi[i][j];
                        double n_x = objs[n_id].x;
                        double n_y = objs[n_id].y;
                        int pos = 2*(int(n_y<y_mean))+(int(n_x>x_mean));
                        vvi_inner[pos].push_back(n_id);
                    }
                    string new_pre = pre;
                    new_pre.append(1,(char)((unsigned char)i));
                    for(int f=0;f<4;f++){
                        if(vvi_inner[f].size()>0){
                            PRNode* new_node = new PRNode(vvi_inner[f],new_pre);
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
    double dis;
    Qe(int type,PRNode* pn,int po,double dis){
        this->type = type;
        this->pn = pn;
        this->po = po;
        this->dis = dis;
    }
    bool operator< (const Qe& b) const{
        /*
        if(type==NODE){
            cout<<"I am NODE "<<pn->node_id<<endl;
        }else{
            cout<<"I am OBJ "<<po<<endl;
        }
        cout<<dis<<endl;
        if(b.type==NODE){
            cout<<"It is NODE "<<b.pn->node_id<<endl;
        }else{
            cout<<"It is OBJ "<<b.po<<endl;
        }
        cout<<b.dis<<endl;
         */
        return this->dis>b.dis;
    }
};
PRNode* root;

void dump_tree(int level,PRNode* p){
    string indent;
    indent.append(level,' ');
    cout<<indent<<"Level "<<level<<endl;
    cout<<indent<<(p->is_leaf?"Leaf":"Inner")<<" ID "<<p->node_id<<endl;
    cout<<indent<<p->b<<" "<<p->u<<" "<<p->l<<" "<<p->r<<" "<<p->pre<<endl;
    if(p->is_leaf){
        for(int i=0;i<p->obj_ids.size();i++){
            int id = p->obj_ids[i];
            cout<<indent<<id<<" ";
            copy(objs[id].words.begin(),objs[id].words.end(),ostream_iterator<string>(cout," "));
            cout<<endl;
        }
    }else{
        for(int i=0;i<p->childs.size();i++){
            dump_tree(level+1, p->childs[i]);
        }
    }
    cout<<endl;
}

void build_index(string fname){
    ifstream ifs(fname);
    string line;
    while(getline(ifs,line)){
        objs.push_back(Obj(line));
    }
    cout<<"read finished"<<endl;
    auto obj_size = objs.size();
    vector<int> init_obj_ids;
    for(int i=0;i<obj_size;i++){
        init_obj_ids.push_back(i);
    }
    root = new PRNode(init_obj_ids,"");
    dump_tree(0, root);
    exit(-1);
}
inline double ldis(double x1,double y1,double x2,double y2){
    return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
double dis(PRNode *n,double x,double y){
    int pos = 3*(int(y<n->u)+int(y<n->b))+(int(x>n->r)+int(x>n->l));
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
vector<int> search(int n,string qp,double x,double y,vector<string> words){
    vector<int> res;
    priority_queue<Qe> q;
    q.push(Qe(NODE,root,0,0));
    while(res.size()<n&&!q.empty()){
        Qe e = q.top();
        q.pop();
        if(e.type==OBJ){
            //cout<<"pop OBJ "<<e.po<<" "<<e.dis<<endl;
            res.push_back(e.po);
        }else{
            //cout<<"pop NODE "<<e.pn->node_id<<" "<<e.dis<<endl;
            if(e.pn->is_leaf){
                for(int id:e.pn->obj_ids){
                    double dd = ldis(x,y,objs[id].x,objs[id].y);
                    //cout<<"pushing OBJ "<<id<<" dis "<<dd<<endl;
                    q.push(Qe(OBJ,nullptr,id,dd));
                }
            }else{//inner node
                for(PRNode* p:e.pn->childs){
                    string tree_pre = p->pre;
                    if(tree_pre.substr(0,qp.size())==qp||qp.substr(0,tree_pre.length())==tree_pre){
                        double d = dis(p,x,y);
                        //cout<<"pushing NODE "<<p->node_id<<" dis "<<d<<endl;
                        q.push(Qe(NODE,p,0,d));
                    }
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
        //f = "/Users/zhuo.zhang/Projects/minimap/zipcode-address.json";
        f = "/Users/zhuo.zhang/Projects/minimap/300data";
         f = "/Users/zhuo.zhang/Projects/minimap/30data";
        build_index(f);
        cout<<"build index finished"<<endl;
        /*
        while(1){
        string in_pre;
        cout<<"prefix:";
        cin>>in_pre;
        string in_x;
        cout<<"x";
        cin>>in_x;
        */
        double q_x = -74.0;
        double q_y = 40.5;
        q_x = 103.811516;
        q_y = 1.2744;
        vector<string> vs;
        string q_p = "Blangah";
        string data = "70 Telok Blangah Heights Singapore 100070";

        vector<int> res = search(10,q_p,q_x,q_y,vs);
        for(int i: res){
            cout<<i<<" "<<ldis(objs[i].x,objs[i].y,q_x,q_y)<<endl;
            copy(objs[i].words.begin(),objs[i].words.end(),ostream_iterator<string>(cout," "));
            cout<<endl;
        }
    }catch(const exception& e){
        cout<<e.what();
    }
}
