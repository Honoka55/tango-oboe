#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
using namespace std;

class tango{
	public:
		char hyoki[100];
		char kana[100];
		char chugokugo[100];
		void Set(char* h,char* k,char* c){
			strcpy(hyoki,h);
			strcpy(kana,k);
			strcpy(chugokugo,c);
		}
};

//生成随机数
int RandomId(int max){
	srand((unsigned)time(NULL));
	int r=rand()%max;
	return r;
}

//移动光标
void Cursor(int x, int y){
        COORD pos;
        HANDLE hOutput;
        pos.X = x;
        pos.Y = y;
        hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOutput, pos);
}

//显示和隐藏光标
void CursorVisible(int i){
        CONSOLE_CURSOR_INFO cursor_info = {1,i};
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

//清行
void ClearLine(int y){
	Cursor(0,y);
	cout<<"\33[2K\r";
}

//绘制菜单
int DrawMenu(char **list,int n,int Sub=0){
	CursorVisible(0);
	int i,j,flag=0;
	Cursor(30+Sub,10);
	cout<<">";
	for(i=0;i<n;i++){
		Cursor(32+Sub,10+i);
		cout<<list[i]<<endl;
	}
	while(1){
		i=((int)getch());
		switch(i){
			case 's': case 'S': case 80: case '2':
				Cursor(30+Sub,10+flag);
				cout<<" ";
				if(flag==n-1) flag=0;
				else flag++;
				Cursor(30+Sub,10+flag);
				cout<<">";
				break;
			case 'w': case 'W': case 72: case '8':
				Cursor(30+Sub,10+flag);
				cout<<" ";
				if(flag==0) flag=n-1;
				else flag--;
				Cursor(30+Sub,10+flag);
				cout<<">";
				break;
			case ' ': case 13: case '0':
				for(j=0;j<n;j++) ClearLine(10+j);
				return flag;
		}
	}
}

//等待确认
void RequireOK(){
	int t=0;
	while(1){
		CursorVisible(1);
		int i=((int)getch());
		if(i==' '||i==13||i=='0'){
			CursorVisible(0);
			break;
		}
	}
}

//清空并归位
void Text(){
	ClearLine(8);
	ClearLine(12);
	ClearLine(15);
	Cursor(30,0);
}

//显示复习读音界面
void DrawHatsuon(tango** risuto,int num){
	while(1){
		Text();
		int n=RandomId(num);
		while((strcmp(risuto[n]->hyoki,risuto[n]->kana)==0)){
			n++;
			if(n==num){
				n=0;
			}
		}
		char answer[100];
		cout<<"输入“0”返回主菜单，输入“1”换一个单词。";
		Cursor(30,8);
		cout<<"当前单词："<<risuto[n]->hyoki;
		Cursor(30,12);
		CursorVisible(1);
		cout<<"请输入读音：";
		cin>>answer;
		Cursor(30,15);
		if(strcmp(answer,"0")==0){
			break;
		}
		else if(strcmp(answer,"1")==0){
			continue;
		}
		else if(strcmp(answer,risuto[n]->kana)==0){
			cout<<"正确！";
		}
		else{
			cout<<"错误！正确发音为"<<risuto[n]->kana<<"。";
		}
		cout<<"该单词含义为："<<risuto[n]->chugokugo<<"。";
		RequireOK();
	}
}

//显示复习含义界面
void DrawImi(tango** risuto,int num){
	while(1){
		Text();
		int n=RandomId(num);
		char answer[100];
		cout<<"输入“0”返回主菜单，输入“1”换一个单词。";
		Cursor(30,8);
		cout<<"当前单词："<<risuto[n]->hyoki;
		if(strcmp(risuto[n]->hyoki,risuto[n]->kana)!=0) cout<<"（"<<risuto[n]->kana<<"）";
		Cursor(30,12);
		CursorVisible(1);
		cout<<"请输入含义：";
		cin>>answer;
		Cursor(30,15);
		if(strcmp(answer,"0")==0){
			break;
		}
		else if(strcmp(answer,"1")==0){
			continue;
		}
		else if(strcmp(answer,risuto[n]->chugokugo)==0){
			cout<<"完全正确！";
			RequireOK();
		}
		else{
			cout<<"完整释义为："<<risuto[n]->chugokugo<<"。请自行检查是否正确。";
			RequireOK();
		}
	}
}

//显示主菜单
int DrawMainMenu(tango** risuto,int num){
	while(1){
		char *a[3]={"复习读音","复习含义","退出复习"};
		system("cls");
		int c=DrawMenu(a,3);
		switch(c){
			case 0:
				DrawHatsuon(risuto,num);
				break;
			case 1:
				DrawImi(risuto,num);
				break;
			case 2:
				return 0;
		}
	}	
}

//主函数 
int main(){
	tango* risuto[999];
	int i;
	char hyoki[100],kana[100],chugokugo[100];
	fstream f;
	f.open("tango-risuto.txt",ios::in);
	if(f.fail()){
		Cursor(30,11);
		cout<<"单词列表加载失败！";
		RequireOK();
		ClearLine(11);
		return 0;
	}
	for(i=0;!f.eof();i++){
		risuto[i]=new tango();
		f>>hyoki;
		f>>kana;
		f>>chugokugo;
		risuto[i]->Set(hyoki,kana,chugokugo);
	}
	DrawMainMenu(risuto,i);
	return 0;
}
