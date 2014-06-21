#include<iostream>
#include<fstream>
#include<vector>
#include<cctype>
using namespace std;
struct obj{
	float x;
	float y;
	vector<string> ss;
	obj (string r){
		int l = r.size();
		int in_s = 0;
		int text_p = 0;
		for(int i=0;i<l;){
			if(r[i]=='"'){
				in_s = !in_s;
				if(in_s&&r.substr(i+1,4)=="addr"){
					text_p = 1;
					i += 6;
					in_s = 0;
				}else{
					i += 1;
				}
				continue;
			}
			if(in_s&&text_p){
				int j = i;
				while(1){
					char c = 
					if(
}

void build_index(string fname){
	ifstream ifs(fname);
{ "_id" : 1000010, "addr" : "Gillman Heights Condominium\n1A Gillman Heights, Singapore 100001", "fetdt" : { "$date" : 1345018964614 }, "latlng" : [ 1.281933, 103.80321 ], "name" : "Gillman Heights Condominium", "pcode" : 100001, "url" : "http://gothere.sg/maps#q:100001" }
{ "_id" : 1000440, "addr" : "44 Telok Blangah Drive\nSingapore 100044", "fetdt" : { "$date" : 1345018961499 }, "latlng" : [ 1.271687, 103.810585 ], "name" : "44 Telok Blangah Drive", "pcode" : 100044, "url" : "http://gothere.sg/maps#q:100044" }
{ "_id" : 1000450, "addr" : "45 Telok Blangah Drive\nSingapore 100045", "fetdt" : { "$date" : 1345018961534 }, "latlng" : [ 1.272116, 103.80951 ], "name" : "45 Telok Blangah Drive", "pcode" : 100045, "url" : "http://gothere.sg/maps#q:100045" }
{ "_id" : 1000460, "addr" : "46 Telok Blangah Drive\nSingapore 100046", "fetdt" : { "$date" : 1345018961538 }, "latlng" : [ 1.271791, 103.80994 ], "name" : "46 Telok Blangah Drive", "pcode" : 100046, "url" : "http://gothere.sg/maps#q:100046" }
{ "_id" : 1000470, "addr" : "47 Telok Blangah Drive\nSingapore 100047", "fetdt" : { "$date" : 1345018961540 }, "latlng" : [ 1.2721345, 103.81018 ], "name" : "47 Telok Blangah Drive", "pcode" : 100047, "url" : "http://gothere.sg/maps#q:100047" }
{ "_id" : 1000480, "addr" : "48 Telok Blangah Drive\nSingapore 100048", "fetdt" : { "$date" : 1345018961542 }, "latlng" : [ 1.272443, 103.81064 ], "name" : "48 Telok Blangah Drive", "pcode" : 100048, "url" : "http://gothere.sg/maps#q:100048" }
{ "_id" : 1000490, "addr" : "49 Telok Blangah Drive\nSingapore 100049", "fetdt" : { "$date" : 1345018961544 }, "latlng" : [ 1.272299, 103.81114 ], "name" : "49 Telok Blangah Drive", "pcode" : 100049, "url" : "http://gothere.sg/maps#q:100049" }
{ "_id" : 1000500, "addr" : "50 Telok Blangah Drive\nSingapore 100050", "fetdt" : { "$date" : 1345018961546 }, "latlng" : [ 1.2731825, 103.81023 ], "name" : "50 Telok Blangah Drive", "pcode" : 100050, "url" : "http://gothere.sg/maps#q:100050" }
{ "_id" : 1000510, "addr" : "51 Telok Blangah Drive\nSingapore 100051", "fetdt" : { "$date" : 1345018961548 }, "latlng" : [ 1.2736145, 103.81023 ], "name" : "51 Telok Blangah Drive", "pcode" : 100051, "url" : "http://gothere.sg/maps#q:100051" }
{ "_id" : 1000520, "addr" : "52 Telok Blangah Drive\nSingapore 100052", "fetdt" : { "$date" : 1345018961550 }, "latlng" : [ 1.2739435, 103.811325 ], "name" : "52 Telok Blangah Drive", "pcode" : 100052, "url" : "http://gothere.sg/maps#q:100052" }
