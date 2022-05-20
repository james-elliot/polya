#define NB_PROCESS 16
#define NB_SLICES 32


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

//#define SIZE 1000000000
#define SIZE 1000000000

typedef
struct _Params {
  int16_t *tab;
  int *p;
  int num;
  int start;
  int end;
} Params;

void build_primes (int *p,int sto) {
  p[0]=2;
  int k=1;
  int i = 3;
  while(true) {
	int j = 0;
	int st = sqrt(i+0.01);
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
  int16_t *tab=pr->tab;
  int *p=pr->p;
  int start = pr->start;
  int end = pr->end;
  bool go_on;
  if (start<3) start=3;
  do {
    go_on=false;
    for (int i=start;i<end;i++) {
      if (tab[i]==0) {
	int j = 0;
	int st = sqrt(i+0.01);
	while(p[j]<=st)  {
	  if ((i % p[j]) == 0) break;
	  j=j+1;
	}
	if (p[j] > st)  tab[i]=1;
	else  {
	  int k = i/p[j];
	  if (tab[k]>0) tab[i]=1+tab[k];
	  else {
	    tab[i]=-p[j];
	    go_on = true;
	  }
	}
      }
      else if (tab[i]<0) {
	int k = i/(-tab[i]);
	if (tab[k]>0) tab[i]=1+tab[k];
	else go_on = true;
      }
    }
  } while(go_on);
  return 0;
}

#define STACK_SIZE 65536
#define SLICE (SIZE/NB_SLICES)
int main() {
  char *stack[NB_PROCESS];
  char *stackTop[NB_PROCESS];
  Params pr[NB_PROCESS];
  int pids[NB_PROCESS]={};
  
  for (int i=0;i<NB_PROCESS;i++) {
    //    stack[i]=malloc(STACK_SIZE);
    stack[i] = mmap(NULL, STACK_SIZE, PROT_READ | PROT_WRITE,
		    MAP_PRIVATE | MAP_ANONYMOUS | MAP_STACK, -1, 0);
    if (stack [i] == MAP_FAILED)  {perror("mmap");exit(-1);}
    stackTop[i]=stack[i]+STACK_SIZE;
  }
  
  int16_t *tab=(int16_t *)calloc(SIZE,sizeof(int16_t));
  int sto = sqrt(SIZE+0.01);
  int *p=(int *)malloc(sizeof(int)*sto);
  
  tab[1]=0;tab[2]=1;
  int sum = 1-1;
  int first=0,last=0;
  int imax=0,maxi=-10000000;
  int imin=0, mini=10000000;
  int start=0;
  build_primes(p,sto);
  
  //  do_the_job();
  int nb_available=NB_PROCESS;
  int rem_slices=NB_SLICES;
  while (rem_slices>0) {
    int num;
    if (nb_available>0) {
      for (num=0;num<NB_PROCESS;num++)
	if (pids[num]==0) break;
      if (num==NB_PROCESS) {perror("pids");exit(-1);}
      pr[num].tab=tab;pr[num].p=p;pr[num].num=num;
      pr[num].start=start;pr[num].end=start+SLICE;
      if (rem_slices==1) pr[num].end=SIZE;
      pids[num]=clone((int(*)(void *))do_the_job,stackTop[num],CLONE_VM|SIGCHLD,&pr[num]);
      printf("%d\n",pids[num]);
      if (pids[num]==-1) {perror("clone");exit(-1);}
      start+=SLICE;
      nb_available--;
      rem_slices--;
    }
    else {
      int status;
      int pid=waitpid(-1,&status,0);
      if (pid==-1) {
	if (errno==ECHILD) break;
	perror("waitpid");
	exit(-1);
      }
      int num;
      for (num=0;num<NB_PROCESS;num++) if (pid==pids[num]) break;
      if (num==NB_PROCESS) {
	perror("Zorglub");
	exit(-1);
      }
      pids[num]=0;
      nb_available++;
    }
  }
  
  for (int i=3;i<SIZE;i++) {
    while (tab[i]<=0) usleep(10);
    if ((tab[i]%2)==0) sum=sum+1;
    else sum=sum-1;
    if (sum > 0) {last=i;}
    if ((sum > 0) && (first == 0)) {first=i;}
    if (sum > maxi) {maxi=sum;imax=i;}
    if (sum < mini) {mini=sum;imin=i;}
  }
  printf("first=%d,imax=%d,maxi=%d,imin=%d,mini=%d,last=%d\n",
	 first,imax,maxi,imin,mini,last);
}
