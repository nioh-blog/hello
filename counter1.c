#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
/*#include <vga.h>*/
#include <sys/types.h>
#include <unistd.h>
#include <termios.h>

/* program liczenie1*/
int threads=1024;
int threads_max=1024;

int counter1=0;
int counter1_start=1;
int counter1_stop=1000000;
int counter1_warn=200000;

int counter1=0;

int sleep_time=1000;

pthread_cond_t warunek1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;

/* dziwny kod zwiazany z enterem*/
int getch (void)
{
        int key;
        struct termios oldSettings, newSettings;    /* stuktury z ustawieniami terminala */
 
        tcgetattr(STDIN_FILENO, &oldSettings);    /* pobranie ustawień terminala */
        newSettings = oldSettings;
        newSettings.c_lflag &= ~(ICANON | ECHO);    /* ustawienie odpowiednich flag */
        tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);    /* zastosowanie ustawień */
        key = getchar();    /* pobranie znaku ze standardowego wejścia */
        tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);    /* przywrócenie poprzednich ustawień terminala */
        return key;
}
/* koniec */

void* counter_in(void *arg)
{
  for(;;)
    {
      pthread_mutex_lock(&mutex1);
      counter1++;
      pthread_mutex_unlock(&mutex1);
      usleep(500);
    }
  return NULL;
}

void* manager(void *arg)
{
  for(;;) /* Petla nieskonczona */
    {
      pthread_mutex_lock(&mutex1);
      if ( counter1>=counter1_warn ) 
	{
	    pthread_cond_signal(&warunek1); 
	}
      pthread_mutex_unlock(&mutex1);
    }
    return NULL;
}

void* counter_de(void *arg);
{
  for(;;)
    {
      pthread_mutex_lock(&mutex1);
      
    }

}



void* worker(void *arg)
{
    int i;
    //printf("COOLING READY, skutecznosc chl.:%d (a+, z-)\t, opor przepl.:%d (s+, x-)\n",cool_nr,sleep_time);
    printf("COOLING READY:\t%d\n",(int)pthread_self());
    for(;;) /* Petla nieskonczon/a */
    {

	pthread_cond_wait(&warunek ,&mutex); /* Czekaj na sygnal od zarzadcy */
		//printf("%d: ",pthread_self());
		for(i=0;i<cool_nr;i++)
		{
			pthread_mutex_trylock(&mutex);   
			    temperatura--;
			printf(".:%d:.",(int)pthread_self());
			pthread_mutex_unlock(&mutex);
		//	pthread_yield();
		usleep(sleep_time);
		}

		printf("\n");
    }
    return NULL;
}
void * klawiatura1()
{
    printf("keyboard active!\n");
    sleep(5);
  int k = 0;
  for(;;)
    {
    /*  k=getchar(); */ /* getch() */
    k=getch();	      
      if (k==120) {
	sleep_time--;
      }
      if (k==115) {
	sleep_time++;
      }
      if (k==97) {
	cool_nr++;
      }
      if (k==122) {
	cool_nr--;
      }
      k=0;
      
    }
}

int main(int argc, char* argv[])
{
    printf("wątków:\t%d\n",threads);
    sleep(1);

    pthread_t workers[threads];		/* identyfikatory wątków - wykonawców */
    pthread_attr_t attr;  		/* atrybuty watku */
    pthread_t klawiatura;		/* identyfikator Wątku obsługującego klawiaturę*/
    pthread_attr_init( &attr );		/* inicjalizuj strukture z atrybutami*/

    pthread_create(&klawiatura,NULL,klawiatura1,NULL);
    printf("keyboard create process ends\n");
    sleep(1);
    char str[4];
    int i=0;

    for (i=0;i<threads;i++)
	{
	    sprintf (str,"%d",i);
	    pthread_create(&workers[i], &attr, worker,str);	/* Tworzymy watki-wykonawcow */
	    printf ("stworzono wątek nr:\t%s\n",str);
	}

    manager(NULL);						/* Sam realizuj zadanie zarzadcy*/

    
    for (i=0;i<threads;i++)
	{
	    pthread_join(workers[i], NULL);			/* Czekaj na zakonczenie watkow - wykonawcow */
	}
    return 0;
}
