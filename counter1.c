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
      //printf("counter increase- for;;!\n");
      pthread_mutex_lock(&mutex1);
      //      printf("counter increase- mutex locked!\n");
      counter1++;
      pthread_mutex_unlock(&mutex1);
      usleep(50);
    }
  return NULL;
}

void* manager(void *arg)
{
  for(;;) /* Petla nieskonczona */
    {
      
      printf("manager! \n");
      pthread_mutex_lock(&mutex1);
      if ( counter1>=counter1_warn ) 
	{
	    pthread_cond_signal(&warunek1); 
	}
      pthread_mutex_unlock(&mutex1);
      sleep(2);
    }
    return NULL;
}

void* counter_de(void *arg)
{
  
  //printf ("%s\t\n",(char[]) arg);
  pthread_mutex_lock(&mutex1);
  for(;;)
    {
      pthread_cond_wait(&warunek1 ,&mutex1); 
      counter1--;
      usleep(500);
    }
  pthread_mutex_unlock(&mutex1);
  return NULL;
}

void* klawiatura1()
{
  printf("keyboard active!\n");
  sleep(1);
  int k = 0;
  for(;;)
    {
    /*  k=getchar(); */ /* getch() */
    k=getch();	      
    if (k==120) {
      //sleep_time--;
    }
    if (k==115) {
      //sleep_time++;
    }
    if (k==97) {
      //cool_nr++;
      printf("c1:\t%d\n",counter1);
    }
    if (k==122) {
      //cool_nr--;
    }
    k=0;
    }
}

int main(int argc, char* argv[])
{
  counter1=counter1_start;
  pthread_t workers[threads_max];	/* ident. procesów dekrease */
  pthread_t counter;                  /* ident. procesu increase */
  pthread_attr_t attr;  		/* atrybuty watku */
  pthread_t klawiatura;		/* identyfikator Wątku obsługującego klawiaturę*/
  pthread_attr_init( &attr );		/* inicjalizuj strukture z atrybutami*/
  pthread_create(&klawiatura,NULL,klawiatura1,NULL);
  char str[4];
  int i=0;
  for (i=0;i<threads_max;i++)
    {
      sprintf (str,"%d",i);
      //pthread_create(&workers[i], &attr, counter_de,str);	/* Tworzymy watki-wykonawcow */
    }
  printf("counter_de: %d\n",i);

  pthread_create(&counter, &attr, counter_in, NULL);	/* Tworzymy watki-wykonawcow */
  manager(NULL);						/* Sam realizuj zadanie zarzadcy*/
  

  for (i=0;i<threads;i++)
	{
	    pthread_join(workers[i], NULL);			/* Czekaj na zakonczenie watkow - wykonawcow */
	}
    return 0;
}
