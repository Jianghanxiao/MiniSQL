#ifndef _RECORD
#define _RECORD
//�����������м�¼
#include<iostream>
#include<iomanip>
#include<string>
#include<windows.h>
#include<fstream>
#include"..\Buffer_manager\Buffer_manager.h"
#include"..\Category_manager\Category_manager.h" 
#include"..\Index_manager\Index_manager.h"
using namespace std;


//ɾ�������
void record_drop_table(string table_name);
//ѡ��table�е�����������record 
int record_select(string table_name,int initial_number,string initial_name[],int initial_type[],int limit_number,string limit_attribute[],string limit_op[],string limit_value[]);
//��table�в���record 
int record_insert(string table_name,int initial_number,string initial_name[],int initial_type[],int initial_limit[],string values[]);
//ɾ��table������������record 
int record_deletion(string table_name,int initial_number,string initial_name[],int initial_type[],int limit_number,string limit_attribute[],string limit_op[],string limit_value[]);

void wBA();

#endif
