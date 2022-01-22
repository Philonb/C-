#include <windows.h>
#include <stdio.h>
#include <strsafe.h>
/*************************************************************************
console_settings.c���������޸Ŀ���̨���õĺ���:���ڻ�,ȫ��,�޸ı���,ȫ����
*************************************************************************/
/*��������:�޸ľֲ���ɫ
  ������ڲ���:��ɫ����
  ��������ֵ:�� */
void set_color(UINT uFore,UINT uBack)
{
 HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
 SetConsoleTextAttribute(handle,uFore+uBack*0x10);
}
/*��������:�޸ı���
  ������ڲ���: �ַ������� 
  ��������ֵ:�� */
void set_title(LPCWSTR lpTitle) 
{
 SetConsoleTitle(lpTitle);
}
/*��������:ȫ����ʾ,ȡ�����������߿� 
  ������ڲ���:��
  ��������ֵ:�� */
void full_screen()
{   
    HWND hwnd = GetForegroundWindow();
    int cx = GetSystemMetrics(SM_CXSCREEN);            /* ��Ļ��� ���� */
    int cy = GetSystemMetrics(SM_CYSCREEN);            /* ��Ļ�߶� ���� */

    LONG_PTR l_WinStyle = GetWindowLongPtr(hwnd,GWL_STYLE);   /* ��ȡ������Ϣ */
    /* ���ô�����Ϣ ��� ȡ�����������߿� */
    SetWindowLongPtr(hwnd,GWL_STYLE,(l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
    SetWindowPos(hwnd, HWND_TOP, 0, 0, cx, cy, 0);
}
/*��������:����ָ�������� 
  ������ڲ���:��
  ��������ֵ:��*/
void hide_scroll_bar()
{
	HWND hwnd;
	hwnd = GetForegroundWindow();
	ShowScrollBar(hwnd,SB_VERT,FALSE);  //FALSEΪ����,TRUEΪ��ʾ 
}
/*��������:���ڻ�
  ������ڲ���:1��ʾ�󴰿�,2��ʾС���� 
  ��������ֵ:�� */
  //void window_minimize() 
//{
//	HWND hwnd = GetForegroundWindow();
 //   int cx = GetSystemMetrics(SM_CXSCREEN);            /* ��Ļ��� ���� */
   // int cy = GetSystemMetrics(SM_CYSCREEN);            /* ��Ļ�߶� ���� */
    //LONG l_WinStyle = GetWindowLongPtr(hwnd,GWL_STYLE);   /* ��ȡ������Ϣ */
     /*���ô�����Ϣ,�ص��������С�� */
    // SetWindowLongPtr(hwnd,GWL_STYLE,(l_WinStyle | WS_POPUP | WS_MINIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
     //SetWindowPos(hwnd, HWND_TOP, 70, 30, cx, cy, SWP_SHOWWINDOW);
//}
/*��������:ָ�������������
  ������ڲ���:����
  ��������ֵ:�� */
 void set_posC(COORD a) 
 { 
 HANDLE out=GetStdHandle(STD_OUTPUT_HANDLE);
 SetConsoleCursorPosition(out, a);
 } 
   
/*��������:ָ�������������
  ������ڲ���:x:��ʾ��������
  			   y:��ʾ��������
				 ��λ:�ַ�����
  ��������ֵ:�� */
 void set_pos(int x, int y) 
 { 
 COORD pos= {x, y};
 set_posC(pos);
 } 
/*��������:���ع��
  ������ڲ���:��
  ��������ֵ:�� */
 void hide_console_cursor() 
 {
 CONSOLE_CURSOR_INFO cursor_info = {1, 0};
 SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
/*��������:�޸������С
  ������ڲ���:x:������
  			   y:����߶�
  ��������ֵ:�� */
 void set_font_size(int x,int y)
 { 
  CONSOLE_FONT_INFOEX cfi;
cfi.cbSize = sizeof(cfi);
cfi.nFont = 0;
cfi.dwFontSize.X = x;                   // ������ 
cfi.dwFontSize.Y = y;                  // ����߶� 
cfi.FontFamily = FF_DONTCARE;
cfi.FontWeight = FW_NORMAL;
wcscpy_s(cfi.FaceName,19, L"ƻ��-��"); // Choose your ���� ,�˴������޸�������ʽ,��ƻ��-��ĳ���Ҫ�õ��������Ƽ���
SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);					  
}















  
