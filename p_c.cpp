#include<iostream>
#ifdef __cplusplus
extern "C" {
#endif 
using namespace std;
void initp_c(){
	cout<<"init"<<endl;
}
int add(int i,int j){
	return i*j;
}
#ifdef __cplusplus
}
#endif
