#include"interpreter.h"
using namespace std;

int pan_name(string);
int zhuan_int(string);
void execute()
{
	char c[10000],d;
	string s[1000];
	int cc,Error,ss;
	while(1)
	{
		Error=0;
		cout<<"jhx_minisql-->";
		cc=0;
		while(1)
		{
			c[cc]=getchar();
			if(c[cc]=='\n') 
			{
				c[cc]=' ';
				cout<<"           -->";
			}
			cc++;
			if(c[cc-1]==';') break;
		}
		cc--;
		while(1)//分号后边仍然有东西 
		{
			d=getchar();
			if(d=='\n') break;
			Error=1;
		}
		if(Error==1) 
		{
			cout<<"Synax error"<<endl;
			continue;
		}
		for(int i=0;i<=cc;++i)
		{
			if(c[i]==';'||c[i]=='('||c[i]==')'||c[i]=='\'') c[i]=' ';
		}
		ss=0;
		for(int i=0;i<=999;++i)
			s[i]="";
		for(int i=0;i<=cc;++i)
		{
			if(c[i]==' ')
			{
				if(s[ss]!="") ss++;
			}
			else
			{
				if(c[i]==',')
				{
					if(s[ss]!="") ss++;
					s[ss]+=c[i];
					ss++;
				}
				else s[ss]+=c[i];
			}
		}
		ss--;
		if(ss<0) 
		{
			cout<<"Synax error"<<endl;
			continue;
		}
		int sss=0;
		if(s[sss]=="create")
		{
			sss++;
			if(sss>ss) 
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			if(s[sss]=="table")
			{
				sss++;
				if(sss>ss)
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				string table_name;
				if(pan_name(s[sss])==0)
				{
					cout<<"Table name error"<<endl;
					continue;
				}
				table_name=s[sss];
				sss++;
				if(sss>ss)
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				int attribute_number=0;
				string attribute_name[32];
				int attribute_type[32];
				int attribute_limit[32];
				int flag=0;
				while(1)//开始读入属性列表 
				{
					if(s[sss]=="primary")
					{
						sss++;
						if(sss>ss)
						{
							cout<<"Synax error"<<endl;
							flag=1;
							break;
						}
						if(s[sss]!="key")
						{
							cout<<"Synax error"<<endl;
							flag=1;
							break;
						}
						sss++;
						if(sss>ss)
						{
							cout<<"Synax error"<<endl;
							flag=1;
							break;
						}
						if(pan_name(s[sss])==0)
						{
							cout<<"Attribute name error"<<endl;
							flag=1;
							break;
						}
						int flag1=0;
						for(int i=0;i<=attribute_number-1;++i)
						{
							if(attribute_name[i]==s[sss])
							{
								flag1=1;
								attribute_limit[i]=2;//主键设定为2 
								break;
							}
						}
						if(flag1==0)
						{
							cout<<"Primary key is not existed"<<endl;
							flag=1;
							break;
						}
						sss++;
						if(sss>ss) break;
						else
						{
							cout<<"Synax error"<<endl;
							flag=1;
							break;
						} 
					}
					else
					{
						if(pan_name(s[sss])==0)
						{
							cout<<"Attribute name error"<<endl;
							flag=1;
							break;
						}
						attribute_name[attribute_number]=s[sss];
						sss++;
						if(sss>ss)
						{
							cout<<"Synax error"<<endl;
							flag=1;
							break;
						}
						if(s[sss]=="int") attribute_type[attribute_number]=0;//用于判断数据类型并记录，int记0，float记-1，char记有几位 
						else if(s[sss]=="float") attribute_type[attribute_number]=-1;
						else if(s[sss]=="char")
						{
							sss++;
							if(sss>ss)
							{
								cout<<"Synax error"<<endl;
								flag=1;
								break;
							}
							attribute_type[attribute_number]=zhuan_int(s[sss]);
						}
						else
						{
							cout<<"Type error"<<endl;
							flag=1;
							break;
						}
						sss++;
						if(sss>ss) 
						{
							cout<<"No primary key"<<endl;
							flag=1;
							break;
						}
						if(s[sss]==",") 
						{	
							attribute_limit[attribute_number]=0;//没有限制设定为0 
						}
						else if(s[sss]=="unique") 
						{
							attribute_limit[attribute_number]=1;//unique设定为1
							sss++;
							if(sss>ss) 
							{
								cout<<"No primary key"<<endl;
								flag=1;
								break;//属性登记结束
							}
							if(s[sss]!=",")
							{
								cout<<"synax error"<<endl;
								flag=1;
								break;
							} 
						}
						else
						{
							cout<<"Limit error"<<endl;
							flag=1;
							break;
						} 
						sss++;
						if(sss>ss) 
						{
							cout<<"synax error"<<endl;
							flag=1;
							break;
						}
						attribute_number++;
					}
				}
				attribute_number--;
				if(flag==1) continue;
				create_table(table_name,attribute_number,attribute_name,attribute_type,attribute_limit);
			}
			else if(s[sss]=="index")
			{
				sss++;
				if(sss>ss)
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				string index_name;
				if(pan_name(s[sss])==0)
				{
					cout<<"Index name error"<<endl;
					continue;
				}
				index_name=s[sss];
				sss++;
				if(sss>ss)
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				if(s[sss]!="on")
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				sss++;
				if(sss>ss)
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				string table_name;
				if(pan_name(s[sss])==0)
				{
					cout<<"Table name error"<<endl;
					continue;
				}
				table_name=s[sss];
				sss++;
				if(sss>ss)
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				string attribute_name;
				if(pan_name(s[sss])==0)
				{
					cout<<"Attribute name error"<<endl;
					continue;
				}
				attribute_name=s[sss];
				sss++;
				if(sss<=ss) 
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				create_index(index_name,table_name,attribute_name);
			}
			else
			{
				cout<<"Synax error"<<endl;
				continue;
			}
		}
		else if(s[sss]=="drop")
		{
			sss++;
			if(sss>ss) 
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			if(s[sss]=="table")
			{
				sss++;
				if(sss>ss) 
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				string table_name;
				if(pan_name(s[sss])==0)
				{
					cout<<"Table name error"<<endl;
					continue;
				}
				table_name=s[sss];
				sss++;
				if(sss<=ss) 
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				drop_table(table_name);
			}
			else if(s[sss]=="index")
			{
				sss++;
				if(sss>ss) 
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				string index_name;
				if(pan_name(s[sss])==0)
				{
					cout<<"Index name error"<<endl;
					continue;
				}
				index_name=s[sss];
				sss++;
				if(sss<=ss) 
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				drop_index(index_name);
			}
			else
			{
				cout<<"Synax error"<<endl;
				continue;
			}
		}
		else if(s[sss]=="select")
		{
			sss++;
			if(sss>ss) 
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			if(s[sss]!="*")
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			sss++;
			if(sss>ss) 
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			if(s[sss]!="from")
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			sss++;
			if(sss>ss) 
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			string table_name;
			if(pan_name(s[sss])==0)
			{
				cout<<"Table name error"<<endl;
				continue;
			}
			table_name=s[sss];
			sss++;
			int limit_number=0;
			string limit_attribute[32];
			string limit_op[32];
			string limit_value[32];
			if(sss>ss) 
			{
				select(table_name,-1,limit_attribute,limit_op,limit_value);
				continue;
			}
			if(s[sss]!="where")
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			sss++;
			if(sss<=ss) 
			{
				int flag=0;
				while(1)
				{
					if(pan_name(s[sss])==0)
					{
						cout<<"Attribute name error"<<endl;
						flag=1;
						break;
					}
					limit_attribute[limit_number]=s[sss];
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						flag=1;
						break;
					}
					if(s[sss]=="="||s[sss]=="<>"||s[sss]=="<"||s[sss]==">"||s[sss]=="<="||s[sss]==">=")
					{
						limit_op[limit_number]=s[sss];
					}
					else 
					{
						cout<<"Opcode error"<<endl;
						flag=1;
						break;
					}
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						flag=1;
						break;
					}
					limit_value[limit_number]=s[sss];
					sss++;
					if(sss>ss) break;
					if(s[sss]!="and")
					{
						cout<<"Synax error"<<endl;
						flag=1;
						break;
					}
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						flag=1;
						break;
					}
					limit_number++;
				}
				if(flag==1) continue;
			}
			select(table_name,limit_number,limit_attribute,limit_op,limit_value);
		}
		else if(s[sss]=="insert")
		{
			sss++;
			if(sss>ss) 
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			if(s[sss]!="into")
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			sss++;
			if(sss>ss) 
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			string table_name;
			if(pan_name(s[sss])==0)
			{
				cout<<"Table name error"<<endl;
				continue;
			}
			table_name=s[sss];
			sss++;
			if(sss>ss) 
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			if(s[sss]!="values")
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			int values_number=0;
			string values[32];
			sss++;
			if(sss>ss) 
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			int flag=0;
			while(1)
			{
				values[values_number]=s[sss];
				sss++;
				if(sss>ss) break;
				if(s[sss]!=",")
				{
					cout<<"Synax error"<<endl;
					flag=1;
					break;
				}
				sss++;
				if(sss>ss)
				{
					cout<<"Synax error"<<endl;
					flag=1;
					break;
				}
				values_number++;
			}
			if(flag==1) continue;
			insert(table_name,values_number,values);
		}
		else if(s[sss]=="delete")
		{
			sss++;
			if(sss>ss) 
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			if(s[sss]!="from")
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			sss++;
			if(sss>ss) 
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			string table_name;
			if(pan_name(s[sss])==0)
			{
				cout<<"Table name error"<<endl;
				continue;
			}
			table_name=s[sss];
			sss++;
			int limit_number=0;
			string limit_attribute[32];
			string limit_op[32];
			string limit_value[32];
			if(sss>ss) 
			{
				deletion(table_name,-1,limit_attribute,limit_op,limit_value);
				continue;
			}
			if(s[sss]!="where")
			{
				cout<<"Synax error"<<endl;
				continue;
			}
			sss++;
			if(sss<=ss) 
			{
				int flag=0;
				while(1)
				{
					if(pan_name(s[sss])==0)
					{
						cout<<"Attribute name error"<<endl;
						flag=1;
						break;
					}
					limit_attribute[limit_number]=s[sss];
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						flag=1;
						break;
					}
					if(s[sss]=="="||s[sss]=="<>"||s[sss]=="<"||s[sss]==">"||s[sss]=="<="||s[sss]==">=")
					{
						limit_op[limit_number]=s[sss];
					}
					else 
					{
						cout<<"Opcode error"<<endl;
						flag=1;
						break;
					}
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						flag=1;
						break;
					}
					limit_value[limit_number]=s[sss];
					sss++;
					if(sss>ss) break;
					if(s[sss]!="and")
					{
						cout<<"Synax error"<<endl;
						flag=1;
						break;
					}
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						flag=1;
						break;
					}
					limit_number++;
				}
				if(flag==1) continue;
			}
			deletion(table_name,limit_number,limit_attribute,limit_op,limit_value);
		}
		else if(s[sss]=="exefile")
		{
			sss++;
			ifstream file(s[sss].c_str());
			if (!file.is_open()) 
			{
				cout<<"file("<<s[sss]<<") can not be open"<<endl;
			}
			while(1)
			{
				int ff=0;
				Error=0;
				cc=0;
				while(1)
				{
					c[cc]=file.get();
					if(c[cc]==EOF) 
					{
						ff=2;
						break;
					}
					if(c[cc]=='\n') 
					{
						c[cc]=' ';
					}
					cc++;
					if(c[cc-1]==';') break;
				}
				if(ff==2) break;
				cc--;
				while(1)//分号后边仍然有东西 
				{
					d=file.get();
					if(d=='\n') break;
					if(d==EOF) 
					{
						ff=1;
						break;
					}
					Error=1;
				}
				if(Error==1) 
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				for(int i=0;i<=cc;++i)
				{
					if(c[i]==';'||c[i]=='('||c[i]==')'||c[i]=='\'') c[i]=' ';
				}
				ss=0;
				for(int i=0;i<=999;++i)
					s[i]="";
				for(int i=0;i<=cc;++i)
				{
					if(c[i]==' ')
					{
						if(s[ss]!="") ss++;
					}
					else
					{
						if(c[i]==',')
						{
							if(s[ss]!="") ss++;
							s[ss]+=c[i];
							ss++;
						}
						else s[ss]+=c[i];
					}
				}
				ss--;
				if(ss<0) 
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				int sss=0;
				if(s[sss]=="create")
				{
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					if(s[sss]=="table")
					{
						sss++;
						if(sss>ss)
						{
							cout<<"Synax error"<<endl;
							continue;
						}
						string table_name;
						if(pan_name(s[sss])==0)
						{
							cout<<"Table name error"<<endl;
							continue;
						}
						table_name=s[sss];
						sss++;
						if(sss>ss)
						{
							cout<<"Synax error"<<endl;
							continue;
						}
						int attribute_number=0;
						string attribute_name[32];
						int attribute_type[32];
						int attribute_limit[32];
						int flag=0;
						while(1)//开始读入属性列表 
						{
							if(s[sss]=="primary")
							{
								sss++;
								if(sss>ss)
								{
									cout<<"Synax error"<<endl;
									flag=1;
									break;
								}
								if(s[sss]!="key")
								{
									cout<<"Synax error"<<endl;
									flag=1;
									break;
								}
								sss++;
								if(sss>ss)
								{
									cout<<"Synax error"<<endl;
									flag=1;
									break;
								}
								if(pan_name(s[sss])==0)
								{
									cout<<"Attribute name error"<<endl;
									flag=1;
									break;
								}
								int flag1=0;
								for(int i=0;i<=attribute_number-1;++i)
								{
									if(attribute_name[i]==s[sss])
									{
										flag1=1;
										attribute_limit[i]=2;//主键设定为1 
										break;
									}
								}
								if(flag1==0)
								{
									cout<<"Primary key is not existed"<<endl;
									flag=1;
									break;
								}
								sss++;
								if(sss>ss) break;
								else
								{
									cout<<"Synax error"<<endl;
									flag=1;
									break;
								} 
							}
							else
							{
								if(pan_name(s[sss])==0)
								{
									cout<<"Attribute name error"<<endl;
									flag=1;
									break;
								}
								attribute_name[attribute_number]=s[sss];
								sss++;
								if(sss>ss)
								{
									cout<<"Synax error"<<endl;
									flag=1;
									break;
								}
								if(s[sss]=="int") attribute_type[attribute_number]=0;//用于判断数据类型并记录，int记0，float记-1，char记有几位 
								else if(s[sss]=="float") attribute_type[attribute_number]=-1;
								else if(s[sss]=="char")
								{
									sss++;
									if(sss>ss)
									{
										cout<<"Synax error"<<endl;
										flag=1;
										break;
									}
									attribute_type[attribute_number]=zhuan_int(s[sss]);
								}
								else
								{
									cout<<"Type error"<<endl;
									flag=1;
									break;
								}
								sss++;
								if(sss>ss) 
								{
									cout<<"No primary key"<<endl;
									flag=1;
									break;
								}
								if(s[sss]==",") 
								{	
									attribute_limit[attribute_number]=0;//没有限制设定为0 
								}
								else if(s[sss]=="unique") 
								{
									attribute_limit[attribute_number]=1;//unique设定为1
									sss++;
									if(sss>ss) 
									{
										cout<<"No primary key"<<endl;
										flag=1;
										break;//属性登记结束
									}
									if(s[sss]!=",")
									{
										cout<<"synax error"<<endl;
										flag=1;
										break;
									} 
								}
								else
								{
									cout<<"Limit error"<<endl;
									flag=1;
									break;
								} 
								sss++;
								if(sss>ss) 
								{
									cout<<"synax error"<<endl;
									flag=1;
									break;
								}
								attribute_number++;
							}
						}
						attribute_number--;
						if(flag==1) continue;
						create_table(table_name,attribute_number,attribute_name,attribute_type,attribute_limit);
					}
					else if(s[sss]=="index")
					{
						sss++;
						if(sss>ss)
						{
							cout<<"Synax error"<<endl;
							continue;
						}
						string index_name;
						if(pan_name(s[sss])==0)
						{
							cout<<"Index name error"<<endl;
							continue;
						}
						index_name=s[sss];
						sss++;
						if(sss>ss)
						{
							cout<<"Synax error"<<endl;
							continue;
						}
						if(s[sss]!="on")
						{
							cout<<"Synax error"<<endl;
							continue;
						}
						sss++;
						if(sss>ss)
						{
							cout<<"Synax error"<<endl;
							continue;
						}
						string table_name;
						if(pan_name(s[sss])==0)
						{
							cout<<"Table name error"<<endl;
							continue;
						}
						table_name=s[sss];
						sss++;
						if(sss>ss)
						{
							cout<<"Synax error"<<endl;
							continue;
						}
						string attribute_name;
						if(pan_name(s[sss])==0)
						{
							cout<<"Attribute name error"<<endl;
							continue;
						}
						attribute_name=s[sss];
						sss++;
						if(sss<=ss) 
						{
							cout<<"Synax error"<<endl;
							continue;
						}
						create_index(index_name,table_name,attribute_name);
					}
					else
					{
						cout<<"Synax error"<<endl;
						continue;
					}
				}
				else if(s[sss]=="drop")
				{
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					if(s[sss]=="table")
					{
						sss++;
						if(sss>ss) 
						{
							cout<<"Synax error"<<endl;
							continue;
						}
						string table_name;
						if(pan_name(s[sss])==0)
						{
							cout<<"Table name error"<<endl;
							continue;
						}
						table_name=s[sss];
						sss++;
						if(sss<=ss) 
						{
							cout<<"Synax error"<<endl;
							continue;
						}
						drop_table(table_name);
					}
					else if(s[sss]=="index")
					{
						sss++;
						if(sss>ss) 
						{
							cout<<"Synax error"<<endl;
							continue;
						}
						string index_name;
						if(pan_name(s[sss])==0)
						{
							cout<<"Index name error"<<endl;
							continue;
						}
						index_name=s[sss];
						sss++;
						if(sss<=ss) 
						{
							cout<<"Synax error"<<endl;
							continue;
						}
						drop_index(index_name);
					}
					else
					{
						cout<<"Synax error"<<endl;
						continue;
					}
				}
				else if(s[sss]=="select")
				{
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					if(s[sss]!="*")
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					if(s[sss]!="from")
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					string table_name;
					if(pan_name(s[sss])==0)
					{
						cout<<"Table name error"<<endl;
						continue;
					}
					table_name=s[sss];
					sss++;
					int limit_number=0;
					string limit_attribute[32];
					string limit_op[32];
					string limit_value[32];
					if(sss>ss) 
					{
						select(table_name,-1,limit_attribute,limit_op,limit_value);
						continue;
					}
					if(s[sss]!="where")
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					sss++;
					if(sss<=ss) 
					{
						int flag=0;
						while(1)
						{
							if(pan_name(s[sss])==0)
							{
								cout<<"Attribute name error"<<endl;
								flag=1;
								break;
							}
							limit_attribute[limit_number]=s[sss];
							sss++;
							if(sss>ss) 
							{
								cout<<"Synax error"<<endl;
								flag=1;
								break;
							}
							if(s[sss]=="="||s[sss]=="<>"||s[sss]=="<"||s[sss]==">"||s[sss]=="<="||s[sss]==">=")
							{
								limit_op[limit_number]=s[sss];
							}
							else 
							{
								cout<<"Opcode error"<<endl;
								flag=1;
								break;
							}
							sss++;
							if(sss>ss) 
							{
								cout<<"Synax error"<<endl;
								flag=1;
								break;
							}
							limit_value[limit_number]=s[sss];
							sss++;
							if(sss>ss) break;
							if(s[sss]!="and")
							{
								cout<<"Synax error"<<endl;
								flag=1;
								break;
							}
							sss++;
							if(sss>ss) 
							{
								cout<<"Synax error"<<endl;
								flag=1;
								break;
							}
							limit_number++;
						}
						if(flag==1) continue;
					}
					select(table_name,limit_number,limit_attribute,limit_op,limit_value);
				}
				else if(s[sss]=="insert")
				{
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					if(s[sss]!="into")
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					string table_name;
					if(pan_name(s[sss])==0)
					{
						cout<<"Table name error"<<endl;
						continue;
					}
					table_name=s[sss];
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					if(s[sss]!="values")
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					int values_number=0;
					string values[32];
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					int flag=0;
					while(1)
					{
						values[values_number]=s[sss];
						sss++;
						if(sss>ss) break;
						if(s[sss]!=",")
						{
							cout<<"Synax error"<<endl;
							flag=1;
							break;
						}
						sss++;
						if(sss>ss)
						{
							cout<<"Synax error"<<endl;
							flag=1;
							break;
						}
						values_number++;
					}
					if(flag==1) continue;
					insert(table_name,values_number,values);
				}
				else if(s[sss]=="delete")
				{
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					if(s[sss]!="from")
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					sss++;
					if(sss>ss) 
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					string table_name;
					if(pan_name(s[sss])==0)
					{
						cout<<"Table name error"<<endl;
						continue;
					}
					table_name=s[sss];
					sss++;
					int limit_number=0;
					string limit_attribute[32];
					string limit_op[32];
					string limit_value[32];
					if(sss>ss) 
					{
						deletion(table_name,-1,limit_attribute,limit_op,limit_value);
						continue;
					}
					if(s[sss]!="where")
					{
						cout<<"Synax error"<<endl;
						continue;
					}
					sss++;
					if(sss<=ss) 
					{
						int flag=0;
						while(1)
						{
							if(pan_name(s[sss])==0)
							{
								cout<<"Attribute name error"<<endl;
								flag=1;
								break;
							}
							limit_attribute[limit_number]=s[sss];
							sss++;
							if(sss>ss) 
							{
								cout<<"Synax error"<<endl;
								flag=1;
								break;
							}
							if(s[sss]=="="||s[sss]=="<>"||s[sss]=="<"||s[sss]==">"||s[sss]=="<="||s[sss]==">=")
							{
								limit_op[limit_number]=s[sss];
							}
							else 
							{
								cout<<"Opcode error"<<endl;
								flag=1;
								break;
							}
							sss++;
							if(sss>ss) 
							{
								cout<<"Synax error"<<endl;
								flag=1;
								break;
							}
							limit_value[limit_number]=s[sss];
							sss++;
							if(sss>ss) break;
							if(s[sss]!="and")
							{
								cout<<"Synax error"<<endl;
								flag=1;
								break;
							}
							sss++;
							if(sss>ss) 
							{
								cout<<"Synax error"<<endl;
								flag=1;
								break;
							}
							limit_number++;
						}
						if(flag==1) continue;
					}
					deletion(table_name,limit_number,limit_attribute,limit_op,limit_value);
				}
				else if(s[sss]=="quit")
				{
					quit();
					break;
				}
				else
				{
					cout<<"Synax error"<<endl;
					continue;
				}
				if(ff==1) break;
			} 
			file.close();
		}
		else if(s[sss]=="quit")
		{
			quit();
			break;
		}
		else
		{
			cout<<"Synax error"<<endl;
			continue;
		}
	} 
}

int pan_name(string x)
{
	if(x[0]>='a'&&x[0]<='z'||x[0]>='A'&&x[0]<='Z'||x[0]=='_') ;
	else return 0;
	for(int i=1;i<=x.length()-1;++i)
	{
		if(x[0]>='a'&&x[0]<='z'||x[0]>='A'&&x[0]<='Z'||x[0]=='_'||x[0]>='0'&&x[0]<='9') ;
		else return 0;
	}
	return 1;
}

int zhuan_int(string x)
{
	int y=0;
	for(int i=0;i<=x.length()-1;++i)
	{
		y=y*10+x[i]-'0';
	}
	return y;
}
