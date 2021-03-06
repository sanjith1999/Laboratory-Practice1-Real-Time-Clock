
#include "Lcd_16X2.h"

void LCD_Commandgiver(unsigned char comand)
{
	LCDER_Port = (LCDER_Port & 0x0F) | (comand & 0xF0);
	LCDER_Port &= ~ (1<<RSel);
	LCDER_Port |= (1<<ENable);
	_delay_us(2);
	LCDER_Port &= ~ (1<<ENable);
	
	_delay_us(200);
	
	LCDER_Port = (LCDER_Port & 0x0F) | (comand << 4);
	LCDER_Port |= (1<<ENable);
	_delay_us(2);
	LCDER_Port &= ~ (1<<ENable);
	_delay_ms(2);
}

void LCD_Initializer (void)
{
	LCD_Dir_00 = 0xFF;              //Make LCD port direction as o/p
	_delay_ms(20);                  //LCD Power ON delay always >15ms
	
	LCD_Commandgiver(0x02);         // send for 4 bit initialization of LCD
	LCD_Commandgiver(0x28);         // 2 line, 5*7 matrix in 4-bit mode
	LCD_Commandgiver(0x0c);         // Display on cursor off
	LCD_Commandgiver(0x06);         // Increment cursor (shift cursor to right)
	LCD_Commandgiver(0x01);         // Clear display screen
	_delay_ms(2);
}

void LCD_String(const char *string_00)
{
	
	for (int ci=0;string_00[ci]!=0;ci++)
	{
		LCD_Character (string_00[ci]);
	}
}

void LCD_Character(unsigned char data_00)
{
	LCDER_Port = ((LCDER_Port & 0x0F) | (data_00 & 0xF0));
	LCDER_Port |= (1<<RSel);
	LCDER_Port |= (1<<ENable);
	_delay_us(1);
	LCDER_Port &= ~ (1<<ENable);
	
	_delay_us(200);
	
	LCDER_Port = (LCDER_Port & 0x0F) | (data_00 << 4);
	LCDER_Port |= (1<<ENable);
	_delay_us(1);
	LCDER_Port &= ~ (1<<ENable);
	_delay_ms(2);
}

void LCD_Clear()
{
	LCD_Commandgiver(0x01);
	_delay_ms(2);
	LCD_Commandgiver(0x80);
}

void LCD_SetCursor (char row_1, char position)
{
	if (row_1 ==0 && position<16)
	LCD_Commandgiver((position & 0x0F)|0x80);
	else if (row_1 == 1 && position<16)
	LCD_Commandgiver((position & 0x0F)|0xC0);
	//LCD_String(string_00);
}
void LCD_Num (int num){
	int digit[10];
	int index=0;
	while (1){
		if (num>9){
			digit[index]=num%10;
			index++;
		num = num/10;}
		else{
			digit[index]=num;
			break;
		}
	}
	for (;index >=0;index--){
		LCD_Character(digit[index]+48);
	}
}

void LCD_TwoDigit (int num){
	if(num<10){
		LCD_String("0");
		LCD_Num(num);}
	else{
		LCD_Num(num);
	}
	
}

void Display_Time(int d_c_mode,int STATE){
	rtc_t r_rtc;
	RTC_Get_Time(&r_rtc);
	if(d_c_mode==0 ){                                // check the time mode and convert the hour appropriately
		LCD_SetCursor(0,12);
		if (r_rtc.hour==0){
			r_rtc.hour=12;
		LCD_String("AM");}
		else if(r_rtc.hour==12){
		LCD_String("PM");}
		else if(r_rtc.hour>12){
			r_rtc.hour=r_rtc.hour-12;
		LCD_String("PM");}
		else{
		LCD_String("AM");}
	LCD_SetCursor(0,3);}
	else{
	LCD_SetCursor(0,4);}
	
	LCD_TwoDigit((r_rtc.hour));                            // print the time in display
	LCD_String(":");
	LCD_TwoDigit((r_rtc.min));
	LCD_String(":");
	LCD_TwoDigit((r_rtc.sec));
	
	LCD_SetCursor(1,3);                                  // print the date in display
	LCD_TwoDigit((r_rtc.date));
	LCD_String(":");
	LCD_TwoDigit((r_rtc.month));
	LCD_String(":");
	LCD_Num((r_rtc.year)+2000);

	if(STATE){
		LCD_String("  *"); 
	}
}