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
int threads_max=100;
int counter1=0; //licznik główny - rośnie-maleje
int counter1_start=180000; // licznik główny -offset
//int counter1_stop=1000000;
int counter1_warn=200000; // górny limit licznika
int counter_de_nr=0;// tworzone /pracujące/ wątki, ilość
int koniec=0;
int sleep_time=1000;
int manager_cmd=0;
int key=0;
//int *konsola = fopen("/dev/tty", "w");
//int *ile_watk;
pthread_cond_t warunek1 = PTHREAD_COND_INITIALIZER;
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER; //counter_de_nr

/* dziwny kod zwiazany z enterem*/
int getch (void)
{
        int key1;
        struct termios oldSettings, newSettings;    /* stuktury z ustawieniami terminala */
 
        tcgetattr(STDIN_FILENO, &oldSettings);    /* pobranie ustawień terminala */
        newSettings = oldSettings;
        newSettings.c_lflag &= ~(ICANON | ECHO);    /* ustawienie odpowiednich flag */
        tcsetattr(STDIN_FILENO, TCSANOW, &newSettings);    /* zastosowanie ustawień */
	printf("getch():manager_cmd:%d\n",manager_cmd);
	if (manager_cmd==1){
	  key1=107;
	}
	else
	  key1 = getchar();    /* pobranie znaku ze standardowego wejścia */
        tcsetattr(STDIN_FILENO, TCSANOW, &oldSettings);    /* przywrócenie poprzednich ustawień terminala */
        return key1;
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
      usleep(sleep_time);
    }
  return NULL;
}

void* manager(void *arg)
{
  printf("manager\n");
  int c1=0;
  int c2=0;
  int first=0;
  time_t cur_time1 = time(NULL);
  time_t cur_time2;
  FILE *konsola = fopen("/dev/tty", "w");
  for(;;) 
    {
      //pthread_mutex_lock(&mutex1);
      //if ( counter1>=counter1_warn ) 
      //pthread_cond_signal(&warunek1);  // to all waiting threads: wake up!
      // mimo to nie pomaga, za szybko rośnie counter1!
      //c1=counter1;
      //pthread_mutex_unlock(&mutex1);
      
      if ( first==0 )
	  {
	    cur_time1=time(NULL);
	    //pthread_mutex_lock(&mutex1);
	    c1=counter1;
	    //pthread_mutex_unlock(&mutex1);
	    first=1;
	  }  
    


      if ( (time(NULL)-cur_time1) >= 10 && first==1)
	  {
	    c2=counter1;
	    first=0;
	    printf("after %d sec, c1 change: (c2-c1):%d\tfirst:%d\tcounter1:%d\tcounter_de_nr:%d\n",time(NULL)-cur_time1,c2-c1,first,counter1,counter_de_nr);
	    if ((c2-c1)<-100)
	      {
		//pthread_cond_signal(&warunek1);
		manager_cmd=1;
		key=getch();//107;
		//putchar(107);
		//konsola = fopen("/dev/tty", "w");     //open the terminal screen
		//fputs(" ",konsola);
		fprintf(konsola,"\n");
		printf("COND_WAIT\n");
	      }
	  }
      //cur_time1=time(NULL);

      pthread_mutex_unlock(&mutex1);
      usleep(10000);
    }

  /*
	if ( (time(NULL)-current_time1) >= 10 )
	  {
	    current_time1=time(NULL);
	    //printf(":%d:",i-k);
	    k=i;
	  }  
	if ((i-k)<100)
	  printf(":%d:",i-k);
	if ((i-k)>100)
	  printf();
  */



  return NULL;
}

void* counter_de(void *arg)
{
  printf("counter_de\n");
  char t = *((int*) arg);
  int i;
  pthread_mutex_lock(&mutex2);
  counter_de_nr++;
  pthread_mutex_unlock(&mutex2);

  while(koniec!=t) //działa /czeka/ dopóki zmienna <koniec> nie będzie równa numerowi nadanemu temu wątkowi
    {
      pthread_mutex_trylock(&mutex1);
      //pthread_cond_wait(&warunek1 ,&mutex1);
      for (i=0;i<1;i++)
      counter1--;
      //printf("%d ",t);
      pthread_mutex_unlock(&mutex1);
      usleep(100000);
    }
  pthread_mutex_unlock(&mutex1); //nie wiem czy potrzebne
  koniec=-1; //zmienna globalna
  pthread_mutex_lock(&mutex2);
  counter_de_nr--;
  pthread_mutex_unlock(&mutex2);
  manager_cmd=0;
  key=0;
  pthread_exit(NULL);
  return NULL;
}

void* klawiatura1(void *workers_main)
{
  printf("keyboard active!\n");

  pthread_t *workers_local=(pthread_t*)workers_main; //unsigned long int *p=(unsigned long int*)workers_main;
  int threads_names[threads_max];
  //int k=0;
  int i=0;
  for(;;)
    {
      printf("key=%d\n",key);
      //if (manager_cmd==0)  
	key=getch();
    if (key==120) {
      sleep_time=sleep_time-100;
    }
    if (key==115) {
      sleep_time=sleep_time+100;
    }
    if (key==107) {
      koniec=counter_de_nr; // press k to kill last thread
     }
    if (key==110 && counter_de_nr<threads_max){
      threads_names[counter_de_nr]=counter_de_nr+1;
      pthread_create(&workers_local[counter_de_nr], NULL, counter_de,&threads_names[counter_de_nr]);	/* Tworzymy watki-wykonawcow */
    }
    if (key==97) {
      printf("\n\n\ncounter1:\t%d\tsleep_time:\t%d\n",counter1,sleep_time);
    }
    if (key==122) {
      printf("\n\n\ncounter_de_nr:\t%d\n",counter_de_nr);
      for (i=0;i<counter_de_nr;i++)
	  printf("%lu\t",(unsigned long int)workers_local[i]);
      printf("\n\n");
    }
    //key=0;
    //    manager_cmd=0;
    }
}

int main(int argc, char* argv[])
{
  printf("klawisze:a ,z ,n ,c\n");
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
