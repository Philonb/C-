#define _CRT_SECURE_NO_WARNINGS true
/*************************************************************** 
  gameboard.c主要包含窗口字体背景颜色初始化，打印游戏名单
***************************************************************/ 
#include "console_settings.h"
#include <stdio.h>
#include "menu.h"
/*函数功能:背景和字体颜色及标题的初始化,默认全屏显示,禁用滚动条,隐藏光标 
  函数入口参数:颜色的代号 
  函数返回值:无 */
void init()
{ 
	system("color F0");
	set_title("menu");
	full_screen();
    hide_scroll_bar();
    hide_console_cursor();
    set_font_size(15,30);  //初始化字体 
}
/*函数功能:打印菜单 
  函数入口参数:无
  函数返回值:玩家的选择 */
 
char menu(void)
{
	char choose;
	set_color(4, 15);
	set_pos(15, 8);
	printf("	   ********************************************\n\
	           *       程序名称:控制台小游戏启动器        *\n\
	           *       版本信息:1.00                      *\n\
	           *       版权说明:此程序为Philo所有         *\n\
	           ********************************************\n");
	set_pos(31, 29);
	hide_scroll_bar();
	system("pause");
	system("cls");
	set_color(1, 15);
	set_pos(30, 15);  //该坐标值为屏幕居中位置 
	printf("欢迎游玩控制台小游戏!\n");
	set_color(0, 15);
	set_pos(31, 29); //该坐标值为屏幕底部居中位置 
	hide_scroll_bar();  //指定坐标打印将导致滚动条显示出来,需重新隐藏
	system("pause");
	system("cls");
	while (1)
	{
		set_pos(12, 8);
		printf("**************************************************************\n\
	    **     1:进入游戏选单   2:帮助   3:使用说明     4:设置      **\n\
	    **************************************************************\n");
		printf("            请选择:");
		scanf(" %c", &choose);
		switch (choose)
		{
		case '1':case '2':case '3':case'4':
			return choose;
		default:
			error();
			break;
		}
	}
}
/*函数功能:打印游戏单
  函数入口参数:无
  函数返回值:玩家的选择*/
char gameboard()
{
	system("cls");
	char game;
	set_pos(8, 1);
	printf("1:模拟小球平抛\n\
	2:跳跃球");
	set_pos(35, 29);
	printf("q:退出");
	set_pos(8, 0);
	printf("请选择:");
	scanf(" %c", &game);
	switch (game)
	{
	case '1':case '2':case 'q':
		return game;
	default:
		error();
		break;
	}
}
/*函数功能:打印输入错误信息
  函数入口参数:无
  返回值:无*/
void error()
{
	system("cls");
	set_pos(30, 15);
	printf("输入出错了呢");
	set_pos(30, 16);
	system("pause");
	system("cls");
	getchar();
}
