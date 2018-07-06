#ifndef _CATEGORY
#define _CATEGORY
//用来管理表和索引的结构定义
#include<iostream>
#include<string>
#include<fstream>
#include<windows.h>
using namespace std; 

//返回1意味着存在，返回0意味着不存在 
int table_find(string table_name);
int index_find(string index_name);
//创建该table 
void category_create_table(string table_name,int attribute_number,string attribute_name[],int attribute_type[],int attribute_limit[]);
//创建该索引 
void category_create_index(string index_name,string table_name,string attribute_name);
//删除该表 
void category_drop_table(string table_name);
//用来找到该表的所有index，存入index_name，并返回index数目 
int table_index_find(string table_name,string index_name[],string attribute_name[]);
//删掉该索引结构 
void category_drop_index(string index_name);
//根据表明返回表的各种属性，返回值为属性个数 
int get_attribute_list(string table_name,string initial_name[],int initial_type[],int initial_limit[]);
//判断该表是否有这个属性的索引 
string attribute_find_index(string table_name,string attribute_name);

#endif
