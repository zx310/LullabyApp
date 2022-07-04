#ifndef SDK_VBM_SRC_USR_BSP_BSP_LULLABY_H_
#define SDK_VBM_SRC_USR_BSP_BSP_LULLABY_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

//! MAX Play time
#define LULLABY_MAX_TIME		(100 * 60 *60)   //s

//! play control interface
typedef void (*LullabyPlayControlFunc) (int);

/**
 * 	Lullaby play status
 */
typedef enum
{
	LULLABY_STATUS_STOP, /**< LULLABY_STATUS_STOPED */
	LULLABY_STATUS_START, /**< LULLABY_STATUS_STOPED */
	LULLABY_STATUS_PLAYING,/**< LULLABY_STATUS_PLAYING */
	LULLABY_STATUS_PAUSE, /**< LULLABY_STATUS_PAUSED */
	LULLABY_STATUS_RESUME, /**< LULLABY_STATUS_RESUME */

	LULLABY_STATUS_NUMBER, /**< LULLABY_STATUS_NUMBER */
}LullabyStatus_e;

typedef struct
{
	uint8_t 			index;		// play repertoire
	LullabyStatus_e     status;		//play status
	uint32_t 			rest_time;  //unit: s
	bool				running;	//modules running
	bool				loop;   	//playing loop or play only one time
	LullabyPlayControlFunc	play_start;
	LullabyPlayControlFunc	play_stop;
	LullabyPlayControlFunc	play_pause;
	LullabyPlayControlFunc	play_resume;

	pthread_mutex_t mutex;
}Lullaby_t;

#define LULLABY_DEFAULT		{0, LULLABY_STATUS_STOP, 0, true, false, PTHREAD_MUTEX_INITIALIZER, NULL, NULL, NULL, NULL};

void Lullaby_Init(Lullaby_t* lullaby_in);
void Lullaby_DeInit(Lullaby_t* lullaby_in);
void Lullaby_Set_PlayStart_Func(Lullaby_t* lullaby_in, LullabyPlayControlFunc play_start);
void Lullaby_Set_PlayStop_Func(Lullaby_t* lullaby_in, LullabyPlayControlFunc play_stop);
void Lullaby_Set_PlayPause_Func(Lullaby_t* lullaby_in, LullabyPlayControlFunc play_pause);
void Lullaby_Set_PlayResume_Func(Lullaby_t* lullaby_in, LullabyPlayControlFunc play_resume);
#endif /* SDK_VBM_SRC_USR_BSP_BSP_LULLABY_H_ */
