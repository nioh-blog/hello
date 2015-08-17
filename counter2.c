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
int threads_max=40;

int counter1=0;
int counter1_start=180000;
int counter1_stop=1000000;
int counter1_warn=200000;
int counter_de_nr=0;// tworzone /pracujące/ wątki
int koniec=0;
int sleep_time=750;


int *ile_watk;

pthread_cond_t warunek1 = PTHREAD_COND_INITIALIZER;

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER; //counter_de_nr




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
  printf("counter_in\n");
  for(;;)
    {
      pthread_mutex_lock(&mutex1);
      counter1++;
      pthread_mutex_unlock(&mutex1);
      usleep(900);
    }
  return NULL;
}

void* manager(void *arg)
{
  printf("manager\n");
  for(;;) 
    {
      pthread_mutex_lock(&mutex1);
      if ( counter1>=counter1_warn ) 
	pthread_cond_signal(&warunek1);
      pthread_mutex_unlock(&mutex1);
      usleep(900);
    }
  return NULL;
}

void* counter_de(void *arg)
{
  printf("counter_de\n");
  char t = *((int*) arg);
  pthread_mutex_lock(&mutex2);
  counter_de_nr++;
  pthread_mutex_unlock(&mutex2);

  while(koniec!=t) //działa /czeka/ dopóki zmienna <koniec> nie będzie równa numerowi nadanemu temu wątkowi
    {
      pthread_mutex_lock(&mutex1);
      pthread_cond_wait(&warunek1 ,&mutex1);
      counter1--;
      pthread_mutex_unlock(&mutex1);
      usleep(90);
    }
  pthread_mutex_unlock(&mutex1); //nie wiem czy potrzebne
  koniec=-1; //zmienna globalna

  pthread_mutex_lock(&mutex2);
  counter_de_nr--;
  pthread_mutex_unlock(&mutex2);
  pthread_exit(NULL);
  return NULL;
}

void* klawiatura1(void *w)
{
  unsigned long int *p=(unsigned long int*)w;
  pthread_t *w2=(pthread_t*)w;
  printf("keyboard active!\n");
  int str3[threads_max];
  sleep(1);
  int k=0;
  int i=0;
  for(;;)
    {
    k=getch();	      
    if (k==120) {
      sleep_time=sleep_time-10;
      k=0;
    }
    if (k==115) {
      sleep_time=sleep_time+10;
      k=0;
    }
    if (k==107) {
      koniec=counter_de_nr; // press k to kill last thread
      //pthread_cancel(p[counter_de_nr-1]); //wrong!
      //if (koniec==-1)
      //counter_de_nr--;
      k=0;
    }
    if (k==110 && counter_de_nr<threads_max){
      //printf("%d\t",counter_de_nr);
      	str3[counter_de_nr]=counter_de_nr+1;
      pthread_create(&w2[counter_de_nr], NULL, counter_de,&str3[counter_de_nr]);	/* Tworzymy watki-wykonawcow */

      k=0;
      //printf("%d\n",counter_de_nr);
      //sleep(3);      
    }


    if (k==97) {
      //cool_nr++;
      printf("\n\n\nc1:\t%d\tsleep:\t%d\n",counter1,sleep_time);
      k=0;
    }
    if (k==122) {
      //cool_nr--;
      printf("\n\n\ncounter_de_nr:\t%d\n",counter_de_nr);
      for (i=0;i<counter_de_nr;i++)
	{
	  printf("%lu\t",(unsigned long int)p[i]);
	  //printf("%d \n",i);
	}
      
      printf("\n\n");
      k=0;
    }
    k=0;
    }
}

int main(int argc, char* argv[])
{
  counter1=counter1_start;
  int i=0;

  pthread_t workers[threads_max];	/* ident. procesów dekrease */
  for (i=0;i<threads_max;i++)
    workers[i]=0;
  
  pthread_t klawiatura;		/* identyfikator Wątku obsługującego klawiaturę*/
  pthread_t counter;            /* ident. w. obsł. proces zwiekszania wartości*/  
  pthread_t manage;

  pthread_create(&klawiatura, NULL, klawiatura1,(void*)workers);
  pthread_create(&counter, NULL, counter_in, NULL);	
  pthread_create(&manage, NULL, manager,NULL);						
  
  sleep(1000);

  for (i=0;i<counter_de_nr;i++)
    pthread_join(workers[i], NULL);			/* Czekaj na zakonczenie watkow - wykonawcow */

  return 0;
}
