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
	set_title(L"menu");
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
	system("cls");
	char choose;
	set_color(4, 15);
	set_pos(15, 8);
	printf("		********************************************\n\
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
	printf("	  欢迎游玩控制台小游戏!\n");
	set_color(0, 15);
	set_pos(31, 29); //该坐标值为屏幕底部居中位置 
	hide_scroll_bar();  //指定坐标打印将导致滚动条显示出来,需重新隐藏
	system("pause");
	system("cls");
	while (1)
	{
		system("cls");
		set_pos(19, 8);
		printf("**************************************************************\n\
		   **     1:进入游戏选单        2:帮助		3:关于         **\n\
		   **************************************************************\n");
		printf("		   请选择:");
		scanf(" %c", &choose);
		while (getchar() != '\n');
		switch (choose)
		{
		case '1':
			return choose;
		case '2':
			help();
			break;
		case '3':
			about();
			break;
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
	printf("	1:模拟小球平抛\n\
		2:跳跃球\n\
		3:旋转蛇错觉\n\
		4:移动方块错觉\n\
		5:见缝插针\n\
		6:见缝插圆\n\
		7,贪吃蛇\n\
		8,十步万度");
	set_pos(35, 25);
	printf("	q:退出");
	set_pos(8, 0);
	printf("	请选择:");
	scanf(" %c", &game);
	while (getchar()!= '\n');
		return game;
}
/*函数功能:打印输入错误信息
  函数入口参数:无
  返回值:无*/
void error()
{
	system("cls");
	set_pos(30, 15);
	printf("	输入出错了呢");
	set_pos(30, 16);
	system("pause");
	system("cls");
}
/*函数功能:打印帮助内容
  函数入口参数:无
  函数返回值:无*/
void help()
{
	system("cls");
	set_pos(8, 8);
	printf("	功能说明:本程序一共由八个游戏构成,有些游戏未制作操作说明。\n\
		特此说明:旋转蛇按空格更改样式\n\
		见缝插针按空格或0发射针\n\
		见缝插圆按空格清屏,1,2，3,4更改圆的样式\n");
	set_pos(8, 22);
	printf("	使用说明:控制台按F11可退出全屏\n\
		选游菜单处若输入一个高于一位的数字,将进入第一位数字所代表的游戏。\n\
		所有游戏都禁用了窗口关闭,但每个都设计了按ESC退出的功能。\n\
		跳跃球增加了文件读写,按P可以看到三个难度下的最高分数排行榜,并且记录了时间。\n\
		退出排行榜只能在hard榜单按ESC,即按3下P再按ESC\n\
		为了更好的显示效果请使用苹方-简字体");

	set_pos(15, 29);
	system("pause");
		return;
}
/*函数功能:关于
  函数入口参数:无
  函数返回值:无*/
void about()
{
	system("cls");
	set_pos(8, 0);
	printf("	关于:\n\
		程序名称:C语言小游戏合集\n\
		程序开发时间:2022.1.01-2022.1.06\n\
		版本信息:1.0.0\n\
		关于开发者:\n\
			姓名:彭武倜\n\
			学号:8008121028\n\
			专业:计算机II类\n\
			班级:2101班\n\
			邮箱:Philo_M@outlook.com\n\
				 edwainmoma3@gmail.com\n\
			个人博客:www.philoblog.top\n");
	_getch();
	return;
}
