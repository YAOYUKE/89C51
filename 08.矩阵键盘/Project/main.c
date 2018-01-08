#include <reg52.h>


/*************************************************************************
- Function    : 矩阵键盘实验
- Description : 按下矩阵键盘key1-key15 数码管显示0-F
- Hardware    : 位选 - P2.7  段选 - P2.6
- Author      : Y.
- Date        : 2017.12
 *************************************************************************/
 
 
/* 定义常用变量类型 */
typedef unsigned char u8;
typedef unsigned int u16;

/* 申明位变量 */
sbit dula = P2^6;                // 申明U2锁存器锁存端 数码管段选
sbit wela = P2^7;                // 申明U3锁存器锁存端 数码管位选

/* 申请全局变量 */
u8 num;

u8 code table[] = {0x3f,0x06,0x5b,0x4f,      // 共阴数码管 码表
                   0x66,0x6d,0x7d,0x07,
                   0x7f,0x6f,0x77,0x7c,
                   0x39,0x5e,0x79,0x71};

/* 声明子函数 */		
void init();									   // 初始化函数
void delay(u16);                 // 延时函数
void desplay(u8);                // 数码管显示函数
void keyscan();                  // 键盘扫描函数
				   
/*************************************************************************
 Function    : 主程序
 Description : None 
**************************************************************************/							 
void main()
{
	init();
	while (1)               
	{
		keyscan();              // 不停调用键盘扫描程序
	}
}


/*************************************************************************
 Function    : 延时子程序
 Description : 延时X毫秒ms
**************************************************************************/
void delay(u16 xms)
{
	u16 x,y;
	for (x = xms; x > 0; x--)
	{
		for (y = 125; y > 0; y--);
	}
}


/*************************************************************************
 Function    : 初始化程序
 Description : None
**************************************************************************/
void init()
{
	dula = 1;
	P0 = 0x00;                     // 初始化 关闭所有段选
	dula = 0;
	
	wela = 1;
	P0 = 0x00;                     // 位选8个数码管
	wela = 0;
}


/*************************************************************************
 Function    : 显示函数
 Description : None
**************************************************************************/
void display(u8 num)
{
	dula = 1;
	P0 = table[num];               // 显示函数只送段选数据
	dula = 0;
}

/*************************************************************************
 Function    : 矩阵键盘扫描函数
 Description : 行扫描
**************************************************************************/
void keyscan()
{
	u8 temp,key;
	
	/* 扫描键盘第一行 */
	P3 = 0xfe;
	temp = P3;
	temp = temp & 0xf0;
	
	if(temp != 0xf0)
	{
		delay(10);
		temp = P3;
		temp = temp & 0xf0;
			
		if(temp != 0xf0)
		{
			temp = P3;
					
			switch(temp)
			{
				case 0xee: key = 0; break;
				case 0xde: key = 1; break;
				case 0xbe: key = 2; break;
				case 0x7e: key = 3; break;
			}
						
			while(temp != 0xf0)                     // 等待按钮释放
			{
				temp = P3;
				temp = temp & 0xf0;
			}
						
			display(key);                          // 显示数据
	
		}
	}
		
	/* 扫描键盘第二行 */  
	P3 = 0xfd;
	temp = P3;
	temp = temp & 0xf0;
		
	if(temp != 0xf0)
	{
		delay(10);
		temp = P3;
		temp = temp & 0xf0;
			
		if(temp != 0xf0)
		{
		temp = P3;
					
			switch(temp)
			{
				case 0xed: key = 4; break;
				case 0xdd: key = 5; break;
				case 0xbd: key = 6; break;
				case 0x7d: key = 7; break;
			}
						
			while(temp != 0xf0)                  
			{
				temp = P3;
				temp = temp & 0xf0;
			}
						
			display(key);
		}
	}		

	
	/* 扫描键盘第三行 */		
    P3 = 0xfb;
    temp = P3;
    temp = temp & 0xf0;
		
    if(temp != 0xf0)
	{
		delay(10);
		temp = P3;
		temp = temp & 0xf0;
			
		if(temp != 0xf0)
		{
			temp = P3;
					
			switch(temp)
			{
				case 0xeb: key = 8; break;
				case 0xdb: key = 9; break;
				case 0xbb: key = 10; break;
				case 0x7b: key = 11; break;
			}
						
			while(temp != 0xf0)
			{
				temp = P3;
				temp = temp & 0xf0;
			}
						
			display(key);
			}
	}	

	/* 扫描键盘第四行 */		
    P3 = 0xf7;
    temp = P3;
    temp = temp & 0xf0;
		
    if(temp != 0xf0)
	{
		delay(10);
		temp = P3;
		temp = temp & 0xf0;
			
		if(temp != 0xf0)
		{
			temp = P3;
					
			switch(temp)
			{
				case 0xe7: key = 12; break;
				case 0xd7: key = 13; break;
				case 0xb7: key = 14; break;
				case 0x77: key = 15; break;
			}
						
			while(temp != 0xf0)
			{
				temp = P3;
				temp = temp & 0xf0;
			}
						
			display(key);
		}
	}		
		
}