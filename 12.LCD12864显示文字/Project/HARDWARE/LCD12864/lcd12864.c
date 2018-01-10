#include "lcd12864.h"
#include <intrins.h>


///* 位变量声明 */
//sbit Busy = P2^7;               // 忙标志 BF 位 -> BF=H：内部正在执行操作;BF=L：空闲状态
//sbit Reset = P3^0;              // 复位

//sbit RS = P3^1;                 // 数据\命令选择端 -> 高电平：数据D0-D7将送入显示RAM；低电平：数据D0-D7将送入指令寄存器执行
//sbit E = P3^2;                  // 读写使能，高电平有效，下降沿锁定数据
//sbit RW = P3^3;                 // 读\写选择端 -> 高电平：读取数据；低电平：写入数据

//sbit CS1 = P3^4;                // 片选择信号: 低电平时选择前64列 左屏幕
//sbit CS5 = P3^5;                // 片选择信号: 低电平时选择后64列 右屏幕


/*************************************************************************
 Function    : LCD状态检测
 Description : 检测忙信号BF位
**************************************************************************/
void Check_Busy()
{
	E = 1;                             // 读写使能 -> 高电平有效，下降沿锁定数据
	RS = 0;                            // 数据\命令 选择 -> 高电平:数据  低电平:命令
	RW = 1;                            // 读\写 选择 -> 高电平：读；低电平：写
	LcdDataPort = 0xff;                // 送1到数据端口
	while(!Busy);                     // 判断BF位状态是否就绪 收到 1 跳出循环    BF=H：内部正在执行操作;BF=L：空闲状态
}




/*************************************************************************
 Function    : 写控制命令
 Description : NONE
**************************************************************************/
void Write_Command(u8 command)
{
	Check_Busy();                                   // 先检测状态
	
	E = 1;                                          // 读写使能
	RS = 0;                                         // 选择写命令
	RW = 0;                                         // 写入
	LcdDataPort = command;                          // 送入命令到端口P2
	E = 0;                                          // 读写使能:下降沿锁定数据
	_nop_();
}


/*************************************************************************
 Function    : 写显示数据
 Description : NONE
**************************************************************************/
void Write_Data(u8 dat)
{
	Check_Busy();                                   // 先检测状态
	
	E = 1;                                          // 读写使能
	RS = 1;                                         // 选择写数据
	RW = 0;                                         // 写入
	LcdDataPort = dat;                              // 送入数据到端口P2
	E = 0;                                          // 读写使能:下降沿锁定数据
	_nop_();
}


/*************************************************************************
 Function    : 屏幕选择
 Description : 0-全屏, 1-左屏, 2-右屏, 3-不选
**************************************************************************/
void Select_Screen(u8 screen)
{
	switch(screen)
	{
		case 0:
				CS1 = 0;                            //   CS1		CS2		  屏幕
				_nop_();                            //    
				CS2 = 0;                            //    0          0        全屏
				_nop_();                            //    0          1        左屏
				break;                             //    1          0        右屏
		                                            //    1          1        不选
		case 1:
				CS1 = 0;
				_nop_();
				CS2 = 1;
				_nop_();
				break;
		
		case 2:
				CS1 = 1;
				_nop_();
				CS2 = 0;
				_nop_();
				break;
		
		case 3:
				CS1 = 1;
				_nop_();
				CS2 = 1;
				_nop_();
				break;
	}
}


/*************************************************************************
 Function    : 设定显示位置
 Description : 
**************************************************************************/
void Set_Line(u8 line)                     //line -> 0 : 7
{
	line = line & 0x07;
	line = line | 0xb8;                    //1011 1xxx
	Write_Command(line);
}

void Set_Columm(u8 columm)                 //columm -> 0 :63
{
	columm = columm & 0x3f;
	columm = columm | 0x40;                //01xx xxxx
	Write_Command(columm);
}



/*************************************************************************
 Function    : 设置显示开始行
 Description : 
**************************************************************************/
void Set_StartLine(u8 startline)         //startline -> 0 : 63
{
	startline = startline & 0x3f;
	startline = startline | 0xc0;         //11xxxxxx
	Write_Command(startline);
}


/*************************************************************************
 Function    : 清屏
 Description : 0-全屏, 1-左屏, 2-右屏, 3-不选
**************************************************************************/
void Clear_Screen(u8 screen)   
{
	u8 x,y;
	Select_Screen(screen);
	for(x=0;x<8;x++)
	{
		Set_Line(x);
		Set_Columm(0);
		for(y=0;y<64;y++)
		{
			Write_Data(0);
		}
	}
	
}


/*************************************************************************
 Function    : 显示开关
 Description : 1-开显示 0-关显示
**************************************************************************/
void On_Off(u8 onoff)
{
	if(onoff == 1)
	{
		Write_Command(0x3f);          // 开显示 0011 1111
	}
	else Write_Command(0x3e);        // 关显示 0011 1110
}


/*************************************************************************
 Function    : 复位
 Description : 低电平有效
**************************************************************************/
void Lcd_Reset()
{
	Reset = 0;
	delay_ms(5);
	Reset = 1;
	delay_ms(5);
	On_Off(1);
}


/*************************************************************************
 Function    : 初始化
 Description : 
**************************************************************************/
void Lcd_Init()
{
	Lcd_Reset();                      // 复位
	On_Off(0);                        // 屏幕显示关
	Clear_Screen(0);                  // 清屏
	Set_Line(0);                      // 第一行
	Set_Columm(0);                    // 第一列
	Set_StartLine(0);                 // 第一列
	On_Off(1);                        // 屏幕显示开
}


/*************************************************************************
 Function    : 显示一个汉字16 * 16
 Description : 两行高度 每行宽16
**************************************************************************/
void Show_1616(u8 line,u8 columm,u8 *address)
{
	u8 i;
	Set_Line(line);                   // 第一行
	Set_Columm(columm);
	for(i=0;i<16;i++)                 // 宽度16
	{
		Write_Data(*(address++));
	}
	
	Set_Line(line+1);                 // 第二行
	Set_Columm(columm);
	for(i=0;i<16;i++)                 // 宽度16
	{
		Write_Data(*(address++));
	}
}

/*************************************************************************
 Function    : 显示一个汉字 24 * 24
 Description : 三行高度 每行宽24
**************************************************************************/
void Show_2424(u8 line,u8 columm,u8 *address)
{
	u8 i;
	Set_Line(line);                    // 第一行
	Set_Columm(columm);
	for(i=0;i<24;i++)                  // 宽度24
	{
		Write_Data(*(address++));
	}
	
	Set_Line(line+1);                  // 第二行
	Set_Columm(columm);
	for(i=0;i<24;i++)                  // 宽度24
	{
		Write_Data(*(address++));
	}
	
	Set_Line(line+2);                  // 第三行
	Set_Columm(columm);
	for(i=0;i<24;i++)                  // 宽度24
	{
		Write_Data(*(address++));
	}
}


/*************************************************************************
 Function    : 显示一个字母16 * 8
 Description : 两行高度 每行宽8
**************************************************************************/
void Show_1608(u8 line,u8 columm,u8 *address)
{
	u8 i;
	Set_Line(line);                    // 第一行
	Set_Columm(columm);
	for(i=0;i<8;i++)                  // 宽度08
	{
		Write_Data(*(address++));
	}
	
	Set_Line(line+1);                  // 第一行
	Set_Columm(columm);
	for(i=0;i<8;i++)                  // 宽度08
	{
		Write_Data(*(address++));
	}
}

/*************************************************************************
 Function    : 延时函数
 Description : 非精确延时
**************************************************************************/
void delay_ms(u16 xms)
{
	u16 x,y;
	for(x=xms;x>0;x--)
	{
		for(y=125;y>0;y--);
	}
}



