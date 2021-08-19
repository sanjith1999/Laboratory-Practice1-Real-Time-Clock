#include "RTC_DS3232.h"
#include "Lcd_16X2.h"
#include "Alarm2.h"
#include "Keys.h"
#include "Time.h"
#include "Tone_Notes.h"

//Functions defined inside manin loop

void setup();
void Main_Menu();
void Clock_Mode();

//Main Loop variables
int key = 0;				//Hadling key Press
int level = 0;				//Setup handling
int item[4] = {0, 0, 0, 0}; // Moving with in items of a level

rtc_t rtc;		// defining the time storing variable
int c_mode = 0; // 0 ->12 hour , 1-> 24hour

int main()
{
	setup();

	while (1)
	{
		RTC_Get_Time(&rtc); // Get Date and time from DS3232 for refreshing the Time
		if (Key_Pressed() == 1)
		{ // check the key pressing status go to the main menu
			level = level + 1;
			PORTD |= (1 << 3); // Output 5 V to port Pin 7.
			Main_Menu();
			PORTD &= ~(1 << 3); // 0 V at port pin 7.
		}

		if (Alarm_Time())
		{ // check the alarm time with current time for activate the alarm

			int position = alarms[ha_count].Tone;
			LCD_Clear();
			LCD_SetCursor(0, 3);
			LCD_String(" ALARM :");
			LCD_Num(ha_count + 1);
			LCD_SetCursor(1, 2);
			LCD_String(alarm_names[alarms[ha_count].Name]);

			if (position == 0)
			{
				key = ring_alarm(melody1);
			}
			else if (position == 1)
			{
				key = ring_alarm(melody2);
			}
			else if (position == 2)
			{
				key = ring_alarm(melody3);
			}
			//				else if(position==3){key=ring_alarm(melody4);}
			// 				else {key=ring_alarm(melody5);}

			if (key == 1)
			{
				// repeat mode analyzing
				Repeat_Handle(ha_count);
			}
			else
			{
				// snooze analyzing
				if (alarms[ha_count].A_Time[1] < (60 - snooze_time))
				{
					alarms[ha_count].A_Time[1] = alarms[ha_count].A_Time[1] + snooze_time;
				}
				else
				{
					alarms[ha_count].A_Time[1] = (alarms[ha_count].A_Time[1]) % snooze_time;
					alarms[ha_count].A_Time[0]++;
				}
			}
			A_Sort();
			LCD_Clear();
		}
		else
		{
			Display_Time(); // Display the current Time and Date on display
		}
	}
}

void setup()
{
	Key_Setup();
	RTC_Initialize();

	LCD_Initializer();

	LCD_SetCursor(0, 2);
	LCD_String("LAB PROJECT");
	LCD_SetCursor(1, 1);
	LCD_String(" ALARM CLOCK");
	_delay_ms(2000);
	LCD_Clear();
}
void Main_Menu()
{
	//main menu level-01 handling
	LCD_Clear();

	while (1)
	{
		LCD_SetCursor(0, 5);
		LCD_String("MAIN MENU");

		// date/time(level=1)
		if (item[0] == 0)
		{
			LCD_SetCursor(1, 2);
			LCD_String("DATE/TIME");
			Key_handle(3, item, &level);

			if (level == 1)
			{
				continue;
			}
			else if (level == 0)
			{
				item[level] = 0;
				LCD_Clear();
				return;
			}

			// date/time level -02 handling
			Date_Time(&level, item);
		}

		// alarm setting(level=1)
		else if (item[0] == 1)
		{
			LCD_SetCursor(1, 2);
			LCD_String("ALARM SETTINGS");
			Key_handle(3, item, &level);

			if (level == 1)
			{
				continue;
			}
			else if (level == 0)
			{
				item[level] = 0;
				LCD_Clear();
				return;
			}

			// alarm setting level -02 handling
			Alarm_setting(&level, item);
		}

		// clock mode changing(level=1)
		else if (item[0] == 2)
		{
			LCD_SetCursor(1, 2);
			LCD_String("CLOCK MODE");
			Key_handle(3, item, &level);

			if (level == 1)
			{
				continue;
			}
			else if (level == 0)
			{
				item[level] = 0;
				LCD_Clear();
				return;
			}

			// Clock Mode level -02 handling
			Clock_Mode();
		}
	}

	LCD_Clear();
}
void Clock_Mode()
{
	LCD_Clear();
	LCD_SetCursor(0, 1);
	LCD_String("CLOCK MODE");

	while (1)
	{

		if (item[1] == 0)
		{
			LCD_SetCursor(1, 1);
			LCD_String("24-HOUR");

			Key_handle(2, item, &level);

			if (level == 1)
			{
				break;
			}
			else if (level == 2)
			{
				continue;
			}

			c_mode = 1;
			LCD_Clear();
			LCD_SetCursor(0, 1);
			LCD_String("24-HOUR MODE");
			LCD_SetCursor(1, 3);
			LCD_String("SELECTED");
			_delay_ms(3000);

			LCD_Clear();
			level--;
			break;
		}

		if (item[1] == 1)
		{
			LCD_SetCursor(1, 1);
			LCD_String("12-HOUR");

			Key_handle(2, item, &level);

			if (level == 1)
			{
				item[1] = 0;
				break;
			}
			else if (level == 2)
			{
				continue;
			}

			c_mode = 0;
			LCD_Clear();
			LCD_SetCursor(0, 1);
			LCD_String("12-HOUR MODE");
			LCD_SetCursor(1, 3);
			LCD_String("SELECTED");
			_delay_ms(3000);

			LCD_Clear();
			level--;
			break;
		}
	}
}
