#ifndef _API
#define _API
//用作解释器以及下面模块的接口 
#include<iostream>
#include<iomanip>
#include<string>
#include"..\Category_manager\Category_manager.h" 
#include"..\Record_manager\Record_manager.h"
#include"..\Index_manager\Index_manager.h"
using namespace std;

void create_table(string table_name,int attribute_number,string attribute_name[],int attribute_type[],int attribute_limit[]);

void create_index(string index_name,string table_name,string attribute_name);

void drop_table(string table_name);

void drop_index(string index_name);

void select(string table_name,int limit_number,string limit_attribute[],string limit_op[],string limit_value[]);

void insert(string table_name,int values_number,string values[]);

void deletion(string table_name,int limit_number,string limit_attribute[],string limit_op[],string limit_value[]);

void quit();

#endif
