#ifndef _BUFFER
#define _BUFFER 
//用来管理block
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

//用来取出文件中的指定块，如果已经有不需要操作 
int getBlock(string table_name,int block_loc);

//返回总共有多少记录，从1开始 
int getRecordTotal(string table_name);
//返回index的根节点 
int getIndexRoot(string index_name);

void setRecordTotal(string table_name,int record_total);

void setIndexRoot(string index_name,int root);

void writeBackAll();

void clear(string table_name);

void clear_index(string index_name);

#endif
