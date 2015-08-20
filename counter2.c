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
int counter1=0; //licznik główny - rośnie-maleje
int counter1_start=180000; // licznik główny -offset
//int counter1_stop=1000000;
int counter1_warn=200000; // górny limit licznika
int counter_de_nr=0;// tworzone /pracujące/ wątki, ilość
int koniec=0;
int sleep_time=750;
//int *ile_watk;
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
      usleep(1000);
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
      usleep(100);
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
      usleep(10000);
    }
  pthread_mutex_unlock(&mutex1); //nie wiem czy potrzebne
  koniec=-1; //zmienna globalna
  pthread_mutex_lock(&mutex2);
  counter_de_nr--;
  pthread_mutex_unlock(&mutex2);
  pthread_exit(NULL);
  return NULL;
}

void* klawiatura1(void *workers_main)
{
  printf("keyboard active!\n");

  pthread_t *workers_local=(pthread_t*)workers_main; //unsigned long int *p=(unsigned long int*)workers_main;
  int threads_names[threads_max];
  int k=0;
  int i=0;
  for(;;)
    {
    k=getch();	      
    if (k==120) {
      sleep_time=sleep_time-10;
    }
    if (k==115) {
      sleep_time=sleep_time+10;
    }
    if (k==107) {
      koniec=counter_de_nr; // press k to kill last thread
     }
    if (k==110 && counter_de_nr<threads_max){
      threads_names[counter_de_nr]=counter_de_nr+1;
      pthread_create(&workers_local[counter_de_nr], NULL, counter_de,&threads_names[counter_de_nr]);	/* Tworzymy watki-wykonawcow */
    }
    if (k==97) {
      printf("\n\n\ncounter1:\t%d\tsleep_time:\t%d\n",counter1,sleep_time);
    }
    if (k==122) {
      printf("\n\n\ncounter_de_nr:\t%d\n",counter_de_nr);
      for (i=0;i<counter_de_nr;i++)
	  printf("%lu\t",(unsigned long int)workers_local[i]);
      printf("\n\n");
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