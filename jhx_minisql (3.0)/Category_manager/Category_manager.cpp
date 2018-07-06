#include"Category_manager.h"

int table_find(string table_name)
{
	ifstream Table("Table.txt");
	string temp_table_name;
	int temp_number;
	string temp_name[32];
	int temp_type[32];
	int temp_limit[32];
	int flag=0;
	while(Table>>temp_table_name)
	{
		if(temp_table_name==table_name) 
		{
			flag=1;
			break;
		}
		Table>>temp_number;
		for(int i=0;i<=temp_number;++i)
		{
			Table>>temp_name[i]>>temp_type[i]>>temp_limit[i];
		}
	}
	Table.close();
	return flag;
}

int index_find(string index_name)
{
	ifstream Index("Index.txt");
	string temp_index_name;
	string temp_table_name;
	string temp_attribute_name;
	int flag=0;
	while(Index>>temp_index_name)
	{
		if(temp_index_name==index_name) 
		{
			flag=1;
			break;
		}
		Index>>temp_table_name>>temp_attribute_name;
	}
	Index.close();
	return flag;
} 

string attribute_find_index(string table_name,string attribute_name)
{
	ifstream Index("Index.txt");
	string temp_index_name;
	string temp_table_name;
	string temp_attribute_name;
	string flag="0";
	while(Index>>temp_index_name)
	{
		Index>>temp_table_name>>temp_attribute_name;
		if(temp_table_name==table_name&&temp_attribute_name==attribute_name) 
		{
			flag=temp_index_name;
			break;
		}
	}
	Index.close();
	return flag;
} 

void category_create_table(string table_name,int attribute_number,string attribute_name[],int attribute_type[],int attribute_limit[])
{
	ofstream Table("Table.txt",ios::app);
	Table<<table_name<<" "<<attribute_number<<" ";
	for(int i=0;i<=attribute_number;++i)
	{
		Table<<attribute_name[i]<<" "<<attribute_type[i]<<" "<<attribute_limit[i]<<" ";
	}
	Table<<endl;
	Table.close();
	return;
}

void category_create_index(string index_name,string table_name,string attribute_name)
{
	ofstream Index("Index.txt",ios::app);
	Index<<index_name<<" "<<table_name<<" "<<attribute_name<<endl;
	Index.close();
	return;
}

void category_drop_table(string table_name)
{
	ifstream Table("Table.txt");
	ofstream Temp("Temp.txt");
	string temp_table_name;
	int temp_number;
	string temp_name[32];
	int temp_type[32];
	int temp_limit[32];
	while(Table>>temp_table_name)
	{
		Table>>temp_number;
		for(int i=0;i<=temp_number;++i)
		{
			Table>>temp_name[i]>>temp_type[i]>>temp_limit[i];
		}
		if(temp_table_name!=table_name)
		{
			Temp<<temp_table_name<<" "<<temp_number<<" ";
			for(int i=0;i<=temp_number;++i)
			{
				Temp<<temp_name[i]<<" "<<temp_type[i]<<" "<<temp_limit[i]<<" ";
			}
			Temp<<endl;
		}
	}
	Table.close();
	Temp.close();
	DeleteFile("Table.txt");
	rename("Temp.txt","Table.txt");
	return;
}

int table_index_find(string table_name,string index_name[],string attribute_name[])
{
	int index_number=0;
	ifstream Index("Index.txt");
	string temp_index_name;
	string temp_table_name;
	string temp_attribute_name;
	while(Index>>temp_index_name)
	{
		Index>>temp_table_name>>temp_attribute_name;
		if(temp_table_name==table_name)
		{
			index_name[index_number]=temp_index_name;
			attribute_name[index_number]=temp_attribute_name;
			index_number++;
		}
	}
	index_number--;
	Index.close();
	return index_number;
}

void category_drop_index(string index_name)
{
	ifstream Index("Index.txt");
	ofstream Temp("Temp.txt");
	string temp_index_name;
	string temp_table_name;
	string temp_attribute_name;
	while(Index>>temp_index_name)
	{
		Index>>temp_table_name>>temp_attribute_name;
		if(temp_index_name!=index_name)
		{
			Temp<<temp_index_name<<" "<<temp_table_name<<" "<<temp_attribute_name<<endl;
		}
	}
	Index.close();
	Temp.close();
	DeleteFile("Index.txt");
	rename("Temp.txt","Index.txt");
}

int get_attribute_list(string table_name,string initial_name[],int initial_type[],int initial_limit[])
{
	int initial_number;
	ifstream Table("Table.txt");
	string temp_table_name;
	int temp_number;
	string temp_name[32];
	int temp_type[32];
	int temp_limit[32];
	while(Table>>temp_table_name)
	{
		Table>>temp_number;
		for(int i=0;i<=temp_number;++i)
		{
			Table>>temp_name[i]>>temp_type[i]>>temp_limit[i];
		}
		if(temp_table_name==table_name) 
		{
			initial_number=temp_number;
			for(int i=0;i<=initial_number;++i)
			{
				initial_name[i]=temp_name[i];
				initial_type[i]=temp_type[i];
				initial_limit[i]=temp_limit[i];
			}
			break;
		}
	}
	Table.close();
	return initial_number;
}
