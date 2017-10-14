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
	double x,y,v,p;               //p=1为馅饼，p=2为炸弹
	struct ball *next;
}BALL;

struct HAM
{
	double	x;
	int		y;
	double	v;
};

//初始化开屏小球
void Initham(int i,HAM ham[]);

//移动开屏小球
void Moveham(int i,HAM ham[]);

//开场动画
void startshow(HAM ham[]);

//选择难度
void setlevel(int *vol, double *k);

//开始游戏
void game(int vol,double k,char username[],FILE *fp1,FILE *fp2);

//爆炸效果
void explode(double dx);

//存档成功效果
void done();

//显示汉堡
void putham(int x,int y);

//显示炸弹
void putbump(int x,int y);

//显示碗
void putbowl(int x,int y);

//输出余时、生命值
int time(int begint,int life);

//小球显露间隔控制
int gaptime(int begint,int gap);

//小球掉落间隔控制
int gaptime2(int vol,int gapt);

//保存成绩
void savescore(FILE *fp1,FILE *fp2,char username[],int score);

//计算最高分
int calmax();

//输出最高分
void outmax(int *high);

//键盘反馈
int kbcommand();

//输出菜单界面
void menu(char username[],int high);

//输出分数
void outscore(int score);

//初始化小球
void Initball(BALL *xin,double k);

//结束按钮
void endgame();

//暂停按钮
int pause();

//停止按钮
void stop();

int main()
{
	mciSendString("open source\\play.mp3 Alias bk", NULL, 0, NULL);	// 打开音乐
	MessageBox(NULL,"        游戏指南：\n欢迎进入大富翁的游戏世界！\n通过左右键可控制宝盆的移动。\n接到汉堡可加10分，\n接到炸弹扣100分同时生命值-1，\n全部3个宝盆都被炸毁后游戏结束。\n祝游戏愉快！","大富翁",MB_OK);
	srand((unsigned)time(NULL));
	int vol=0;       //小球掉落间隔时
	double k=0;       //难度系数
	char username[50]="";
	FILE *fp1=NULL,*fp2=NULL;  //玩家信息文件和单独的分数存储文件
	HAM ham[MAXHAM];
	IMAGE img;

	if((fp1=fopen("info.txt","a"))==NULL)
		MessageBox(NULL,"      读取信息文件失败","大富翁",MB_OK);
	if((fp2=fopen("score.txt","a"))==NULL)
		MessageBox(NULL,"      读取分数失败","大富翁",MB_OK);

	initgraph(640,480);
	startshow(ham);      //开头动画
	setlevel(&vol,&k);   //设定难度
	
	loadimage(&img, "source\\title.jpg");
	putimage(0,0,&img);
	InputBox(username,20,"请输入用户名：","大富翁","PLAYER");
	mciSendString("play bk", NULL, 0, NULL);								// 播放音乐
	game(vol,k,username,fp1,fp2);    //开始游戏

	fclose(fp1);
	fclose(fp2);
	return 0;
}

//初始化开屏小球
void Initham(int i,HAM ham[])
{
	ham[i].x = 0;
	ham[i].y = rand() % 480;
	ham[i].v = (rand() % 9000) / 1000.0 + 5;
}

//移动开屏小球
void Moveham(int i,HAM ham[])
{
	IMAGE imga,imgb;

	// 计算新位置
	ham[i].x += ham[i].v;
	if (ham[i].x > 640)	Initham(i,ham);

	// 画新开屏小球
	loadimage(&imga, "source\\ham1.jpg");
	loadimage(&imgb, "source\\ham2.jpg");
	putimage(int(ham[i].x),ham[i].y,&imgb,SRCAND);
	putimage(int(ham[i].x),ham[i].y,&imga,SRCINVERT);
}

//开场动画
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

//开场动画
void startshow(HAM ham[])
{
	float H = 60;		// 色相
	float S = 1;		// 饱和度
	float L = 0.9f;		// 亮度
	for(int i = 0; i < MAXHAM; i++)       //初始化第一波动画
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

//爆炸效果
void explode(double dx)
{
	IMAGE imga,imgb;
	loadimage(&imga, "source\\exp1.jpg");
	loadimage(&imgb, "source\\exp2.jpg");
	putimage(int(dx-70),180,&imgb,SRCAND);
	putimage(int(dx-70),180,&imga,SRCINVERT);
	Sleep(20);
}

//存档成功效果
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

//小球掉落间隔控制
int gaptime(int begint,int gap)
{
	int tnow = gap - (GetTickCount()-begint)/100;
	return tnow;
}

//输出得分
void outscore(int score)
{
    char iscore[10];
	itoa(score,iscore,10);
	setbkmode(TRANSPARENT);
	outtextxy(315,404,iscore);
	setbkmode(OPAQUE);
}

//键盘控制
int kbcommand()
{
	int c=0;
	if(GetAsyncKeyState(VK_LEFT)&0x8000)       c|=LEFT;                    //按位与运算
	if(GetAsyncKeyState(VK_RIGHT)&0x8000)      c|=RIGHT;
	return c;
}

//输出菜单界面
void menu(char username[],int high)
{
	IMAGE img;
	char str[10]="100";
	itoa(high,str,10);
	setbkmode(TRANSPARENT);
	settextcolor(BLUE);
	setlinecolor(BLACK);
	line(0,384,640,384);
	char s1[20]="剩余时间：";
	outtextxy(500,404,s1);
	char s2[20]="本盘得分：";
	outtextxy(235,404,s2);
	char s3[20]="最高得分：";
	outtextxy(380,404,s3);
	char s4[20]="生命值：";
	outtextxy(155,404,s4);
	char s5[20]="用户名：";
	outtextxy(30,404,s5);
	outtextxy(90,404,username);
	outtextxy(460,404,str);
	setbkmode(OPAQUE);
}

//输出余时、生命值
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

//小球显露间隔控制
int gaptime2(int vol,int gapt)
{
	int tnow = vol - (GetTickCount()-gapt)/100;
	return tnow;
}

//计算最高分
int calmax()
{
	int max=-9999,temp;
	FILE *fp=NULL;
	if((fp=fopen("score.txt","r"))==NULL)
		MessageBox(NULL,"        打开文件失败！","接馅饼",MB_OK);
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

//输出最高分
void outmax(int *high)
{
	setbkmode(TRANSPARENT);
	setfillcolor(RGB(250,197,121));
	settextcolor(RGB(84,16,0));
	fillrectangle(360,433,460,460);
	outtextxy(370,440,"显示最高分");
	FlushBatchDraw();
	Sleep(50);
	*high=calmax();
}

//显示汉堡
void putham(int x,int y)
{
	IMAGE imga,imgb;
	loadimage(&imga, "source\\ham1.jpg");
	loadimage(&imgb, "source\\ham2.jpg");
	putimage(x,y,&imgb,SRCAND);
	putimage(x,y,&imga,SRCINVERT);
}

//显示炸弹
void putbump(int x,int y)
{
	IMAGE imga,imgb;
	loadimage(&imga, "source\\bump1.jpg");
	loadimage(&imgb, "source\\bump2.jpg");
	putimage(x,y,&imgb,SRCAND);
	putimage(x,y,&imga,SRCINVERT);
}

//显示碗
void putbowl(int x,int y)
{
	IMAGE imga,imgb;
	loadimage(&imga, "source\\bowl1.jpg");
	loadimage(&imgb, "source\\bowl2.jpg");
	putimage(x,y,&imgb,SRCAND);
	putimage(x,y,&imga,SRCINVERT);
}

//难度设定
void setlevel(int *vol, double *k)
{
	int con=0;     //循环控制变量
	int bannerx=0,bannery=0;      //banner小球坐标
	int xv=rand()%4+1,yv=rand()%4+1;    //banner小球速度
    IMAGE l,l1,l2,l3,b1,b2;
	MOUSEMSG m;
	loadimage(&l, "source\\level.jpg");
	loadimage(&b1, "source\\banner1.jpg");
	loadimage(&b2, "source\\banner2.jpg");
	while(1)          //难度设定部分
	{
	    putimage(0,0,&l);
		while(MouseHit())
		{
			m=GetMouseMsg();
			if(m.uMsg == WM_LBUTTONDOWN && m.x<384 && m.x>244 && m.y<352 && m.y>314)   //简单
			{
				*vol=35;
				*k=3.0;
				loadimage(&l1, "source\\1.jpg");
	            putimage(249,314,&l1);
				Sleep(80);
				putimage(0,0,&l);
				con=1;
			}
			else if(m.uMsg == WM_LBUTTONDOWN && m.x<384 && m.x>244 && m.y<393 && m.y>358)   //中等
			{
				*vol=20;
				*k=2.0;
				loadimage(&l2, "source\\2.jpg");
	            putimage(249,358,&l2);
				Sleep(80);
				putimage(0,0,&l);
				con=1;
			}
			else if(m.uMsg == WM_LBUTTONDOWN && m.x<384 && m.x>244 && m.y<435 && m.y>400)   //困难
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

//保存成绩
void savescore(FILE *fp1,FILE *fp2,char username[],int score)
{
	setbkmode(TRANSPARENT);
	setfillcolor(RGB(250,197,121));
	settextcolor(RGB(84,16,0));
	fillrectangle(235,433,320,460);
	outtextxy(245,440,"保存游戏");
	FlushBatchDraw();
	Sleep(20);
	fprintf(fp1,"%s",username);
	fprintf(fp1,"%d\n",score);
	fprintf(fp2,"%d\n",score);
	done();
}

//结束按钮
void endgame()
{
	setbkmode(TRANSPARENT);
	setfillcolor(RGB(250,197,121));
	settextcolor(RGB(84,16,0));
	fillrectangle(510,433,557,460);
	outtextxy(518,440,"退出");
	FlushBatchDraw();
	Sleep(50);
	MessageBox(NULL,"        游戏结束！","大富翁",MB_OK);
}

//暂停按钮
int pause()
{
	int addtime=GetTickCount(),con=0;
	MOUSEMSG m;
	setbkmode(TRANSPARENT);
	setfillcolor(RGB(250,197,121));
	settextcolor(RGB(84,16,0));
	fillrectangle(30,433,80,460);
	outtextxy(40,440,"继续");
	FlushBatchDraw();
	while(1){
		while(MouseHit())
		{
		    m=GetMouseMsg();
			if(m.uMsg == WM_LBUTTONDOWN && m.x<80 && m.x>30 && m.y<460 && m.y>430)   //继续
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

//停止按钮
void stop()
{
	setbkmode(TRANSPARENT);
	setfillcolor(RGB(250,197,121));
	settextcolor(RGB(84,16,0));
	fillrectangle(110,433,162,460);
	outtextxy(120,440,"停止");
	FlushBatchDraw();
}

//开始游戏
void game(int vol,double k,char username[],FILE *fp1,FILE *fp2)
{
	double bx=280,by=330,xchange=0;      //小球x坐标,y坐标,x改变量
	int c=0,score=0,life=3,onoff=1;     //c:接受键盘输入;onoff 默认计时中
	int high=0;    //high:最高分 
	int con=0,con1=0,con2=0;  //循环控制变量
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
			if(m.uMsg == WM_LBUTTONDOWN && m.x<80 && m.x>30 && m.y<460 && m.y>430)   //开始游戏
			{
	            //初始化
            	head=(BALL*)malloc(sizeof(BALL));
            	Initball(head,k);
				bx=280;     
				c=0;         
				score=0;
				life=3;
				onoff=1;

		        int begint=GetTickCount();          //time 总倒计时系统初始化    
				int begingt=GetTickCount();         //gaptime 交替变换计时系统初始化 
				int gapt=GetTickCount();            //gaptime2 小球掉落间隔初始化

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
                        //onoff=1,4为计时中，0长时间段（gap）计时结束，3为闪烁（0.5s）计时结束
		    			if(onoff<2)   //长时间段 1s
		    			    if(gaptime(begingt,10)<=0)
		    			    {
		    				    onoff=0;
			    				if(point->p==1)
		                            putham(int(point->x-15),int(point->y-15));
		                        else
				    			    putbump(int(point->x-15),int(point->y-15));
	    						onoff=4;       //开始闪烁
		    			    	begingt=GetTickCount();
			    		    }
				    	    else
					        {
						    	setfillcolor(RGB(36,140,21));
							    fillcircle(int(point->x),int(point->y),15);
							}
    					else    //闪烁  0.1s
	    				{
		    				if(gaptime(begingt,2)<=0)
			    		    {
				    		    onoff=3;
					    		if(point->p==1)
						    	    putham(int(point->x-15),int(point->y-15));
		                        else
				    			    putbump(int(point->x-15),int(point->y-15));
	    						onoff=1;       //开始长时间段
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
					    if(m.uMsg == WM_LBUTTONDOWN && m.x<160 && m.x>110 && m.y<460 && m.y>430)       //停止
						{
					        stop();
					        Sleep(50);
					        con=1;
						}
					    else if(m.uMsg == WM_LBUTTONDOWN && m.x<80 && m.x>30 && m.y<460 && m.y>430)    //暂停
					        begint-=pause();
					    else if(m.uMsg == WM_LBUTTONDOWN && m.x<315 && m.x>235 && m.y<460 && m.y>430)   //保存成绩
					        savescore(fp1,fp2,username,score);
					    else if(m.uMsg == WM_LBUTTONDOWN && m.x<460 && m.x>360 && m.y<460 && m.y>430)   //显示最高分
					        outmax(&high);
					    else if(m.uMsg == WM_LBUTTONDOWN && m.x<560 && m.x>510 && m.y<460 && m.y>430)   //退出
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
					FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));              //清空键盘缓存
	            }
				EndBatchDraw();
				clearrectangle(0,0,640,383);
				clearrectangle(0,385,640,403);
				clearrectangle(585,404,630,418);
			}
			else if(m.uMsg == WM_LBUTTONDOWN && m.x<560 && m.x>510 && m.y<460 && m.y>430)   //结束
			{
				endgame();
				exit(0);
			}
			else if(m.uMsg == WM_LBUTTONDOWN && m.x<460 && m.x>360 && m.y<460 && m.y>430)   //显示最高分
			    outmax(&high);
			else if(m.uMsg == WM_LBUTTONDOWN && m.x<315 && m.x>235 && m.y<460 && m.y>430)   //保存成绩
			{
				savescore(fp1,fp2,username,score);
				clearrectangle(0,0,640,383);
			}
		}
		Sleep(20);
	}
}