#include"API.h"

void create_table(string table_name,int attribute_number,string attribute_name[],int attribute_type[],int attribute_limit[])
{
	int exist;
	exist=table_find(table_name);
	if(exist==1)
	{
		cout<<"Table has existed"<<endl;
		return;
	}
	category_create_table(table_name,attribute_number,attribute_name,attribute_type,attribute_limit);
	string primary_attribute;
	for(int i=0;i<=attribute_number;++i)
	{
		if(attribute_limit[i]==2)
		{
			primary_attribute=attribute_name[i];
			break;
		}
	}
	int initial_number;
	string initial_name[32];
	int initial_type[32];
	int initial_limit[32];
	initial_number=get_attribute_list(table_name,initial_name,initial_type,initial_limit);
	category_create_index(table_name+"primary_index",table_name,primary_attribute);
	index_create(table_name,initial_number,initial_name,initial_type,table_name+"primary_index",primary_attribute);
	cout<<"Succeed"<<endl;
	return;
}

void create_index(string index_name,string table_name,string attribute_name)
{
	int exist;
	exist=index_find(index_name);
	if(exist==1)
	{
		cout<<"Index has existed"<<endl;
		return;
	}
	string exist_1; 
	exist_1=attribute_find_index(table_name,attribute_name);
	if(exist_1!="0")
	{
		cout<<"This attribute has already had index"<<endl;
		return;
	}
	int initial_number;
	string initial_name[32];
	int initial_type[32];
	int initial_limit[32];
	initial_number=get_attribute_list(table_name,initial_name,initial_type,initial_limit);
	for(int i=0;i<=initial_number;++i)
	{
		if(initial_name[i]==attribute_name)
		{
			if(initial_limit[i]!=1)
			{
				cout<<"This attribute is not unique"<<endl;
				return;
			}
		}
	}
	int flag=0;
	for(int i=0;i<=initial_number;++i)
	{
		if(attribute_name==initial_name[i])
		{
			flag=1;
			if(initial_limit[i]==0) 
			{
				cout<<"Illegal attribute"<<endl;
				return;
			}
			break;
		}
	}
	if(flag==0) 
	{
		cout<<"No such attribute"<<endl;
		return;
	 } 
	category_create_index(index_name,table_name,attribute_name);
	cout<<"Succeed"<<endl;
	index_create(table_name,initial_number,initial_name,initial_type,index_name,attribute_name);
	return;
}

void drop_table(string table_name)
{
	int exist;
	exist=table_find(table_name);
	if(exist==0) 
	{
		cout<<"Table has not existed"<<endl;
		return;
	}
	record_drop_table(table_name);
	string index_name[32];
	string attribute_name[32];
	int index_number;
	index_number=table_index_find(table_name,index_name,attribute_name);
	for(int i=0;i<=index_number;i++)
	{
		index_drop_index(index_name[i]);
		category_drop_index(index_name[i]);
	}
	category_drop_table(table_name);
	cout<<"Succeed"<<endl;
	return;
}

void drop_index(string index_name)
{
	int exist;
	exist=index_find(index_name);
	if(exist==0)
	{
		cout<<"Index has not existed"<<endl;
		return;
	}
	index_drop_index(index_name);
	category_drop_index(index_name);
	cout<<"Succeed"<<endl;
	return;
}

void select(string table_name,int limit_number,string limit_attribute[],string limit_op[],string limit_value[])
{
	int exist;
	exist=table_find(table_name);
	if(exist==0) 
	{
		cout<<"Table has not existed"<<endl;
		return;
	}
	int initial_number;
	string initial_name[32];
	int initial_type[32];
	int initial_limit[32];
	initial_number=get_attribute_list(table_name,initial_name,initial_type,initial_limit);
	int flag=0;
	for(int i=0;i<=limit_number;++i)
	{
		flag=0;
		for(int j=0;j<=initial_number;++j)
		{
			if(limit_attribute[i]==initial_name[j])
			{
				flag=1;
				break;
			}
		}
		if(flag==0) 
		{
			cout<<"No such attribute"<<endl;
			return;
		}
	}
	int index_pos=-1;
	string index_name;
	for(int i=0;i<=limit_number;++i)
	{
		index_name=attribute_find_index(table_name,limit_attribute[i]);
		if(index_name!="0")
		{
			index_pos=i;
			break;
		}
	}
	if(index_pos==-1)
	{
		int flag_1;
		flag_1=record_select(table_name,initial_number,initial_name,initial_type,limit_number,limit_attribute,limit_op,limit_value);
		if(flag_1==1) cout<<"Succeed"<<endl;
	}
	else
	{
		string temp_attribute,temp_op,temp_value;
		temp_attribute=limit_attribute[index_pos];
		temp_op=limit_op[index_pos];
		temp_value=limit_value[index_pos];
		limit_attribute[index_pos]=limit_attribute[0];
		limit_op[index_pos]=limit_op[0];
		limit_value[index_pos]=limit_value[0];
		limit_attribute[0]=temp_attribute;
		limit_op[0]=temp_op;
		limit_value[0]=temp_value;
		int flag_1;
		flag_1=index_select(table_name,index_name,initial_number,initial_name,initial_type,limit_number,limit_attribute,limit_op,limit_value);
		if(flag_1==1) cout<<"Succeed"<<endl;
	}
	return;
}

void insert(string table_name,int values_number,string values[])
{
	int exist;
	exist=table_find(table_name);
	if(exist==0) 
	{
		cout<<"Table has not existed"<<endl;
		return;
	}
	int initial_number;
	string initial_name[32];
	int initial_type[32];
	int initial_limit[32];
	initial_number=get_attribute_list(table_name,initial_name,initial_type,initial_limit);
	if(initial_number!=values_number)
	{
		cout<<"Synax error"<<endl;
		return;
	}
	int flag=record_insert(table_name,initial_number,initial_name,initial_type,initial_limit,values);
	if(flag!=-1) 
	{
		cout<<"Succeed"<<endl;
		string attribute_name[32];
		string index_name[32];
		int index_number;
		index_number=table_index_find(table_name,index_name,attribute_name);
		for(int i=0;i<=index_number;++i)
		{
			for(int j=0;j<=initial_number;++j)
			{
				if(attribute_name[i]==initial_name[j])
				{
					index_insert(table_name,index_name[i],attribute_name[i],initial_type[j],values[j],flag);
					break;
				}
			}
		}
	}
	return;
}

void deletion(string table_name,int limit_number,string limit_attribute[],string limit_op[],string limit_value[])
{
	int exist;
	exist=table_find(table_name);
	if(exist==0) 
	{
		cout<<"Table has not existed"<<endl;
		return;
	}
	int initial_number;
	string initial_name[32];
	int initial_type[32];
	int initial_limit[32];
	initial_number=get_attribute_list(table_name,initial_name,initial_type,initial_limit);
	int flag=0;
	for(int i=0;i<=limit_number;++i)
	{
		flag=0;
		for(int j=0;j<=initial_number;++j)
		{
			if(limit_attribute[i]==initial_name[j])
			{
				flag=1;
				break;
			}
		}
		if(flag==0) 
		{
			cout<<"No such attribute"<<endl;
			return;
		}
	}
	int index_pos=-1;
	string index_name;
	for(int i=0;i<=limit_number;++i)
	{
		index_name=attribute_find_index(table_name,limit_attribute[i]);
		if(index_name!="0")
		{
			index_pos=i;
			break;
		}
		if(index_pos!=-1) break;
	}
	if(index_pos==-1)
	{
		int flag_1=record_deletion(table_name,initial_number,initial_name,initial_type,limit_number,limit_attribute,limit_op,limit_value);
		if(flag_1==1) cout<<"Succeed"<<endl;
	}
	else
	{
		string temp_attribute,temp_op,temp_value;
		temp_attribute=limit_attribute[index_pos];
		temp_op=limit_op[index_pos];
		temp_value=limit_value[index_pos];
		limit_attribute[index_pos]=limit_attribute[0];
		limit_op[index_pos]=limit_op[0];
		limit_value[index_pos]=limit_value[0];
		limit_attribute[0]=temp_attribute;
		limit_op[0]=temp_op;
		limit_value[0]=temp_value;
		int flag_1;
		flag_1=index_deletion(table_name,index_name,initial_number,initial_name,initial_type,limit_number,limit_attribute,limit_op,limit_value);
		if(flag_1==1) cout<<"Succeed"<<endl;
	}
	return; 
}

void quit()
{
	wIA();
	wBA();
}

