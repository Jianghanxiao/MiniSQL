#ifndef _CATEGORY
#define _CATEGORY
//���������������Ľṹ����
#include<iostream>
#include<string>
#include<fstream>
#include<windows.h>
using namespace std; 

//����1��ζ�Ŵ��ڣ�����0��ζ�Ų����� 
int table_find(string table_name);
int index_find(string index_name);
//������table 
void category_create_table(string table_name,int attribute_number,string attribute_name[],int attribute_type[],int attribute_limit[]);
//���������� 
void category_create_index(string index_name,string table_name,string attribute_name);
//ɾ���ñ� 
void category_drop_table(string table_name);
//�����ҵ��ñ������index������index_name��������index��Ŀ 
int table_index_find(string table_name,string index_name[],string attribute_name[]);
//ɾ���������ṹ 
void category_drop_index(string index_name);
//���ݱ������ر�ĸ������ԣ�����ֵΪ���Ը��� 
int get_attribute_list(string table_name,string initial_name[],int initial_type[],int initial_limit[]);
//�жϸñ��Ƿ���������Ե����� 
string attribute_find_index(string table_name,string attribute_name);

#endif
