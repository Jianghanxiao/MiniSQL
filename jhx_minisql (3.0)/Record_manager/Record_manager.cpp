#include"Record_manager.h"
extern struct Block block[100];
extern struct B_node index_node[100000];
extern string tree_index_name;
extern int tree_root;
extern int type;

int pan_int(int temp_int,string op,string value);
int pan_float(float temp_float,string op,string value);
int pan_string(string temp_string,string op,string value);
int zhuan_int_record(string value);
float zhuan_float_record(string value);
int pan_cun(string table_name,int initial_number,string initial_name[],int initial_type[],int limit_number,string limit_attribute[],string limit_op[],string limit_value[]);
void record_write_tree_back();
int record_zhuan_int_index(string value);
float record_zhuan_float_index(string value);

void wBA()
{
	writeBackAll();
	return;
}

void record_drop_table(string table_name)
{
	DeleteFile((table_name+".data").c_str());
	clear(table_name);
	return;
}

int pan_cun(string table_name,int initial_number,string initial_name[],int initial_type[],int limit_number,string limit_attribute[],string limit_op[],string limit_value[])
{
	int record_size=0;
	for(int i=0;i<=initial_number;++i)
	{
		if(initial_type[i]==0) record_size+=4;
		else if(initial_type[i]==-1) record_size+=4;
		else record_size+=initial_type[i];
	}
	record_size=record_size+1;//一位valid_bit,其他属性的大小
	int block_record_number;
	block_record_number=4096/record_size;
	int record_total;
	record_total=getRecordTotal(table_name);
	int offset;
	int record_loc=0;
	int block_loc;
	int ff;
	ff=1;
	while(record_loc<record_total)
	{
		block_loc=record_loc/block_record_number;
		offset=(record_loc-block_loc*block_record_number)*record_size;
		int buffer_block_number;
		buffer_block_number=getBlock(table_name,block_loc);
		char* p;
		char* temp_p;
		p=new char[record_size];
		memcpy(p,block[buffer_block_number].buffer+offset,record_size);
		if(p[0]!='0')
		{
			temp_p=p+1;
			char *temp_value[32];
			for(int i=0;i<=31;++i) temp_value[i]=NULL;
			for(int i=0;i<=initial_number;++i)
			{
				if(initial_type[i]==0)
				{
					temp_value[i]=new char[4];
					memcpy(temp_value[i],temp_p,4);
					temp_p+=4;
				}
				else if(initial_type[i]==-1)
				{
					temp_value[i]=new char[4];
					memcpy(temp_value[i],temp_p,4);
					temp_p+=4;
				}
				else
				{
					temp_value[i]=new char[initial_type[i]+1];
					memcpy(temp_value[i],temp_p,initial_type[i]);
					temp_p+=initial_type[i];
				}
			}
			int flag=1;
			for(int i=0;i<=limit_number;++i)
			{
				for(int j=0;j<=initial_number;++j)
				{
					if(limit_attribute[i]==initial_name[j])
					{
						if(initial_type[j]==0)
	 					{
	 						int temp_int;
	 						memcpy(&temp_int,temp_value[j],4);
	 						flag=pan_int(temp_int,limit_op[i],limit_value[i]);
						}
						else if(initial_type[j]==-1)
						{
							float temp_float;
	 						memcpy(&temp_float,temp_value[j],4);
	 						flag=pan_float(temp_float,limit_op[i],limit_value[i]);
						}
						else
						{
							temp_value[j][initial_type[j]]=0;
							string temp_string=temp_value[j];
							if(pan_string(temp_string,limit_op[i],limit_value[i])==0) flag=0;
						}
						break;
					}
				}
				if(flag==0) break;
 			}
 			if(flag==1)
 			{
 				return 1;
			}
			for(int i=0;i<=31;++i)
			{
				if(temp_value[i]!=NULL)
				delete temp_value[i];
			}
		}
		delete p;
		record_loc++;
	}
	return 0;
}

int record_select(string table_name,int initial_number,string initial_name[],int initial_type[],int limit_number,string limit_attribute[],string limit_op[],string limit_value[])
{
	int record_size=0;
	for(int i=0;i<=initial_number;++i)
	{
		if(initial_type[i]==0) record_size+=4;
		else if(initial_type[i]==-1) record_size+=4;
		else record_size+=initial_type[i];
	}
	record_size=record_size+1;//一位valid_bit,其他属性的大小
	int block_record_number;
	block_record_number=4096/record_size;
	int record_total;
	record_total=getRecordTotal(table_name);
	int offset;
	int record_loc=0;
	int block_loc;
	int ff;
	for(int i=0;i<=limit_number;++i)
	{
		for(int j=0;j<=initial_number;++j)
		{
			if(limit_attribute[i]==initial_name[j])
			{
				if(initial_type[j]==0)
				{
					ff=pan_int(0,limit_op[j],limit_value[i]);
					if(ff==-1) 
					{
						cout<<"Value error"<<endl;
						return 0;	
					}
				}
				else if(initial_type[j]==-1)
				{
					ff=pan_float(0,limit_op[j],limit_value[i]);
					if(ff==-1) 
					{
						cout<<"Value error"<<endl;
						return 0;	
					}
				}
			}
		}
	}
	for(int i=0;i<=initial_number;++i)
	{
		cout<<setw(10)<<initial_name[i]<<" ";
	}
	cout<<endl;
	ff=1;
	while(record_loc<record_total)
	{
		block_loc=record_loc/block_record_number;
		offset=(record_loc-block_loc*block_record_number)*record_size;
		int buffer_block_number;
		buffer_block_number=getBlock(table_name,block_loc);
		char* p;
		char* temp_p;
		p=new char[record_size];
		memcpy(p,block[buffer_block_number].buffer+offset,record_size);
		if(p[0]!='0')
		{
			temp_p=p+1;
			char *temp_value[32];
			for(int i=0;i<=31;++i) temp_value[i]=NULL;
			for(int i=0;i<=initial_number;++i)
			{
				if(initial_type[i]==0)
				{
					temp_value[i]=new char[4];
					memcpy(temp_value[i],temp_p,4);
					temp_p+=4;
				}
				else if(initial_type[i]==-1)
				{
					temp_value[i]=new char[4];
					memcpy(temp_value[i],temp_p,4);
					temp_p+=4;
				}
				else
				{
					temp_value[i]=new char[initial_type[i]+1];
					memcpy(temp_value[i],temp_p,initial_type[i]);
					temp_p+=initial_type[i];
				}
			}
			int flag=1;
			for(int i=0;i<=limit_number;++i)
			{
				for(int j=0;j<=initial_number;++j)
				{
					if(limit_attribute[i]==initial_name[j])
					{
						if(initial_type[j]==0)
	 					{
	 						int temp_int;
	 						memcpy(&temp_int,temp_value[j],4);
	 						flag=pan_int(temp_int,limit_op[i],limit_value[i]);
	 						if(flag==-1) 
	 						{
	 							cout<<"Value error"<<endl;
								return 0;	
							}
						}
						else if(initial_type[j]==-1)
						{
							float temp_float;
	 						memcpy(&temp_float,temp_value[j],4);
	 						flag=pan_float(temp_float,limit_op[i],limit_value[i]);
	 						if(flag==-1) 
	 						{
	 							cout<<"Value error"<<endl;
								return 0;	
							}
						}
						else
						{
							temp_value[j][initial_type[j]]=0;
							string temp_string=temp_value[j];
							if(pan_string(temp_string,limit_op[i],limit_value[i])==0) flag=0;
						}
						break;
					}
				}
				if(flag==0) break;
 			}
 			if(flag==1)
 			{
 				for(int i=0;i<=initial_number;++i)
 				{
 					if(initial_type[i]==0)
 					{
 						int temp_int;
	 					memcpy(&temp_int,temp_value[i],4);
	 					cout<<setw(10)<<temp_int<<" ";
					}
					else if(initial_type[i]==-1)
					{
						float temp_float;
	 					memcpy(&temp_float,temp_value[i],4);
	 					cout<<setw(10)<<temp_float<<" ";
					}
					else
					{
						temp_value[i][initial_type[i]]=0;
						string temp_string=temp_value[i];
						cout<<setw(10)<<temp_string<<" ";
					}
				}
				cout<<endl;
			}
			for(int i=0;i<=31;++i)
			{
				if(temp_value[i]!=NULL)
				delete temp_value[i];
			}
		}
		delete p;
		record_loc++;
	}
	return 1;
}

int record_insert(string table_name,int initial_number,string initial_name[],int initial_type[],int initial_limit[],string values[])
{
	for(int i=0;i<=initial_number;++i)
	{
		if(initial_type[i]==0)
		{
			int temp_int;
			temp_int=zhuan_int_record(values[i]);
			if(temp_int==2147483647) 
			{
				cout<<"Value error"<<endl;
				return -1;
			}
		}
		else if(initial_type[i]==-1)
		{
			float temp_float;
			temp_float=zhuan_float_record(values[i]);
			if(temp_float==7.41) 
			{
				cout<<"Value error"<<endl;
				return -1;
			}
		}
	}
	int ff;
	for(int i=0;i<=initial_number;++i) 
	{
		if(initial_limit[i]!=0)
		{
			string limit_attribute[32];
			limit_attribute[0]=initial_name[i];
			string limit_op[32];
			limit_op[0]="=";
			string limit_value[32];
			limit_value[0]=values[i];
			ff=pan_cun(table_name,initial_number,initial_name,initial_type,0,limit_attribute,limit_op,limit_value);
			if(ff==1)
			{
				cout<<"Multiple error"<<endl;
				return -1;
			}	
		}
	}
	int record_size=0;
	for(int i=0;i<=initial_number;++i)
	{
		if(initial_type[i]==0) record_size+=4;
		else if(initial_type[i]==-1) record_size+=4;
		else record_size+=initial_type[i];
	}
	record_size=record_size+1;//一位valid_bit,其他属性的大小
	int block_record_number;
	block_record_number=4096/record_size;
	int record_total;
	record_total=getRecordTotal(table_name);
	int record_loc;
	record_loc=record_total;
	setRecordTotal(table_name,record_total+1);
	int block_loc;
	int offset;
	block_loc=record_loc/block_record_number;
	offset=(record_loc-block_loc*block_record_number)*record_size;
	int buffer_block_number;
	buffer_block_number=getBlock(table_name,block_loc);
	block[buffer_block_number].dirty=1;
	char* p=block[buffer_block_number].buffer+offset;
	char pp='1';
	char* ppp=&pp;
	memcpy(p,ppp,1);//存进去valid_bit 
	p=p+1;
	for(int i=0;i<=initial_number;++i)
	{
		if(initial_type[i]==0)
		{
			int temp_int;
			temp_int=zhuan_int_record(values[i]);
			if(temp_int==2147483647) 
			{
				cout<<"Value error"<<endl;
				return -1;
			}
			int* temp_int_p=&temp_int;
			memcpy(p,temp_int_p,4);
			p=p+4;
		}
		else if(initial_type[i]==-1)
		{
			float temp_float;
			temp_float=zhuan_float_record(values[i]);
			if(temp_float==7.41) 
			{
				cout<<"Value error"<<endl;
				return -1;
			}
			float* temp_float_p=&temp_float;
			memcpy(p,temp_float_p,4);
			p=p+4;
		}
		else
		{
			memcpy(p,values[i].c_str(),initial_type[i]);
			p=p+initial_type[i];
		}
	}
	return record_total;
}

int record_deletion(string table_name,int initial_number,string initial_name[],int initial_type[],int limit_number,string limit_attribute[],string limit_op[],string limit_value[])
{
	int record_size=0;
	for(int i=0;i<=initial_number;++i)
	{
		if(initial_type[i]==0) record_size+=4;
		else if(initial_type[i]==-1) record_size+=4;
		else record_size+=initial_type[i];
	}
	record_size=record_size+1;//一位valid_bit,其他属性的大小
	int block_record_number;
	block_record_number=4096/record_size;
	int record_total;
	record_total=getRecordTotal(table_name);
	int offset;
	int record_loc=0;
	int block_loc;
	while(record_loc<record_total)
	{
		block_loc=record_loc/block_record_number;
		offset=(record_loc-block_loc*block_record_number)*record_size;
		int buffer_block_number;
		buffer_block_number=getBlock(table_name,block_loc);
		char* p;
		char* temp_p;
		p=new char[record_size];
		memcpy(p,block[buffer_block_number].buffer+offset,record_size);
		if(p[0]!='0')
		{
			temp_p=p+1;
			char *temp_value[32];
			for(int i=0;i<=31;++i) temp_value[i]=NULL;
			for(int i=0;i<=initial_number;++i)
			{
				if(initial_type[i]==0)
				{
					temp_value[i]=new char[4];
					memcpy(temp_value[i],temp_p,4);
					temp_p+=4;
				}
				else if(initial_type[i]==-1)
				{
					temp_value[i]=new char[4];
					memcpy(temp_value[i],temp_p,4);
					temp_p+=4;
				}
				else
				{
					temp_value[i]=new char[initial_type[i]+1];
					memcpy(temp_value[i],temp_p,initial_type[i]);
					temp_p+=initial_type[i];
				}
			}
			int flag=1;
			for(int i=0;i<=limit_number;++i)
			{
				for(int j=0;j<=initial_number;++j)
				{
					if(limit_attribute[i]==initial_name[j])
					{
						if(initial_type[j]==0)
	 					{
	 						int temp_int;
	 						memcpy(&temp_int,temp_value[j],4);
	 						flag=pan_int(temp_int,limit_op[i],limit_value[i]);
	 						if(flag==-1) 
	 						{
	 							cout<<"Value error"<<endl;
								return 0;	
							}
						}
						else if(initial_type[j]==-1)
						{
							float temp_float;
	 						memcpy(&temp_float,temp_value[j],4);
	 						flag=pan_float(temp_float,limit_op[i],limit_value[i]);
	 						if(flag==-1) 
	 						{
	 							cout<<"Value error"<<endl;
								return 0;	
							}
						}
						else
						{
							temp_value[j][initial_type[j]]=0;
							string temp_string=temp_value[j];
							if(pan_string(temp_string,limit_op[i],limit_value[i])==0) flag=0;
						}
						break;
					}
				}
				if(flag==0) break;
 			}
 			if(flag==1)
 			{
 				char pp='0';
 				char* ppp=&pp;
 				memcpy(block[buffer_block_number].buffer+offset,ppp,1);
 				block[buffer_block_number].dirty=1;
 				string temp_attribute_name[32];
				string temp_index_name[32];
				int temp_index_number;
				temp_index_number=table_index_find(table_name,temp_index_name,temp_attribute_name);
				for(int i=0;i<=temp_index_number;++i)
				{
					if(tree_index_name!=temp_index_name[i])
					{
						if(tree_index_name!="") record_write_tree_back;
						DeleteFile((temp_index_name[i]+".data").c_str());
						tree_index_name=temp_index_name[i];
						for(int j=0;j<=initial_number;++j)
						{
							if(temp_attribute_name[i]==initial_name[j])
							{
								type=initial_type[j];
							}
						}
						tree_root=0;
						for(int j=0;j<=99999;++j)
						{
							index_node[j].valid=0;
							index_node[j].is_leaf=0;
							index_node[j].number=0;
							index_node[j].father=-1;
							index_node[j].left_sib=-1;
							index_node[j].right_sib=-1;
							for(int k=0;k<=3;++k) index_node[j].child[k]=-1;
						}
						index_node[0].valid=1;
						index_node[0].is_leaf=1;
					}
					else
					{
						DeleteFile((temp_index_name[i]+".data").c_str());
						tree_index_name=temp_index_name[i];
						for(int j=0;j<=initial_number;++j)
						{
							if(temp_attribute_name[i]==initial_name[j])
							{
								type=initial_type[j];
							}
						}
						tree_root=0;
						for(int j=0;j<=99999;++j)
						{
							index_node[j].valid=0;
							index_node[j].is_leaf=0;
							index_node[j].number=0;
							index_node[j].father=-1;
							index_node[j].left_sib=-1;
							index_node[j].right_sib=-1;
							for(int k=0;k<=3;++k) index_node[j].child[k]=-1;
						}
						index_node[0].valid=1;
						index_node[0].is_leaf=1;
					}
				}
			}
			for(int i=0;i<=31;++i)
			if(temp_value[i]!=NULL)
				delete temp_value[i];
		}
		delete p;
		record_loc++;
	}
	return 1;
}

int pan_int(int temp_int,string op,string value)
{
	int x;
	int flag=0;
	if(value[0]=='-') flag=1;
	else x=value[0]-'0';
	for(int i=1;i<=value.length()-1;++i) 
	{
		if(value[i]>='0'&&value[i]<='9')
			x=x*10+value[i]-'0';
		else return -1;
	}
	if(flag==1) x=-x;
	if(op=="=")
	{
		if(temp_int==x) return 1;
		else return 0;
	}
	else if(op=="<>")
	{
		if(temp_int!=x) return 1;
		else return 0;
	}
	else if(op=="<")
	{
		if(temp_int<x) return 1;
		else return 0;
	}
	else if(op==">")
	{
		if(temp_int>x) return 1;
		else return 0;
	}
	else if(op=="<=")
	{
		if(temp_int<=x) return 1;
		else return 0;
	}
	else if(op==">=")
	{
		if(temp_int>=x) return 1;
		else return 0;
	}
}

int pan_float(float temp_float,string op,string value)
{
	float x;
	int dian=-1;
	int flag=0;
	if(value[0]=='-') flag=1;
	else x=value[0]-'0';
	for(int i=1;i<=value.length()-1;++i) 
	{
		if(value[i]=='.') 
		{
			dian=i;
			break;
		}
		else if(value[i]>='0'&&value[i]<='9')
			x=x*10+value[i]-'0';
		else return -1;
	}
	float y=0;
	if(dian!=-1)
	{
		for(int i=value.length()-1;i>=dian+1;--i)
		{
			if(value[i]>='0'&&value[i]<='9')
				y=y*0.1+value[i]-'0';
			else return -1;
		}
		x=x+y*0.1;
	}
	if(flag==1) x=-x;
	if(op=="=")
	{
		if(temp_float==x) return 1;
		else return 0;
	}
	else if(op=="<>")
	{
		if(temp_float!=x) return 1;
		else return 0;
	}
	else if(op=="<")
	{
		if(temp_float<x) return 1;
		else return 0;
	}
	else if(op==">")
	{
		if(temp_float>x) return 1;
		else return 0;
	}
	else if(op=="<=")
	{
		if(temp_float<=x) return 1;
		else return 0;
	}
	else if(op==">=")
	{
		if(temp_float>=x) return 1;
		else return 0;
	}
}

int pan_string(string temp_string,string op,string value)
{
	string x=value;
	if(op=="=")
	{
		if(temp_string==x) return 1;
		else return 0;
	}
	else if(op=="<>")
	{
		if(temp_string!=x) return 1;
		else return 0;
	}
	else if(op=="<")
	{
		if(temp_string<x) return 1;
		else return 0;
	}
	else if(op==">")
	{
		if(temp_string>x) return 1;
		else return 0;
	}
	else if(op=="<=")
	{
		if(temp_string<=x) return 1;
		else return 0;
	}
	else if(op==">=")
	{
		if(temp_string>=x) return 1;
		else return 0;
	}
}

int zhuan_int_record(string value)
{
	int x;
	int flag=0;
	if(value[0]=='-') flag=1;
	else x=value[0]-'0';
	for(int i=1;i<=value.length()-1;++i) 
	{
		if(value[i]>='0'&&value[i]<='9')
			x=x*10+value[i]-'0';
		else return 2147483647;
	}
	if(flag==1) x=-x;
	return x;
}

float zhuan_float_record(string value)
{
	float x;
	int dian=-1;
	int flag=0;
	if(value[0]=='-') flag=1;
	else x=value[0]-'0';
	for(int i=1;i<=value.length()-1;++i) 
	{
		if(value[i]=='.') 
		{
			dian=i;
			break;
		}
		else if(value[i]>='0'&&value[i]<='9')
			x=x*10+value[i]-'0';
		else return 7.41;
	}
	float y=0;
	if(dian!=-1)
	{
		for(int i=value.length()-1;i>=dian+1;--i)
		{
			if(value[i]>='0'&&value[i]<='9')
				y=y*0.1+value[i]-'0';
			else return 7.41;
		}
		x=x+y*0.1;
	}
	if(flag==1) x=-x;
	return x;
}

void record_write_tree_back()
{
	setIndexRoot(tree_index_name,tree_root);
	int index_size=0;
	index_size+=46;
	if(type==0) index_size+=3*4;
	else if(type==-1) index_size+=3*4;
	else index_size+=3*type;
	int block_index_number;
	block_index_number=4096/index_size;
	for(int i=0;i<=999;++i)
	{
		int block_loc;
		int offset;
		block_loc=i/block_index_number;
		offset=(i-block_loc*block_index_number)*index_size;
		int buffer_block_number;
		buffer_block_number=getBlock(tree_index_name,block_loc);
		block[buffer_block_number].dirty=1;
		char* p=block[buffer_block_number].buffer+offset;
		char* ppp;
		ppp=&index_node[i].valid;
		memcpy(p,ppp,1);//存进去valid_bit 
		p=p+1;
		ppp=&index_node[i].is_leaf;
		memcpy(p,ppp,1);//存进去是否为叶子 
		p=p+1;
		int* p_int;
		p_int=&index_node[i].number;
		memcpy(p,p_int,4);//存进去叶子节点个数 
		p=p+4;
		p_int=&index_node[i].father;
		memcpy(p,p_int,4);//存进去父亲节点 
		p=p+4;
		p_int=&index_node[i].left_sib;
		memcpy(p,p_int,4);//存进去左兄弟节点 
		p=p+4;
		p_int=&index_node[i].right_sib;
		memcpy(p,p_int,4);//存进去右兄弟节点
		p=p+4;
		for(int j=0;j<=2;++j)
		{
			p_int=&index_node[i].child[j];
			memcpy(p,p_int,4);//存进去孩子节点
			p=p+4;
			if(type==0)//存进去孩子的值 
			{
				int temp_int;
				temp_int=record_zhuan_int_index(index_node[i].child_value[j]);
				int* temp_int_p=&temp_int;
				memcpy(p,temp_int_p,4);
				p=p+4;
			}
			else if(type==-1)
			{
				float temp_float;
				temp_float=record_zhuan_float_index(index_node[i].child_value[j]);
				float* temp_float_p=&temp_float;
				memcpy(p,temp_float_p,4);
				p=p+4;
			}
			else
			{
				memcpy(p,index_node[i].child_value[j].c_str(),type);
				p=p+type;
			}
			p_int=&index_node[i].child_value_loc[j];
			memcpy(p,p_int,4);//存进去孩子节点的值的位置 
			p=p+4;
		}
		p_int=&index_node[i].child[3];
		memcpy(p,p_int,4);//存进去第四个孩子节点
		p=p+4;
	}
}

int record_zhuan_int_index(string value)
{
	int x;
	int flag=0;
	if(value[0]=='-') flag=1;
	else x=value[0]-'0';
	for(int i=1;i<=value.length()-1;++i) 
	{
		if(value[i]>='0'&&value[i]<='9')
			x=x*10+value[i]-'0';
		else return 2147483647;
	}
	if(flag==1) x=-x;
	return x;
}

float record_zhuan_float_index(string value)
{
	float x;
	int dian=-1;
	int flag=0;
	if(value[0]=='-') flag=1;
	else x=value[0]-'0';
	for(int i=1;i<=value.length()-1;++i) 
	{
		if(value[i]=='.') 
		{
			dian=i;
			break;
		}
		else if(value[i]>='0'&&value[i]<='9')
			x=x*10+value[i]-'0';
		else return 7.41;
	}
	float y=0;
	if(dian!=-1)
	{
		for(int i=value.length()-1;i>=dian+1;--i)
		{
			if(value[i]>='0'&&value[i]<='9')
				y=y*0.1+value[i]-'0';
			else return 7.41;
		}
		x=x+y*0.1;
	}
	if(flag==1) x=-x;
	return x;
}
