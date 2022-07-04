/*
 * LullabyApp.h
 *
 *  Created on: 2022年6月30日
 *      Author: zhouxin
 */

#ifndef LULLABYAPP_H_
#define LULLABYAPP_H_

#include <string.h>
#include <pthread.h>
#include "Lullaby.h"

/**
 * 	Sound activated
 */
typedef enum
{
	LULLABY_SOUND_ACTIVATED_ON,    /**< LULLABY_SOUND_ACTIVED_ON */
	LULLABY_SOUND_ACTIVATED_OFF,   /**< LULLABY_SOUND_ACTIVED_OFF */

	LULLABY_SOUND_ACTIVED_NUMBER,  /**< LULLABY_SOUND_ACTIVED_NUMBER */
} LullabySoundActivate_e;

/**
 * 	Duration
 */
typedef enum
{
	LULLABY_DURATION_ON,           /**< LULLABY_DURATION_ON */
	LULLABY_DURATION_OFF,          /**< LULLABY_DURATION_OFF */
	LULLABY_DURATION_30MIN,        /**< LULLABY_DURATION_30MIN */
	LULLABY_DURATION_1HOUR,        /**< LULLABY_DURATION_1HOUR */
	LULLABY_DURATION_2HOUR,        /**< LULLABY_DURATION_2HOUR */
	LULLABY_DURATION_6HOUR,        /**< LULLABY_DURATION_6HOUR */
	LULLABY_DURATION_SOUND_ACTIVED,/**< LULLABY_DURATION_SOUND_ACTIVED */

	LULLABY_DURATION_NUMBER,       /**< LULLABY_DURATION_NUMBER */
} LullabyDuration_e;

/**
 * 	Lullaby Repertpire
 */
typedef enum
{
	LULLABY_INDEX_1,           /**< LULLABY_INDEX_1 */
	LULLABY_INDEX_2,           /**< LULLABY_INDEX_2 */
	LULLABY_INDEX_3,           /**< LULLABY_INDEX_3 */
	LULLABY_INDEX_NATURESOUNDS,/**< LULLABY_INDEX_NATURESOUNDS */
	LULLABY_INDEX_WHITENOISE,  /**< LULLABY_INDEX_WHITENOISE */

	LULLABY_INDEX_NUMBER,      /**< LULLABY_INDEX_NUMBER */
} LullabyIndex_e;

typedef struct
{
	Lullaby_t* 			  	lullaby;
	LullabySoundActivate_e  sound_activated;
	LullabyDuration_e		duration;
}LullabyApp_t;

void LullabyApp_Init(LullabyApp_t* lullaby_app);
void LullabyApp_DeInit(LullabyApp_t* lullaby_app);

void LullabyApp_Set_Index(LullabyApp_t* lullaby_app, LullabyIndex_e index);
void LullabyApp_Set_Sound_Activated(LullabyApp_t* lullaby_app, LullabySoundActivate_e activated);
int  LullabyApp_Set_Duration(LullabyApp_t* lullaby_app, LullabyDuration_e duration);
void LullabyApp_Set_Status(LullabyApp_t* lullaby_app, LullabyStatus_e status);

void LullabyApp_Set_Start(LullabyApp_t* lullaby_app, LullabyPlayControlFunc play_start);
void LullabyApp_Set_Stop(LullabyApp_t* lullaby_app, LullabyPlayControlFunc play_stop);
void LullabyApp_Set_Pause(LullabyApp_t* lullaby_app, LullabyPlayControlFunc play_pause);
void LullabyApp_Set_Resume(LullabyApp_t* lullaby_app, LullabyPlayControlFunc play_resume);

#endif /* LULLABYAPP_H_ */
