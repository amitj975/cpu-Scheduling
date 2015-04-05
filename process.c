#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include <pthread.h>
#include<semaphore.h>
#include<stdint.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>

int count=0,fcount=0,done=0;
pthread_mutex_t mut;
unsigned int microseconds=10000;

int process[500][6];
clock_t ttime[500][4];
void selectsort(int *array,int n,int f){
		int c, d, position, swap;
   //printf("Enter %d integers\n", n);

   //for ( c = 0 ; c < n ; c++ )
      //scanf("%d", &array[c]);

   for ( c = 0 ; c < ( n - 1 ) ; c++ )
   {
      position = c;

      for ( d = c + 1 ; d < n ; d++ )
      {
         if ( array[position] > array[d] )
            position = d;
      }
      if ( position != c )
      {
         swap = array[c];
         array[c] = array[position];
         array[position] = swap;
      }
	process[f][5]=c;
   }

}
void* injector(void *);

void* waitx(void * para){
	int *f=(int *) para;

	waitpid(process[(*f)][0],NULL,0);
	process[(*f)][4]=1;
	ttime[process[(*f)][3]][3]=clock()-ttime[process[(*f)][3]][0];
}

int main()
{
	pid_t pid1,pid2;
	long long int i,j,total=0,x,y,flag,num=0;
	char c;

	pthread_t tid;



printf("\t\tWhich scheduling algorithm do you want :-\n");
printf("\t\t Press 		for\n :-");
printf("\t\t f			FCFS\n");
printf("\t\t p			Priority Scheduling\n");
printf("\t\t s			SJF non pre-emptive\n");
printf("\t\t b			SJF pre-emptive\n");
printf("\t\t r			Round Robin\n");
printf("\t\t m			multilevel feedback queue\n");
scanf("%c",&c);

	pthread_create(&tid,NULL,injector,NULL);

	switch(c){
	   case 'f':	while(done==0||count!=0)
		    {
			while(count)
			{
			kill(process[fcount-count][0],SIGCONT);
			ttime[process[fcount-count][3]][1]=clock();
			ttime[process[fcount-count][3]][2]=ttime[process[fcount-count][3]][1]-	ttime[process[fcount-count][3]][0];
			waitpid(process[fcount-count][0],NULL,0);

			ttime[process[fcount-count][3]][3]=clock()-
			ttime[process[fcount-count][3]][0];
			pthread_mutex_lock(&mut);
			count--;
			pthread_mutex_unlock(&mut);
			}
		}

			break;
	case 's':
		while(done==0||count>0)
		    {
			int key[4];
			x=count;
			y=fcount;

			for(i=y-x+1;i<y;i++){


				key[0]=process[i][0];
				key[1]=process[i][1];
				key[2]=process[i][2];
				key[3]=process[i][3];

					j=i-1;
				while(j>=0 && process[j][1]>key[1]){
					process[j+1][0]=process[j][0];
					process[j+1][1]=process[j][1];
					process[j+1][2]=process[j][2];
					process[j+1][3]=process[j][3];
					j=j-1;
				}
				process[j+1][0]=key[0];
				process[j+1][1]=key[1];
				process[j+1][2]=key[2];
				process[j+1][3]=key[3];

			}


			while(x)
			{

			kill(process[y-x][0],SIGCONT);
			ttime[process[y-x][3]][1]=clock();
			ttime[process[y-x][3]][2]=ttime[process[y-x][3]][1]-	ttime[process[y-x][3]][0];
			waitpid(process[y-x][0],NULL,0);

			ttime[process[y-x][3]][3]=clock()-
			ttime[process[y-x][3]][0];
			x--;
			}

			pthread_mutex_lock(&mut);
			count=count-y;
			pthread_mutex_unlock(&mut);
		}
		break;

	case 'b':
		while(done==0||count>0)
		    {
			int key[6];
			x=count;
			y=fcount;

			for(i=y-x+1;i<y;i++){


				key[0]=process[i][0];
				key[1]=process[i][1];
				key[2]=process[i][2];
				key[3]=process[i][3];
				key[5]=process[i][5];

					j=i-1;
				while(j>=0 && process[j][1]>key[1]){
					process[j+1][0]=process[j][0];
					process[j+1][1]=process[j][1];
					process[j+1][2]=process[j][2];
					process[j+1][3]=process[j][3];
					process[j+1][5]=process[j][5];
					j=j-1;
				}
				process[j+1][0]=key[0];
				process[j+1][1]=key[1];
				process[j+1][2]=key[2];
				process[j+1][3]=key[3];
				process[j+1][5]=process[j][5];
			}


			while(x)
			{
			if(y==fcount)
			{
				kill(process[y-x][0],SIGCONT);
				ttime[process[y-x][3]][1]=clock();
				ttime[process[y-x][3]][2]=ttime[process[y-x][3]][1]-	ttime[process[y-x][3]][0];
				waitpid(process[y-x][0],NULL,0);

				ttime[process[y-x][3]][3]=clock()-
				ttime[process[y-x][3]][0];
				x--;
			}
			else{
				if(process[y-x][1]>process[fcount][5]){
						y=y+fcount;
						x=x+fcount;
						kill(process[y-x][0],SIGCONT);
						ttime[process[y-x][3]][1]=clock();
						ttime[process[y-x][3]][2]=ttime[process[y-x][3]][1]-	ttime[process[y-x][3]][0];
						waitpid(process[y-x][0],NULL,0);

						ttime[process[y-x][3]][3]=clock()-
						ttime[process[y-x][3]][0];
						x--;
						y=y-fcount;
						x=x-fcount;

				}
				else{
							kill(process[y-x][0],SIGCONT);
							ttime[process[y-x][3]][1]=clock();
							ttime[process[y-x][3]][2]=ttime[process[y-x][3]][1]-	ttime[process[y-x][3]][0];
							waitpid(process[y-x][0],NULL,0);

							ttime[process[y-x][3]][3]=clock()-
							ttime[process[y-x][3]][0];
							x--;
				}
			}
			}

			pthread_mutex_lock(&mut);
			count=count-y;
			pthread_mutex_unlock(&mut);
		}
		break;

	case 'p':
		while(done==0||count>0)
		    {
			int key[4];
			x=count;
			y=fcount;

			for(i=y-x+1;i<y;i++){


				key[0]=process[i][0];
				key[1]=process[i][1];
				key[2]=process[i][2];
				key[3]=process[i][3];

					j=i-1;
				while(j>=0 && process[j][2]>key[2]){
					process[j+1][0]=process[j][0];
					process[j+1][1]=process[j][1];
					process[j+1][2]=process[j][2];
					process[j+1][3]=process[j][3];
					j=j-1;
				}
				process[j+1][0]=key[0];
				process[j+1][1]=key[1];
				process[j+1][2]=key[2];
				process[j+1][3]=key[3];

			}


			while(x)
			{

			kill(process[y-x][0],SIGCONT);
			ttime[process[y-x][3]][1]=clock();
			ttime[process[y-x][3]][2]=ttime[process[y-x][3]][1]-	ttime[process[y-x][3]][0];
			waitpid(process[y-x][0],NULL,0);

			ttime[process[y-x][3]][3]=clock()-
			ttime[process[y-x][3]][0];
			x--;
			}

			pthread_mutex_lock(&mut);
			count=count-y;
			pthread_mutex_unlock(&mut);
		}
		break;

	case 'r':
		flag=0;
		while(done==0 || flag==0)
		    {
			flag=1;

			for(i=0;i<fcount;i++){

				if(process[i][4]==0)
				{
					flag=0;
					kill(process[i][0],SIGCONT);
					ttime[process[i][3]][2]+=clock()-ttime[process[i][3]][1];
					usleep(microseconds);
					kill(process[i][0],SIGSTOP);
					ttime[process[i][3]][1]=clock();
				}

			}

		}

			break;
	case 'm':
		flag=0;
		while(done==0 || flag==0)
		    {
			flag=1;
			num++;

			for(i=0;i<fcount;i++){

				if(process[i][4]==0)
				{
					flag=0;
					kill(process[i][0],SIGCONT);
					ttime[process[i][3]][2]+=clock()-ttime[process[i][3]][1];
					usleep((microseconds)*num);
					kill(process[i][0],SIGSTOP);
					ttime[process[i][3]][1]=clock();
				}

			}

		}

			break;


    }
	for(i=0;i<fcount;i++){
		total+=ttime[i][2];
	}

	printf("\nTotal waiting time is %lld and average waiting time is %lld\n",total,total/20);

	for(i=0;i<fcount;i++){
		total+=ttime[i][3];
	}

	printf("Average turnaround time is %lld\n",total/20);
}
s
void* injector(void * para){
	int t,i,j;
	pid_t pid;
	pthread_t tid1[200];
	process[count][1]=rand()%50000;
	process[count][2]=rand()%10;

	pid=fork();

	if(pid==0){
		usleep(microseconds/10);
		printf("Child no.%d started my no. is %d burst time is %d and my priority is %d\n",getpid(),fcount,process[fcount][1],process[fcount][2]);
		int i,*a;
		j=process[count][1];
		// 	printf("Child no.%d sorting the array\n",getpid());
		a=(int *)malloc(sizeof(int)*j);
		for(i=0;i<j;++i)
			a[i]=rand()%50000;
		//printf("Child no.%d sorting the array\n",getpid());
		selectsort(a,j,fcount);
		printf("Child no.%d sorted the array\n",getpid());

		}

	else{
			int *y = malloc(sizeof(int));
			*y=fcount;
			pthread_create(&tid1[fcount],NULL,waitx,y);
			kill(pid,SIGSTOP);
			ttime[fcount][0]=clock();
			ttime[fcount][1]=clock();
			printf("injecting process %d\n",pid);
			process[count][0]=pid;

			process[count][3]=fcount;

			pthread_mutex_lock(&mut);
			count++;
			fcount++;
			pthread_mutex_unlock(&mut);



		if(fcount<20){
		injector(NULL);
		}
		else{
			done=1;
		}

	}


}

