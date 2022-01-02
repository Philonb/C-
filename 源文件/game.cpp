#define _CRT_SECURE_NO_WARNINGS true
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
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
		initgraph(600, 600);  //初始化游戏窗口画面,宽600，高600
		do
		{
			ax = -0.015;
			g = 0.5;
			*pRestart = 0; //初始化Restart为0
			y = 20; //初始化y坐标
			vy = 0;  //初始化下落速度为0
			x = 20;  //初始化x坐标
			InputBox(pVx, 4, L"请输入向左抛的速度(最大为999)(单位像素点)", NULL, L"10", 0, 0, true);  //弹出提示窗口,true表示禁止取消按键
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
				peekmessage(msg, EM_KEY, true); //捕捉运动时按键消息
				if (msg->vkcode == VK_ESCAPE)  //当按下的按键是ESC时
				{
					InputBox(pString, 4, L"确认退出吗?", NULL, L"Yes", 0, 0, false); //调出对话框,回答默认是Yes
					flushmessage(EM_KEY); //清空消息缓冲区
					if (_tcscmp(pString, L"Yes") == 0) //当玩家的输入为Yes时
						break;
					else
						msg->vkcode = 0; //将接受按键消息的内存更改为0
				}
				if (vx == 0 && vy == 0 &&y==580) //当x方向，y方向速度都为0,且小球位于地面
				{
					Sleep(500);  //暂停500毫秒
					InputBox(pRestart, 4, L"是否重新开始", NULL, L"Yes", 0, 0, false); 
					flushmessage(EM_KEY);
					break;
				}
			}
		} while (_tcscmp(pRestart, L"Yes") == 0);  //当玩家的回答是否重新开始时输入Yes
		_getch(); //等待按键
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
		initgraph(width, height);
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

		TCHAR Difficulty[7] = _T("easy");
		TCHAR Answer[7] = L"Yes";
		ExMessage Message;
		ExMessage* pMessage = &Message;
		LPTSTR pDifficulty = Difficulty, pAnswer = Answer;

		do {
			InputBox(pDifficulty, 7, _T("请选择难易度(easy,normal,hard)"), NULL, _T("easy"), 0, 0, true);
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
			peekmessage(pMessage, EM_KEY, true);
			if (pMessage->vkcode == VK_ESCAPE)
			{
				InputBox(pAnswer, 7, L"你想退出吗(Yes or no)?或者你想改变难度(输入easy等)", NULL, L"Yes", 0, 0, false);
				if (_tcscmp(pAnswer, L"Yes") == 0)
				{
					closegraph();
					return;
				}
				else if (_tcscmp(pDifficulty, _T("easy")) == 0
					|| _tcscmp(pDifficulty, _T("normal")) == 0
					|| _tcscmp(pDifficulty, _T("hard")) == 0)
				{
					score = 0;
					_tcscpy(pDifficulty, pAnswer);
				}
				pMessage->vkcode = 0;
			}

		}
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
