#include"Buffer_manager.h"
extern struct Block block[100];
extern int order[100];
int addBlock(string table_name,int block_loc);
void writeBack(int block_number);

int getBlock(string table_name,int block_loc)
{
	for(int i=0;i<=99;++i)
	{
		if(block[i].full==1&&block[i].file_name==table_name&&block[i].file_block_number==block_loc)
		{
			int temp;
			for(int j=0;j<=99;++j)			//将读取的block在替换顺序上移到最后面 
				if(order[j]==i) temp=j;
			for(int j=temp+1;j<=99;++j)
				order[j-1]=order[j];
			order[99]=i;
			return i;
		}
	}
	int return_value=addBlock(table_name,block_loc); 
	int temp;
	for(int j=0;j<=99;++j)			//将读取的block在替换顺序上移到最后面 
		if(order[j]==return_value) temp=j;
	for(int j=temp+1;j<=99;++j)
		order[j-1]=order[j];
	order[99]=return_value;
	return return_value;
}

int getRecordTotal(string table_name)
{
	int x=0;
	ifstream file("Record_number.txt");
	string table;
	while(file>>table)
	{
		if(table==table_name)
		{
			file>>x;
			break;
		}
	}
	file.close();
	return x;
}

int getIndexRoot(string index_name)
{
	int x=0;
	ifstream file("Index_root.txt");
	string index;
	while(file>>index)
	{
		if(index==index_name)
		{
			file>>x;
			break;
		}
	}
	file.close();
	return x;
}

void setRecordTotal(string table_name,int record_total)
{
	int flag=0;
	int x;
	ifstream file("Record_number.txt");
	ofstream temp("Temp.txt");
	string table;
	while(file>>table)
	{
		file>>x;
		temp<<table<<" ";
		if(table==table_name)
		{
			flag=1;
			temp<<record_total<<endl;
		}
		else temp<<x<<endl;
	}
	x=1;
	if(flag==0) 
	{
		temp<<table_name<<" "<<x<<endl;
	}
	file.close();
	temp.close();
	DeleteFile("Record_number.txt");
	rename("Temp.txt","Record_number.txt");
}

void setIndexRoot(string index_name,int root)
{
	int flag=0;
	int x;
	ifstream file("Index_root.txt");
	ofstream temp("Temp.txt");
	string index;
	while(file>>index)
	{
		file>>x;
		temp<<index<<" ";
		if(index==index_name)
		{
			flag=1;
			temp<<root<<endl;
		}
		else temp<<x<<endl;
	}
	x=1;
	if(flag==0) 
	{
		temp<<index_name<<" "<<root<<endl;
	}
	file.close();
	temp.close();
	DeleteFile("Index_root.txt");
	rename("Temp.txt","Index_root.txt");
}

void writeBackAll()
{
	for(int i=0;i<=99;++i)
	{
		if(block[i].dirty==1&&block[i].full==1)
		{
			writeBack(i);
		}
	}
	return;
}

int addBlock(string table_name,int block_loc)
{
	string file_name=table_name+".data";
	ifstream file_in(file_name.c_str());
	int flag=-1;
	for(int i=0;i<=99;++i)
	{
		if(block[i].full==0)
		{
			block[i].file_name=table_name;
			block[i].file_block_number=block_loc;
			flag=i;
			block[i].full=1;
			break;
		}
	}
	if(flag!=-1)
	{
		file_in.seekg(4096*block_loc,ios::beg);
		file_in.read(block[flag].buffer,4096);
		file_in.close();
		return flag;
	}
	else
	{
		int block_number=0;
		for(int i=0;i<=99;++i)
		{
			if(block[order[i]].pin==0)
			{
				block_number=order[i];
				break;
			}
		}
		if(block[block_number].dirty==1&&block[block_number].full==1)
		{
			writeBack(block_number);
			block[block_number].dirty=0;
			block[block_number].file_name=table_name;
			block[block_number].file_block_number=block_loc;
		}
		file_in.seekg(4096*block_loc,ios::beg);
		file_in.read(block[block_number].buffer,4096);
		file_in.close();
		return block_number;
	}
}

void writeBack(int block_number)
{
	string file_name=block[block_number].file_name+".data";
	ofstream file_out(file_name.c_str(),ios::ate|ios::in);
	if(!file_out) 
	{
		ofstream temp(file_name.c_str());
		temp.close();
		ofstream file_out(file_name.c_str(),ios::ate|ios::in);
		file_out.seekp(block[block_number].file_block_number*4096,ios::beg);
		file_out.write(block[block_number].buffer,4096);
		file_out.close();
	}
	else
	{
		file_out.seekp(block[block_number].file_block_number*4096,ios::beg);
		file_out.write(block[block_number].buffer,4096);
		file_out.close();
	}
}

void clear(string table_name)
{
	for(int i=0;i<=99;++i)
	{
		if(block[i].file_name==table_name)
		{
			block[i].full=0;
		}
	}
	int x;
	ifstream file("Record_number.txt");
	ofstream temp("Temp.txt");
	string table;
	while(file>>table)
	{
		file>>x;
		temp<<table<<" ";
		if(table==table_name)
		{
			temp<<0<<endl;
		}
		else temp<<x<<endl;
	}
	file.close();
	temp.close();
	DeleteFile("Record_number.txt");
	rename("Temp.txt","Record_number.txt");
}

void clear_index(string index_name)
{
	for(int i=0;i<=99;++i)
	{
		if(block[i].file_name==index_name)
		{
			block[i].full=0;
		}
	}
	int x;
	ifstream file("Index_root.txt");
	ofstream temp("Temp.txt");
	string index;
	while(file>>index)
	{
		file>>x;
		temp<<index<<" ";
		if(index==index_name)
		{
			temp<<-1<<endl;
		}
		else temp<<x<<endl;
	}
	file.close();
	temp.close();
	DeleteFile("Index_root.txt");
	rename("Temp.txt","Index_root.txt");
}
