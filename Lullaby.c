#include "Lullaby.h"

#define return_if_null(arg) if(NULL == arg) return;

static void* Lullaby_Thread_Func(void* arg)
{
	pthread_detach(pthread_self());

	Lullaby_t *lullaby = NULL;
	LullabyStatus_e pre_status = LULLABY_STATUS_STOP;
	LullabyStatus_e curr_status = LULLABY_STATUS_STOP;

	if(arg == NULL)
	{
		pthread_exit(NULL);
	}

	lullaby = (Lullaby_t*)arg;
	pre_status = LULLABY_STATUS_STOP;

	while(lullaby->running)
	{
		pthread_mutex_lock(&lullaby->mutex);
		curr_status = lullaby->status;
		//! If play status changed
		if(pre_status != curr_status)
		{
			pre_status = curr_status;
			switch(curr_status)
			{
			case LULLABY_STATUS_STOP:
				printf("----------------Lullaby play stop!----------------\n");
				if(lullaby->play_stop != NULL)
				{
					lullaby->play_stop(lullaby->index);
				}
				break;
			case LULLABY_STATUS_START:
				printf("----------------Lullaby play start, play index:%d!----------------\n", lullaby->index);
				curr_status = lullaby->status = LULLABY_STATUS_PLAYING;
				if(lullaby->play_start != NULL)
				{
					lullaby->play_start(lullaby->index);
				}
				break;
			case LULLABY_STATUS_PLAYING:
				break;
			case LULLABY_STATUS_PAUSE:
				printf("----------------Lullaby play pause!----------------\n");
				if(lullaby->play_pause != NULL)
				{
					lullaby->play_pause(lullaby->index);
				}
				break;
			case LULLABY_STATUS_RESUME:
				printf("----------------Lullaby play resume!----------------\n");
				curr_status = lullaby->status = LULLABY_STATUS_PLAYING;
				if(lullaby->play_resume != NULL)
				{
					lullaby->play_resume(lullaby->index);
				}
				break;
			default:
				lullaby->status = LULLABY_STATUS_STOP;
				break;
			}
		}

		if(curr_status == LULLABY_STATUS_PLAYING)
		{
			printf("----------------Lullaby play playing, rest time:%d----------------\n", lullaby->rest_time);
			if(!lullaby->loop)
			{
				if(lullaby->rest_time > 0)
				{
					lullaby->rest_time--;
				}
				else if(lullaby->rest_time == 0)
				{
					lullaby->status = LULLABY_STATUS_STOP;
				}
			}
		}
		pthread_mutex_unlock(&lullaby->mutex);
		sleep(1);
	}
	pthread_exit(NULL);
}

void Lullaby_Init(Lullaby_t* lullaby_in)
{
	return_if_null(lullaby_in);

	pthread_t lullaby_thread_id;
	Lullaby_t lullaby_tmp = LULLABY_DEFAULT;
	int ret = 0;

	memcpy(lullaby_in, &lullaby_tmp, sizeof(Lullaby_t));
	ret = pthread_create(&lullaby_thread_id, NULL, Lullaby_Thread_Func, (void*)lullaby_in);
	if(ret != 0)
	{
		printf("%s: pthread_create error!!!\n", __func__);
	}
}

void Lullaby_DeInit(Lullaby_t* lullaby_in)
{
	return_if_null(lullaby_in);
	pthread_mutex_lock(&lullaby_in->mutex);
	lullaby_in->running = false;
	pthread_mutex_unlock(&lullaby_in->mutex);
}

void Lullaby_Set_PlayStart_Func(Lullaby_t* lullaby_in, LullabyPlayControlFunc play_start)
{
	return_if_null(lullaby_in);
	pthread_mutex_lock(&lullaby_in->mutex);
	lullaby_in->play_start = play_start;
	pthread_mutex_unlock(&lullaby_in->mutex);
}
void Lullaby_Set_PlayStop_Func(Lullaby_t* lullaby_in, LullabyPlayControlFunc play_stop)
{
	return_if_null(lullaby_in);
	pthread_mutex_lock(&lullaby_in->mutex);
	lullaby_in->play_stop = play_stop;
	pthread_mutex_unlock(&lullaby_in->mutex);
}
void Lullaby_Set_PlayPause_Func(Lullaby_t* lullaby_in, LullabyPlayControlFunc play_pause)
{
	return_if_null(lullaby_in);
	pthread_mutex_lock(&lullaby_in->mutex);
	lullaby_in->play_pause = play_pause;
	pthread_mutex_unlock(&lullaby_in->mutex);
}
void Lullaby_Set_PlayResume_Func(Lullaby_t* lullaby_in, LullabyPlayControlFunc play_resume)
{
	return_if_null(lullaby_in);
	pthread_mutex_lock(&lullaby_in->mutex);
	lullaby_in->play_resume = play_resume;
	pthread_mutex_unlock(&lullaby_in->mutex);
}
