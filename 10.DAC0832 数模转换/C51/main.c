#include <reg52.h>


/*************************************************************************
- Function    : D/A
- Description : DA控制二极管亮度:LED由灭变为最亮,在最暗和最亮时,蜂鸣器响一声
- Hardware    : WR - P3.6  CS - P3.2  蜂鸣器 - P1.0
- Author      : Y.
- Date        : 2017.12
 *************************************************************************/
 

/* 定义常用变量类型 */
typedef unsigned char u8;
typedef unsigned int u16;

/* 声明位变量 */
sbit dawr = P3^6;                            // 定义DA的WR端口
sbit dacs = P3^2;                            // 定义DA的CS端口
sbit beep = P1^0;                            // 蜂鸣器

/* 变量 */
u8 val,flag;

/* 声明子函数 */
void delay(u16);

/*************************************************************************
 Function    : 主程序
 Description : None
**************************************************************************/							 
void main()
{
	dawr = 0;                       // 打开寄存器输入端 D0 - D7 
	dacs = 0;                       // 打开片选信号输入端
	P2 = 0x00;                      // 初始化
	flag = 0;
	val = 0;
	beep = 0;                       // 初始化蜂鸣器端口
	
	while(1)
	{
		if(flag == 0)
		{
			val += 5;
			P2 = val;
			
			if(val == 255)
			{
				beep = 1;
				delay(1000);
				beep = 0;
				flag = 1;
			}
			delay(100);
		}

		else
		{
			val -= 5;
			P2 = val;
			
			if(val == 0)
			{
				beep = 1;
				delay(1000);
				beep = 0;
				flag = 0;
			}
			delay(100);
		}

	}

} 


/*************************************************************************
 Function    : 延时子程序
 Description : 延时X毫秒ms
**************************************************************************/
void delay(u16 xms)
{
	  u16 x,y;
    for(x=xms;x>0;x--)
		{
		    for(y=125;y>0;y--);
		}
}