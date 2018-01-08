#include <reg52.h>
#include <intrins.h>


/*************************************************************************
- Function    : A/D转换实验  ADC0804
- Description : 数码管显示电位器数值
- Hardware    : CS - P3.5   WR - P3.6  RD - P3.7
- Author      : Y.
- Date        : 2017.12
 *************************************************************************/
 

/* 定义常用变量类型 */
typedef unsigned char u8;
typedef unsigned int u16;


/* 申明位变量 */
sbit dula = P2^6;                                   // 申明U2锁存器锁存端 数码管段选
sbit wela = P2^7;                                   // 申明U3锁存器锁存端 数码管位选
sbit adcs = P3^5;                                   // ADC0804 片选信号输入端(低电平有效)
sbit adwr = P3^6;                                   // ADC0804 写信号
sbit adrd = P3^7;                                   // ADC0804 读信号


/* 申请全局变量 */
u8 code table[] = {0x3f,0x06,0x5b,0x4f,             // 共阴数码管 码表
                   0x66,0x6d,0x7d,0x07,
                   0x7f,0x6f,0x77,0x7c,
                   0x39,0x5e,0x79,0x71};


/* 声明子函数 */		
void init();									     // 初始化函数
void delay(u16);                                    // 延时函数
void display(u8,u8,u8);                             // 数码管显示函数

/*************************************************************************
 Function    : 主程序
 Description : None
**************************************************************************/							 
void main()
{
	init();
	adcs = 0;                                     // 启动ADC0804
	while(1)
	{
		u8 adval,A1,A2,A3,a;  
		
		/* 写信号 完成一个低电平脉冲周期 */
		adwr = 1;
		_nop_();
		adwr = 0;                                 // 写信号 低电平启动转换
		_nop_();
		adwr = 1;

		for(a=10;a>0;a--)
		{
			display(A1,A2,A3);
		}

		P1 = 0xFF;                                 // 初始化
		
		/* 读信号 完成一个低电平脉冲周期 */
		adrd = 1;
		_nop_();
		adrd = 0;                                  // 低电平有效
		_nop_();
		adval = P1;                                // 读信号 读取ADC转换后数值
		adrd = 1;
		
		/* 分离转换后数值个十百位 */
		A1 = adval/100;
		A2 = adval%100/10;
		A3 = adval%10;

	}
}



/*************************************************************************
 Function    : 初始化程序
 Description : None
**************************************************************************/
void init()
{
	dula = 1;
	P0 = 0x00;                                     // 初始化 关闭所有段选
	dula = 0;

	wela = 1;
	P0 = 0x00;                                     // 初始化 位选8个数码管
	wela = 0;
	
}



/*************************************************************************
 Function    : 延时子程序
 Description : 延时X毫秒ms
**************************************************************************/
void delay(u16 xms)
{
	u16 x,y;
	for (x=xms;x>0;x--)
	{
		for (y=125;y>0;y--);
	}
}



/*************************************************************************
 Function    : 显示函数
 Description : None
**************************************************************************/
void display(u8 bai,u8 shi,u8 ge)
{
	dula = 1;
	P0 = table[bai];
	dula = 0;

	wela = 1;
	P0 = 0xfe;
	wela = 0;

	delay(5);
	P0 = 0x00;

	dula = 1;
	P0 = table[shi];
	dula = 0;

	wela = 1;
	P0 = 0xfd;
	wela = 0;

	delay(5);
	P0 = 0x00;
	 
	dula = 1;
	P0 = table[ge];
	dula = 0;

	wela = 1;
	P0 = 0xfb;
	wela = 0;

	delay(5);
	P0 = 0x00;
}

