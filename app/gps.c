#include "gps.h"

u8 step = 0;
u8 cont = 0;
u8 i;
u8 ClassID = 0;
u8 msgLenL = 0;
u8 msgLenH = 0;
u8 CK_A = 0;
u8 CK_B = 0;
u8 gpsdata[255];
u16 Len = 0;
s32 x = 0;
s32 y = 0;
double xx, yy;
double l, s;

void ubx_CheckSum(u8 data)
{
	CK_A = CK_A + data;
	CK_B = CK_B + CK_A;
}

void change_Pos()
{
	for (i = 4; i > 0; i--)
	{
		x = x << 8;
		x |= gpsdata[18 + i - 1];
		y = y << 8;
		y |= gpsdata[22 + i - 1];
	}

	xx = x / 100;
	yy = y / 100;
	l = sqrt((xx * xx) + (yy * yy));
	s = atan2(yy, xx) / iPI;
	x = l * cos((s + 50) * iPI) * 100;
	y = l * sin((s + 50) * iPI) * 100;

	for (i = 0; i < 4; i++)
	{
		gpsdata[i + 18] = x;
		x = x >> 8;
		gpsdata[i + 22] = y;
		y = y >> 8;
	}

	usart_Putchar(0xb5);
	usart_Putchar(0x62);

	for (i = 2; i < 58; i++)
	{
		usart_Putchar(gpsdata[i]);
		ubx_CheckSum(gpsdata[i]);
	}

	usart_Putchar(CK_A);
	usart_Putchar(CK_B);
}

void gps_Get(u8 temp)
{
	switch (step)
	{
		case 0:
		{
			if (temp == 0xB5)
			{
				gpsdata[0] = 0xB5;
				step++;
			}

			else
			{
				step = 0;
			}
			break;
		}

		case 1:
		{
			if (temp == 0x62)
			{
				gpsdata[1] = 0x62;
				step++;
			}

			else
			{
				step = 0;
			}
			break;
		}

		case 2:
		{
			if (temp == 0x01)
			{
				gpsdata[2] = 0x01;
				step++;
			}
			else
			{
				step = 0;
			}
			break;
		}

		case 3:
		{
			ClassID = temp;
			gpsdata[3] = temp;
			step++;
			break;
		}

		case 4:
		{
			msgLenL = temp;
			gpsdata[4] = temp;
			step++;
			break;
		}

		case 5:
		{
			if (temp == 0)
			{
				msgLenH = temp;
				gpsdata[5] = temp;
				Len = ((0x0000 | msgLenH) << 8) | msgLenL;
				step++;
			}
			else
			{
			    step = 0;
			}
			break;
		}

		case 6:
		{
			gpsdata[cont + 6] = temp;
			cont++;
			if (cont == (Len + 1))
			{
				step++;
			}
			break;
		}

		case 7:
		{
			gpsdata[Len + 7] = temp;
			if (ClassID == 0x06)
			{
				change_Pos();
			}
			else
			{
				for (i = 0; i < Len + 8; i++)
				{
					usart_Putchar(gpsdata[i]);
				}
			}

			ClassID = 0;
			msgLenL = 0;
			msgLenH = 0;
			Len = 0;
			x = 0;
			y = 0;
			step = 0;
			cont = 0;
			CK_A = 0;
			CK_B = 0;
			break;
		}
	}
}
