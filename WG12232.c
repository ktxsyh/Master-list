//SBN1661G For WG12232A/C With 6800
//Data : 2017/6/29
// Evan

#include <reg52.h>
#include <intrins.h>

#define uchar unsigned char 
#define uint unsigned int
#define First_Page 0xb8
#define First_Row 0xc0  //起始列地址
#define First_Line 0x00  //起始行地址
#define NOP ()   _nop_ () ;_nop_ () ;_nop_ () ;_nop_ () ;_nop_ () 

//uchar code Tab[]={};

bit R = 1,W = 0, C = 0, D = 1; //6800数据控制字
//sbit CS1 = P2^1;
//sbit CS2 = P2^2;
//sbit RES = P2^3;
//sbit RW = P2^4;
//sbit DC = P^5;
/***********Command************
Do_12232(W,C,1,1,0XAE);  //开关显示 +1为显示开
Do_12232(W,C,1,1,0XC0);   //起始行
Do_12232(W,C,1,1,0XB8);  //页地址
Do_12232(W,C,1,1,0X00);   //列地址设置
Do_12232(W,C,1,1,0XB1);   //设置显示方向 B1正向 B0反向
Do_12232(W,C,1,1,0XA4);   //驱动模式 正常 +1静态显示
Do_12232(W,C,1,1,0XA8);   //DUTY设置 1/16  +1位1/32
Do_12232(W,C,1,1,0XE0);
Do_12232(W,C,1,1,0XAE);   //结束
Do_12232(W,C,1,1,0XE2);   //复位   
************Command************/

void Delay_ms (uint t)
{
	uchar a,b;
	for (;t>0;t--)
	{
		for (b=199;b>0;b--)
		{
			for (a>1;a>0;a--)
				;
		}
	}
}

void Do_12232 (bit Temp_RW,bit Temp_DC,bit EN1,bit EN2,uchar Date)
{
	A0 = Temp_DC;
	RW = Temp_DC;
	P1 = Date;
	if (EN1)
	{
		CS1 = 1;
		CS2 = 0;
	}
	if (EN2)
	{
		CS1 = 0;
		CS2 = 1;
	}
}

void  Init (void)
{
	RES = 1;  //硬件置位
	Do_12232 (W,C,1,1,0XE2);  //软件复位
	Do_12232 (W,C,1,1,0XA4);  //动态驱动
	Do_12232 (W,C,1,1,0xAE);  //显示关闭
	Do_12232 (W,C,1,1,0xA9);  //设置1/32 DUTY
	Do_12232 (W,C,1,1,0xA0);  //顺时针显示
	Do_12232 (W,C,1,1,0xEE);  //写模式
	Do_12232 (W,C,1,1,0xAF);  //开显示
}

void Clear_Screen ()
{
	uchar i,j;
	for (i = 0;i < 4;i ++) //扫描页
	{
		Do_12232 (W,C,1,1,First_Page+i);  //设置页起始地址，并扫描4次
		Do_12232 (W,C,1,1,First_Row);   //设置起始列
		Do_12232 (W,C,1,1,First_Line);   //设置起始行
		for (j = 0; j <61;j++)  //扫描列
		{
			Do_12232 (W,D,1,1,0);   //向DDRAM填充数据
		}
	}
}

void Write_One_Char (uchar x,uchar y,uchar code *Tab)  //写7*16像素的图像到指定的位置
{
	uchar i,j;
	for (j = 0;j < 2;j ++)
	{
		if (y/8) 
		{
			Do_12232 (W,C,0,1,First_Page + x + j);
			Do_12232 (W,C,0,1,First_Row + 8*x);
			Do_12232 (W,C,0,1,First_Line + 7*(y-8)); 
			for (i=0;i<7;i++)
			{
				Do_12232 (W,D,0,1,Tab[7*j + i]); 
			}
			Do_12232 (W,C,0,1,First_Row); 
		}
		else 
		{
			Do_12232 (W,C,1,0,First_Page + x + j); 
			Do_12232 (W,C,1,0,Frist_Row + 8*x);
			Do_12232 (W,C,1,0,First_Line + 7*y + 5);
			for (i = 0;i <7;i++)
			{
				Do_12232 (W,D,1,0,Tab[7*j + i]); 
			}
			Do_12232 (W,C,1,0,First_Row); 
		}	
	}
}

void Write_One_Chinese (uchar x,uchar y,uchar code *Tab)  //写任意像素15*16到指定位置
{
	uchar i,j;
	for (i = 0;i < 2;i ++)
	{
		if (y/4)
		{
			Do_12232 (W,C,0,1,First_Page + x + i);
			Do_12232 (W,C,0,1,First_Row + 8*x);
			Do_12232 (W,C,0,1,First_Line + 15*(y-4)); 
			for (i=0;i<15;i++)
			{
				Do_12232 (W,D,0,1,Tab[15*i + j]); 
			}
			Do_12232 (W,C,0,1,First_Row); 
		}
		else
		{	
			Do_12232 (W,C,1,0,First_Page + x + i);
			Do_12232 (W,C,1,0,First_Row + 8*x);
			Do_12232 (W,C,1,0,First_Line + 15*(y-4)); 
			for (i=0;i<15;i++)
			{
				Do_12232 (W,D,1,0,Tab[15*i + j]); 
			}
			Do_12232 (W,C,1,0,First_Row); 
		}
	}
}

void Write_Chars (uchar x,uchar y,uchar code *Tab,uchar Cnt)
{
	uchar i;
	for (i = 0;i < cnt;i++)
	{
		Write_One_Char (x,y+i,Tab + 14*i);
	}
}


void Write_Characters (uchar x,uchar y,uchar code *Tab,uchar Cnt)
{
	uchar i;
	for (i = 0;i < Cnt;i ++)
	{
		Write_One_Chinese (x,y+i,Tab + 30*i);
	}
}


void Display_Pic (uchar *Tab)
{
	uchar i,j;
	for (i = 0;i < 4;i ++)
	{
		Do_12232 (W,C,1,1,First_Page + i);
		Do_12232 (W,C,1,1,First_Row);
		Do_12232 (W,C,1,1,First_Line);
		for (j = 0;j = 122; j ++)
		{
			if (j<61)
			{
				Do_12232 (W,D,1,0,Tab[122*i + j]);
			}
			else
			{
				Do_12232 (W,D,0,1,Tab[122*i + j]);
			}
		}
	}
}

void main ()
{
	Init ();
	Clear_Screen ();
	while (1)
	{
		Clear_Screen ();
		Write_Characters(0,0,num,10);
		Delay_ms (5000);
		
		Clear_Screen ();
		Display_Pic (PIC1);
		Delay_ms (5000);
	}
}






