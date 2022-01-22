#define _CRT_SECURE_NO_WARNINGS true
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "game.h"
//全局变量
ExMessage message; //多个游戏都有使用,用于储存按键ESC的信息，用于退出游戏

/*****************************************************************************
 该区块是为了实现c文件调用cpp函数能够成功而不报错
 *****************************************************************************/
#ifdef __cplusplus
#if __cplusplus
extern "C"{
#endif
#endif
/******************************************************************************/
/*函数功能:游戏平抛小球的本体
  函数返回值:无
  函数入口参数:无*/
	void BallFlatthrow()
	{
		double y, vy, x, vx;  //y是小球y坐标,x是x坐标,vy,vx分别是其对应速度
		double g, ax;  //g模拟重力加速度,a模拟摩擦阻力的作用
		TCHAR String[4], ChangeableVx[4], Restart[4];  //String储存是否退出,ChangeableVx则是改变的速度,Restart储存是否重新开始
		LPTSTR pString = String, pVx = ChangeableVx, pRestart = Restart; //依次指向上面的字符数组的指针
		ExMessage mess; //接受消息的结构体变量
		ExMessage* msg = &mess; //指向mess的指针
		initgraph(600, 600,EW_NOCLOSE);  //初始化游戏窗口画面,宽600，高600,禁用关闭按钮
		do
		{
			ax = -0.015;
			g = 0.5;
			*pRestart = 0; //初始化Restart为0
			y = 20; //初始化y坐标
			vy = 0;  //初始化下落速度为0
			x = 20;  //初始化x坐标
			InputBox(pVx, 4, L"请输入向左抛的速度(最大为999)(单位像素点)", L"模拟小球平抛", L"10", 0, 0, true);  //弹出提示窗口,true表示禁止取消按键
			vx = _wtoi(pVx); //将接受到的字符转化为数字
			while (1)
			{
				x += vx; //利用x方向速度更新x坐标
				vy += g; //利用加速度g更新vy速度
				y += vy; //利用速度vy更新y坐标
				if (x > 580 || x < 20) //当碰到左右墙壁时
				{
					vx = -0.98 * vx; //x方向速度改变方向,碰撞损失能量，绝对值变小
					ax = -ax; //x方向加速度的方向总是与x方向速度的方向相反
				}
				if (x > 580) //防止小球穿过右墙壁
					x = 580;
				if (x < 20)//防止小球穿过左墙壁
					x = 20;

				if (y > 580) //当碰到地面时
				{
					vy = -0.95 * vy; //y方向速度改变方向，受阻力影响，绝对值变小
					y = 580;
				}
				if (y == 580)  //位于地面时
					vx += ax;
				if (fabs(vx) <= 0.0075)  //当x方向速度很小时
				{
					ax = 0.0;
					vx = 0.0;
				}
				if (fabs(vy) < 0.25 && y == 580)  //当y方向速度很小时,且位于地面上
				{
					vy = 0;
					g = 0;
				}
				cleardevice(); //清楚掉之前绘制的内容
				fillcircle(x, y, 20); //在坐标xy处绘制半径为20的圆
				Sleep(10); //暂停10毫秒
				if (_kbhit())//捕捉运动时按键消息
				{
					msg->vkcode = _getch();
				}
				if (msg->vkcode == VK_ESCAPE)  //当按下的按键是ESC时
				{
					msg->vkcode = 0;//将接受按键消息的内存更改为0
					InputBox(pString, 4, L"确认退出吗?", L"模拟小球平抛", L"Yes", 0, 0, false); //调出对话框,回答默认是Yes
					flushmessage(-1); //清空消息缓冲区
					if (_tcscmp(pString, L"Yes") == 0) //当玩家的输入为Yes时
						break;
				}
				if (vx == 0 && vy == 0 &&y==580) //当x方向，y方向速度都为0,且小球位于地面
				{
					Sleep(500);  //暂停500毫秒
					InputBox(pRestart, 4, L"是否重新开始", L"模拟小球平抛", L"Yes", 0, 0, false);
					flushmessage(EM_KEY);
					break;
				}
			}
		} while (_tcscmp(pRestart, L"Yes") == 0);  //当玩家的回答是否重新开始时输入Yes
		closegraph();//关闭窗口
	}

/**********************************跳跃球*********************************************/

	struct gamedata {
		char t[26]; //储存产生分数的游戏时间
		int score;
	};
	//将char转为TCHAR  
//*tchar是TCHAR类型指针，*_char是char类型指针   
	void CharToTchar(const char* _char, TCHAR* tchar)
	{
		int iLength;

		iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
		MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
	}
/*函数功能:读入历史分数并排序打印,并替代原先未排序的数据
  函数入口参数:data1[]:简单模式的数据
			   data2[]:普通模式的数据
			   data3[]:困难模式的数据
  函数返回值:无*/
	void RankScore(gamedata data1[], gamedata data2[], gamedata data3[])
	{
		//读取数据
		FILE* fp1, * fp2, * fp3;
		fp1 = fopen("JumpBallEasy.txt", "r");
		fp2 = fopen("JumpBallNormal.txt", "r");
		fp3 = fopen("JumpBallHard.txt", "r");
		int i;
		int end1=0, end2=0, end3=0;
		i = 0;
		while (!feof(fp1))
		{
				fgets(data1[i].t, 26, fp1);
				fscanf(fp1,"%d", &data1[i].score);
				i++;
		}
				end1 = i;
				i = 0;
				while (!feof(fp2))
				{
					fgets(data2[i].t, 26, fp2);
					fscanf(fp2, "%d", &data2[i].score);
					i++;
				}
				end2 = i;
				i = 0;
				while (!feof(fp3))
				{
					fgets(data3[i].t, 26, fp3);
					fscanf(fp3, "%d", &data3[i].score);
					i++;
				}
				end3 = i;
		
		//开始排序
		
		gamedata temp;
		int j = 0;
		int max1;
		int max2;
		int max3;
		for (j = 0; j < end1-1; j++)  //选择法排序
		{
			max1 = j;
			for (i = j+1; i < end1; i++)
			{
				if (data1[max1].score < data1[i].score)
					max1 = i;
			}
			temp= data1[max1];
			data1[max1] = data1[j];
			data1[j] = temp;
		}
		for (j = 0; j < end2 - 1; j++)  //选择法排序
		{
			max2 = j;
			for (i = j + 1; i < end2; i++)
			{
				if (data2[max2].score < data2[i].score)
					max2 = i;
			}
			temp = data2[max2];
			data2[max2] = data2[j];
			data2[j] = temp;
		}
		for (j = 0; j < end3 - 1; j++)  //选择法排序
		{
			max3 = j;
			for (i = j + 1; i < end3; i++)
			{
				if (data3[max3].score < data3[i].score)
					max3 = i;
			}
			temp = data3[max3];
			data3[max3] = data3[j];
			data3[j] = temp;
		}
		fclose(fp1);
		fclose(fp2);
		fclose(fp3);
		
		//开始绘图
			cleardevice();
			settextstyle(40, 0, L"微软雅黑");
			settextcolor(RGB(255, 255, 255));
			outtextxy(30, 20, L"easy");
			int x1 = 30, y1 = 0, x2 = 200;
			for (i = 0; i < end1; i++)
			{
				TCHAR s[10],s2[27];
					settextstyle(40, 0, L"微软雅黑");
					settextcolor(RGB(0, 200, 0));
					_stprintf(s, L"Points:%d", data1[i].score);
					y1 += 60;
					outtextxy(x1, y1, s);
					CharToTchar(data1[i].t, s2);
					settextstyle(30, 0, L"微软雅黑");
					outtextxy(x2, y1 + 5, s2);
					FlushBatchDraw();
			}
			while (_getch() != 'p');
				cleardevice();
				settextstyle(40, 0, L"微软雅黑");
				settextcolor(RGB(255, 255, 255));
				outtextxy(30, 20, L"normal");
				x1 = 30;
				y1 = 0;
				x2 = 200;
				for (i = 0; i < end2; i++)
				{
					TCHAR s[10], s2[27];
					settextstyle(40, 0, L"微软雅黑");
					settextcolor(RGB(0, 200, 0));
					_stprintf(s, L"Points:%d", data2[i].score);
					y1 += 60;
					outtextxy(x1, y1, s);
					CharToTchar(data2[i].t, s2);
					settextstyle(30, 0, L"微软雅黑");
					outtextxy(x2, y1 + 5, s2);
					FlushBatchDraw();
				}
				while (_getch() != 'p');
				cleardevice();
				settextstyle(40, 0, L"微软雅黑");
				settextcolor(RGB(255, 255, 255));
				outtextxy(30, 20, L"hard");
				x1 = 30;
				y1 = 0;
				x2 = 200;
				for (i = 0; i < end3; i++)
				{
					TCHAR s[10], s2[27];
					settextstyle(40, 0, L"微软雅黑");
					settextcolor(RGB(0, 200, 0));
					_stprintf(s, L"Points:%d", data3[i].score);
					y1 += 60;
					outtextxy(x1, y1, s);
					CharToTchar(data3[i].t, s2);
					settextstyle(30, 0, L"微软雅黑");
					outtextxy(x2, y1 + 5, s2);
					FlushBatchDraw();
				}
				while(_getch()!=VK_ESCAPE);
				return;
		}
		float Dist2Points(float x1, float y1, float x2, float y2);
/*函数功能:计算点到直线之间的距离
  函数入口参数:line_x1:直线第一个端点的x坐标
			   line_y1:直线第一个端点的y坐标
			   line_x2:
			   line_y2:
			   point_x
			   point_y
  函数返回值:点到直线的距离。*/
		float PointToLine(float line_x1, float line_y1, float line_x2, float line_y2, float point_x, float point_y)
		{
			float Sin = (line_y2 - line_y1) / Dist2Points(line_x1, line_y1, line_x2, line_y2);
			float Distance = fabs(Dist2Points(line_x1, line_y1, point_x, point_y)*Sin);
			return Distance;
	}
/*函数功能:跳跃球数据写入
  函数入口参数:score:此刻的分数
  函数返回值:无*/
	void JumpBallWrite(int score,LPTSTR pDifficulty)
	{
		FILE* fp;
		if(_tcscmp(pDifficulty,L"easy")==0)
		fp = fopen("JumpBallEasy.txt", "a");
		else if(_tcscmp(pDifficulty,L"normal")==0)
		fp = fopen("JumpBallNormal.txt", "a");
		else 
		fp = fopen("JumpBallHard.txt","a");
		time_t t;
		tm* lt;
		time(&t);
		lt = localtime(&t);
		fprintf(fp, "%s%d", asctime(lt), score);
		fclose(fp);
  }
	
/*函数功能:跳跃球游戏本体
  函数入口参数:void
  返回参数:无*/
	void JumpBall()
	{
		
		gamedata data_easy[1000]; //cpp的规范不需要struct,最多读取50次游戏数据
		gamedata data_normal[1000];
		gamedata data_hard[1000];
		float width, height, gravity; //游戏画面宽度,高度，重力加速度
		float ball_x, ball_y, ball_vy, radius; //小球原点坐标,y方向速度,半径
		float rect_left_x, rect_top_y, rect_width, rect_height, rect_vx;
		int score = 0;
		int isBallOnFloor = 1;

		width = 600;
		height = 400;
		initgraph(width, height,EW_NOCLOSE);
		gravity = 0.6;
		radius = 20;
		ball_x = width / 4;
		ball_y = height - radius;
		ball_vy = 0;

		rect_width = 20;
		rect_height = 100;
		rect_vx = -3;
		rect_left_x = width * 3 / 4;
		rect_top_y = height - rect_height;

		TCHAR Difficulty[7];
		TCHAR Answer[7];
		ExMessage Message;
		ExMessage* pMessage = &Message;
		LPTSTR pDifficulty = Difficulty, pAnswer = Answer;

		BeginBatchDraw();

		do {
			InputBox(pDifficulty, 7, _T("请选择难易度(easy,normal,hard)"), L"跳跃球", _T("easy"), 0, 0, true);
		} while (!(
			_tcscmp(pDifficulty, _T("easy")) == 0
			|| _tcscmp(pDifficulty, _T("normal")) == 0
			|| _tcscmp(pDifficulty, _T("hard")) == 0));
		while (1)
		{
			if (_kbhit())  //当按键时
			{
				char input = _getch();
				if (input == ' ' && isBallOnFloor)
				{
					ball_vy = -17;
					isBallOnFloor = 0;
				}
				else if (input == 'p' || input == 'P')
				{
					RankScore(data_easy, data_normal, data_hard);
				}
				pMessage->vkcode = input;

			}
			ball_vy += gravity;
			ball_y += ball_vy;
			rect_left_x += rect_vx;
			if (ball_y > height - radius)
			{
				isBallOnFloor = 1;
				ball_vy = 0;
				ball_y = height - radius;
			}
			if (ball_y < 0)
			{
				ball_vy = 0;
				ball_y = 0;
			}
			if (rect_left_x < 0)
			{
				score++;
				rect_left_x = width;
				rect_height = rand() % (int(height) / 4) + height / 4;
				if (_tcscmp(pDifficulty, _T("easy")) == 0)
					rect_vx = rand() / float(RAND_MAX) * 4 - 7;
				else if (_tcscmp(pDifficulty, _T("normal")) == 0)
					rect_vx = rand() / float(RAND_MAX) * 4 - 9;
				else
					rect_vx = rand() / float(RAND_MAX) * 2 - 9;
			}

			if (((rect_left_x <= ball_x + radius)
				&& (rect_left_x + rect_width >= ball_x - radius)
				&& (rect_top_y <= ball_y + radius)))
				 
			{
				JumpBallWrite(score,pDifficulty);
				score = 0;
				rect_left_x = width;
				rect_height = rand() % (int(height) / 4) + height / 4;
				rect_vx = rand() / float(RAND_MAX) * 4 - 7;
				Sleep(500);
			}

			cleardevice();
			fillcircle(ball_x, ball_y, radius);
			fillrectangle(rect_left_x, height - rect_height, rect_left_x + rect_width, height);
			settextcolor(RGB(255, 255, 255));
			TCHAR s[20];
			TCHAR info[6] = L"按空格跳跃";
			_stprintf(s, _T("%d"), score); //将score转换为字符串
			settextstyle(40, 0, _T("微软雅黑"));
			outtextxy(50, 30, s);//输出得分分数
			settextstyle(20, 0, L"微软雅黑");
			outtextxy(30, 80, info);
			FlushBatchDraw();
			Sleep(10);
			if (pMessage->vkcode == VK_ESCAPE)
			{
				InputBox(pAnswer, 7, L"你想退出吗(Yes or no)?或者你想改变难度(输入easy等)", L"跳跃球", L"Yes", 0, 0, false);
				if (_tcscmp(pAnswer, L"Yes") == 0)
				{
					EndBatchDraw();
					break;
				}
				else if (_tcscmp(pAnswer, _T("easy")) == 0
					|| _tcscmp(pAnswer, _T("normal")) == 0
					|| _tcscmp(pAnswer, _T("hard")) == 0)
				{
					JumpBallWrite(score,pDifficulty);
					score = 0;
					_tcscpy(pDifficulty, pAnswer);
				}
				pMessage->vkcode = 0;
				Answer[1] = '0';
			}

		}
		closegraph();
		return;
	}
/*函数功能:旋转蛇游戏本体
  函数入口参数:void
  函数返回值:无*/
	void RotateSnake()
	{
		const float PI = 3.14159;
		initgraph(800, 600, EW_NOCLOSE);
		setbkcolor(RGB(128, 128, 128));
		cleardevice();
		srand(time(0));

		int centerX;
		int centerY;
		int radius;
		int i;
		float offset;
		float totalOffset;
		TCHAR Answer[4];
		LPTSTR pAnswer = Answer;
		ExMessage message;
		ExMessage* msg = &message;

		while (1)
		{
			for (centerX = 100; centerX < 800; centerX += 200)
			{
				for (centerY = 100; centerY < 600; centerY += 200)
				{
					totalOffset = 0;
					float h = rand() % 180;
					COLORREF color1 = HSVtoRGB(h, 0.9, 0.8);
					COLORREF color2 = HSVtoRGB(h + 180, 0.9, 0.8);

					for (radius = 100; radius > 0; radius -= 25)
					{
						int left = centerX - radius;
						int top = centerY - radius;
						int right = centerX + radius;
						int bottom = centerY + radius;
						for (i = 0; i < 20; i++)
						{
							offset = i * PI / 10 + totalOffset;
							setfillcolor(color1);
							solidpie(left, top, right, bottom, 0 + offset, PI / 30 + offset);
							setfillcolor(RGB(255, 255, 255));
							solidpie(left, top, right, bottom, PI / 30 + offset, PI / 20 + offset);
							setfillcolor(color2);
							solidpie(left, top, right, bottom, PI / 20 + offset, PI / 12 + offset);
							setfillcolor(RGB(0, 0, 0));
							solidpie(left, top, right, bottom, PI / 12 + offset, PI / 10 + offset);
						}
						totalOffset += PI / 20;
					}
				}
			}
			msg->vkcode = _getch();
			if (msg->vkcode == VK_ESCAPE)
			{
				InputBox(pAnswer, 4, L"确认退出吗?", L"旋转蛇", L"Yes", 0, 0, false);
				if (_tcscmp(pAnswer, L"Yes") == 0)
				{
					closegraph();
					return;
				}
				else
					msg->vkcode = 0;
			}
		}
		closegraph();
		return;
	}
	/*函数功能:移动方块错觉本体
	  函数入口参数:无
	  函数返回值:无*/
	void MovingRectangle()
	{
		initgraph(750, 500,NOCLOSE); // 新开画面
		setbkcolor(RGB(255, 255, 255)); // 背景白色
		cleardevice();
		int x, xStart, xEnd; // x起始、终止坐标
		int c, cStart, cEnd; //  起始颜色、终止颜色
		xStart = 50; // x起始坐标为50
		TCHAR Answer[4];
		LPTSTR pAnswer = Answer;
		ExMessage message;
		ExMessage* msg = &message;
		while (1)
		{
			while (xStart < 600) // 当x起始坐标小于600时
			{
				xEnd = xStart + 50; // 这一段的x终止坐标+50，也就是画一共50像素的块
				cStart = 255; // 初始为全紫色
				cEnd = 128; //  最后为暗紫色
				for (x = xStart; x < xEnd; x++) // 渐变颜色，绘制这么多根线条，绘制紫色过渡区域 
				{
					c = (x - xStart) * (cEnd - cStart) / (xEnd - xStart) + cStart; // 
					setlinecolor(RGB(c, 0, c)); // 设定颜色 
					line(x, 50, x, 100); // 第一大行
					line(x, 250, x, 300); // 第三大行
				}

				xStart = xEnd;
				xEnd = xStart + 100;
				cStart = 0;
				cEnd = 255;
				for (x = xStart; x < xEnd; x++) // 绘制多根线条，黑色到黄色渐变区域
				{
					c = (x - xStart) * (cEnd - cStart) / (xEnd - xStart) + cStart; // 
					setlinecolor(RGB(c, c, 0)); // 设定颜色
					line(x, 50, x, 100); // 第一大行 
					line(x, 250, x, 300); //  第三大行
				}
				xStart = xEnd + 20; // 还隔了一点白色区域
			} // 循环，继续绘制下面一段

			xStart = 50; // 下面反向绘制，第二大行、第四大行的图案
			while (xStart < 600) // 
			{
				xEnd = xStart + 100;
				cStart = 255;
				cEnd = 0;
				for (x = xStart; x < xEnd; x++)
				{
					c = (x - xStart) * (cEnd - cStart) / (xEnd - xStart) + cStart;
					setlinecolor(RGB(c, c, 0));
					line(x, 150, x, 200);
					line(x, 350, x, 400);
				}

				xStart = xEnd;
				xEnd = xStart + 50;
				cStart = 128;
				cEnd = 255;
				for (x = xStart; x < xEnd; x++)
				{
					c = (x - xStart) * (cEnd - cStart) / (xEnd - xStart) + cStart;
					setlinecolor(RGB(c, 0, c));
					line(x, 150, x, 200);
					line(x, 350, x, 400);
				}
				xStart = xEnd + 20;
			}
			msg->vkcode = _getch();
			if (msg->vkcode == VK_ESCAPE)
			{
				InputBox(pAnswer, 4, L"确认退出吗?", L"移动方块", L"Yes", 0, 0, false);
				if (_tcscmp(pAnswer, L"Yes") == 0)
				{
					closegraph();
					return;
				}
				else
					msg->vkcode = 0;
			}
		}
	}
	/*函数功能:见缝插针本体
	  函数返回值:无
	  函数入口参数:无*/
	void ThrowPins()
	{
		const float PI = 3.1415926;
		int width = 800;
		int height = 600;
		initgraph(width, height, NOCLOSE);  //绘制窗口
		setbkcolor(RGB(255, 255, 255));  //设置背景颜色为白色
		setlinestyle(PS_SOLID, 3);  //将线设定为实线，粗度为3
		cleardevice();

		float lineLength = 160; //针的长度
		float xEnd, yEnd;  //针的终止x坐标
		float rotateSpeed = PI / 360;  //针旋转速度
		int lineNum = 0;  //插在圆盘上的针的数目
		float Angles[1000]; //针的角度
		int i;
		int addOne_1 = 0; //记录玩家1针丢出的状态
		int addOne_2 = 0;  //记录玩家2针丢出的状态
		float vx = 10;  //针丢出的速度
		float xStart = 0, xStart2 = width;  //针的起始x坐标
		int score = 0;
		int counter = 0;
		ExMessage message;  //按键信息
		ExMessage* pMessage = &message;
		TCHAR answer[4], restart[4];  //退出游戏，重新开始
		LPTSTR pAnswer = answer;
		LPTSTR pRestart = restart;

		int playerNum;
		do
		{
			while (1)
			{
				TCHAR singleOrDouble[2];
				LPTSTR psingleOrDouble = singleOrDouble;
				InputBox(psingleOrDouble, 2, L"1 player(单人)or 2 player(双人)(input the number)", L"见缝插针", L"1", 0, 0, true);
				if (_tcscmp(psingleOrDouble, L"1") == 0)
				{
					playerNum = 1;
					break;
				}
				else if (_tcscmp(psingleOrDouble, L"2") == 0)
				{
					playerNum = 2;
					break;
				}
				else
					continue;
			}

			BeginBatchDraw();  //开始批量绘制，用于消除画面闪烁
			while (1)
			{
				cleardevice();
				setlinecolor(RGB(0, 0, 0));
				if (addOne_1 == 0)
					line(0, height / 2, lineLength, height / 2);
				if (playerNum == 2 && addOne_2 == 0)
					line(width - lineLength, height / 2, width, height / 2);

				if (_kbhit())
				{
					char input = _getch();
					if (input == ' ' && rotateSpeed != 0)
						addOne_1 = 1;
					else if (input == '0' && rotateSpeed != 0)
						addOne_2 = 1;
					else if (input == VK_ESCAPE)
						pMessage->vkcode = VK_ESCAPE;
				}
				if (addOne_1 == 1)
				{
					xStart += vx;
					line(xStart, height / 2, xStart + lineLength, height / 2);
					if (xStart >= width / 2 - lineLength)
					{
						addOne_1 = 0;
						lineNum++;
						Angles[lineNum - 1] = PI;
						xStart = 0;
						if (playerNum == 1)
							rotateSpeed += PI / 1800;  //每多一根针提高圆盘旋转速度
					}
				}
				if (addOne_2 == 1)
				{
					xStart2 -= vx;
					line(xStart2 - lineLength, height / 2, xStart2, height / 2);
					if (xStart2 - lineLength <= width / 2)
					{
						addOne_2 = 0;
						lineNum++;
						Angles[lineNum - 1] = 0;
						xStart2 = width;
						rotateSpeed += PI / 1800;
					}
				}
				for (i = 0; i < lineNum - 1; i++)  //拿新增加的针和之前的针比较
				{
					if (fabs(Angles[lineNum - 1] - Angles[i]) < PI / 60)
					{
						rotateSpeed = 0;
						break;
					}
				}
				if (rotateSpeed != 0)  //当没有失败时，分数等于针数
					score = lineNum;
				for (i = 0; i < lineNum; i++)
				{
					Angles[i] += rotateSpeed;
					if (Angles[i] > 2 * PI)
						Angles[i] -= 2 * PI;
					xEnd = width / 2 + lineLength * cos(-Angles[i]);
					yEnd = height / 2 + lineLength * sin(-Angles[i]);
					setlinecolor(RGB(0, 0, 255));
					if (i == lineNum - 1)
						setlinecolor(RGB(255, 0, 0));
					line(width / 2, height / 2, xEnd, yEnd);
				}

				setlinecolor(HSVtoRGB(0, 0.9, 0.8));
				circle(width / 2, height / 2, 60);

				TCHAR s[5];
				_stprintf(s, L"%d", score);
				settextstyle(50, 0, L"Times");
				settextcolor(RGB(50, 50, 50));
				outtextxy(65, 200, s);

				setfillcolor(HSVtoRGB(0, lineNum / 60.0, 0.8));//绘制中间的圆盘，针越多颜色越鲜艳
				setlinecolor(HSVtoRGB(0, 0.9, 0.8));  //设置圆盘线条颜色为红色
				fillcircle(width / 2, height / 2, 60);
				FlushBatchDraw();
				Sleep(10);
				if (pMessage->vkcode == VK_ESCAPE)
				{
					pMessage->vkcode = 0;
					InputBox(pAnswer, 4, L"你想退出吗?(Yes),或者你想更改单双人模式吗?(1or2)", L"见缝插针", L"Yes", 0, 0, false);
					if (_tcscmp(pAnswer, L"Yes") == 0)
					{
						EndBatchDraw(); //结束批量绘制
						closegraph();
						return;
					}
					else if (_tcscmp(pAnswer, L"1") == 0)
						playerNum = 1;
					else if (_tcscmp(pAnswer, L"2") == 0)
						playerNum = 2;
				}
				if (rotateSpeed == 0 && counter == 0)
				{
					counter++;
					Sleep(1000);
					*pRestart = 0;
					InputBox(pRestart, 4, L"重新开始", L"见缝插针", L"Yes", 0, 0, false);
					if (_tcscmp(pRestart, L"Yes") == 0)
					{
						lineNum = 0;
						rotateSpeed = PI / 360;
						counter = 0;
						break;
					}

				}
			}
		} while (_tcscmp(pRestart, L"Yes") == 0);

		_getch();
		closegraph();
		return;
	}
	/*****************************************见缝插圆的封装小函数*****************************/
	//求解两个点之间的距离
	float Dist2Points(float x1, float y1, float x2, float y2)
	{
		float result;
		result = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
		return result;
	}
	//判断两圆是否相交或相切
	int IsTwoCirclesIntersect(float x1, float y1, float r1, float x2, float y2, float r2)
	{
		if (Dist2Points(x1, y1, x2, y2) <= r1 + r2)
			return 1;
		else
			return 0;
	}
	//生成min和max之间的随机整数
	int RandBetweenMinMax(int Min, int Max)
	{
		int r = rand() % (Max - Min + 1) + Min;
		return r;
	}
	//填充黄色圆绘制
	void DrawCircles1(float x, float y, float r)
	{
		setlinecolor(RGB(0, 0, 0));
		setfillcolor(RGB(255, 255, 0));
		fillcircle(x, y, r);
	}
	//填充随机颜色圆绘制
	void DrawCircles2(float x, float y, float r)
	{
		float h = rand() % 360;
		setlinecolor(RGB(255, 255, 255));
		setfillcolor(HSVtoRGB(h, 0.6, 0.8));
		fillcircle(x, y, r);
	}
	//填充随机颜色同心圆绘制
	void DrawCircles3(float x, float y, float r)
	{
		while (r > 0)
		{
			float h = rand() % 360;
			setlinecolor(RGB(255, 255, 255));
			setfillcolor(HSVtoRGB(h, 0.6, 0.8));
			fillcircle(x, y, r);
			r -= 5;
		}
	}
	//填充颜色同心圆线条
	void DrawCircles4(float x, float y, float r)
	{
		while (r > 0)
		{
			float h = rand() % 360;
			setlinecolor(HSVtoRGB(h, 0.6, 0.8));
			circle(x, y, r);
			r -= 5;
		}
	}
/********************************见缝插圆的封装小函数************************************/
	/*函数功能:见缝插圆
	  函数入口参数:无
	  函数返回值:无*/
	void BeatifulCircle()
	{
		int width = 800;
		int height = 600;
		initgraph(width, height,NOCLOSE);
		setbkcolor(RGB(255, 255, 255));
		srand(time(0));
		cleardevice();

		int xArray[1000];
		int yArray[1000];
		int rArray[1000];
		int circleNum = 0;
		int rMax = 50, rMin = 8;
		float x, y, r;
		int isNewCircleOk;
		int i, j;
		int drawMode = 1;

		TCHAR answer[4];
		LPTSTR pAnswer = answer;

		settextstyle(50, 0, L"微软雅黑");
		settextcolor(BLACK);
		outtextxy(350, 100, L"说明");
		settextstyle(30, 0, L"微软雅黑");
		outtextxy(300, 150, L"按空格清屏");
		outtextxy(300, 180, L"1,2,3,4切换绘图模式");
		outtextxy(300, 210, L"按ESC退出");
		settextcolor(RED);
		settextstyle(25, 0, L"微软雅黑");
		outtextxy(300, 500, L"任意按键继续");
		_getch();
		cleardevice();

		while (circleNum < 1000)
		{
			isNewCircleOk = 0;
			while (isNewCircleOk == 0)
			{
				if (_kbhit()) //当按键时;放置此处的原因是当屏幕上没有位置再放圆时也能够切换绘图模式,而放置大循环里无法做到。
				{
					char input = _getch();
					if (input == ' ')
					{
						circleNum = 0;
						cleardevice();
					}
					else if (input == '1')
						drawMode = 1;
					else if (input == '2')
						drawMode = 2;
					else if (input == '3')
						drawMode = 3;
					else if (input == '4')
						drawMode = 4;
					else if (input == VK_ESCAPE)
					{
						InputBox(pAnswer, 4, L"确认退出吗?", L"见缝插圆", L"Yes", 0, 0, false);
						if (_tcscmp(pAnswer, L"Yes") == 0)
						{
							closegraph();
							return;
						}
						else
							*pAnswer = 0;
					}
				}
				x = rand() % width;
				y = rand() % height;
				r = rMin;
				for (i = 0; i < circleNum; i++)
				{
					if (IsTwoCirclesIntersect(x, y, r, xArray[i], yArray[i], rArray[i]))
						break;
				}
				if (i == circleNum)
					isNewCircleOk = 1;
			}
			isNewCircleOk = 0;
			while (isNewCircleOk == 0 && r < rMax)
			{
				r++;
				for (j = 0; j < circleNum; j++)
				{
					if (IsTwoCirclesIntersect(x, y, r, xArray[j], yArray[j], rArray[j]))
					{
						isNewCircleOk = 1;
						break;
					}
				}

			}
			xArray[circleNum] = x;
			yArray[circleNum] = y;
			rArray[circleNum] = r;
			circleNum++;

			switch (drawMode)
			{
			case 1:
				DrawCircles1(x, y, r);
				break;
			case 2:
				DrawCircles2(x, y, r);
				break;
			case 3:
				DrawCircles3(x, y, r);
				break;
			case 4:
				DrawCircles4(x, y, r);
				break;
			}
			Sleep(10);
		}
		_getch();
		closegraph();
		return;
	}
/*******************************贪吃蛇函数区********************************************/
#define HEIGHT 30
#define WIDTH 40
#define BLOCK_SIZE 20

//全局变量的定义
	int Blocks[HEIGHT][WIDTH];
	int Blocks2[HEIGHT][WIDTH];
	char moveDirection, moveDirection2;  //蛇的运动方向
	int isFailure; //记录是否失败
	int food_i, food_j; //食物的位置
	int slow_i, slow_j; //减速道具的位置
	int IndexCounter; //帧数计数
	int moveIndex; //蛇每移动一格的帧数间隔，用于控制移动速度
	TCHAR playerNum[2];
	TCHAR difficulty[7];
	
	LPTSTR pPlayerNum = playerNum;
	LPTSTR pDifficulty = difficulty;

	void startup() //初始化函数
	{
		int i, j;
		for (i = 0; i < HEIGHT; i++)  //按行遍历初始化
		{
			for (j = 0; j < WIDTH; j++) //按列遍历初始化
			{
				Blocks[i][j] = 0;
				Blocks2[i][j] = 0;
			}
		}
		isFailure = 0; 
		moveDirection = 'D';
		moveDirection2 = 'J';
		IndexCounter = 0;
		initgraph(WIDTH * BLOCK_SIZE, HEIGHT * BLOCK_SIZE,NOCLOSE);
		settextstyle(40, 0, L"微软雅黑");
		outtextxy(50, 50, L"操作说明:WSAD玩家1，IKJL玩家2");
		outtextxy(50, 100, L"道具说明:蓝色方块减速,绿色方块加长度,");
		do
		{
			InputBox(pPlayerNum, 2, L"单人游戏or双人游戏(1 or 2)", L"贪吃蛇", L"1", 0, 0, true);
		} while (*pPlayerNum != '1' && *pPlayerNum != '2');
		do
		{
			InputBox(pDifficulty, 7, L"难度:easy,normal,hard,change(随体长改变移动速度)", L"贪吃蛇", L"easy", 0, 0, true);
		} while (_tcscmp(pDifficulty, L"easy") != 0
			&& _tcscmp(pDifficulty, L"normal") != 0
			&& _tcscmp(pDifficulty, L"hard") != 0
			&& _tcscmp(pDifficulty, L"change") != 0);
		if (_tcscmp(pDifficulty, L"easy") == 0)
			moveIndex = 12;
		else if (_tcscmp(pDifficulty, L"normal") == 0)
			moveIndex = 8;
		else if (_tcscmp(pDifficulty, L"hard") == 0)
			moveIndex = 6;
		else if (_tcscmp(pDifficulty, L"change") == 0)
			moveIndex = 12;
		srand(time(0));
		do
		{
			food_i = rand() % HEIGHT;
			food_j = rand() % WIDTH;
		} while (Blocks[food_i][food_j] > 0 || Blocks2[food_i][food_j] > 0); //当食物生成在蛇身
		do
		{
			slow_i = rand() % HEIGHT;
			slow_j = rand() % WIDTH;
		} while (Blocks[slow_i][slow_j] > 0 || Blocks2[slow_i][slow_j] > 0 || (slow_i == food_i) && (slow_j == food_j)); //当道具生成在蛇身上时
		if (*pPlayerNum == '1')
		{
			for (i = 0; i < 5; i++)
			{
				Blocks[HEIGHT / 2][WIDTH / 2 - i] = i + 1; //画面中间为蛇头，数字为1，向左依次为蛇身，数字依次为2,3,4,5
			}
		}
		else if (*pPlayerNum == '2')
		{
			for (i = 0; i < 5; i++)
			{
				Blocks[HEIGHT * 1 / 3][WIDTH * 1 / 4 - i] = i + 1; //玩家1
				Blocks2[HEIGHT * 2 / 3][WIDTH * 3 / 4 + i] = i + 1; //玩家2
			}
		}
		setlinecolor(RGB(200, 200, 200));
		BeginBatchDraw();
	}

	void show() //绘制函数
	{
		cleardevice();
		int i, j;
		for (i = 0; i < HEIGHT; i++)
		{
			for (j = 0; j < WIDTH; j++)
			{
				if (Blocks[i][j] > 0)
					setfillcolor(HSVtoRGB(Blocks[i][j] * 10, 0.9, 1));
				else
					setfillcolor(RGB(150, 150, 150));

				fillrectangle(j * BLOCK_SIZE, i * BLOCK_SIZE, (j + 1) * BLOCK_SIZE, (i + 1) * BLOCK_SIZE);
				if (Blocks2[i][j] > 0)
					setfillcolor(HSVtoRGB(360 - Blocks2[i][j] * 10, 0.9, 1));

				fillrectangle(j * BLOCK_SIZE, i * BLOCK_SIZE, (j + 1) * BLOCK_SIZE, (i + 1) * BLOCK_SIZE);
			}
		}

		setfillcolor(RGB(0, 255, 0)); //设定食物方块颜色为绿色
		fillrectangle(food_j * BLOCK_SIZE, food_i * BLOCK_SIZE, (food_j + 1) * BLOCK_SIZE, (food_i + 1) * BLOCK_SIZE);
		setfillcolor(RGB(0, 0, 255)); //设定减速道具颜色为蓝色
		fillrectangle(slow_j * BLOCK_SIZE, slow_i * BLOCK_SIZE, (slow_j + 1) * BLOCK_SIZE, (slow_i + 1) * BLOCK_SIZE);
		if (isFailure)
		{
			setbkmode(TRANSPARENT); //文本字体透明
			settextcolor(RGB(255, 0, 0));
			settextstyle(80, 0, L"微软雅黑");
			outtextxy(240, 220, L"游戏失败");
		}
		FlushBatchDraw();
	}
	void MoveSnake() //移动小蛇及相关处理函数
	{
		int i, j;
		for (i = 0; i < HEIGHT; i++) //对行遍历
		{
			for (j = 0; j < WIDTH; j++) //对列遍历
			{
				if (Blocks[i][j] > 0)
					Blocks[i][j]++;
			}
		}

		int oldTail_i, oldTail_j, oldHead_i, oldHead_j; //定义变量储存旧蛇头蛇尾坐标
		int max = 0;
		for (i = 0; i < HEIGHT; i++)
		{
			for (j = 0; j < WIDTH; j++)
			{
				if (max < Blocks[i][j])
				{
					max = Blocks[i][j];
					oldTail_i = i; //最大值的坐标就是蛇尾的位置
					oldTail_j = j;
				}
				if (Blocks[i][j] == 2) //数字为2的恰好是蛇头位置
				{
					oldHead_i = i;
					oldHead_j = j;
				}
			}
		}
		int newHead_i = oldHead_i;
		int newHead_j = oldHead_j;
		if (moveDirection == 'D' || moveDirection == 'd')
			newHead_j += 1;
		else if (moveDirection == 'A' || moveDirection == 'a')
			newHead_j -= 1;
		else if (moveDirection == 'W' || moveDirection == 'w')
			newHead_i -= 1;
		else if (moveDirection == 'S' || moveDirection == 's')
			newHead_i += 1;

		if (newHead_i >= HEIGHT
			|| newHead_i < 0
			|| newHead_j >= WIDTH
			|| newHead_j < 0
			|| Blocks[newHead_i][newHead_j]>0)
		{
			isFailure = 1;
			return;
		}
		Blocks[newHead_i][newHead_j] = 1; //新蛇头数字为1
		if (newHead_i == food_i && newHead_j == food_j)
		{
			do
			{
				food_i = rand() % HEIGHT;
				food_j = rand() % WIDTH;
			} while (Blocks[food_i][food_j] > 0 || Blocks2[food_i][food_j] || (slow_i == food_i && slow_j == food_j)); //当食物生成在蛇身上时
			if (_tcscmp(pDifficulty, L"change") == 0 && moveIndex > 4)
				moveIndex--;
		}
		else
			Blocks[oldTail_i][oldTail_j] = 0; //旧蛇尾为空白

		if (newHead_i == slow_i && newHead_j == slow_j)  //吃到道具
		{
			slow_i = -1;
			slow_j = -1;
			moveIndex = moveIndex * 2;
			IndexCounter = 1;
		}

		if (*pPlayerNum == '2')  //双人模式下移动的处理
		{
			max = 0;
			for (i = 0; i < HEIGHT; i++) //对行遍历
			{
				for (j = 0; j < WIDTH; j++) //对列遍历
				{
					if (Blocks2[i][j] > 0)
						Blocks2[i][j]++;
				}
			}
			for (i = 0; i < HEIGHT; i++)
			{
				for (j = 0; j < WIDTH; j++)
				{
					if (max < Blocks2[i][j])
					{
						max = Blocks2[i][j];
						oldTail_i = i; //最大值的坐标就是蛇尾的位置
						oldTail_j = j;
					}
					if (Blocks2[i][j] == 2) //数字为2的恰好是蛇头位置
					{
						oldHead_i = i;
						oldHead_j = j;
					}
				}
			}
			newHead_i = oldHead_i;
			newHead_j = oldHead_j;
			if (moveDirection2 == 'L' || moveDirection2 == 'l')
				newHead_j += 1;
			else if (moveDirection2 == 'J' || moveDirection2 == 'j')
				newHead_j -= 1;
			else if (moveDirection2 == 'I' || moveDirection2 == 'i')
				newHead_i -= 1;
			else if (moveDirection2 == 'K' || moveDirection2 == 'k')
				newHead_i += 1;

			if (newHead_i >= HEIGHT
				|| newHead_i < 0
				|| newHead_j >= WIDTH
				|| newHead_j < 0
				|| Blocks2[newHead_i][newHead_j]>0)
			{
				isFailure = 1;
				return;
			}
			Blocks2[newHead_i][newHead_j] = 1; //新蛇头数字为1
			if (newHead_i == food_i && newHead_j == food_j)
			{
				do
				{
					food_i = rand() % HEIGHT;
					food_j = rand() % WIDTH;
				} while (Blocks2[food_i][food_j] > 0 || Blocks[food_i][food_j]); //当食物生成在蛇身上时
				if (_tcscmp(pDifficulty, L"change") == 0 && moveIndex > 5)
					moveIndex--;
			}
			else
				Blocks2[oldTail_i][oldTail_j] = 0; //旧蛇尾为空白

			if (newHead_i == slow_i && newHead_j == slow_j)  //吃到道具
			{
				slow_i = -1;
				slow_j = -1;
				moveIndex = moveIndex * 2;
				IndexCounter = 1;
			}
		}
	}
	void updateWithoutInput() //与输入无关的更新
	{
		if (isFailure)
			return;
		if (IndexCounter)
			IndexCounter++;
		static int waitIndex = 1; //静态局部变量
		waitIndex++; //每一帧加1
		if (waitIndex == moveIndex) //每10帧蛇运动一次
		{
			MoveSnake();
			waitIndex = 1;
		}
		if (IndexCounter == 300)
			moveIndex = moveIndex / 2;
		if (moveIndex < 4) //修复change难度下速度超过设定的下限的bug
			moveIndex = 4;
		if (IndexCounter == 1000)
		{
			do
			{
				slow_i = rand() % HEIGHT;
				slow_j = rand() % WIDTH;
			} while (Blocks[slow_i][slow_j] > 0 || Blocks2[slow_i][slow_j] || ((slow_i == food_i) && (slow_j == food_j)));
			IndexCounter = 0;

		}
	}

	void updateWithInput() //与输入有关的更新
	{
		if (_kbhit())
		{
			char input = _getch();
			if ((input == 's' || input == 'S')
				&& (moveDirection != 'w' && moveDirection != 'W'))
				moveDirection = input;
			else if ((input == 'w' || input == 'W')
				&& (moveDirection != 's' && moveDirection != 'S'))
				moveDirection = input;
			else if ((input == 'a' || input == 'A')
				&& (moveDirection != 'd' && moveDirection != 'D'))
				moveDirection = input;
			else if ((input == 'd' || input == 'D')
				&& (moveDirection != 'a' && moveDirection != 'A'))
				moveDirection = input;
			if ((input == 'k' || input == 'K')
				&& (moveDirection2 != 'I' && moveDirection2 != 'i'))
				moveDirection2 = input;
			else if ((input == 'i' || input == 'I')
				&& (moveDirection2 != 'k' && moveDirection2 != 'K'))
				moveDirection2 = input;
			else if ((input == 'j' || input == 'J')
				&& (moveDirection2 != 'l' && moveDirection2 != 'L'))
				moveDirection2 = input;
			else if ((input == 'l' || input == 'L')
				&& (moveDirection2 != 'j' && moveDirection2 != 'J'))
				moveDirection2 = input;

			if (input == VK_ESCAPE)
				message.vkcode = VK_ESCAPE;
		}
	}

	void Snake()  //贪吃蛇主函数(伪)
	{
		TCHAR answer[4]; //是否退出游戏的回答储存在这里
		LPTSTR pAnswer = answer; //pAnswer是指向answer的指针
		startup();  //初始化函数,仅执行一次
		while (1)  //一直循环 
		{
			show(); //进行绘制
			updateWithoutInput(); //与输入无关的更新
			updateWithInput(); //与输入有关的更新
			if (message.vkcode == VK_ESCAPE)
			{
				message.vkcode = 0;
				InputBox(pAnswer, 4, L"确认退出吗?", L"贪吃蛇", L"Yes", 0, 0, false);
				if (_tcscmp(pAnswer, L"Yes") == 0)
				{
					EndBatchDraw();
					closegraph();
					return;
				}
			}
		}
		return;
	}
/******************************贪吃蛇函数区***************************************/
/******************************十步万度函数区*************************************/
#define PI 3.14159 
	typedef struct ROUND //定义结构体,用来表示带角度的小圆圈
	{
		double x, y; //小圆圈的圆心xy坐标
		double r; //小圆圈的半径
		int angleNum; //对应角度类型，只能是0，1,2,3
	}Round;
	//全局变量的定义
	Round rounds[5][5]; //结构体二维数组
	int click_i = 5, click_j = 5;//鼠标点击的小圆圈对应的结构体二维数组的i和j,初始化为5表示未指向任何圆圈
	int step;  //还剩下的操作步数
	int score; //得分，也就是总共转了多少度。
	double PointsDistance(double x, double y, double a, double b) //计算两点之间的距离
	{
		double Distance;
		Distance = sqrt((x - a) * (x - a) + (y - b) * (y - b));
		return Distance;
	}
	int IsMouseHitInRound(double x, double y, int i, int j)  //判断该点是否在此圆内
	{
		if (PointsDistance(x, y, rounds[i][j].x, rounds[i][j].y) <= rounds[i][j].r)
			return 1;
		else
			return 0;
	}
	void RotateRound(int i, int j)  //小圆圈指针顺时针旋转90度
	{
		score += 90;
		rounds[i][j].angleNum -= 1; //顺时针旋转90度角度类型减1
		if (rounds[i][j].angleNum < 0)
			rounds[i][j].angleNum = 3;
	}
	//获得当前圆圈指向的下一个圆圈的序号
	//当前圆圈序号储存在int indexes[2]中，下一个圆圈也储存在这中
	//如果指向边界返回0，指向圆圈返回1
	int GetNextIndexes(int indexes[2])
	{
		if (rounds[indexes[0]][indexes[1]].angleNum == 0)
			indexes[1]++;
		else if (rounds[indexes[0]][indexes[1]].angleNum == 1)
			indexes[0]--;
		else if (rounds[indexes[0]][indexes[1]].angleNum == 2)
			indexes[1]--;
		else if (rounds[indexes[0]][indexes[1]].angleNum == 3)
			indexes[0]++;
		if (indexes[0] < 0 || indexes[0]>4 || indexes[1] < 0 || indexes[1]>4)
			return 0;
		else
			return 1;
	}
	void startup2() //初始化函数
	{
		step = 10;
		score = 0;
		initgraph(600, 700,NOCLOSE);
		setbkcolor(RGB(50, 50, 50)); //设置背景颜色
		setlinestyle(PS_SOLID, 3);
		cleardevice();
		BeginBatchDraw(); //开始批量绘制

		int i, j;
		for (i = 0; i < 5; i++)
		{
			for (j = 0; j < 5; j++)
			{
				rounds[i][j].x = 100 + 100 * j;
				rounds[i][j].y = 200 + 100 * i;
				rounds[i][j].r = 30;
				rounds[i][j].angleNum = 1;
			}
		}
	}

	void show2() //绘制函数
	{
		cleardevice();
		TCHAR s[20]; //要输出的字符串
		setbkmode(TRANSPARENT); //设定字体模式为透明
		_stprintf(s, L"%d步    %d度", step, score); //把整数转化为字符串
		settextstyle(70, 0, L"微软雅黑");
		outtextxy(150, 30, s);
		settextstyle(20, 0, L"宋体");
		outtextxy(15, 100, L"点击一个圆圈 其指针顺时针旋转90度后 指向的指针依次旋转");
		int i, j;
		float angle;
		for (i = 0; i < 5; i++)
		{
			for (j = 0; j < 5; j++)
			{
				angle = rounds[i][j].angleNum * PI / 2; //角度等于角度类型*PI/2
				setlinecolor(RGB(200, 200, 200));
				circle(rounds[i][j].x, rounds[i][j].y, rounds[i][j].r);
				setlinecolor(RGB(255, 0, 0));
				line(rounds[i][j].x, rounds[i][j].y, rounds[i][j].x + rounds[i][j].r * cos(-angle), rounds[i][j].y + rounds[i][j].r * sin(-angle));
			}
		}
		FlushBatchDraw(); //结束批量绘制

	}
	//void updateWithoutInput() //与输入无关的更新
	//{

	//}

	void updateWithInput2() //与输入有关的更新
	{
		int i, j;
		MOUSEMSG msg; //定义鼠标信息
		char input;
		if (MouseHit()&&step>0)
		{
			msg = GetMouseMsg();
			if (msg.uMsg == WM_LBUTTONDOWN)
			{
				for (i = 0; i < 5; i++)
				{
					for (j = 0; j < 5; j++)
					{
						if (IsMouseHitInRound(msg.x, msg.y, i, j))
						{
							click_i = i;
							click_j = j;
						}
					}

				}
				int Indexes[2] = { click_i,click_j };
				if (click_i != 5 && click_j != 5)
				{
					RotateRound(click_i, click_j);
					step--;
					show2();
					Sleep(300);
					while (GetNextIndexes(Indexes))
					{
						RotateRound(Indexes[0], Indexes[1]);
						show2();
						Sleep(300);
					}
				}
			}
		}
		if (_kbhit())
		{
			input = _getch();
			if (input == VK_ESCAPE)
				message.vkcode = VK_ESCAPE;
		}
	}

	void TenStep()  //十步万度主函数(伪)
	{
		TCHAR answer[4]; //是否退出游戏的回答储存在这里
		LPTSTR pAnswer = answer; //pAnswer是指向answer的指针
		startup2();  //初始化函数,仅执行一次
		while (1)  //一直循环
		{
			show2(); //进行绘制
			//updateWithoutInput(); //与输入无关的更新
			updateWithInput2(); //与输入有关的更新
			if (message.vkcode == VK_ESCAPE)
			{
				message.vkcode = 0;
				InputBox(pAnswer, 4, L"确认退出吗?", L"十步万步", L"Yes", 0, 0, false);
				if (_tcscmp(pAnswer, L"Yes") == 0)
				{
					EndBatchDraw();
					closegraph();
					return;
				}
			}
		}
		return ;
	}
/*****************************十步万度函数区**************************************/
/*********************************************************************************
 此区块是为了实现c文件调用cpp函数而不出错
 *********************************************************************************/
#ifdef __cplusplus
#if __cplusplus
	}
#endif
#endif
