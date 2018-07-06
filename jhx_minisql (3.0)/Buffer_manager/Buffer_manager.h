#ifndef _BUFFER
#define _BUFFER 
//��������block
#include<iostream>
#include<fstream>
#include<string>
#include<windows.h>
using namespace std;

struct Block
{
	string file_name;
	int file_block_number;
	int dirty;
	int pin;
	int full;
	char buffer[4096];
};

//����ȡ���ļ��е�ָ���飬����Ѿ��в���Ҫ���� 
int getBlock(string table_name,int block_loc);

//�����ܹ��ж��ټ�¼����1��ʼ 
int getRecordTotal(string table_name);
//����index�ĸ��ڵ� 
int getIndexRoot(string index_name);

void setRecordTotal(string table_name,int record_total);

void setIndexRoot(string index_name,int root);

void writeBackAll();

void clear(string table_name);

void clear_index(string index_name);

#endif
