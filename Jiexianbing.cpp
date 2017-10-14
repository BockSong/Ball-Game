#include <graphics.h>
#include <stdio.h>
#include <time.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "Winmm.lib")
#define LEFT 1
#define RIGHT 2
#define MAXHAM 12

typedef struct ball{
	double x,y,v,p;               //p=1Ϊ�ڱ���p=2Ϊը��
	struct ball *next;
}BALL;

struct HAM
{
	double	x;
	int		y;
	double	v;
};

//��ʼ������С��
void Initham(int i,HAM ham[]);

//�ƶ�����С��
void Moveham(int i,HAM ham[]);

//��������
void startshow(HAM ham[]);

//ѡ���Ѷ�
void setlevel(int *vol, double *k);

//��ʼ��Ϸ
void game(int vol,double k,char username[],FILE *fp1,FILE *fp2);

//��ըЧ��
void explode(double dx);

//�浵�ɹ�Ч��
void done();

//��ʾ����
void putham(int x,int y);

//��ʾը��
void putbump(int x,int y);

//��ʾ��
void putbowl(int x,int y);

//�����ʱ������ֵ
int time(int begint,int life);

//С����¶�������
int gaptime(int begint,int gap);

//С�����������
int gaptime2(int vol,int gapt);

//����ɼ�
void savescore(FILE *fp1,FILE *fp2,char username[],int score);

//������߷�
int calmax();

//�����߷�
void outmax(int *high);

//���̷���
int kbcommand();

//����˵�����
void menu(char username[],int high);

//�������
void outscore(int score);

//��ʼ��С��
void Initball(BALL *xin,double k);

//������ť
void endgame();

//��ͣ��ť
int pause();

//ֹͣ��ť
void stop();

int main()
{
	mciSendString("open source\\play.mp3 Alias bk", NULL, 0, NULL);	// ������
	MessageBox(NULL,"        ��Ϸָ�ϣ�\n��ӭ������̵���Ϸ���磡\nͨ�����Ҽ��ɿ��Ʊ�����ƶ���\n�ӵ������ɼ�10�֣�\n�ӵ�ը����100��ͬʱ����ֵ-1��\nȫ��3�����趼��ը�ٺ���Ϸ������\nף��Ϸ��죡","����",MB_OK);
	srand((unsigned)time(NULL));
	int vol=0;       //С�������ʱ
	double k=0;       //�Ѷ�ϵ��
	char username[50]="";
	FILE *fp1=NULL,*fp2=NULL;  //�����Ϣ�ļ��͵����ķ����洢�ļ�
	HAM ham[MAXHAM];
	IMAGE img;

	if((fp1=fopen("info.txt","a"))==NULL)
		MessageBox(NULL,"      ��ȡ��Ϣ�ļ�ʧ��","����",MB_OK);
	if((fp2=fopen("score.txt","a"))==NULL)
		MessageBox(NULL,"      ��ȡ����ʧ��","����",MB_OK);

	initgraph(640,480);
	startshow(ham);      //��ͷ����
	setlevel(&vol,&k);   //�趨�Ѷ�
	
	loadimage(&img, "source\\title.jpg");
	putimage(0,0,&img);
	InputBox(username,20,"�������û�����","����","PLAYER");
	mciSendString("play bk", NULL, 0, NULL);								// ��������
	game(vol,k,username,fp1,fp2);    //��ʼ��Ϸ

	fclose(fp1);
	fclose(fp2);
	return 0;
}

//��ʼ������С��
void Initham(int i,HAM ham[])
{
	ham[i].x = 0;
	ham[i].y = rand() % 480;
	ham[i].v = (rand() % 9000) / 1000.0 + 5;
}

//�ƶ�����С��
void Moveham(int i,HAM ham[])
{
	IMAGE imga,imgb;

	// ������λ��
	ham[i].x += ham[i].v;
	if (ham[i].x > 640)	Initham(i,ham);

	// ���¿���С��
	loadimage(&imga, "source\\ham1.jpg");
	loadimage(&imgb, "source\\ham2.jpg");
	putimage(int(ham[i].x),ham[i].y,&imgb,SRCAND);
	putimage(int(ham[i].x),ham[i].y,&imga,SRCINVERT);
}

//��������
void Initball(BALL *xin,double k)
{
	xin->x=30*(rand()%20+1)+15;
	xin->y=15;
	xin->v=(1+rand()%3)/k;
	if(rand()%3==2)
		xin->p=2;
	else
	    xin->p=1;
	xin->next=NULL;
}

//��������
void startshow(HAM ham[])
{
	float H = 60;		// ɫ��
	float S = 1;		// ���Ͷ�
	float L = 0.9f;		// ����
	for(int i = 0; i < MAXHAM; i++)       //��ʼ����һ������
	{
		Initham(i,ham);
		ham[i].x = rand() % 640;
	}
	BeginBatchDraw();
	for(int y = 0; y < 100; y++)
	{
		L -= 0.0085f;
		setfillcolor( HSLtoRGB(H, S, L) );
		fillrectangle(0,0,640,480);
		for(int i = 0; i < MAXHAM; i++)
			Moveham(i,ham);
		FlushBatchDraw();
		Sleep(5);
	}
	EndBatchDraw();
	cleardevice();
	Sleep(1000);
}

//��ըЧ��
void explode(double dx)
{
	IMAGE imga,imgb;
	loadimage(&imga, "source\\exp1.jpg");
	loadimage(&imgb, "source\\exp2.jpg");
	putimage(int(dx-70),180,&imgb,SRCAND);
	putimage(int(dx-70),180,&imga,SRCINVERT);
	Sleep(20);
}

//�浵�ɹ�Ч��
void done()
{
	IMAGE imgc,imgd;
	loadimage(&imgc, "source\\done1.jpg");
	loadimage(&imgd, "source\\done2.jpg");
	putimage(540,0,&imgd,SRCAND);
	putimage(540,0,&imgc,SRCINVERT);
	Sleep(80);
	FlushBatchDraw();
}

//С�����������
int gaptime(int begint,int gap)
{
	int tnow = gap - (GetTickCount()-begint)/100;
	return tnow;
}

//����÷�
void outscore(int score)
{
    char iscore[10];
	itoa(score,iscore,10);
	setbkmode(TRANSPARENT);
	outtextxy(315,404,iscore);
	setbkmode(OPAQUE);
}

//���̿���
int kbcommand()
{
	int c=0;
	if(GetAsyncKeyState(VK_LEFT)&0x8000)       c|=LEFT;                    //��λ������
	if(GetAsyncKeyState(VK_RIGHT)&0x8000)      c|=RIGHT;
	return c;
}

//����˵�����
void menu(char username[],int high)
{
	IMAGE img;
	char str[10]="100";
	itoa(high,str,10);
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	setlinecolor(BLACK);
	line(0,384,640,384);
	char s1[20]="ʣ��ʱ�䣺";
	outtextxy(500,404,s1);
	char s2[20]="���̵÷֣�";
	outtextxy(235,404,s2);
	char s3[20]="��ߵ÷֣�";
	outtextxy(380,404,s3);
	char s4[20]="����ֵ��";
	outtextxy(155,404,s4);
	char s5[20]="�û�����";
	outtextxy(30,404,s5);
	outtextxy(90,404,username);
	outtextxy(460,404,str);
	setbkmode(OPAQUE);
}

//�����ʱ������ֵ
int time(int begint,int life)
{
	char str[10],st[10];
	int tnow = 60 - (GetTickCount()-begint)/1000;
	itoa(tnow,str,10);
	itoa(life,st,10);
	setbkmode(TRANSPARENT);
	outtextxy(585,404,str);
	outtextxy(215,404,st);
	return tnow;
}

//С����¶�������
int gaptime2(int vol,int gapt)
{
	int tnow = vol - (GetTickCount()-gapt)/100;
	return tnow;
}

//������߷�
int calmax()
{
	int max=-9999,temp;
	FILE *fp=NULL;
	if((fp=fopen("score.txt","r"))==NULL)
		MessageBox(NULL,"        ���ļ�ʧ�ܣ�","���ڱ�",MB_OK);
	if(!feof(fp))
	{
	    while(!feof(fp)){
		    fscanf(fp,"%d\n",&temp);
	    	if(temp>max)
	    		max=temp;
    	}
    	fclose(fp);
		return max;
	}
	return 0;
}

//�����߷�
void outmax(int *high)
{
	setbkmode(TRANSPARENT);
	setfillcolor(RGB(250,197,121));
	settextcolor(RGB(84,16,0));
	fillrectangle(360,433,460,460);
	outtextxy(370,440,"��ʾ��߷�");
	FlushBatchDraw();
	Sleep(50);
	*high=calmax();
}

//��ʾ����
void putham(int x,int y)
{
	IMAGE imga,imgb;
	loadimage(&imga, "source\\ham1.jpg");
	loadimage(&imgb, "source\\ham2.jpg");
	putimage(x,y,&imgb,SRCAND);
	putimage(x,y,&imga,SRCINVERT);
}

//��ʾը��
void putbump(int x,int y)
{
	IMAGE imga,imgb;
	loadimage(&imga, "source\\bump1.jpg");
	loadimage(&imgb, "source\\bump2.jpg");
	putimage(x,y,&imgb,SRCAND);
	putimage(x,y,&imga,SRCINVERT);
}

//��ʾ��
void putbowl(int x,int y)
{
	IMAGE imga,imgb;
	loadimage(&imga, "source\\bowl1.jpg");
	loadimage(&imgb, "source\\bowl2.jpg");
	putimage(x,y,&imgb,SRCAND);
	putimage(x,y,&imga,SRCINVERT);
}

//�Ѷ��趨
void setlevel(int *vol, double *k)
{
	int con=0;     //ѭ�����Ʊ���
	int bannerx=0,bannery=0;      //bannerС������
	int xv=rand()%4+1,yv=rand()%4+1;    //bannerС���ٶ�
    IMAGE l,l1,l2,l3,b1,b2;
	MOUSEMSG m;
	loadimage(&l, "source\\level.jpg");
	loadimage(&b1, "source\\banner1.jpg");
	loadimage(&b2, "source\\banner2.jpg");
	while(1)          //�Ѷ��趨����
	{
	    putimage(0,0,&l);
		while(MouseHit())
		{
			m=GetMouseMsg();
			if(m.uMsg == WM_LBUTTONDOWN && m.x<384 && m.x>244 && m.y<352 && m.y>314)   //��
			{
				*vol=35;
				*k=3.0;
				loadimage(&l1, "source\\1.jpg");
	            putimage(249,314,&l1);
				Sleep(80);
				putimage(0,0,&l);
				con=1;
			}
			else if(m.uMsg == WM_LBUTTONDOWN && m.x<384 && m.x>244 && m.y<393 && m.y>358)   //�е�
			{
				*vol=20;
				*k=2.0;
				loadimage(&l2, "source\\2.jpg");
	            putimage(249,358,&l2);
				Sleep(80);
				putimage(0,0,&l);
				con=1;
			}
			else if(m.uMsg == WM_LBUTTONDOWN && m.x<384 && m.x>244 && m.y<435 && m.y>400)   //����
			{
				*vol=10;
				*k=1.0;
				loadimage(&l2, "source\\3.jpg");
	            putimage(249,400,&l2);
				Sleep(80);
				putimage(0,0,&l);
				con=1;
			}
		}
		if(con==1)
			break;
		if(bannerx<=0)
		{
			bannerx=0;
		    xv*=-1;
		}
		else if(bannerx>=560)
		{
			bannerx=560;
		    xv*=-1;
		}
		if(bannery<=0)
		{
			bannery=0;
			yv*=-1;
		}
		else if(bannery>=400)
		{
			bannery=400;
			yv*=-1;
		}
		bannerx+=xv;
		bannery+=yv;
	    putimage(bannerx,bannery,&b2,SRCAND);
	    putimage(bannerx,bannery,&b1,SRCINVERT);
		Sleep(20);
	}
}

//����ɼ�
void savescore(FILE *fp1,FILE *fp2,char username[],int score)
{
	setbkmode(TRANSPARENT);
	setfillcolor(RGB(250,197,121));
	settextcolor(RGB(84,16,0));
	fillrectangle(235,433,320,460);
	outtextxy(245,440,"������Ϸ");
	FlushBatchDraw();
	Sleep(20);
	fprintf(fp1,"%s",username);
	fprintf(fp1,"%d\n",score);
	fprintf(fp2,"%d\n",score);
	done();
}

//������ť
void endgame()
{
	setbkmode(TRANSPARENT);
	setfillcolor(RGB(250,197,121));
	settextcolor(RGB(84,16,0));
	fillrectangle(510,433,557,460);
	outtextxy(518,440,"�˳�");
	FlushBatchDraw();
	Sleep(50);
	MessageBox(NULL,"        ��Ϸ������","����",MB_OK);
}

//��ͣ��ť
int pause()
{
	int addtime=GetTickCount(),con=0;
	MOUSEMSG m;
	setbkmode(TRANSPARENT);
	setfillcolor(RGB(250,197,121));
	settextcolor(RGB(84,16,0));
	fillrectangle(30,433,80,460);
	outtextxy(40,440,"����");
	FlushBatchDraw();
	while(1){
		while(MouseHit())
		{
		    m=GetMouseMsg();
			if(m.uMsg == WM_LBUTTONDOWN && m.x<80 && m.x>30 && m.y<460 && m.y>430)   //����
			{
				con=1;
			}
		}
		if(con==1)
		{
			con=0;
			break;
		}
	}
	int addtime2=GetTickCount();
	return (addtime-addtime2);
}

//ֹͣ��ť
void stop()
{
	setbkmode(TRANSPARENT);
	setfillcolor(RGB(250,197,121));
	settextcolor(RGB(84,16,0));
	fillrectangle(110,433,162,460);
	outtextxy(120,440,"ֹͣ");
	FlushBatchDraw();
}

//��ʼ��Ϸ
void game(int vol,double k,char username[],FILE *fp1,FILE *fp2)
{
	double bx=280,by=330,xchange=0;      //С��x����,y����,x�ı���
	int c=0,score=0,life=3,onoff=1;     //c:���ܼ�������;onoff Ĭ�ϼ�ʱ��
	int high=0;    //high:��߷� 
	int con=0,con1=0,con2=0;  //ѭ�����Ʊ���
	bool key=true;
	BALL *point=NULL,*head=NULL,*pre=NULL,*xin=NULL;;
	IMAGE img;
	MOUSEMSG m;
	while(true)
	{
		loadimage(&img, "source\\title.jpg");
	    putimage(0,0,&img);
	    menu(username,high);
		while(MouseHit())
		{
			m=GetMouseMsg();
			if(m.uMsg == WM_LBUTTONDOWN && m.x<80 && m.x>30 && m.y<460 && m.y>430)   //��ʼ��Ϸ
			{
	            //��ʼ��
            	head=(BALL*)malloc(sizeof(BALL));
            	Initball(head,k);
				bx=280;     
				c=0;         
				score=0;
				life=3;
				onoff=1;

		        int begint=GetTickCount();          //time �ܵ���ʱϵͳ��ʼ��    
				int begingt=GetTickCount();         //gaptime ����任��ʱϵͳ��ʼ�� 
				int gapt=GetTickCount();            //gaptime2 С���������ʼ��

				BeginBatchDraw();
	            while(key)
	            {
				    loadimage(&img, "source\\bk.jpg");
	                putimage(0,0,&img);
				    menu(username,high);
					xchange=0;

					if(gaptime2(vol,gapt)<=0)
					{	
	                    for(point=head,pre=point;point!=NULL;pre=point,point=point->next);
	                    xin=(BALL*)malloc(sizeof(BALL));
	                    Initball(xin,k);
	                    pre->next=xin;
						gapt=GetTickCount();
					}
					
					for(point=head,pre=point;point!=NULL;pre=point,point=point->next)
					{
                        //onoff=1,4Ϊ��ʱ�У�0��ʱ��Σ�gap����ʱ������3Ϊ��˸��0.5s����ʱ����
		    			if(onoff<2)   //��ʱ��� 1s
		    			    if(gaptime(begingt,10)<=0)
		    			    {
		    				    onoff=0;
			    				if(point->p==1)
		                            putham(int(point->x-15),int(point->y-15));
		                        else
				    			    putbump(int(point->x-15),int(point->y-15));
	    						onoff=4;       //��ʼ��˸
		    			    	begingt=GetTickCount();
			    		    }
				    	    else
					        {
						    	setfillcolor(RGB(36,140,21));
							    fillcircle(int(point->x),int(point->y),15);
							}
    					else    //��˸  0.1s
	    				{
		    				if(gaptime(begingt,2)<=0)
			    		    {
				    		    onoff=3;
					    		if(point->p==1)
						    	    putham(int(point->x-15),int(point->y-15));
		                        else
				    			    putbump(int(point->x-15),int(point->y-15));
	    						onoff=1;       //��ʼ��ʱ���
		    			    	begingt=GetTickCount();
			    		    }
				    	    else
					        {
						    	if(point->p==1)
							        putham(int(point->x-15),int(point->y-15));
		                        else
						    	    putbump(int(point->x-15),int(point->y-15));
			    		    }
				    	}
    		            if(point->x>=bx&&point->x<=bx+80&&point->y>=315)
	    	            {
		    	            if(point->p==1)
			    			{
			                    score+=10;
						    }
    			            else
	    		            {
		    	                explode(bx);
			    				bx=280;
				    			FlushBatchDraw();
				                score-=100;
				                life--;  
    			            }
	    		            if(head==point)
		    				{
	    	    				head=point->next;
				    			free(point);
					    		point=head;
						    }
						    else
						    {
							    pre->next=point->next;
							    free(point);
							    point=pre;
							}
    		            }
	    	            if(point->y>=369)
		                {
			                if(point->p==1)
				    		{
			                    score-=10;
    						}
	    		            if(head==point)
		    				{
	    	    				head=point->next;
				    			free(point);
					    		point=head;
						    }
    						else
	    					{
		    					pre->next=point->next;
			    				free(point);
				    			point=pre;
					    	}
		                }
					}

					outscore(score);
		            if(time(begint,life)<=0)     break;
					putbowl(int(bx),int(by));
					
					while(MouseHit())
					{ 
					    m=GetMouseMsg();
					    if(m.uMsg == WM_LBUTTONDOWN && m.x<160 && m.x>110 && m.y<460 && m.y>430)       //ֹͣ
						{
					        stop();
					        Sleep(50);
					        con=1;
						}
					    else if(m.uMsg == WM_LBUTTONDOWN && m.x<80 && m.x>30 && m.y<460 && m.y>430)    //��ͣ
					        begint-=pause();
					    else if(m.uMsg == WM_LBUTTONDOWN && m.x<315 && m.x>235 && m.y<460 && m.y>430)   //����ɼ�
					        savescore(fp1,fp2,username,score);
					    else if(m.uMsg == WM_LBUTTONDOWN && m.x<460 && m.x>360 && m.y<460 && m.y>430)   //��ʾ��߷�
					        outmax(&high);
					    else if(m.uMsg == WM_LBUTTONDOWN && m.x<560 && m.x>510 && m.y<460 && m.y>430)   //�˳�
						{
					        endgame();
					        exit(0);
						}
					}
					if(con==1)
					{
						con=0;
						break;
					}

					FlushBatchDraw();
					for(point=head;point!=NULL;point=point->next)
					{
						point->y+=point->v;
					}
					c=kbcommand();
					if(c&LEFT)      xchange=-4;
					if(c&RIGHT)     xchange=4;
					if(life==0)          
					{
						PlaySound(TEXT("source\\gameover.wav"),0,SND_FILENAME);
						break;
					}
					
					clearrectangle(0,0,640,383);
					clearrectangle(0,385,640,403);
					clearrectangle(315,404,370,418);
	                clearrectangle(460,404,495,419);
					bx+=xchange;
					if(bx<0)         bx=0;
					if(bx>560)       bx=560;
					FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));              //��ռ��̻���
	            }
				EndBatchDraw();
				clearrectangle(0,0,640,383);
				clearrectangle(0,385,640,403);
				clearrectangle(585,404,630,418);
			}
			else if(m.uMsg == WM_LBUTTONDOWN && m.x<560 && m.x>510 && m.y<460 && m.y>430)   //����
			{
				endgame();
				exit(0);
			}
			else if(m.uMsg == WM_LBUTTONDOWN && m.x<460 && m.x>360 && m.y<460 && m.y>430)   //��ʾ��߷�
			    outmax(&high);
			else if(m.uMsg == WM_LBUTTONDOWN && m.x<315 && m.x>235 && m.y<460 && m.y>430)   //����ɼ�
			{
				savescore(fp1,fp2,username,score);
				clearrectangle(0,0,640,383);
			}
		}
		Sleep(20);
	}
}