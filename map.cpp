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
#include<unordered_map>
#include<cstdlib>
#define NODE 1
#define OBJ 2
#define M 10
#define SDBG 0
using namespace std;
char py_buf[8192];
unordered_map<string,vector<int>> stov;
int pr_id;
struct Obj{
	double x;
	double y;
	vector<string> words;
	Obj (string s);
	friend ostream& operator<<(ostream& os,const Obj& o){
		os<<"{\"x\":"<<o.x<<",\"y\":"<<o.y<<",\"text\":\"";
		copy(o.words.begin(),o.words.end(),ostream_iterator<string>(os," "));
		os<<"\"}";
		return os;
	}
};
vector<Obj> objs;
Obj::Obj (string s){
	int my_id = int(objs.size());
	smatch m;
	string ptn = "\"addr\" : \"([^\"]+)\".*\"latlng\" : \\[ (\\S+), (\\S+) \\], \"name\" : \"([^\"]*)\"";
	if(!regex_search(s,m,regex(ptn))){
		cout<<"no match"<<endl;
		cout<<s<<endl;
		exit(-1);
	}
	y = stof(m[2]);
	x = stof(m[3]);
	stringstream ss(string(m[1]).substr(string(m[4]).length()+2)+" "+string(m[4]));
	string word;
	while(ss>>word){
		word.erase(0,word.find_first_not_of("("));
		word.erase(word.find_last_not_of(",)")+1);
		if(word=="")
			continue;
		if(isdigit(word[0])&&word.length()>=6)
			continue;
		transform(word.begin(),word.end(),word.begin(),::tolower);
		if(stov.find(word)==stov.end()){
			vector<int> vi;
			vi.push_back(my_id);
			stov[word]=vi;
		}else{
			if(stov[word].back()!=my_id){
				stov[word].push_back(my_id);
			}
		}
		words.push_back(word);
	}
	sort(words.begin(),words.end());
	words.resize(distance(words.begin(),unique(words.begin(),words.end())));
}
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
    PRNode* parent;
	PRNode(vector<int>& ids,string pre,PRNode* parent){
		this->parent = parent;
		node_id = pr_id++;
        int depth = 0;//0 for root
        PRNode* cur = this;
        while(cur->parent!=nullptr){
            depth++;
            cur = cur->parent;
        }
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
		if(ids.size()<=M||depth>20){
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
					if(w.substr(0,pre.length())!=pre){
						continue;
					}
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
					if(i!=0){
						new_pre.append(1,(char)((unsigned char)i));
					}
					for(int f=0;f<4;f++){
						if(vvi_inner[f].size()>0){
							PRNode* new_node = new PRNode(vvi_inner[f],new_pre,this);
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
	int type;//NODE 1 OBJ 2
	PRNode* pn;
	int po;
	double dis;
	Qe(int type,PRNode* pn,int po,double dis){
        this->type = type;
		this->pn = pn;
		this->po = po;
		this->dis = dis;
	}
	bool operator< (const Qe& b) const{
		return this->dis>b.dis;
	}
};
PRNode* root;

void dump_tree(int level,PRNode* p){
	
    string indent;
    indent.append(level,' ');
    cout<<indent<<"Level "<<level<<endl;
    cout<<indent<<(p->is_leaf?"Leaf":"Inner")<<" ID "<<p->node_id<<" P "<<p->parent<<endl;
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
vector<pair<string,int>> stat_str(){
	unordered_map<string,int> freq;
	vector<pair<string,int>> freq_v;
	for(int i=0;i<objs.size();i++){
		for(int j=0;j<objs[i].words.size();j++){
			string w = objs[i].words[j];
			auto it = freq.find(w);
			if(it==freq.end()){
				freq[w]=1;
			}else{
				freq[w]+=1;
			}
		}
	}
	copy(freq.begin(),freq.end(),back_inserter(freq_v));
	sort(freq_v.begin(),freq_v.end(),[](const pair<string,int>& a,const pair<string,int>& b)->bool{
		return a.second>b.second;
	});
	return freq_v;
}
void build_index(string fname){
	ifstream ifs(fname);
	string line;
	while(getline(ifs,line)){
		objs.push_back(Obj(line));
	}
	auto obj_size = objs.size();
	vector<int> init_obj_ids;
	for(int i=0;i<obj_size;i++){
		init_obj_ids.push_back(i);
	}
	root = new PRNode(init_obj_ids,"",nullptr);
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
			exit(-1);
	}
}
vector<int> search(int n,string qp,double x,double y,vector<string> words){
	vector<int> inter;
	vector<int> res;
	if(words.size()>0){
		inter = stov[words[0]];
		for(int i=1;i<words.size();i++){
			vector<int> buf;
			set_intersection(inter.begin(),inter.end(),stov[words[i]].begin(),stov[words[i]].end(),
                             back_inserter(buf));
			inter = buf;
		}
	}
	priority_queue<Qe> q;
    auto seed = Qe(NODE,root,0,0);
	q.push(seed);
	while(res.size()<n&&!q.empty()){
		Qe e = q.top();
		q.pop();
		if(e.type==OBJ){
            if(SDBG){
                cout<<"pop OBJ "<<e.po<<" "<<e.dis<<endl;
            }
			res.push_back(e.po);
		}else{
            if(SDBG){
                cout<<"pop NODE "<<e.pn->node_id<<" "<<e.dis<<endl;
            }
			if(e.pn->is_leaf){
				for(int id:e.pn->obj_ids){
					bool exist = true;
					if(words.size()>0){
						exist = binary_search(inter.begin(),inter.end(),id);
					}
                    if(exist==false){
						continue;
					}
                    exist = false;
                    for(int ii=0;ii<objs[id].words.size();ii++){
                        string word_pre = objs[id].words[ii];
                        if(word_pre.substr(0,qp.size())==qp){
                            exist = true;
                            break;
                        }
                    }
                    if(exist==false){
						continue;
					}
					double dd = ldis(x,y,objs[id].x,objs[id].y);
                    if(SDBG){
                        cout<<"pushing OBJ "<<id<<" dis "<<dd<<endl;
                    }
					q.push(Qe(OBJ,nullptr,id,dd));
				}
			}else{//inner node
				for(PRNode* p:e.pn->childs){
					string tree_pre = p->pre;
					if(tree_pre.substr(0,qp.size())==qp||qp.substr(0,tree_pre.length())==tree_pre){
						double d = dis(p,x,y);
                        if(SDBG){
                            cout<<"pushing NODE "<<p->node_id<<" dis "<<d<<" pre "<<tree_pre<<endl;
                        }
						q.push(Qe(NODE,p,0,d));
					}
				}
			}
		}
	}
	return res;
}
void output_res(vector<int> res,double x,double y){
	for(int i: res){
		cout<<i<<" "<<ldis(objs[i].x,objs[i].y,x,y)<<endl;
		copy(objs[i].words.begin(),objs[i].words.end(),ostream_iterator<string>(cout," "));
		cout<<endl;
	}
}
void search_and_output(int n,string qp,double x,double y,vector<string> words){
	vector<int> res = search(n,qp,x,y,words);
	output_res(res,x,y);
}
vector<int> query(int n,double x,double y,string raw_words){
	vector<int> res;
	stringstream ss(raw_words);
	string word;
	vector<string> vs;
	while(ss>>word){
		transform(word.begin(),word.end(),word.begin(),::tolower);
		vs.push_back(word);
	}
	if(vs.size()==0){
		return res;
	}
	string qp = vs.back();
	vs.pop_back();
	res = search(n,qp,x,y,vs);
	return res;
    
}
extern "C" {
    int init(){
        string f = "zipcode-address.json";
        build_index(f);
        return 0;
    }
    char* json_query(int n,double x,double y,const char* craw_words){
    	try{
	        string raw_words = craw_words;
	        vector<int> res = query(n,x,y,raw_words);
	        stringstream ss;
	        ss<<"{\"data\":[";
	        for(int i=0;i<res.size();i++){
	            if(i!=0){
	                ss<<",";
	            }
	            ss<<objs[res[i]];
	        }
	        ss<<"]}"<<endl;
	        strcpy(py_buf,ss.str().c_str());
	        return py_buf;
        }catch(const exception& e){
        	stringstream ss;
        	string err = "{\"data\":[],\"err\":\"";
			ss<<err<<e.what()<<"\"}";
        	strcpy(py_buf,ss.str().c_str());
        	return py_buf;
		}
    }
}