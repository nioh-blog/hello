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
int threads_max=240;

int counter1=0;
int counter1_start=180000;
int counter1_stop=1000000;
int counter1_warn=200000;
int counter_de_nr=0;// liczyć będziemy tworzone /pracujące/ wątki

int sleep_time=750;

int *ile_watk;

pthread_cond_t warunek1 = PTHREAD_COND_INITIALIZER;
//pthread_cond_t warunek2 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
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
      //printf("+");
      counter1++;
      pthread_mutex_unlock(&mutex1);
      usleep(900);
    }
  return NULL;
}

void* manager(void *arg)
{
  for(;;) /* Petla nieskonczona */
    {
      //      printf("sleep_time:%d\tcounter1:%d\n",sleep_time,counter1);
      //printf("manager! \n");
      //pthread_mutex_lock(&mutex1);
      if ( counter1>=counter1_warn ) 
	{
	    pthread_cond_signal(&warunek1);
	    //printf("manager cond signal! \n"); 
	}
      //pthread_mutex_unlock(&mutex1);
      usleep(sleep_time);
    }
    return NULL;
}

void* counter_de(void *arg)
{
  int k=0;
  char t;
  t = *((int*) arg);
  pthread_mutex_lock(&mutex2);
  counter_de_nr++;
  pthread_mutex_unlock(&mutex2);
  //printf ("%s\t\n",(char[]) arg);
  printf("\n");
  //pthread_mutex_lock(&mutex1);
  for(;;)
    {
      pthread_mutex_lock(&mutex1);
      pthread_cond_wait(&warunek1 ,&mutex1);
      printf("proc.nr:%d pthread_self:%x counter1:%d\t\n",t,pthread_self(),counter1);
      //printf ("%d\t",t);
      
      counter1--;
      pthread_mutex_unlock(&mutex1);
      usleep(50);
    }
  return NULL;
}

void* klawiatura1(void *w)
{
  printf("keyboard active!\n");
  sleep(1);
  int k,i = 0;
  for(;;)
    {
    /*  k=getchar(); */ /* getch() */
    k=getch();	      
    if (k==120) {
      sleep_time=sleep_time-10;
    }
    if (k==115) {
      sleep_time=sleep_time+10;
    }
    if (k==97) {
      //cool_nr++;
      printf("\n\n\nc1:\t%d\tsleep:\t%d\n",counter1,sleep_time);
    }
    if (k==122) {
      //cool_nr--;
      printf("\n\n\ncounter_de_nr:\t%d\n",counter_de_nr);
      for (i=0;i<threads_max;i++)
	{
	  printf("%d\t%d",&w+i);
	  //printf("%d ",i);
	}
      
      printf("\n\n");
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
  pthread_create(&klawiatura,NULL,klawiatura1,(void*)workers);
  char str1[4];
  int str2[threads_max];
  int i=0;
  for (i=0;i<threads_max;i++)
    {
      //sprintf (str1,"%d",i);
      str2[i]=i;
      pthread_create(&workers[i], &attr, counter_de,&str2[i]);	/* Tworzymy watki-wykonawcow */
      //sprintf (str,"%s","....");
    }
  printf("counter_de: %d\n",i);
  for (i=0;i<threads_max;i++)
    printf("%d\t",workers[i]);


  ile_watk=&i;
  pthread_create(&counter, &attr, counter_in, NULL);	/* Tworzymy watki-wykonawcow */
  manager(NULL);						/* Sam realizuj zadanie zarzadcy*/
  

  for (i=0;i<threads;i++)
	{
	    pthread_join(workers[i], NULL);			/* Czekaj na zakonczenie watkow - wykonawcow */
	}
    return 0;
}
