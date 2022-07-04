/*
 ============================================================================
 Name        : Lullaby.c
 Author      : 
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "LullabyApp.h"

void play_start(int index)
{
	printf("API:--------play start[%d]-------\n", index);
}
void play_stop(int index)
{
	printf("API:--------play stop[%d]-------\n", index);
}
void play_pause(int index)
{
	printf("API:--------play pause[%d]-------\n", index);
}
void play_resume(int index)
{
	printf("API:--------play resume[%d]-------\n", index);
}

int main(void) {
	setbuf(stdout,NULL);
	uint32_t count = 300;
	LullabyApp_t lullaby_app;
	LullabyApp_Init(&lullaby_app);
	LullabyApp_Set_Index(&lullaby_app, LULLABY_INDEX_2);
	LullabyApp_Set_Duration(&lullaby_app, LULLABY_DURATION_SOUND_ACTIVED);
	LullabyApp_Set_Start(&lullaby_app, play_start);
	LullabyApp_Set_Stop(&lullaby_app, play_stop);
	LullabyApp_Set_Pause(&lullaby_app, play_pause);
	LullabyApp_Set_Resume(&lullaby_app, play_resume);
	while(1)
	{
		usleep(100 * 1000);
		--count;
		if( count == 200)
		{
			LullabyApp_Set_Sound_Activated(&lullaby_app, LULLABY_SOUND_ACTIVATED_ON);
		}else if(count == 100)
		{
			LullabyApp_Set_Duration(&lullaby_app, LULLABY_DURATION_30MIN);
		}else if(count == 50)
		{
			LullabyApp_Set_Status(&lullaby_app, LULLABY_STATUS_PAUSE);
		}else if(count == 0)
		{
			LullabyApp_Set_Status(&lullaby_app, LULLABY_STATUS_RESUME);
		}
	}
	return EXIT_SUCCESS;
}
