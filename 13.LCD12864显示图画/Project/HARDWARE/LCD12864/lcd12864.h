#ifndef __LCD12864_H
#define __LCD12864_H
#include <reg52.h>

/* 宏定义 */
#define LcdDataPort P2          // 数据端口 P2

/* 定义常用变量类型 */
#ifndef __UCHAR_DEF
#define __UCHAR_DEF
typedef unsigned char u8;
typedef unsigned int u16;
#endif


/* 位变量声明 */
sbit Busy = P2^7;               // 忙标志 BF 位 -> BF=H：内部正在执行操作;BF=L：空闲状态
sbit Reset = P3^0;              // 复位

sbit RS = P3^1;                 // 数据\命令选择端 -> 高电平：数据D0-D7将送入显示RAM；低电平：数据D0-D7将送入指令寄存器执行
sbit E = P3^2;                  // 读写使能，高电平有效，下降沿锁定数据
sbit RW = P3^3;                 // 读\写选择端 -> 高电平：读取数据；低电平：写入数据

sbit CS1 = P3^4;                // 片选择信号: 低电平时选择前64列 左屏幕
sbit CS2 = P3^5;                // 片选择信号: 低电平时选择后64列 右屏幕



/* 函数*/
void delay_ms(u16 xms);                                // 延时函数
void Check_Busy();                                     // LCD状态检测
void Write_Command(u8 command);                        // 写命令
void Write_Data(u8 dat);                               // 写显示数据
void Select_Screen(u8 screen);                         // 选择屏幕
void Set_Line(u8 line);                                // 设置显示行
void Set_Columm(u8 columm);                            // 设置显示列
void Set_StartLine(u8 startline);                      // 设置显示开始行
void Clear_Screen(u8 screen);                          // 清屏
void On_Off(u8 onoff);                                 // 设置显示开关
void Lcd_Reset();                                      // 复位

void Show_6464(u8 line,u8 columm,u8 *address);         // 显示一个64*64图画


void Lcd_Init();                                       // 初始化

#endif




