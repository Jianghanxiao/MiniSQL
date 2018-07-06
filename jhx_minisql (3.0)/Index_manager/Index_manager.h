#ifndef _INDEX
#define _INDEX
//用来管理所有索引 
#include<iostream>
#include<iomanip>
#include<sstream>
#include<string>
#include<windows.h>
#include<fstream>
#include"..\Buffer_manager\Buffer_manager.h"
#include"..\Category_manager\Category_manager.h" 
using namespace std;

struct B_node//3值4指针节点 
{
	char valid;//'0'代表没有,'1'代表有 
	char is_leaf;//'0'非叶节点,'1'叶节点
	int number;//当前实际存了几个值 
	int father;//父节点的数
	int left_sib;//左兄弟
	int right_sib;//右兄弟
	int child[4];
	string child_value[3];
	int child_value_loc[3]; 
};

//删掉该索引 
void index_drop_index(string index_name);
//通过索引进行select操作 
int index_select(string table_name,string index_name,int initial_number,string initial_name[],int initial_type[],int limit_number,string limit_attribute[],string limit_op[],string limit_value[]);
//插入索引中一个新的值以及他所指向的record的行数 
void index_insert(string table_name,string index_name,string attribute_name,int initial_type,string values,int record_loc);
//通过索引进行delete操作 
int index_deletion(string table_name,string index_name,int initial_number,string initial_name[],int initial_type[],int limit_number,string limit_attribute[],string limit_op[],string limit_value[]);
//建立索引（对之前的值建立索引） 
void index_create(string table_name,int initial_number,string initial_name[],int initial_type[],string index_name,string attribute_name);
//将所有的索引的树写回去 
void wIA(); 
#endif
