#include <windows.h>
#include <stdio.h>
#include <strsafe.h>
/*************************************************************************
console_settings.c包含各种修改控制台设置的函数:窗口化,全屏,修改标题,全屏等
*************************************************************************/
/*函数功能:修改局部颜色
  函数入口参数:颜色代号
  函数返回值:无 */
void set_color(UINT uFore,UINT uBack)
{
 HANDLE handle=GetStdHandle(STD_OUTPUT_HANDLE);
 SetConsoleTextAttribute(handle,uFore+uBack*0x10);
}
   
/*函数功能:修改标题
  函数入口参数: 字符串常量 
  函数返回值:无 */
void set_title(LPCWSTR lpTitle) 
{
 SetConsoleTitle(lpTitle);
}
/*函数功能:全屏显示,取消标题栏及边框 
  函数入口参数:无
  函数返回值:无 */
void full_screen()
{   
    HWND hwnd = GetForegroundWindow();
    int cx = GetSystemMetrics(SM_CXSCREEN);            /* 屏幕宽度 像素 */
    int cy = GetSystemMetrics(SM_CYSCREEN);            /* 屏幕高度 像素 */

    LONG_PTR l_WinStyle = GetWindowLongPtr(hwnd,GWL_STYLE);   /* 获取窗口信息 */
    /* 设置窗口信息 最大化 取消标题栏及边框 */
    SetWindowLongPtr(hwnd,GWL_STYLE,(l_WinStyle | WS_POPUP | WS_MAXIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
    SetWindowPos(hwnd, HWND_TOP, 0, 0, cx, cy, 0);
}
/*函数功能:隐藏指定滚动条 
  函数入口参数:无
  函数返回值:无*/
void hide_scroll_bar()
{
	HWND hwnd;
	hwnd = GetForegroundWindow();
	ShowScrollBar(hwnd,SB_VERT,FALSE);  //FALSE为隐藏,TRUE为显示 
}
/*函数功能:窗口化
  函数入口参数:1表示大窗口,2表示小窗口 
  函数返回值:无 */
  //void window_minimize() 
//{
//	HWND hwnd = GetForegroundWindow();
 //   int cx = GetSystemMetrics(SM_CXSCREEN);            /* 屏幕宽度 像素 */
   // int cy = GetSystemMetrics(SM_CYSCREEN);            /* 屏幕高度 像素 */
    //LONG l_WinStyle = GetWindowLongPtr(hwnd,GWL_STYLE);   /* 获取窗口信息 */
     /*设置窗口信息,回到最初的最小化 */
    // SetWindowLongPtr(hwnd,GWL_STYLE,(l_WinStyle | WS_POPUP | WS_MINIMIZE) & ~WS_CAPTION & ~WS_THICKFRAME & ~WS_BORDER);
     //SetWindowPos(hwnd, HWND_TOP, 70, 30, cx, cy, SWP_SHOWWINDOW);
//}
/*函数功能:指定文字输出坐标
  函数入口参数:坐标
  函数返回值:无 */
 void set_posC(COORD a) 
 { 
 HANDLE out=GetStdHandle(STD_OUTPUT_HANDLE);
 SetConsoleCursorPosition(out, a);
 } 
   
/*函数功能:指定文字输出坐标
  函数入口参数:x:表示横向坐标
  			   y:表示纵向坐标
				 单位:字符长宽
  函数返回值:无 */
 void set_pos(int x, int y) 
 { 
 COORD pos= {x, y};
 set_posC(pos);
 } 
/*函数功能:隐藏光标
  函数入口参数:无
  函数返回值:无 */
 void hide_console_cursor(VOID) 
 {
 CONSOLE_CURSOR_INFO cursor_info = {1, 0};
 SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursor_info);
}
/*函数功能:修改字体大小
  函数入口参数:x:字体宽度
  			   y:字体高度
  函数返回值:无 */
 void set_font_size(int x,int y)
 { 
  CONSOLE_FONT_INFOEX cfi;
cfi.cbSize = sizeof(cfi);
cfi.nFont = 0;
cfi.dwFontSize.X = x;                   // 字体宽度 
cfi.dwFontSize.Y = y;                  // 字体高度 
cfi.FontFamily = FF_DONTCARE;
cfi.FontWeight = FW_NORMAL;
wcscpy_s(cfi.FaceName,19, L"Microsoft YaHei UI"); // Choose your 字体 
SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);					  
}















  
