#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include "C:\Program Files (x86)\LabJack\Drivers\LabJackUD.h"
int main()
{
	//the variables will be set up
	LJ_ERROR lj_cue;
	LJ_HANDLE lj_handle = 0;
	int dutycycle = 0, counter =0;
	double dblValueFIO4=0, dblValueDAC0=0, dblValueAIN0=0;
	//the ports will be reset at the start
	lj_cue = OpenLabJack(LJ_dtU3, LJ_ctUSB, "1", 1, &lj_handle);
	lj_cue = ePut(lj_handle, LJ_ioPIN_CONFIGURATION_RESET, 0, 0, 0);
	//the PWM setting will be set for the buzzer on port FIO4
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chTIMER_COUNTER_PIN_OFFSET, 4, 0, 0);
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chTIMER_CLOCK_BASE, LJ_tc48MHZ_DIV, 0, 0);
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chTIMER_CLOCK_DIVISOR, 48, 0, 0);
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_CONFIG, LJ_chNUMBER_TIMERS_ENABLED, 1, 0, 0);
	lj_cue = AddRequest(lj_handle, LJ_ioPUT_TIMER_MODE, 0, LJ_tmPWM8, 0, 0);
	lj_cue = Go();
	//this code will now enter the first will loop, a nested while loop is used making it an infinite loop
	while (dblValueAIN0 < 1){
		counter = 0; // counter is reset to 0 so if the sensor is triggered the progrm will not end
		lj_cue = AddRequest(lj_handle, LJ_ioGET_AIN, 0, 0, 0, 0);// used the ADD-Go-Get sequence to get the redding on the sensor, it will be printed every 250 miliseconds
		lj_cue = Go();
		lj_cue = GetResult(lj_handle, LJ_ioGET_AIN, 0, &dblValueAIN0);
		printf("AIN = %lf\n", dblValueAIN0);
		Sleep(250);
		while (dblValueAIN0 > 1) // this loop will be for when the sensor is blocked
		{
				lj_cue = AddRequest(lj_handle, LJ_ioGET_AIN, 0, 0, 0, 0);//same add-go-get sequence so the sensor reading is shown, even when the sensor is blocked.
				lj_cue = Go();
				lj_cue = GetResult(lj_handle, LJ_ioGET_AIN, 0, &dblValueAIN0);
				printf("AIN = %lf\n", dblValueAIN0);
				dutycycle = 32768; // the dutyclycle is set to 50%, so the buzzer is sound
				lj_cue = AddRequest(lj_handle, LJ_ioPUT_TIMER_VALUE, 0, dutycycle, 0, 0);
				lj_cue = Go();
				lj_cue = AddRequest(lj_handle, LJ_ioPUT_DAC, 0, 0, 0, 0);// the led will turn on when the buzzer is on aswell.
				lj_cue = Go();
				counter++; //counter will be used to exit the program, counter will add 1 every 250 miliseconds, to get the 15 secs, 15secs / 0.250 = 60
				Sleep(250);
				if (counter == 60)// when the senso is blocked for 15 secs the program will do this
				{

					lj_cue = ePut(lj_handle, LJ_ioPIN_CONFIGURATION_RESET, 0, 0, 0);// the ports are reset
					lj_cue = eGet(lj_handle, LJ_ioGET_DIGITAL_BIT, 4, &dblValueFIO4, 0);
					system("pause");// system will pause and ask user to to press anything
					lj_cue = AddRequest(lj_handle, LJ_ioPUT_DAC, 0, 5, 0, 0);// the led will turn off when the user presses anything.
					lj_cue = Go();
					
					return 0;
				}
			
			
		}
		
		dutycycle = 0; 
		lj_cue = AddRequest(lj_handle, LJ_ioPUT_DAC, 0, 5, 0, 0);/*this portion of the code is for the first while loop, the LED will be off when the sensor is not blocked, and the dutycle is set to 0, so the buzzer will also be quiet*/
		lj_cue = Go();
		lj_cue = AddRequest(lj_handle, LJ_ioPUT_TIMER_VALUE, 0, dutycycle, 0, 0);
		lj_cue = Go();
	}
	
	
}
