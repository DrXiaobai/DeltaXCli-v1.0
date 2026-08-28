#define _USE_MATH_DEFINES
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <cstring>
#include <unistd.h>

#define VERSION "DXCv1.0 -alpha-1.1\n"
#define GRAY_TEXT "\033[90m"
#define RED_LIGHT "\033[31m"
#define RESET_COLOR "\033[0m"
#define YELLOW_COLOR_C "\033[1;33m"
#define YELLOW_COLOR_B "\033[33m"

using namespace std;

int fontSet=1;

struct VirtualFolder;

struct VirtualFile
{
	std::string name;
	std::string content;
	VirtualFile(std::string n) : name(n) {}
};

struct VirtualFolder
{
	std::string name;
	std::vector<VirtualFile> files;
	std::vector<VirtualFolder> subFolders;
	VirtualFolder* parent = nullptr;
	
	VirtualFolder(std::string n, VirtualFolder* p = nullptr)
	: name(n), parent(p) {}
};

class VirtualFileSystem
{
private:
	VirtualFolder root;
	VirtualFolder* currentDir; 
	
public:
	VirtualFileSystem() : root("/"), currentDir(&root) {}
	bool mkdir(const std::string& dirName)
	{
		for (auto& f : currentDir->subFolders)
		{
			if (f.name == dirName){
				cout<<"Cannot make dir\n";
				return false;
			}
		}
		currentDir->subFolders.emplace_back(dirName, currentDir);
		return true;
	}
	bool touch(const std::string& fileName)
	{
		for (auto& f : currentDir->files)
		{
			if (f.name == fileName)
				return false;
		}
		currentDir->files.emplace_back(fileName);
		return true;
	}
	void ls() const
	{
		std::cout << "===== " << currentDir->name << " =====" << std::endl;
		for (auto& folder : currentDir->subFolders)
		{
			std::cout << "[DIR]  " << folder.name << std::endl;
		}
		for (auto& file : currentDir->files)
		{
			std::cout << "[FILE] " << file.name << std::endl;
		}
	}
	
	bool cd(const std::string& target)
	{
		if (target == "..")
		{
			if (currentDir->parent == nullptr)
				return false;
			currentDir = currentDir->parent;
			return true;
		}
		
		for (auto& folder : currentDir->subFolders)
		{
			if (folder.name == target)
			{
				currentDir = &folder;
				return true;
			}
		}
		return false;
	}
	
	void pwd(bool endls=0) const
	{
		std::vector<std::string> pathParts;
		VirtualFolder* temp = currentDir;
		while (temp != nullptr)
		{
			pathParts.push_back(temp->name);
			temp = temp->parent;
		}
		std::reverse(pathParts.begin(), pathParts.end());
		
		for (size_t i = 0; i < pathParts.size(); ++i)
		{
			std::cout << pathParts[i];
			if (i != pathParts.size() - 1)
				std::cout << "|";
		}
		if(endls) cout<<endl;
	}
};

VirtualFileSystem vfs;

void copyrightNotice()
{
	cout<<"====================================================================================\n";
	cout<<"版权声明\n此程序最终解释权归属于Δx工作室，此程序版权属于Δx工作室，遵循CC-BY-SA协议\n";
	cout<<"====================================================================================\n";
}

void personsList()
{
	cout<<"制作组名单（不分前后）\n@H2o - 轨道计算器\n@xiaobai - 主要架构和命令函数\n";
}

void commandHelp()
{
	cout<<"dockingRailCalculator / drc -> 对接轨道计算器\n";
	cout<<"cmmdHelp / chelp -> 命令帮助\n";
	cout<<"personsList -> 制作人员名单\n";
	cout<<"copyrightNotice / crn -> 版权声明\n";
	cout<<"version / vs -> 版本信息\n";
	cout<<"pwd -> 输出当前目录\n";
	cout<<"mkdir\"文件夹名\"-> 创建文件夹\n";
	cout<<"cd \"文件夹名\" / .. -> 进入文件夹/退出到上级目录\n";
	cout<<"touch \"文件名\" -> 创建文件\n";
	cout<<"ls -> 输出当前目录下所有目录及文件\n";
	cout<<"exit / quit -> 退出\n";
	cout<<"clear / cls -> 清屏\n";
}

std::vector<std::string> split(const std::string& str, char delim = ' ')
{
	std::vector<std::string> res;
	std::stringstream ss(str);
	std::string item;
	
	while (std::getline(ss, item, delim))
	{
		if (!item.empty())
		{
			res.push_back(item);
		}
	}
	return res;
}

void version()
{
	cout<<VERSION;
	cout<<"By Δx studio(c)\n";
}

void raodClaude()
{
	bool run=1;
	while(run)
	{
		cout<<"q离开;claude开始计算\n";
		string choose;
		cin>>choose;
		if(choose=="claude"){
			double r1,r2;
			bool s1 = false;
			printf("转移窗口计算程序");
			cout<<endl; 
			cout << GRAY_TEXT;
			printf("声明：该程序仅适用于假设绝对圆轨的情况，真实情况可能会有误差，如需减小误差，请确保轨道尽可能接近正圆");
			cout << RESET_COLOR;
			cout<<endl<<endl;
			
			cout<<"请输入出发轨道半径：";
			cin>>r1;
			cout<<"请输入目标轨道半径：";
			while(s1==false){
				cin>>r2;
				if(r2==0){
					cout << RED_LIGHT << "数据溢出！" << RESET_COLOR << endl << "请重新输入目标轨道半径：";
					s1=false;
				}else{
					s1=true;
				} 
			}
			if(r2<0){
				r2=abs(r2);
			}
			double M_PI_rad = M_PI * (1.0 - pow((r1 + r2) / (2.0 * r2), 1.5));
			double M_PI_deg = M_PI_rad * 180.0 / M_PI;
			
			cout<<endl<<"发射时目标物应领先的角度："<<M_PI_deg<<"°"<<endl<<endl;
			cout << GRAY_TEXT;
			cout<<"输入drc开始下一轮计算..."<<endl;
			cout << RESET_COLOR;
			cin.ignore();
			break;
		}
		else return;
	}
}

void errorByEmpty(string command){cout<<"\x1b[31mEmpty Command:"<<command<<"\x1b[0m\n";}

bool executeCommand(string command ,const vector<std::string>& args)
{
	if (args.empty()) return 1;
	else if(command=="chelp"||command=="cmmdHelp") commandHelp();
	else if(command=="dockingRailCalculator"||command=="drc") raodClaude();
	else if(command=="personsList") personsList();
	else if(command=="copyrightNotice"||command=="crn") copyrightNotice();
	else if(command=="version"||command=="vs") version();
	else if(command=="pwd") vfs.pwd(1);
	else if(command=="ls") vfs.ls();
	else if(command=="exit"||command=="quit") return 0;
	else if(command=="clear"||command=="cls") system("clear");
	else if(args[0]=="mkdir"){if(!(empty(args[1]))) vfs.mkdir(args[1]);else errorByEmpty(command);}
	else if(args[0]=="cd") {if(!(empty(args[1]))) vfs.cd(args[1]);else errorByEmpty(command);}
	else if(args[0]=="touch") {if(!(empty(args[1]))) vfs.touch(args[1]);else errorByEmpty(command);}
	else if(args[0]=="fontset"){if(!(empty(args[1]))) fontSet=stoi(args[1]);else errorByEmpty(command);}
	else if(command!="") cout<<"\x1b[31mUnkown Command:"<<command<<"\x1b[0m\n";
	return 1;
}

int main()
{
	cout<<"输入chelp查看可用命令\n";
	bool exits=1;
	while(exits){
		if(fontSet==1)
			cout<<YELLOW_COLOR_C;
		else if(fontSet==2)
			cout<<YELLOW_COLOR_B;
		else cout<<"无效数字(取值1~2)\n";
		vfs.pwd();
		cout<<">";
		string command;
		getline(cin,command);
		auto arg = split(command);
		exits = executeCommand(command,arg);
	}
}
