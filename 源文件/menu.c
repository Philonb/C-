#define _CRT_SECURE_NO_WARNINGS true
/*************************************************************** 
  gameboard.c��Ҫ�����������屳����ɫ��ʼ������ӡ��Ϸ����
***************************************************************/ 
#include "console_settings.h"
#include <stdio.h>
#include "menu.h"
/*��������:������������ɫ������ĳ�ʼ��,Ĭ��ȫ����ʾ,���ù�����,���ع�� 
  ������ڲ���:��ɫ�Ĵ��� 
  ��������ֵ:�� */
void init()
{ 
	system("color F0");
	set_title("menu");
	full_screen();
    hide_scroll_bar();
    hide_console_cursor();
    set_font_size(15,30);  //��ʼ������ 
}
/*��������:��ӡ�˵� 
  ������ڲ���:��
  ��������ֵ:��ҵ�ѡ�� */
 
char menu(void)
{
	char choose;
	set_color(4, 15);
	set_pos(15, 8);
	printf("	   ********************************************\n\
	           *       ��������:����̨С��Ϸ������        *\n\
	           *       �汾��Ϣ:1.00                      *\n\
	           *       ��Ȩ˵��:�˳���ΪPhilo����         *\n\
	           ********************************************\n");
	set_pos(31, 29);
	hide_scroll_bar();
	system("pause");
	system("cls");
	set_color(1, 15);
	set_pos(30, 15);  //������ֵΪ��Ļ����λ�� 
	printf("��ӭ�������̨С��Ϸ!\n");
	set_color(0, 15);
	set_pos(31, 29); //������ֵΪ��Ļ�ײ�����λ�� 
	hide_scroll_bar();  //ָ�������ӡ�����¹�������ʾ����,����������
	system("pause");
	system("cls");
	while (1)
	{
		set_pos(12, 8);
		printf("**************************************************************\n\
	    **     1:������Ϸѡ��   2:����   3:ʹ��˵��     4:����      **\n\
	    **************************************************************\n");
		printf("            ��ѡ��:");
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
/*��������:��ӡ��Ϸ��
  ������ڲ���:��
  ��������ֵ:��ҵ�ѡ��*/
char gameboard()
{
	system("cls");
	char game;
	set_pos(8, 1);
	printf("1:ģ��С��ƽ��\n\
	2:��Ծ��\n\
	3:��ת�ߴ��\n\
	4:�ƶ�������\n\
	5:�������");
	set_pos(35, 29);
	printf("q:�˳�");
	set_pos(8, 0);
	printf("��ѡ��:");
	scanf(" %c", &game);
		return game;
}
/*��������:��ӡ���������Ϣ
  ������ڲ���:��
  ����ֵ:��*/
void error()
{
	system("cls");
	set_pos(30, 15);
	printf("�����������");
	set_pos(30, 16);
	system("pause");
	system("cls");
	getchar();
}
