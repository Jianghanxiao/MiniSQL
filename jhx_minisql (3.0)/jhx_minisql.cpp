#include<iostream>
#include<string>
#include"Interpreter\interpreter.h"
using namespace std;
struct Block block[100];
struct B_node index_node[1000000];
string tree_index_name;
int tree_root;
int type;
int order[100]; 

int main()
{
	tree_index_name="";
	for(int i=0;i<=99;++i)
	{
		order[i]=i;
		block[i].full=0;
		block[i].dirty=0;
		block[i].pin=0;
	}
	for(int i=0;i<=99999;++i)
	{
		index_node[i].valid=0;
		index_node[i].is_leaf=0;
	}
	execute();
	return 0;
}
