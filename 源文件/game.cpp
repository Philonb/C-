#define _CRT_SECURE_NO_WARNINGS true
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "game.h"
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
/*函数功能:跳跃球游戏本体
  函数入口参数:void
  返回参数:无*/
	void JumpBall()
	{
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
					rect_vx = rand() / float(RAND_MAX) * 3 - 9;
				else
					rect_vx = rand() / float(RAND_MAX) * 7 - 12;
			}

			if ((rect_left_x <= ball_x + radius)
				&& (rect_left_x + rect_width >= ball_x - radius)
				&& (rect_top_y <= ball_y + radius))
			{
				score = 0;
				rect_left_x = width;
				rect_height = rand() % (int(height) / 4) + height / 4;
				rect_vx = rand() / float(RAND_MAX) * 4 - 7;
				Sleep(500);
			}

			cleardevice();
			fillcircle(ball_x, ball_y, radius);
			fillrectangle(rect_left_x, height - rect_height, rect_left_x + rect_width, height);
			TCHAR s[20];
			TCHAR info[6] = L"按空格跳跃";
			_stprintf(s, _T("%d"), score); //将score转换为字符串
			settextstyle(40, 0, _T("微软雅黑"));
			outtextxy(50, 30, s);//输出得分分数
			settextstyle(20, 0, L"微软雅黑");
			outtextxy(30, 80, info);
			Sleep(10);
			if (pMessage->vkcode == VK_ESCAPE)
			{
				InputBox(pAnswer, 7, L"你想退出吗(Yes or no)?或者你想改变难度(输入easy等)", L"跳跃球", L"Yes", 0, 0, false);
				if (_tcscmp(pAnswer, L"Yes") == 0)
				{
					break;
				}
				else if (_tcscmp(pAnswer, _T("easy")) == 0
					|| _tcscmp(pAnswer, _T("normal")) == 0
					|| _tcscmp(pAnswer, _T("hard")) == 0)
				{
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
		initgraph(750, 500); // 新开画面
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
/*********************************************************************************
 此区块是为了实现c文件调用cpp函数而不出错
 *********************************************************************************/
#ifdef __cplusplus
#if __cplusplus
	}
#endif
#endif
