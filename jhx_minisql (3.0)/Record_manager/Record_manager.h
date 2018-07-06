#ifndef _RECORD
#define _RECORD
//用来管理所有记录
#include<iostream>
#include<iomanip>
#include<string>
#include<windows.h>
#include<fstream>
#include"..\Buffer_manager\Buffer_manager.h"
#include"..\Category_manager\Category_manager.h" 
#include"..\Index_manager\Index_manager.h"
using namespace std;


//删掉这个表
void record_drop_table(string table_name);
//选择table中的满足条件的record 
int record_select(string table_name,int initial_number,string initial_name[],int initial_type[],int limit_number,string limit_attribute[],string limit_op[],string limit_value[]);
//向table中插入record 
int record_insert(string table_name,int initial_number,string initial_name[],int initial_type[],int initial_limit[],string values[]);
//删除table中满足条件的record 
int record_deletion(string table_name,int initial_number,string initial_name[],int initial_type[],int limit_number,string limit_attribute[],string limit_op[],string limit_value[]);

void wBA();

#endif
