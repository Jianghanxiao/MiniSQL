#ifndef _INDEX
#define _INDEX
//���������������� 
#include<iostream>
#include<iomanip>
#include<sstream>
#include<string>
#include<windows.h>
#include<fstream>
#include"..\Buffer_manager\Buffer_manager.h"
#include"..\Category_manager\Category_manager.h" 
using namespace std;

struct B_node//3ֵ4ָ��ڵ� 
{
	char valid;//'0'����û��,'1'������ 
	char is_leaf;//'0'��Ҷ�ڵ�,'1'Ҷ�ڵ�
	int number;//��ǰʵ�ʴ��˼���ֵ 
	int father;//���ڵ����
	int left_sib;//���ֵ�
	int right_sib;//���ֵ�
	int child[4];
	string child_value[3];
	int child_value_loc[3]; 
};

//ɾ�������� 
void index_drop_index(string index_name);
//ͨ����������select���� 
int index_select(string table_name,string index_name,int initial_number,string initial_name[],int initial_type[],int limit_number,string limit_attribute[],string limit_op[],string limit_value[]);
//����������һ���µ�ֵ�Լ�����ָ���record������ 
void index_insert(string table_name,string index_name,string attribute_name,int initial_type,string values,int record_loc);
//ͨ����������delete���� 
int index_deletion(string table_name,string index_name,int initial_number,string initial_name[],int initial_type[],int limit_number,string limit_attribute[],string limit_op[],string limit_value[]);
//������������֮ǰ��ֵ���������� 
void index_create(string table_name,int initial_number,string initial_name[],int initial_type[],string index_name,string attribute_name);
//�����е���������д��ȥ 
void wIA(); 
#endif
