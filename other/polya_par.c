//#define MAXV 100000000000L
#define MAXV 1000000000L
#define NB_PROCESS 16

#define _GNU_SOURCE
#include <math.h>
#include <malloc.h>
#include <stdbool.h>
#include <stdint.h>
#include <sched.h>
#include <sys/wait.h>
#include <unistd.h>
#include<assert.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/mman.h>


#define SIZE ((MAXV/8)+1)

typedef
struct _Params {
  uint8_t *tab;
  int64_t *p;
  int64_t start;
  int64_t end;
} Params;

void build_primes (int64_t *p,int sto,int nb_primes) {
  p[0]=2;
  int k=1;
  int i = 3;
  while(true) {
    if (k>=nb_primes) {perror("nb_primes");exit(-1);}
    int j = 0;
    int st = sqrt((double)i+0.01);
    while(true)  {
      if (((i % p[j]) == 0) || (p[j] > st)) {break;}
      j=j+1;
    }
    if (p[j] > st) {
      p[k++]=i;
      if (i > sto) {break;}
    }
    i=i+2;
  }
}

int do_the_job(Params *pr) {
  uint8_t *tab=pr->tab;
  int64_t *primes =pr->p;
  int64_t a = pr->start;
  int64_t b = pr->end;

  int64_t bsup = sqrt(b+0.01);
  for (int i=0;;i++) {
    int p = primes[i];
    if (p>bsup) {break;}
    int64_t k = a/p;
    int64_t mp = k*p;
    if (mp<a) {mp=mp+p;k=k+1;}
    while(mp<=b) {
      int64_t i1=mp/8,i2=mp%8;
      int64_t j1=k/8,j2=k%8;
      if ((tab[j1]&(1<<j2))==0) {tab[i1]=tab[i1]|(1<<i2);}
      mp=mp+p;
      k=k+1;
    }
  }
  return 0;
}


#define STACK_SIZE (1024*1024)
int main() {
  char *stack[NB_PROCESS];
  char *stackTop[NB_PROCESS];
  Params pr[NB_PROCESS];
  int pids[NB_PROCESS]={};
  
  for (int i=0;i<NB_PROCESS;i++) {
    stack[i] = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
		    MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (stack [i] == MAP_FAILED)  {perror("mmap");exit(-1);}
    stackTop[i]=stack[i]+STACK_SIZE;
  }
  
  int sto = sqrt((double)MAXV+0.01);
  int nb_primes = 1.25*((double)sto)/log(sto);
  int64_t *primes=(int64_t *)malloc(sizeof(int64_t)*nb_primes);
  build_primes(primes,sto,nb_primes);
  uint8_t *tab = (uint8_t *)calloc(SIZE,sizeof(uint8_t));
  tab[0]=0x2;

  int64_t a = 3;
  int64_t b;
  while (a<MAXV) {
    b=2*a-1;
    printf("%ld %ld\n",a,b);
    if (b>=MAXV) {b=MAXV-1;}
    if ((b-a)<65536) {
      pr[0].tab=tab;pr[0].p=primes;
      pr[0].start=a;pr[0].end=b;
      do_the_job(&pr[0]);
    }
    else { 
      for(int num=0;num<NB_PROCESS;num++) {
	pr[num].tab=tab;pr[num].p=primes;
	if (num==0) pr[num].start=a;
	else pr[num].start=a+((b-a)*num)/NB_PROCESS+1;
	pr[num].end=a+((b-a)*(num+1))/NB_PROCESS;
	pids[num]=clone((int(*)(void *))do_the_job,
			stackTop[num],
			CLONE_VM|SIGCHLD,
			&pr[num]);
      }
      int finished=0;
      while (finished!=NB_PROCESS) {
	int status;
	int pid=waitpid(-1,&status,0);
	if (pid==-1) {
	  perror("waitpid");
	  exit(-1);
	}
	finished++;
      }
    }
    a=b+1;
  }

  
  int64_t first=0, last=0;
  int64_t imax=0, maxi=-10000000;
  int64_t imin=0, mini=10000000;
  int64_t num = 0 ;
  for (int64_t i=3;i<8;i++) {
    int64_t i1=i/8,i2=i%8;
    if ((tab[i1]&(1<<i2))==0) {num=num-1;}
    else {num=num+1;}
    if (num > 0) {last=i;}
    if ((num > 0) && (first == 0)) {first=i;}
    if (num > maxi) {maxi=num;imax=i;}
    if (num < mini) {mini=num;imin=i;}
  }

  int64_t i1=1;
  uint8_t i2=0;
  while (true) {
    if ((tab[i1]&(1<<i2))==0) num--; else num++;
    if (num > 0) {
      last=8*i1+i2;
      if (first==0) {first=last;}
      if (num > maxi) {maxi=num;imax=last;}
    }
    if (num < mini) {mini=num;imin=8*i1+i2;}
    i2++;
    if (i2==8) {
      i2=0;
      i1++;
      if (i1==MAXV/8) break;
    }
  }

  
  printf(
	 "first=%ld,imax=%ld,maxi=%ld,last=%ld,imin=%ld,mini=%ld\n",
	 first,imax,maxi,last,imin,mini);
}
