/*
 * LullabyApp.c
 *
 *  Created on: 2022年6月30日
 *      Author: zhouxin
 */
#include "LullabyApp.h"


#define return_if_null(arg) if(NULL == arg) return;

static uint32_t LullabyAppTime[LULLABY_DURATION_NUMBER]=
{
		[LULLABY_DURATION_ON] = 0,
		[LULLABY_DURATION_OFF] = 0,
		[LULLABY_DURATION_30MIN] = 30 * 60 * 60,
		[LULLABY_DURATION_1HOUR] = 60 * 60 *60,
		[LULLABY_DURATION_2HOUR] = 120 * 60 * 60,
		[LULLABY_DURATION_6HOUR] = 360 * 60 * 60,
		[LULLABY_DURATION_SOUND_ACTIVED] = 5 * 60 *60,
};

/**
 * @fn void LullabyApp_Init(LullabyApp_t*)
 * @brief
 * 			init function. input param lullaby_app can't be NULL.
 * @param lullaby_app
 */
void LullabyApp_Init(LullabyApp_t* lullaby_app)
{
	return_if_null(lullaby_app);

	lullaby_app->lullaby = (Lullaby_t*)calloc(1, sizeof(Lullaby_t));
	return_if_null(lullaby_app->lullaby);

	Lullaby_Init(lullaby_app->lullaby);
	lullaby_app->duration = LULLABY_DURATION_OFF;
	lullaby_app->sound_activated = LULLABY_SOUND_ACTIVATED_OFF;
}
/**
 * @fn void LullabyApp_DeInit(LullabyApp_t*)
 * @brief
 * 			deinit function
 * @param lullaby_app
 */
void LullabyApp_DeInit(LullabyApp_t* lullaby_app)
{
	return_if_null(lullaby_app);
	Lullaby_DeInit(lullaby_app->lullaby);
	if(lullaby_app->lullaby)
	{
		free(lullaby_app->lullaby);
		lullaby_app->lullaby = NULL;
	}
}
/**
 * @fn void LullabyApp_Set_Index(LullabyApp_t*, LullabyIndex_e)
 * @brief
 *		when play stop or pause status, only change index.
 *		when playing, change index and play restart, but don't reset play time.
 * @param lullaby_app
 * @param index
 */
void LullabyApp_Set_Index(LullabyApp_t* lullaby_app, LullabyIndex_e index)
{
	return_if_null(lullaby_app);
	pthread_mutex_lock(&lullaby_app->lullaby->mutex);
	lullaby_app->lullaby->index = (index >= LULLABY_INDEX_NUMBER) ? LULLABY_INDEX_1 : index;
	if(lullaby_app->lullaby->status == LULLABY_STATUS_PLAYING)
	{
		lullaby_app->lullaby->status = LULLABY_STATUS_START;
	}
	pthread_mutex_unlock(&lullaby_app->lullaby->mutex);
}

/**
 * @fn void LullabyApp_Set_Sound_Activated(LullabyApp_t*, LullabySoundActivate_e)
 * @brief
 * 		when set sound activated on or off, need to stop play.
 * @param lullaby_app
 * @param activated
 */
void LullabyApp_Set_Sound_Activated(LullabyApp_t* lullaby_app, LullabySoundActivate_e activated)
{
	return_if_null(lullaby_app);
	lullaby_app->sound_activated = activated;
	lullaby_app->duration = LULLABY_DURATION_OFF;
	pthread_mutex_lock(&lullaby_app->lullaby->mutex);
	lullaby_app->lullaby->rest_time = 0;
	lullaby_app->lullaby->status = LULLABY_STATUS_STOP;
	pthread_mutex_unlock(&lullaby_app->lullaby->mutex);
}

/**
 * @fn void LullabyApp_Set_Duration(LullabyApp_t*, LullabyDuration_e)
 * @brief
 *		When lullaby play stop status, duration on --> play all the time, set loop to true,
 *													   whether sound activated on or off,set duration to on;
 *									   duration off --> keep stop status and set loop to false;
 *									   					whether sound activated on or off,set duration to off;
 *									   30m/1H/2H/6H ---> play start and set rest time to  30m/1H/2H/6H and set loop to false;
 *									   					whether sound activated on or off, set duration to 30m;
 *									   sound activated ---> play start and set rest time to 5m and set loop to false, and set duration to LULLABY_DURATION_30MIN
 *
 *		When lullaby play pause status, duration on --> play resume and keep playing all the time, set loop to true;
 *														 whether sound activated on or off,set duration to on;
 *									   duration off --> play stop and set reset time to 0 and set loop to false;
 *									    				whether sound activated on or off,set duration to off;
 *									   30m/1H/2H/6H ---> play resume and add rest time with 30m/1H/2H/6H;
 *									   					 whether sound activated on or off, set duration to 30m;
 *									   sound activated --->keep pause and change nothing;
 *
 *		When lullaby play playing status, duration on --> keep playing and set loop to true;
 *								 						  whether sound activated on or off,set duration to on;
 *									   duration off --> play stop and set rest time to 0 and set loop to false;
 *									   					whether sound activated on or off,set duration to off;
 *									   30m/1H/2H/6H ---> keep playing and add rest time with 30m/1H/2H/6H and set loop to false;
 *									   					whether sound activated on or off, set duration to 30m;
 *									   sound activated ---> keep playing and change nothing;
 * @param lullaby_app
 * @param duration  input param, pls refer to LullabyDuration_e
 * @return 0: success, -1: failed.
 */
int LullabyApp_Set_Duration(LullabyApp_t* lullaby_app, LullabyDuration_e duration)
{
	if(NULL == lullaby_app)
	{
		return -1;
	}
	int ret = 0;
	pthread_mutex_lock(&lullaby_app->lullaby->mutex);
	switch(lullaby_app->lullaby->status)
	{
	case LULLABY_STATUS_STOP:
		switch(duration)
		{
		case LULLABY_DURATION_ON:
			lullaby_app->lullaby->rest_time = LullabyAppTime[duration];
			lullaby_app->lullaby->status = LULLABY_STATUS_START;
			lullaby_app->lullaby->loop = true;
			lullaby_app->duration = duration;
			break;
		case LULLABY_DURATION_OFF:
			lullaby_app->lullaby->rest_time = LullabyAppTime[duration];
			lullaby_app->lullaby->status = LULLABY_STATUS_STOP;
			lullaby_app->lullaby->loop = false;
			lullaby_app->duration = duration;
			break;
		case LULLABY_DURATION_30MIN:
		case LULLABY_DURATION_1HOUR:
		case LULLABY_DURATION_2HOUR:
		case LULLABY_DURATION_6HOUR:
		case LULLABY_DURATION_SOUND_ACTIVED:
			lullaby_app->lullaby->rest_time = LullabyAppTime[duration];
			lullaby_app->lullaby->status = LULLABY_STATUS_START;
			lullaby_app->lullaby->loop = false;
			lullaby_app->duration = LULLABY_DURATION_30MIN;
			break;
		default:
			break;
		}
		break;
	case LULLABY_STATUS_PAUSE:
		switch(duration)
		{
		case LULLABY_DURATION_ON:
			lullaby_app->lullaby->rest_time = 0;
			lullaby_app->lullaby->status = LULLABY_STATUS_RESUME;
			lullaby_app->lullaby->loop = true;
			lullaby_app->duration = duration;
			break;
		case LULLABY_DURATION_OFF:
			lullaby_app->lullaby->rest_time = 0;
			lullaby_app->lullaby->status = LULLABY_STATUS_STOP;
			lullaby_app->lullaby->loop = false;
			lullaby_app->duration = duration;
			break;
		case LULLABY_DURATION_30MIN:
		case LULLABY_DURATION_1HOUR:
		case LULLABY_DURATION_2HOUR:
		case LULLABY_DURATION_6HOUR:
			lullaby_app->lullaby->status = LULLABY_STATUS_RESUME;
			lullaby_app->lullaby->rest_time += LullabyAppTime[duration];
			lullaby_app->lullaby->loop = false;
			lullaby_app->duration = LULLABY_DURATION_30MIN;
			break;
		case LULLABY_DURATION_SOUND_ACTIVED:
			break;
		default:
			break;
		}
		break;
	case LULLABY_STATUS_PLAYING:
		switch(duration)
		{
		case LULLABY_DURATION_ON:
			lullaby_app->lullaby->loop = true;
		    lullaby_app->lullaby->rest_time = 0;
		    lullaby_app->duration = duration;
			break;
		case LULLABY_DURATION_OFF:
			lullaby_app->lullaby->status = LULLABY_STATUS_STOP;
			lullaby_app->lullaby->loop = false;
			lullaby_app->lullaby->rest_time = 0;
			lullaby_app->duration = duration;
			break;
		case LULLABY_DURATION_30MIN:
		case LULLABY_DURATION_1HOUR:
		case LULLABY_DURATION_2HOUR:
		case LULLABY_DURATION_6HOUR:
			 lullaby_app->lullaby->rest_time += LullabyAppTime[duration];
			 lullaby_app->lullaby->loop = false;
			 lullaby_app->duration = LULLABY_DURATION_30MIN;
			break;
		case LULLABY_DURATION_SOUND_ACTIVED:
			break;
		default:
			break;
		}
		break;
	default:
		ret = -1;
		break;
	}
	pthread_mutex_unlock(&lullaby_app->lullaby->mutex);
	return ret;
}
/**
 * @fn void LullabyApp_Set_Status(LullabyApp_t*, LullabyStatus_e)
 * @brief
 * 			1. can't set play status
 * 			2. normally, use pause and resume status by this function.
 * @param lullaby_app
 * @param status
 */
void LullabyApp_Set_Status(LullabyApp_t* lullaby_app, LullabyStatus_e status)
{
	return_if_null(lullaby_app);
	pthread_mutex_lock(&lullaby_app->lullaby->mutex);
	if(LULLABY_STATUS_PLAYING != status)
	{
		lullaby_app->lullaby->status = status;
	}
	pthread_mutex_unlock(&lullaby_app->lullaby->mutex);
}
/**
 * @fn void LullabyApp_Set_Start(LullabyApp_t*, LullabyPlayControlFunc)
 * @brief
 *		set play start callback function.
 * @param lullaby_app
 * @param play_start
 */
void LullabyApp_Set_Start(LullabyApp_t* lullaby_app, LullabyPlayControlFunc play_start)
{
	return_if_null(lullaby_app);
	Lullaby_Set_PlayStart_Func(lullaby_app->lullaby, play_start);
}
/**
 * @fn void LullabyApp_Set_Stop(LullabyApp_t*, LullabyPlayControlFunc)
 * @brief
 *			set play stop callback function.
 * @param lullaby_app
 * @param play_stop
 */
void LullabyApp_Set_Stop(LullabyApp_t* lullaby_app, LullabyPlayControlFunc play_stop)
{
	return_if_null(lullaby_app);
	Lullaby_Set_PlayStop_Func(lullaby_app->lullaby, play_stop);
}
/**
 * @fn void LullabyApp_Set_Pause(LullabyApp_t*, LullabyPlayControlFunc)
 * @brief
 *		set play pause callback function.
 * @param lullaby_app
 * @param play_pause
 */
void LullabyApp_Set_Pause(LullabyApp_t* lullaby_app, LullabyPlayControlFunc play_pause)
{
	return_if_null(lullaby_app);
	Lullaby_Set_PlayPause_Func(lullaby_app->lullaby, play_pause);
}
/**
 * @fn void LullabyApp_Set_Resume(LullabyApp_t*, LullabyPlayControlFunc)
 * @brief
 *		set play resume callback function.
 * @param lullaby_app
 * @param play_resume
 */
void LullabyApp_Set_Resume(LullabyApp_t* lullaby_app, LullabyPlayControlFunc play_resume)
{
	return_if_null(lullaby_app);
	Lullaby_Set_PlayResume_Func(lullaby_app->lullaby, play_resume);
}
