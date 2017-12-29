#include <reg52.h>


/*************************************************************************
- Function    : 按键实验
- Description : 按一下key1 数码管显示数字加1（0-F） 按键状态指示灯led1亮 
- Hardware    : key1 - P3.0  led1 - P1.0  位选 - P2.7  段选 - P2.6
- Author      : Y.
- Date        : 2017.12
 *************************************************************************/

 /* 定义常用变量类型 */
#define u8 unsigned char
#define u16 unsigned int
	
/* 位变量声明 */
sbit led1 = P1^0;                                   // 按键状态指示灯
sbit key1 = P3^0;                                   // 按键1
sbit key2 = P3^1;
sbit key3 = P3^2;
sbit key4 = P3^3;
sbit dula = P2^6;                                   // 段选
sbit wela = P2^7;                                   // 位选

/* 定义全局变量 */
u8 code table[] = {0x3f,0x06,0x5b,0x4f,             // 共阴数码管码表
                   0x66,0x6d,0x7d,0x07,
                   0x7f,0x6f,0x77,0x7c,
                   0x39,0x5e,0x79,0x71};
u8 num;
				   
/* 子函数声明 */	
void delay(u16);


/*************************************************************************
 Function    : 主程序
 Description : None 
**************************************************************************/				   
void main()
{
	led1 = 1;
	wela = 1;
	P0 = 0xfe;
	wela = 0;
	while(1)
	{
		if(key1 == 0)
		{
			delay(10);                              // 按键消抖
			if(key1 == 0)
			{
				led1 = 0;
				num++;                              // 显示数据加1
				if(num == 16)	
				{
					num = 0;
				}
			}
						
			while(!key1);                           // 检测是否松开按键
			delay(10);                               // 消抖
			while(!key1);
		}
		else led1 = 1;
				
		dula = 1;
		P0 = table[num];                             // 送入显示数据
		dula = 0;
				
		}
}


/*************************************************************************
 Function    : 延时子程序
 Description : 延时 x 毫秒(非精确延时)
**************************************************************************/
void delay(u16 xms)
{
	u16 x,y;
	for(x=xms;x>0;x--)
	{
		for(y=125;y>0;y--);
	}
}
