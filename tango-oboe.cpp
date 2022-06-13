#include <iostream>
#include <fstream>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <conio.h>
using namespace std;
#define TANGO struct word

class tango{
	public:
		char hyoki[30];
		char kana[30];
		char chugokugo[30];
		void Set(char* h,char* k,char* c){
			strcpy(hyoki,h);
			strcpy(kana,k);
			strcpy(chugokugo,c);
		}
};
/* TODO 
TANGO{
	tango data;
	TANGO *next;
};*/

//��������� 
int RandomId(int max){
	srand((unsigned)time(NULL));
	int r=rand()%max;
	return r;
}

//�ƶ����
void Cursor(int x, int y){
        COORD pos;
        HANDLE hOutput;
        pos.X = x;
        pos.Y = y;
        hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleCursorPosition(hOutput, pos);
}

//��ʾ�����ع��
void CursorVisible(int i){
        CONSOLE_CURSOR_INFO cursor_info = {1,i};
        SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}

//����
void ClearLine(int y){
	Cursor(0,y);
	cout<<"\33[2K\r";
} 

//���Ʋ˵� 
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
			case 's': case 'S': case 80:
				Cursor(30+Sub,10+flag);
				cout<<" ";
				if(flag==n-1) flag=0;
				else flag++;
				Cursor(30+Sub,10+flag);
				cout<<">";
				break;
			case 'w': case 'W': case 72:
				Cursor(30+Sub,10+flag);
				cout<<" ";
				if(flag==0) flag=n-1;
				else flag--;
				Cursor(30+Sub,10+flag);
				cout<<">";
				break;
			case ' ': case 13:
				for(j=0;j<n;j++) ClearLine(10+j);
				return flag;
		}
	}
} 

//�ȴ�ȷ�� 
void RequireOK(){
	int t=0;
	while(1){
		CursorVisible(1);
		int i=((int)getch());
		if(i==' '||i==13){
			CursorVisible(0);
			break; 
		}
	}
}

//��ղ���λ 
void Text(){
	ClearLine(8);
	ClearLine(12);
	ClearLine(15);
	Cursor(30,0);
} 

//��ʾ��ϰ��������
void DrawHatsuon(tango** risuto,int num){
	//int i=0,j,benkyoshita[999];
	while(1){
		Text();
		//while(n<=num){
			int n=RandomId(num);
			/*for(j=0;j<i;j++){
				if(n==benkyoshita[j]) n++;
			}
			if(strcmp(risuto[n]->hyoki,risuto[n]->kana)==0){
				benkyoshita[i++]=n++;
			}
			 
		}*/
		char answer[30];
		cout<<"���롰0���������˵���";
		Cursor(30,8);
		cout<<"��ǰ���ʣ�"<<risuto[n]->hyoki;
		Cursor(30,12);
		cout<<"�����������";
		cin>>answer;
		Cursor(30,15);
		if(strcmp(answer,"0")==0){
			break;
		} 
		else if(strcmp(answer,risuto[n]->kana)==0){
			cout<<"��ȷ��";
			RequireOK();
		}
		else{
			cout<<"������ȷ����Ϊ"<<risuto[n]->kana;
			RequireOK();
		}
	} 
} 

//��ʾ��ϰ�������
void DrawImi(tango** risuto,int num){
	while(1){
		Text();
		int n=RandomId(num);
		char answer[30];
		cout<<"���롰0���������˵���";
		Cursor(30,8);
		cout<<"��ǰ���ʣ�"<<risuto[n]->hyoki;
		Cursor(30,12);
		cout<<"�����뺬�壺";
		cin>>answer;
		Cursor(30,15);
		if(strcmp(answer,"0")==0){
			break;
		} 
		else if(strcmp(answer,risuto[n]->chugokugo)==0){
			cout<<"��ȫ��ȷ��";
			RequireOK();
		}
		else{
			cout<<"��������Ϊ��"<<risuto[n]->chugokugo<<"�������м���Ƿ���ȷ��";
			RequireOK();
		}
	}
} 

//��ʾ���˵�
int DrawMainMenu(tango** risuto,int num){
	while(1){ 
		char *a[3]={"��ϰ����","��ϰ����","�˳���ϰ"};
		system("cls");
		int c=DrawMenu(a,3),i,t1,t2,t[3],d;
		fstream f;
		char nm[18];
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

int main(){
	tango* risuto[999];
	int i;
	char hyoki[30],kana[30],chugokugo[30];
	fstream f;
	f.open("tango-risuto.txt",ios::in);
	if(f.fail()){
		Cursor(30,11);
		cout<<"�����б����ʧ�ܣ�";
		RequireOK();
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
