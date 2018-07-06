#include"Index_manager.h" 
extern struct Block block[100];
extern struct B_node index_node[100000];
extern string tree_index_name;
extern int tree_root;
extern int type;

void write_tree_back();
void construct_tree(string index_name,int initial_type);
void index_initial(string index_name,int initial_type);
int zhuan_int_index(string value);
float zhuan_float_index(string value);
string int_to_string(int x);
string float_to_string(float x);
int pan_int_index(int temp_int,string op,string value);
int pan_float_index(float temp_float,string op,string value);
int pan_string_index(string temp_string,string op,string value);
void index_delete(string table_name,string index_name,int initial_type,string values);

void index_drop_index(string index_name)
{
	if(tree_index_name==index_name) 
	{
		tree_index_name="";
	}
	DeleteFile((index_name+".data").c_str());
	clear_index(index_name);
	return;
}

void index_create(string table_name,int initial_number,string initial_name[],int initial_type[],string index_name,string attribute_name)
{
	if(tree_index_name!="") write_tree_back();
	for(int i=0;i<=initial_number;++i)
	{
		if(attribute_name==initial_name[i])
		{
			index_initial(index_name,initial_type[i]);
			break;
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
			for(int j=0;j<=initial_number;++j)
			{
				if(attribute_name==initial_name[j])
				{
					index_insert(table_name,index_name,attribute_name,initial_type[j],temp_value[j],record_loc);
				}
				break;
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
}

int index_select(string table_name,string index_name,int initial_number,string initial_name[],int initial_type[],int limit_number,string limit_attribute[],string limit_op[],string limit_value[])
{
	int ff=0;
	for(int i=0;i<=limit_number;++i)
	{
		for(int j=0;j<=initial_number;++j)
		{
			if(limit_attribute[i]==initial_name[j])
			{
				if(initial_type[j]==0)
				{
					ff=pan_int_index(0,limit_op[j],limit_value[i]);
					if(ff==-1) 
					{
						cout<<"Value error"<<endl;
						return 0;	
					}
				}
				else if(initial_type[j]==-1)
				{
					ff=pan_float_index(0,limit_op[j],limit_value[i]);
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
	int temp_type;
	for(int i=0;i<=initial_number;++i)
	{
		if(limit_attribute[0]==initial_name[i])
		{
			temp_type=initial_type[i];
			break;
		}
	}
	if(tree_index_name!=index_name)
	{
		if(tree_index_name!="") write_tree_back();
		construct_tree(index_name,temp_type);
	}
	int cur_node=tree_root;
	while(index_node[cur_node].is_leaf==0)
	{
		int point_loc;
		if(type==0)
		{
			point_loc=index_node[cur_node].number;
			int insert_value=atoi(limit_value[0].c_str());
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				int border=atoi(index_node[cur_node].child_value[i].c_str());
				if(border>insert_value)
				{
					point_loc=i;
					break;
				}
			}
		}
		else if(type==-1)
		{
			point_loc=index_node[cur_node].number;
			float insert_value=atof(limit_value[0].c_str());
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				int border=atof(index_node[cur_node].child_value[i].c_str());
				if(border>insert_value)
				{
					point_loc=i;
					break;
				}
			}
		}
		else
		{
			point_loc=index_node[cur_node].number;
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				if(index_node[cur_node].child_value[i]>limit_value[0])
				{
					point_loc=i;
					break;
				}
			}
		}
		cur_node=index_node[cur_node].child[point_loc];
	}
	int point_loc;
	if(type==0)
	{
		point_loc=index_node[cur_node].number-1;
		int select_value=atoi(limit_value[0].c_str());
		for(int i=0;i<index_node[cur_node].number;++i)
		{
			int border=atoi(index_node[cur_node].child_value[i].c_str());
			if(border>=select_value)
			{
				point_loc=i;
				break;
			}
		}
	}
	else if(type==-1)
	{
		point_loc=index_node[cur_node].number-1;
		float select_value=atof(limit_value[0].c_str());
		for(int i=0;i<index_node[cur_node].number;++i)
		{
			int border=atof(index_node[cur_node].child_value[i].c_str());
			if(border>=select_value)
			{
				point_loc=i;
				break;
			}
		}
	}
	else
	{
		point_loc=index_node[cur_node].number-1;
		for(int i=0;i<index_node[cur_node].number;++i)
		{
			if(index_node[cur_node].child_value[i]>=limit_value[0])
			{
				point_loc=i;
				break;
			}
		}
	}
	if(point_loc==-1) return 1;
	int cur_loc=point_loc;
	int select_array[100001];
	int sa_number=-1;
	if(limit_op[0]=="=")
	{
		if(index_node[cur_node].child_value[cur_loc]==limit_value[0])
		{
			sa_number++;
			select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
		}
	}
	else if(limit_op[0]=="<>")
	{
		if(index_node[cur_node].child_value[cur_loc]!=limit_value[0])
		{
			for(int i=0;i<getRecordTotal(table_name);++i)
			{
				sa_number++;
				select_array[sa_number]=i;
			}
		}
		else
		{
			for(int i=0;i<getRecordTotal(table_name);++i)
			{
				if(i!=index_node[cur_node].child_value_loc[cur_loc])
				{
					sa_number++;
					select_array[sa_number]=i;
				}
			}
		}
	}
	else if(limit_op[0]=="<")
	{
		while(1)
		{
			if(type==0)
			{
				if(atoi(index_node[cur_node].child_value[cur_loc].c_str())<atoi(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else if(type==-1)
			{
				if(atof(index_node[cur_node].child_value[cur_loc].c_str())<atof(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else
			{
				if(index_node[cur_node].child_value[cur_loc]<limit_value[0])
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			if(cur_loc!=0)
			{
				cur_loc--;
			}
			else
			{
				if(index_node[cur_node].left_sib!=-1)
				{
					cur_node=index_node[cur_node].left_sib;
					cur_loc=index_node[cur_node].number-1;
				}
				else break;
			}
		}
	}
	else if(limit_op[0]==">")
	{
		while(1)
		{
			if(type==0)
			{
				if(atoi(index_node[cur_node].child_value[cur_loc].c_str())>atoi(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else if(type==-1)
			{
				if(atof(index_node[cur_node].child_value[cur_loc].c_str())>atof(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else
			{
				if(index_node[cur_node].child_value[cur_loc]>limit_value[0])
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			if(cur_loc!=index_node[cur_node].number-1)
			{
				cur_loc++;
			}
			else
			{
				if(index_node[cur_node].right_sib!=-1)
				{
					cur_node=index_node[cur_node].right_sib;
					cur_loc=0;
				}
				else break;
			}
		}
	}
	else if(limit_op[0]=="<=")
	{
		while(1)
		{
			if(type==0)
			{
				if(atoi(index_node[cur_node].child_value[cur_loc].c_str())<=atoi(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else if(type==-1)
			{
				if(atof(index_node[cur_node].child_value[cur_loc].c_str())<=atof(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else
			{
				if(index_node[cur_node].child_value[cur_loc]<=limit_value[0])
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			if(cur_loc!=0)
			{
				cur_loc--;
			}
			else
			{
				if(index_node[cur_node].left_sib!=-1)
				{
					cur_node=index_node[cur_node].left_sib;
					cur_loc=index_node[cur_node].number-1;
				}
				else break;
			}
		}
	}
	else if(limit_op[0]==">=")
	{
		while(1)
		{
			if(type==0)
			{
				if(atoi(index_node[cur_node].child_value[cur_loc].c_str())>=atoi(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else if(type==-1)
			{
				if(atof(index_node[cur_node].child_value[cur_loc].c_str())>=atof(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else
			{
				if(index_node[cur_node].child_value[cur_loc]>=limit_value[0])
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			if(cur_loc!=index_node[cur_node].number-1)
			{
				cur_loc++;
			}
			else
			{
				if(index_node[cur_node].right_sib!=-1)
				{
					cur_node=index_node[cur_node].right_sib;
					cur_loc=0;
				}
				else break;
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
	int offset;
	int record_loc;
	int block_loc;
	for(int k=0;k<=sa_number;++k)
	{
		record_loc=select_array[k];
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
			for(int i=1;i<=limit_number;++i)
			{
				for(int j=0;j<=initial_number;++j)
				{
					if(limit_attribute[i]==initial_name[j])
					{
						if(initial_type[j]==0)
	 					{
	 						int temp_int;
	 						memcpy(&temp_int,temp_value[j],4);
	 						flag=pan_int_index(temp_int,limit_op[i],limit_value[i]);
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
	 						flag=pan_float_index(temp_float,limit_op[i],limit_value[i]);
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
							if(pan_string_index(temp_string,limit_op[i],limit_value[i])==0) flag=0;
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

void index_insert(string table_name,string index_name,string attribute_name,int initial_type,string values,int record_loc)
{
	if(tree_index_name!=index_name)
	{
		if(tree_index_name!="") write_tree_back();
		construct_tree(index_name,initial_type);
	}
	int cur_node=tree_root;
	while(index_node[cur_node].is_leaf==0)
	{
		int point_loc;
		if(initial_type==0)
		{
			point_loc=index_node[cur_node].number;
			int insert_value=atoi(values.c_str());
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				int border=atoi(index_node[cur_node].child_value[i].c_str());
				if(border>insert_value)
				{
					point_loc=i;
					break;
				}
			}
		}
		else if(initial_type==-1)
		{
			point_loc=index_node[cur_node].number;
			float insert_value=atof(values.c_str()); 
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				int border=atof(index_node[cur_node].child_value[i].c_str());
				if(border>insert_value)
				{
					point_loc=i;
					break;
				}
			}
		}
		else
		{
			point_loc=index_node[cur_node].number;
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				if(index_node[cur_node].child_value[i]>values)
				{
					point_loc=i;
					break;
				}
			}
		}
		cur_node=index_node[cur_node].child[point_loc];
	}
	if(index_node[cur_node].number<=2)
	{
		int point_loc;
		if(initial_type==0)
		{
			point_loc=index_node[cur_node].number;
			int insert_value=atoi(values.c_str());
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				int border=atoi(index_node[cur_node].child_value[i].c_str());
				if(border>insert_value)
				{
					point_loc=i;
					break;
				}
			}
		}
		else if(initial_type==-1)
		{
			point_loc=index_node[cur_node].number;
			float insert_value=atof(values.c_str());
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				int border=atof(index_node[cur_node].child_value[i].c_str());
				if(border>insert_value)
				{
					point_loc=i;
					break;
				}
			}
		}
		else
		{
			point_loc=index_node[cur_node].number;
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				if(index_node[cur_node].child_value[i]>values)
				{
					point_loc=i;
					break;
				}
			}
		}
		for(int i=index_node[cur_node].number;i>point_loc;--i)
		{
			index_node[cur_node].child_value[i]=index_node[cur_node].child_value[i-1];
			index_node[cur_node].child_value_loc[i]=index_node[cur_node].child_value_loc[i-1];
		}
		index_node[cur_node].child_value[point_loc]=values;
		index_node[cur_node].child_value_loc[point_loc]=record_loc;
		index_node[cur_node].number++;
	}
	else
	{
		int new_node;
		for(int i=0;i<=99999;++i)
		{
			if(index_node[i].valid==0)
			{
				new_node=i;
				index_node[i].valid=1;
				index_node[i].number=0;
				index_node[i].father=-1;
				index_node[i].left_sib=-1;
				index_node[i].right_sib=-1;
				for(int j=0;j<=3;++j)
				{
					index_node[i].child[j]=-1;
				}
				break;
			}
		}
		int point_loc;
		if(initial_type==0)
		{
			point_loc=index_node[cur_node].number;
			int insert_value=atoi(values.c_str());
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				int border=atoi(index_node[cur_node].child_value[i].c_str());
				if(border>insert_value)
				{
					point_loc=i;
					break;
				}
			}
		}
		else if(initial_type==-1)
		{
			point_loc=index_node[cur_node].number;
			float insert_value=atof(values.c_str());
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				int border=atof(index_node[cur_node].child_value[i].c_str());
				if(border>insert_value)
				{
					point_loc=i;
					break;
				}
			}
		}
		else
		{
			point_loc=index_node[cur_node].number;
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				if(index_node[cur_node].child_value[i]>values)
				{
					point_loc=i;
					break;
				}
			}
		}
		index_node[cur_node].number=2;
		index_node[new_node].number=2;
		index_node[new_node].right_sib=index_node[cur_node].right_sib;
		index_node[cur_node].right_sib=new_node;
		index_node[new_node].left_sib=cur_node;
		index_node[new_node].is_leaf=1;
		index_node[new_node].father=index_node[cur_node].father;
		string new_value;
		int new_loc;
		if(point_loc==0)
		{
			index_node[new_node].child_value[1]=index_node[cur_node].child_value[2];
			index_node[new_node].child_value[0]=index_node[cur_node].child_value[1];
			index_node[cur_node].child_value[1]=index_node[cur_node].child_value[0];
			index_node[cur_node].child_value[0]=values;
			index_node[new_node].child_value_loc[1]=index_node[cur_node].child_value_loc[2];
			index_node[new_node].child_value_loc[0]=index_node[cur_node].child_value_loc[1];
			index_node[cur_node].child_value_loc[1]=index_node[cur_node].child_value_loc[0];
			index_node[cur_node].child_value_loc[0]=record_loc;
			new_value=index_node[cur_node].child_value[1];
			new_loc=index_node[cur_node].child_value_loc[1];
		}
		else if(point_loc==1)
		{
			index_node[new_node].child_value[1]=index_node[cur_node].child_value[2];
			index_node[new_node].child_value[0]=index_node[cur_node].child_value[1];
			index_node[cur_node].child_value[1]=values;
			index_node[new_node].child_value_loc[1]=index_node[cur_node].child_value_loc[2];
			index_node[new_node].child_value_loc[0]=index_node[cur_node].child_value_loc[1];
			index_node[cur_node].child_value_loc[1]=record_loc;
			new_value=index_node[cur_node].child_value[1];
			new_loc=index_node[cur_node].child_value_loc[1];
		}
		else if(point_loc==2)
		{
			index_node[new_node].child_value[1]=index_node[cur_node].child_value[2];
			index_node[new_node].child_value[0]=values;
			index_node[new_node].child_value_loc[1]=index_node[cur_node].child_value_loc[2];
			index_node[new_node].child_value_loc[0]=record_loc;
			new_value=values;
			new_loc=record_loc;
		}
		else
		{
			index_node[new_node].child_value[1]=values;
			index_node[new_node].child_value[0]=index_node[cur_node].child_value[2];
			index_node[new_node].child_value_loc[1]=record_loc;
			index_node[new_node].child_value_loc[0]=index_node[cur_node].child_value_loc[2];
			new_value=index_node[cur_node].child_value[2];
			new_loc=index_node[cur_node].child_value_loc[2];
		}
		while(1)
		{
			if(index_node[cur_node].father==-1)
			{
				int new_father;
				for(int i=0;i<=99999;++i)
				{
					if(index_node[i].valid==0)
					{
						new_father=i;
						index_node[i].valid=1;
						index_node[i].number=0;
						index_node[i].father=-1;
						index_node[i].left_sib=-1;
						index_node[i].right_sib=-1;
						for(int j=0;j<=3;++j)
						{
							index_node[i].child[j]=-1;
						}
						break;
					}
				}
				index_node[cur_node].father=new_father;
				index_node[new_node].father=new_father;
				if(cur_node==tree_root) tree_root=new_father;
				index_node[new_father].is_leaf=0;
				index_node[new_father].number=1;
				index_node[new_father].father=-1;
				index_node[new_father].left_sib=-1;
				index_node[new_father].right_sib=-1;
				index_node[new_father].child[0]=cur_node;
				index_node[new_father].child_value[0]=new_value;
				index_node[new_father].child_value_loc[0]=new_loc;
				index_node[new_father].child[1]=new_node;
				break;
			}
			else
			{
				int update_node=index_node[cur_node].father;
				int point_loc;
				if(initial_type==0)
				{
					point_loc=index_node[update_node].number;
					int insert_value=atoi(new_value.c_str());
					for(int i=0;i<index_node[update_node].number;++i)
					{
						int border=atoi(index_node[update_node].child_value[i].c_str());
						if(border>insert_value)
						{
							point_loc=i;
							break;
						}
					}
				}
				else if(initial_type==-1)
				{
					point_loc=index_node[update_node].number;
					float insert_value=atof(new_value.c_str());
					for(int i=0;i<index_node[update_node].number;++i)
					{
						int border=atof(index_node[update_node].child_value[i].c_str());
						if(border>insert_value)
						{
							point_loc=i;
							break;
						}
					}
				}
				else
				{
					point_loc=index_node[update_node].number;
					for(int i=0;i<index_node[update_node].number;++i)
					{
						if(index_node[update_node].child_value[i]>new_value)
						{
							point_loc=i;
							break;
						}
					}
				}
				if(index_node[update_node].number<=2)
				{
					for(int i=index_node[update_node].number;i>point_loc;--i)
					{
						index_node[update_node].child_value[i]=index_node[update_node].child_value[i-1];
						index_node[update_node].child_value_loc[i]=index_node[update_node].child_value_loc[i-1];
						index_node[update_node].child[i+1]=index_node[update_node].child[i];
					}
					index_node[update_node].child_value[point_loc]=new_value;
					index_node[update_node].child_value_loc[point_loc]=new_loc;
					index_node[update_node].child[point_loc+1]=new_node;
					index_node[update_node].number++;
					break;
				}
				else
				{
					int temp_new;
					for(int i=0;i<=99999;++i)
					{
						if(index_node[i].valid==0)
						{
							temp_new=i;
							index_node[i].valid=1;
							index_node[i].number=0;
							index_node[i].father=-1;
							index_node[i].left_sib=-1;
							index_node[i].right_sib=-1;
							for(int j=0;j<=3;++j)
							{
								index_node[i].child[j]=-1;
							}
							break;
						}
					}
					index_node[update_node].number=2;
					index_node[temp_new].number=1;
					index_node[temp_new].right_sib=index_node[update_node].right_sib;
					index_node[update_node].right_sib=temp_new;
					index_node[temp_new].left_sib=update_node;
					index_node[temp_new].is_leaf=0;
					index_node[temp_new].father=index_node[update_node].father;
					if(point_loc==0)
					{
						index_node[temp_new].child_value[0]=index_node[update_node].child_value[2];
						index_node[update_node].child_value[1]=index_node[update_node].child_value[0];
						index_node[update_node].child_value[0]=new_value;
						index_node[temp_new].child_value_loc[0]=index_node[update_node].child_value_loc[2];
						index_node[update_node].child_value_loc[1]=index_node[update_node].child_value_loc[0];
						index_node[update_node].child_value_loc[0]=new_loc;
						new_value=index_node[update_node].child_value[1];
						new_loc=index_node[update_node].child_value_loc[1];
						index_node[update_node].child[2]=index_node[update_node].child[1];
						index_node[update_node].child[1]=new_node;
						index_node[new_node].father=update_node;
						index_node[temp_new].child[0]=index_node[update_node].child[2];
						index_node[index_node[update_node].child[2]].father=temp_new;
						index_node[temp_new].child[1]=index_node[update_node].child[3];
						index_node[index_node[update_node].child[3]].father=temp_new;
					}
					else if(point_loc==1)
					{
						index_node[temp_new].child_value[0]=index_node[update_node].child_value[2];
						index_node[update_node].child_value[1]=new_value;
						index_node[temp_new].child_value_loc[0]=index_node[update_node].child_value_loc[2];
						index_node[update_node].child_value_loc[1]=new_loc;
						new_value=index_node[update_node].child_value[1];
						new_loc=index_node[update_node].child_value_loc[1];
						index_node[update_node].child[2]=new_node;
						index_node[new_node].father=update_node;
						index_node[temp_new].child[0]=index_node[update_node].child[2];
						index_node[index_node[update_node].child[2]].father=temp_new;
						index_node[temp_new].child[1]=index_node[update_node].child[3];
						index_node[index_node[update_node].child[3]].father=temp_new;
					}
					else if(point_loc==2)
					{
						index_node[temp_new].child_value[0]=index_node[update_node].child_value[2];
						index_node[temp_new].child_value_loc[0]=index_node[update_node].child_value_loc[2];
						index_node[temp_new].child[0]=new_node;
						index_node[new_node].father=temp_new;
						index_node[temp_new].child[1]=index_node[update_node].child[3];
						index_node[index_node[update_node].child[3]].father=temp_new;
					}
					else
					{
						index_node[temp_new].child_value[0]=new_value;
						index_node[temp_new].child_value_loc[0]=new_loc;
						new_value=index_node[update_node].child_value[2];
						new_loc=index_node[update_node].child_value_loc[2];
						index_node[temp_new].child[0]=index_node[update_node].child[3];
						index_node[index_node[update_node].child[3]].father=temp_new;
						index_node[temp_new].child[1]=new_node;
						index_node[new_node].father=temp_new;
					}
					cur_node=update_node;
					new_node=temp_new;
				}
			}
		}
	}
}

int index_deletion(string table_name,string index_name,int initial_number,string initial_name[],int initial_type[],int limit_number,string limit_attribute[],string limit_op[],string limit_value[])
{
	int ff=0;
	for(int i=0;i<=limit_number;++i)
	{
		for(int j=0;j<=initial_number;++j)
		{
			if(limit_attribute[i]==initial_name[j])
			{
				if(initial_type[j]==0)
				{
					ff=pan_int_index(0,limit_op[j],limit_value[j]);
					if(ff==-1) 
					{
						cout<<"Value error"<<endl;
						return 0;	
					}
				}
				else if(initial_type[j]==-1)
				{
					ff=pan_float_index(0,limit_op[j],limit_value[j]);
					if(ff==-1) 
					{
						cout<<"Value error"<<endl;
						return 0;	
					}
				}
			}
		}
	}
	int temp_type;
	for(int i=0;i<=initial_number;++i)
	{
		if(limit_attribute[0]==initial_name[i])
		{
			temp_type=initial_type[i];
			break;
		}
	}
	if(tree_index_name!=index_name)
	{
		if(tree_index_name!="") write_tree_back();
		construct_tree(index_name,temp_type);
	}
	int cur_node=tree_root;
	while(index_node[cur_node].is_leaf==0)
	{
		int point_loc;
		if(type==0)
		{
			point_loc=index_node[cur_node].number;
			int insert_value=atoi(limit_value[0].c_str());
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				int border=atoi(index_node[cur_node].child_value[i].c_str());
				if(border>insert_value)
				{
					point_loc=i;
					break;
				}
			}
		}
		else if(type==-1)
		{
			point_loc=index_node[cur_node].number;
			float insert_value=atof(limit_value[0].c_str());
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				int border=atof(index_node[cur_node].child_value[i].c_str());
				if(border>insert_value)
				{
					point_loc=i;
					break;
				}
			}
		}
		else
		{
			point_loc=index_node[cur_node].number;
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				if(index_node[cur_node].child_value[i]>limit_value[0])
				{
					point_loc=i;
					break;
				}
			}
		}
		cur_node=index_node[cur_node].child[point_loc];
	}
	int point_loc;
	if(type==0)
	{
		point_loc=index_node[cur_node].number-1;
		int select_value=atoi(limit_value[0].c_str());
		for(int i=0;i<index_node[cur_node].number;++i)
		{
			int border=atoi(index_node[cur_node].child_value[i].c_str());
			if(border>=select_value)
			{
				point_loc=i;
				break;
			}
		}
	}
	else if(type==-1)
	{
		point_loc=index_node[cur_node].number-1;
		float select_value=atof(limit_value[0].c_str());
		for(int i=0;i<index_node[cur_node].number;++i)
		{
			int border=atof(index_node[cur_node].child_value[i].c_str());
			if(border>=select_value)
			{
				point_loc=i;
				break;
			}
		}
	}
	else
	{
		point_loc=index_node[cur_node].number-1;
		for(int i=0;i<index_node[cur_node].number;++i)
		{
			if(index_node[cur_node].child_value[i]>=limit_value[0])
			{
				point_loc=i;
				break;
			}
		}
	}
	if(point_loc==-1) 
	{
		cout<<"Not ";
		return 1;
	}
	int cur_loc=point_loc;
	int select_array[100001];
	int sa_number=-1;
	if(limit_op[0]=="=")
	{
		if(index_node[cur_node].child_value[cur_loc]==limit_value[0])
		{
			sa_number++;
			select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
		}
	}
	else if(limit_op[0]=="<>")
	{
		if(index_node[cur_node].child_value[cur_loc]!=limit_value[0])
		{
			for(int i=0;i<getRecordTotal(table_name);++i)
			{
				sa_number++;
				select_array[sa_number]=i;
			}
		}
		else
		{
			for(int i=0;i<getRecordTotal(table_name);++i)
			{
				if(i!=index_node[cur_node].child_value_loc[cur_loc])
				{
					sa_number++;
					select_array[sa_number]=i;
				}
			}
		}
	}
	else if(limit_op[0]=="<")
	{
		while(1)
		{
			if(type==0)
			{
				if(atoi(index_node[cur_node].child_value[cur_loc].c_str())<atoi(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else if(type==-1)
			{
				if(atof(index_node[cur_node].child_value[cur_loc].c_str())<atof(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else
			{
				if(index_node[cur_node].child_value[cur_loc]<limit_value[0])
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			if(cur_loc!=0)
			{
				cur_loc--;
			}
			else
			{
				if(index_node[cur_node].left_sib!=-1)
				{
					cur_node=index_node[cur_node].left_sib;
					cur_loc=index_node[cur_node].number-1;
				}
				else break;
			}
		}
	}
	else if(limit_op[0]==">")
	{
		while(1)
		{
			if(type==0)
			{
				if(atoi(index_node[cur_node].child_value[cur_loc].c_str())>atoi(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else if(type==-1)
			{
				if(atof(index_node[cur_node].child_value[cur_loc].c_str())>atof(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else
			{
				if(index_node[cur_node].child_value[cur_loc]>limit_value[0])
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			if(cur_loc!=index_node[cur_node].number-1)
			{
				cur_loc++;
			}
			else
			{
				if(index_node[cur_node].right_sib!=-1)
				{
					cur_node=index_node[cur_node].right_sib;
					cur_loc=0;
				}
				else break;
			}
		}
	}
	else if(limit_op[0]=="<=")
	{
		while(1)
		{
			if(type==0)
			{
				if(atoi(index_node[cur_node].child_value[cur_loc].c_str())<=atoi(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else if(type==-1)
			{
				if(atof(index_node[cur_node].child_value[cur_loc].c_str())<=atof(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else
			{
				if(index_node[cur_node].child_value[cur_loc]<=limit_value[0])
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			if(cur_loc!=0)
			{
				cur_loc--;
			}
			else
			{
				if(index_node[cur_node].left_sib!=-1)
				{
					cur_node=index_node[cur_node].left_sib;
					cur_loc=index_node[cur_node].number-1;
				}
				else break;
			}
		}
	}
	else if(limit_op[0]==">=")
	{
		while(1)
		{
			if(type==0)
			{
				if(atoi(index_node[cur_node].child_value[cur_loc].c_str())>=atoi(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else if(type==-1)
			{
				if(atof(index_node[cur_node].child_value[cur_loc].c_str())>=atof(limit_value[0].c_str()))
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			else
			{
				if(index_node[cur_node].child_value[cur_loc]>=limit_value[0])
				{
					sa_number++;
					select_array[sa_number]=index_node[cur_node].child_value_loc[cur_loc];
				} 
			}
			if(cur_loc!=index_node[cur_node].number-1)
			{
				cur_loc++;
			}
			else
			{
				if(index_node[cur_node].right_sib!=-1)
				{
					cur_node=index_node[cur_node].right_sib;
					cur_loc=0;
				}
				else break;
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
	int offset;
	int record_loc;
	int block_loc;
	for(int k=0;k<=sa_number;++k)
	{
		record_loc=select_array[k];
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
			for(int i=1;i<=limit_number;++i)
			{
				for(int j=0;j<=initial_number;++j)
				{
					if(limit_attribute[i]==initial_name[j])
					{
						if(initial_type[j]==0)
	 					{
	 						int temp_int;
	 						memcpy(&temp_int,temp_value[j],4);
	 						flag=pan_int_index(temp_int,limit_op[i],limit_value[i]);
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
	 						flag=pan_float_index(temp_float,limit_op[i],limit_value[i]);
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
							if(pan_string_index(temp_string,limit_op[i],limit_value[i])==0) flag=0;
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
					for(int j=0;j<=initial_number;++j)
					{
						if(temp_attribute_name[i]==initial_name[j])
						{
							string values;
							if(initial_type[j]==0)
							{
								int temp_int;
								memcpy(&temp_int,temp_value[j],4);
								stringstream ss;
								ss<<temp_int;
								ss>>values;
							}
							else if(initial_type[j]==-1)
							{
								float temp_float;
								memcpy(&temp_float,temp_value[j],4);
								stringstream ss;
								ss<<temp_float;
								ss>>values;
							}
							else 
							{
								temp_value[j][initial_type[j]]=0;
								values=temp_value[j];
							}
							index_delete(table_name,temp_index_name[i],initial_type[j],values);
							break;
						}
					}
				}
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

void write_tree_back()
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
				temp_int=zhuan_int_index(index_node[i].child_value[j]);
				int* temp_int_p=&temp_int;
				memcpy(p,temp_int_p,4);
				p=p+4;
			}
			else if(type==-1)
			{
				float temp_float;
				temp_float=zhuan_float_index(index_node[i].child_value[j]);
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

void construct_tree(string index_name,int initial_type)
{
	tree_index_name=index_name;
	type=initial_type;
	tree_root=getIndexRoot(index_name);
	int index_size=0;
	index_size+=46;
	if(initial_type==0) index_size+=3*4;
	else if(initial_type==-1) index_size+=3*4;
	else index_size+=3*initial_type;
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
		char* temp_p;
		p=new char[index_size];
		memcpy(p,block[buffer_block_number].buffer+offset,index_size);
		index_node[i].valid=p[0];//valid
		index_node[i].is_leaf=p[1];//isleaf
		temp_p=p+2;
		memcpy(&(index_node[i].number),temp_p,4);//number
		temp_p+=4;
		memcpy(&(index_node[i].father),temp_p,4);//father
		temp_p+=4;
		memcpy(&(index_node[i].left_sib),temp_p,4);//左孩子 
		temp_p+=4;
		memcpy(&(index_node[i].right_sib),temp_p,4);//右孩子 
		temp_p+=4;
		for(int j=0;j<=2;++j)
		{
			memcpy(&(index_node[i].child[j]),temp_p,4);//孩子指针 
			temp_p+=4;
			if(type==0)
			{
				int* p_int=new int();
				memcpy(p_int,temp_p,4);//孩子值 
				temp_p+=4;
				index_node[i].child_value[j]=int_to_string(*p_int);
				delete p_int;
			}
			else if(type==-1)
			{
				float* p_float=new float();
				memcpy(p_float,temp_p,4);//孩子值 
				temp_p+=4;
				index_node[i].child_value[j]=float_to_string(*p_float);
				delete p_float;
			} 
			else
			{
				char* p_char=new char[type+1];
				memcpy(p_char,temp_p,type);
				temp_p+=type;
				p_char[type]='0';
				index_node[i].child_value[j]=p_char;
				delete p_char;
			}
			memcpy(&(index_node[i].child_value_loc[j]),temp_p,4);//孩子值位置 
			temp_p+=4;
		}
		memcpy(&(index_node[i].child[3]),temp_p,4);//孩子指针 
		temp_p+=4;
		if(p!=NULL) delete p;
	}
}

int zhuan_int_index(string value)
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

float zhuan_float_index(string value)
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

string int_to_string(int x)
{
	ostringstream stream;
	stream<<x; 
	return stream.str();
}

string float_to_string(float x)
{
	ostringstream stream;
	stream<<x; 
	return stream.str();
}

void index_initial(string index_name,int initial_type)
{
	tree_index_name=index_name;
	type=initial_type;
	tree_root=0;
	for(int i=0;i<=99999;++i)
	{
		index_node[i].valid=0;
		index_node[i].is_leaf=0;
		index_node[i].number=0;
		index_node[i].father=-1;
		index_node[i].left_sib=-1;
		index_node[i].right_sib=-1;
		for(int j=0;j<=3;++j) index_node[i].child[j]=-1;
	}
	index_node[0].valid=1;
	index_node[0].is_leaf=1;
}

int pan_int_index(int temp_int,string op,string value)
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

int pan_float_index(float temp_float,string op,string value)
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

int pan_string_index(string temp_string,string op,string value)
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

void index_delete(string table_name,string index_name,int initial_type,string values)
{
	if(tree_index_name!=index_name)
	{
		if(tree_index_name!="") write_tree_back();
		construct_tree(index_name,initial_type);
	}
	int cur_node=tree_root;
	while(index_node[cur_node].is_leaf==0)
	{
		int point_loc;
		if(initial_type==0)
		{
			point_loc=index_node[cur_node].number;
			int insert_value=atoi(values.c_str());
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				int border=atoi(index_node[cur_node].child_value[i].c_str());
				if(border>insert_value)
				{
					point_loc=i;
					break;
				}
			}
		}
		else if(initial_type==-1)
		{
			point_loc=index_node[cur_node].number;
			float insert_value=atof(values.c_str());
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				int border=atof(index_node[cur_node].child_value[i].c_str());
				if(border>insert_value)
				{
					point_loc=i;
					break;
				}
			}
		}
		else
		{
			point_loc=index_node[cur_node].number;
			for(int i=0;i<index_node[cur_node].number;++i)
			{
				if(index_node[cur_node].child_value[i]>values)
				{
					point_loc=i;
					break;
				}
			}
		}
		cur_node=index_node[cur_node].child[point_loc];
	}
	int point_loc;
	for(int i=0;i<=index_node[cur_node].number-1;++i)
	{
		if(index_node[cur_node].child_value[i]==values)
		{
			point_loc=i;
			break;
		}
	}
	string update_value=index_node[cur_node].child_value[0];
	int update_loc=index_node[cur_node].child_value_loc[0];
	for(int i=point_loc;i<index_node[cur_node].number-1;++i)
	{
		index_node[cur_node].child_value[i]=index_node[cur_node].child_value[i+1];
		index_node[cur_node].child_value_loc[i]=index_node[cur_node].child_value_loc[i+1];		
	}
	index_node[cur_node].number--;
	if(index_node[cur_node].father==-1) 
	{	
		return;
	}
	if(index_node[cur_node].number<1)
	{
		int operate_left_node=index_node[cur_node].left_sib;
		int operate_right_node=index_node[cur_node].right_sib;
		if(operate_left_node!=-1&&index_node[cur_node].father==index_node[operate_left_node].father)//一定有一个同父的相邻节点 
		{
			if(index_node[operate_left_node].number>1)
			{
				for(int i=index_node[cur_node].number;i>0;--i)
				{
					index_node[cur_node].child_value[i]=index_node[cur_node].child_value[i-1];
					index_node[cur_node].child_value_loc[i]=index_node[cur_node].child_value_loc[i-1];
				}
				index_node[cur_node].number++;
				index_node[cur_node].child_value[0]=index_node[operate_left_node].child_value[index_node[operate_left_node].number-1];
				index_node[cur_node].child_value_loc[0]=index_node[operate_left_node].child_value_loc[index_node[operate_left_node].number-1];
				index_node[operate_left_node].number--;
				string change_value=index_node[cur_node].child_value[0];
				int change_loc=index_node[cur_node].child_value_loc[0];
				int update_node=index_node[cur_node].father;
				for(int i=0;i<index_node[update_node].number;++i)
				{
					if(index_node[update_node].child_value[i]==update_value)
					{
						index_node[update_node].child_value[i]=change_value;
						index_node[update_node].child_value_loc[i]=change_loc;
						break;
					}
				}
			}
			else
			{
				for(int i=0;i<index_node[cur_node].number;++i)
				{
					index_node[operate_left_node].child_value[index_node[operate_left_node].number+i]=index_node[cur_node].child_value[i];
					index_node[operate_left_node].child_value_loc[index_node[operate_left_node].number+i]=index_node[cur_node].child_value_loc[i];
				}
				index_node[cur_node].valid=0;
				index_node[operate_left_node].right_sib=index_node[cur_node].right_sib;
				index_node[index_node[cur_node].right_sib].left_sib=operate_left_node;
				index_node[operate_left_node].number+=index_node[cur_node].number;
				int update_node=index_node[cur_node].father;
				if(index_node[update_node].father==-1&&index_node[update_node].number==1)
				{
					index_node[update_node].valid=0;
					index_node[cur_node].father=-1;
					tree_root=cur_node;
					return;
				}
				int father_point;
				for(int i=0;i<index_node[update_node].number;++i)
				{
					if(update_value==index_node[update_node].child_value[i])
					{
						father_point=i;
						break;
					}
				}
				for(int i=father_point;i<index_node[update_node].number-1;++i)
				{
					index_node[update_node].child_value[i]=index_node[update_node].child_value[i+1];
					index_node[update_node].child_value_loc[i]=index_node[update_node].child_value_loc[i+1];
					index_node[update_node].child[i+1]=index_node[update_node].child[i+2];
				}
				index_node[update_node].number--;
				cur_node=update_node;
				while(1)
				{
					if(index_node[cur_node].number<1)
					{
						int temp_left_node=index_node[cur_node].left_sib;
						int temp_right_node=index_node[cur_node].right_sib;
						if(temp_left_node!=-1&&index_node[cur_node].father==index_node[temp_left_node].father)
						{
							if(index_node[temp_left_node].number>1)
							{
								update_node=index_node[cur_node].father;
								for(int i=0;i<=index_node[update_node].number;++i)
								{
									if(index_node[update_node].child[i]==cur_node)
									{
										father_point=i-1;
										break;
									}
								}
								for(int i=index_node[cur_node].number;i>0;--i)
								{
									index_node[cur_node].child_value[i]=index_node[cur_node].child_value[i-1];
									index_node[cur_node].child_value_loc[i]=index_node[cur_node].child_value_loc[i-1];
									index_node[cur_node].child[i+1]=index_node[cur_node].child[i];
								}
								index_node[cur_node].child[1]=index_node[cur_node].child[0];
								index_node[cur_node].number++;
								index_node[cur_node].child_value[0]=index_node[update_node].child_value[father_point];
								index_node[cur_node].child_value_loc[0]=index_node[update_node].child_value_loc[father_point];
								index_node[cur_node].child[0]=index_node[temp_left_node].child[index_node[temp_left_node].number];
								index_node[index_node[cur_node].child[0]].father=cur_node;
								index_node[update_node].child_value[father_point]=index_node[temp_left_node].child_value[index_node[temp_left_node].number-1];
								index_node[update_node].child_value_loc[father_point]=index_node[temp_left_node].child_value_loc[index_node[temp_left_node].number-1];
								index_node[temp_left_node].number--;
								break;
							}
							else
							{
								update_node=index_node[cur_node].father;
								for(int i=0;i<=index_node[update_node].number;++i)
								{
									if(index_node[update_node].child[i]==cur_node)
									{
										father_point=i-1;
										break;
									}
								}
								index_node[temp_left_node].number++;
								index_node[temp_left_node].child_value[index_node[temp_left_node].number-1]=index_node[update_node].child_value[father_point];
								index_node[temp_left_node].child_value_loc[index_node[temp_left_node].number-1]=index_node[update_node].child_value_loc[father_point];
								index_node[temp_left_node].child[index_node[temp_left_node].number]=index_node[cur_node].child[0];
								index_node[index_node[cur_node].child[0]].father=temp_left_node;
								index_node[temp_left_node].right_sib=index_node[cur_node].right_sib;
								index_node[index_node[cur_node].right_sib].left_sib=temp_left_node;
								index_node[cur_node].valid=0;
								if(index_node[update_node].father==-1&&index_node[update_node].number==1)
								{
									index_node[update_node].valid=0;
									index_node[temp_left_node].father=-1;
									tree_root=temp_left_node;
								}
								else
								{
									for(int i=father_point;i<index_node[update_node].number-1;++i)
									{
										index_node[update_node].child_value[i]=index_node[update_node].child_value[i+1];
										index_node[update_node].child_value_loc[i]=index_node[update_node].child_value_loc[i+1];
										index_node[update_node].child[i+1]=index_node[update_node].child[i+2];
									}
									index_node[update_node].number--;
									cur_node=update_node;
								}
							}
						}
						else
						{
							if(index_node[temp_right_node].number>1)
							{
								update_node=index_node[cur_node].father;
								for(int i=0;i<=index_node[update_node].number;++i)
								{
									if(index_node[update_node].child[i]==cur_node)
									{
										father_point=i;
										break;
									}
								}
								index_node[cur_node].number++;
								index_node[cur_node].child_value[index_node[cur_node].number-1]=index_node[update_node].child_value[father_point];
								index_node[cur_node].child_value_loc[index_node[cur_node].number-1]=index_node[update_node].child_value_loc[father_point];
								index_node[update_node].child_value[father_point]=index_node[temp_right_node].child_value[0];
								index_node[update_node].child_value_loc[father_point]=index_node[temp_right_node].child_value_loc[0];
								index_node[cur_node].child[index_node[cur_node].number]=index_node[temp_right_node].child[0];
								index_node[index_node[temp_right_node].child[0]].father=cur_node;
								for(int i=0;i<index_node[temp_right_node].number-1;++i)
								{
									index_node[temp_right_node].child_value[i]=index_node[temp_right_node].child_value[i+1];
									index_node[temp_right_node].child_value_loc[i]=index_node[temp_right_node].child_value_loc[i+1];
									index_node[temp_right_node].child[i]=index_node[temp_right_node].child[i+1];
								}
								index_node[temp_right_node].child[index_node[temp_right_node].number-1]=index_node[temp_right_node].child[index_node[temp_right_node].number];
								index_node[temp_right_node].number--;
								break;
							}
							else
							{
								update_node=index_node[cur_node].father;
								for(int i=0;i<=index_node[update_node].number;++i)
								{
									if(index_node[update_node].child[i]==cur_node)
									{
										father_point=i;
										break;
									}
								}
								index_node[cur_node].number++;
								index_node[cur_node].child_value[index_node[cur_node].number-1]=index_node[update_node].child_value[father_point];
								index_node[cur_node].child_value_loc[index_node[cur_node].number-1]=index_node[update_node].child_value_loc[father_point];
								index_node[cur_node].child[1]=index_node[temp_right_node].child[0];
								index_node[index_node[temp_right_node].child[0]].father=cur_node;
								for(int i=0;i<=index_node[temp_right_node].number-1;++i)
								{
									index_node[cur_node].child_value[index_node[cur_node].number+i]=index_node[temp_right_node].child_value[i];
									index_node[cur_node].child_value_loc[index_node[cur_node].number+i]=index_node[temp_right_node].child_value_loc[i];
									index_node[cur_node].child[index_node[cur_node].number+1+i]=index_node[temp_right_node].child[i+1];
									index_node[index_node[temp_right_node].child[i+1]].father=cur_node;
								}
								index_node[cur_node].number+=index_node[temp_right_node].number;
								index_node[cur_node].right_sib=index_node[temp_right_node].right_sib;
								index_node[index_node[temp_right_node].right_sib].left_sib=cur_node;
								index_node[temp_right_node].valid=0;
								if(index_node[update_node].father==-1&&index_node[update_node].number==1)
								{
									index_node[update_node].valid=0;
									index_node[cur_node].father=-1;
									tree_root=cur_node;
								}
								else
								{
									for(int i=father_point;i<index_node[update_node].number-1;++i)
									{
										index_node[update_node].child_value[i]=index_node[update_node].child_value[i+1];
										index_node[update_node].child_value_loc[i]=index_node[update_node].child_value_loc[i+1];
										index_node[update_node].child[i+1]=index_node[update_node].child[i+2];
									}
									index_node[update_node].number--;
									cur_node=update_node;
								}
							}
						}
					}
					else break;	
				}	
			}
		}
		else 
		{
			if(index_node[operate_right_node].number>1)
			{
				string temp_update_value=update_value;
				update_value=index_node[operate_right_node].child_value[0];
				update_loc=index_node[operate_right_node].child_value_loc[0];
				index_node[cur_node].number++;
				index_node[cur_node].child_value[index_node[cur_node].number-1]=index_node[operate_right_node].child_value[0];
				index_node[cur_node].child_value_loc[index_node[cur_node].number-1]=index_node[operate_right_node].child_value_loc[0];
				for(int i=0;i<index_node[operate_right_node].number-1;++i)
				{
					index_node[operate_right_node].child_value[i]=index_node[operate_right_node].child_value[i+1];
					index_node[operate_right_node].child_value_loc[i]=index_node[operate_right_node].child_value_loc[i+1];
				}
				index_node[operate_right_node].number--;
				string change_value=index_node[operate_right_node].child_value[0];
				int change_loc=index_node[operate_right_node].child_value_loc[0];
				int update_node=index_node[operate_right_node].father;
				for(int i=0;i<index_node[update_node].number;++i)
				{
					if(index_node[update_node].child_value[i]==update_value)
					{
						index_node[update_node].child_value[i]=change_value;
						index_node[update_node].child_value_loc[i]=change_loc;
						break;
					}
				}
				string temp_change_value=index_node[cur_node].child_value[0];
				int temp_change_loc=index_node[cur_node].child_value_loc[0];
				int flag_change=0;
				while(1)
				{
					int temp_update_node=index_node[cur_node].father;
					if(temp_update_node==-1) break;
					for(int i=0;i<index_node[temp_update_node].number;++i)
					{
						if(index_node[temp_update_node].child_value[i]==temp_update_value)
						{
							index_node[temp_update_node].child_value[i]=temp_change_value;
							index_node[temp_update_node].child_value_loc[i]=temp_change_loc;
							flag_change=1;
							break;
						}
					}
					if(flag_change==1) break;
					cur_node=temp_update_node;
				}
			}
			else
			{
				index_node[cur_node].number++;
				index_node[cur_node].child_value[index_node[cur_node].number-1]=index_node[operate_right_node].child_value[0];
				index_node[cur_node].child_value_loc[index_node[cur_node].number-1]=index_node[operate_right_node].child_value_loc[0];
				index_node[cur_node].right_sib=index_node[operate_right_node].right_sib;
				index_node[index_node[operate_right_node].right_sib].left_sib=cur_node;
				index_node[operate_right_node].valid=0;
				if(point_loc==0)
				{
					int temp_cur_node=cur_node;
					string change_value=index_node[cur_node].child_value[0];
					int change_loc=index_node[cur_node].child_value_loc[0];
					int flag_change=0;
					while(1)
					{
						int update_node=index_node[temp_cur_node].father;
						if(update_node==-1) break;
						for(int i=0;i<index_node[update_node].number;++i)
						{
							if(index_node[update_node].child_value[i]==update_value)
							{
								index_node[update_node].child_value[i]=change_value;
								index_node[update_node].child_value_loc[i]=change_loc;
								flag_change=1;
								break;
							}
						}
						if(flag_change==1) break;
						temp_cur_node=update_node;
					}
				}
				int update_node=index_node[cur_node].father;
				if(index_node[update_node].father==-1&&index_node[update_node].number==1)
				{
					index_node[update_node].valid=0;
					index_node[cur_node].father=-1;
					tree_root=cur_node;
					return;
				}
				int father_point;
				for(int i=0;i<index_node[update_node].number;++i)
				{
					if(update_value==index_node[update_node].child_value[i])
					{
						father_point=i;
						break;
					}
				}
				for(int i=father_point;i<index_node[update_node].number-1;++i)
				{
					index_node[update_node].child_value[i]=index_node[update_node].child_value[i+1];
					index_node[update_node].child_value_loc[i]=index_node[update_node].child_value_loc[i+1];
					index_node[update_node].child[i+1]=index_node[update_node].child[i+2];
				}
				index_node[update_node].number--;
				cur_node=update_node;
				while(1)
				{
					if(index_node[cur_node].number<1)
					{
						int temp_left_node=index_node[cur_node].left_sib;
						int temp_right_node=index_node[cur_node].right_sib;
						if(temp_left_node!=-1&&index_node[cur_node].father==index_node[temp_left_node].father)
						{
							if(index_node[temp_left_node].number>1)
							{
								update_node=index_node[cur_node].father;
								for(int i=0;i<=index_node[update_node].number;++i)
								{
									if(index_node[update_node].child[i]==cur_node)
									{
										father_point=i-1;
										break;
									}
								}
								for(int i=index_node[cur_node].number;i>0;--i)
								{
									index_node[cur_node].child_value[i]=index_node[cur_node].child_value[i-1];
									index_node[cur_node].child_value_loc[i]=index_node[cur_node].child_value_loc[i-1];
									index_node[cur_node].child[i+1]=index_node[cur_node].child[i];
								}
								index_node[cur_node].child[1]=index_node[cur_node].child[0];
								index_node[cur_node].number++;
								index_node[cur_node].child_value[0]=index_node[update_node].child_value[father_point];
								index_node[cur_node].child_value_loc[0]=index_node[update_node].child_value_loc[father_point];
								index_node[cur_node].child[0]=index_node[temp_left_node].child[index_node[temp_left_node].number];
								index_node[index_node[cur_node].child[0]].father=cur_node;
								index_node[update_node].child_value[father_point]=index_node[temp_left_node].child_value[index_node[temp_left_node].number-1];
								index_node[update_node].child_value_loc[father_point]=index_node[temp_left_node].child_value_loc[index_node[temp_left_node].number-1];
								index_node[temp_left_node].number--;
								break;
							}
							else
							{
								update_node=index_node[cur_node].father;
								for(int i=0;i<=index_node[update_node].number;++i)
								{
									if(index_node[update_node].child[i]==cur_node)
									{
										father_point=i-1;
										break;
									}
								}
								index_node[temp_left_node].number++;
								index_node[temp_left_node].child_value[index_node[temp_left_node].number-1]=index_node[update_node].child_value[father_point];
								index_node[temp_left_node].child_value_loc[index_node[temp_left_node].number-1]=index_node[update_node].child_value_loc[father_point];
								index_node[temp_left_node].child[index_node[temp_left_node].number]=index_node[cur_node].child[0];
								index_node[index_node[cur_node].child[0]].father=temp_left_node;
								index_node[temp_left_node].right_sib=index_node[cur_node].right_sib;
								index_node[index_node[cur_node].right_sib].left_sib=temp_left_node;
								index_node[cur_node].valid=0;
								if(index_node[update_node].father==-1&&index_node[update_node].number==1)
								{
									index_node[update_node].valid=0;
									index_node[temp_left_node].father=-1;
									tree_root=temp_left_node;
								}
								else
								{
									for(int i=father_point;i<index_node[update_node].number-1;++i)
									{
										index_node[update_node].child_value[i]=index_node[update_node].child_value[i+1];
										index_node[update_node].child_value_loc[i]=index_node[update_node].child_value_loc[i+1];
										index_node[update_node].child[i+1]=index_node[update_node].child[i+2];
									}
									index_node[update_node].number--;
									cur_node=update_node;
								}
							}
						}
						else
						{
							if(index_node[temp_right_node].number>1)
							{
								update_node=index_node[cur_node].father;
								for(int i=0;i<=index_node[update_node].number;++i)
								{
									if(index_node[update_node].child[i]==cur_node)
									{
										father_point=i;
										break;
									}
								}
								index_node[cur_node].number++;
								index_node[cur_node].child_value[index_node[cur_node].number-1]=index_node[update_node].child_value[father_point];
								index_node[cur_node].child_value_loc[index_node[cur_node].number-1]=index_node[update_node].child_value_loc[father_point];
								index_node[update_node].child_value[father_point]=index_node[temp_right_node].child_value[0];
								index_node[update_node].child_value_loc[father_point]=index_node[temp_right_node].child_value_loc[0];
								index_node[cur_node].child[index_node[cur_node].number]=index_node[temp_right_node].child[0];
								index_node[index_node[temp_right_node].child[0]].father=cur_node;
								for(int i=0;i<index_node[temp_right_node].number-1;++i)
								{
									index_node[temp_right_node].child_value[i]=index_node[temp_right_node].child_value[i+1];
									index_node[temp_right_node].child_value_loc[i]=index_node[temp_right_node].child_value_loc[i+1];
									index_node[temp_right_node].child[i]=index_node[temp_right_node].child[i+1];
								}
								index_node[temp_right_node].child[index_node[temp_right_node].number-1]=index_node[temp_right_node].child[index_node[temp_right_node].number];
								index_node[temp_right_node].number--;
								break;
							}
							else
							{
								update_node=index_node[cur_node].father;
								for(int i=0;i<=index_node[update_node].number;++i)
								{
									if(index_node[update_node].child[i]==cur_node)
									{
										father_point=i;
										break;
									}
								}
								index_node[cur_node].number++;
								index_node[cur_node].child_value[index_node[cur_node].number-1]=index_node[update_node].child_value[father_point];
								index_node[cur_node].child_value_loc[index_node[cur_node].number-1]=index_node[update_node].child_value_loc[father_point];
								index_node[cur_node].child[1]=index_node[temp_right_node].child[0];
								index_node[index_node[cur_node].child[1]].father=cur_node;
								for(int i=0;i<=index_node[temp_right_node].number-1;++i)
								{
									index_node[cur_node].child_value[index_node[cur_node].number+i]=index_node[temp_right_node].child_value[i];
									index_node[cur_node].child_value_loc[index_node[cur_node].number+i]=index_node[temp_right_node].child_value_loc[i];
									index_node[cur_node].child[index_node[cur_node].number+1+i]=index_node[temp_right_node].child[i+1];
									index_node[index_node[temp_right_node].child[i+1]].father=cur_node;
								}
								index_node[cur_node].number+=index_node[temp_right_node].number;
								index_node[cur_node].right_sib=index_node[temp_right_node].right_sib;
								index_node[index_node[temp_right_node].right_sib].left_sib=cur_node;
								index_node[temp_right_node].valid=0;
								if(index_node[update_node].father==-1&&index_node[update_node].number==1)
								{
									index_node[update_node].valid=0;
									index_node[cur_node].father=-1;
									tree_root=cur_node;
								}
								else
								{
									for(int i=father_point;i<index_node[update_node].number-1;++i)
									{
										index_node[update_node].child_value[i]=index_node[update_node].child_value[i+1];
										index_node[update_node].child_value_loc[i]=index_node[update_node].child_value_loc[i+1];
										index_node[update_node].child[i+1]=index_node[update_node].child[i+2];
									}
									index_node[update_node].number--;
									cur_node=update_node;
								}
							}
						}
					}
					else break;	
				}	
			} 
		}
	}
	else
	{
		if(point_loc==0)
		{
			string change_value=index_node[cur_node].child_value[0];
			int change_loc=index_node[cur_node].child_value_loc[0];
			int flag_change=0;
			while(1)
			{
				int update_node=index_node[cur_node].father;
				if(update_node==-1) break;
				for(int i=0;i<index_node[update_node].number;++i)
				{
					if(index_node[update_node].child_value[i]==update_value)
					{
						index_node[update_node].child_value[i]=change_value;
						index_node[update_node].child_value_loc[i]=change_loc;
						flag_change=1;
						break;
					}
				}
				if(flag_change==1) break;
				cur_node=update_node;
			}
		}
	}
	return;
}

void wIA()
{
	if(tree_index_name!="") write_tree_back();
	return;
}
