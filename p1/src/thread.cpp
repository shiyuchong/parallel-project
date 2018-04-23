/*
copyright xu jing Ðì¾¸ 16337262 shiyuchongf@126.com
*/
#include<pthread.h> 
#include<windows.h>
#include<stdio.h>
#include<iostream>
#include<queue>
using namespace std;
queue<int > overall;							//queue 
int sum = 0;									//num of total insertion||extraction 
pthread_mutex_t mutex_in;						//mutex lock for insertion 
pthread_mutex_t	mutex_out;						//mutex lock for extraction 
FILE * in;										//input file
FILE * out;										//output file

void* insert(void* id){
	int temp;
	while(sum<1000){	
		pthread_mutex_lock(&mutex_in);			//lock
		sum++;
		if(sum>1000){				//in case of too many insertion
			pthread_mutex_unlock(&mutex_in);	
			return NULL;
		}
		fscanf(in,"%d",&temp);		//get data
		overall.push(temp);			//insert 
		fprintf(out,"thread %d inserting %d\n",*((int *)id),temp); //output
		pthread_mutex_unlock(&mutex_in);		//unlock
	}
}
 
void* extract(void* id){
	int temp;
	while(sum>=0){
		pthread_mutex_lock(&mutex_out);			//lock
		sum--;
		if(sum<0){								//in case of too many extraction
			pthread_mutex_unlock(&mutex_out);
			return NULL;
		}
		temp=overall.front();					//get data
		overall.pop();
		fprintf(out,"thread %d extracting %d\n",*((int *)id),temp);	//output
		pthread_mutex_unlock(&mutex_out);		//unlock
	}
}           

int main(){
	pthread_t id[64];						//store thread id
	int ret, i;								
	in = fopen("input.txt","r");			//input&output file
	out = fopen("output.txt","w");
	
	pthread_mutex_init(&mutex_in,NULL);		//initilize mutex locks
	pthread_mutex_init(&mutex_out,NULL);
	pthread_mutex_lock(&mutex_in);			//give other thread chances.
	pthread_mutex_lock(&mutex_out);
	
	void* (*p)(void*);
	p = insert;
	for(i=0;i<64;i++){
		ret = pthread_create(&id[i],NULL,p,&id[i]);			//creat thread
		if(ret!=0)							//in case failed to creat thread 
			cout<<"no"<<i<<" ";
	}
	pthread_mutex_unlock(&mutex_in);		//start signal
	while(sum<1000){
	Sleep(64);								//wait and give time to threads
	}
	Sleep(16);								//wait till all threads finished
	pthread_mutex_lock(&mutex_in);
	pthread_mutex_destroy(&mutex_in);		//destory locks
	fprintf(out,"\n\n");
	
	//extration
	sum = 1000;								//initialize count.	creat threads	
	p = extract;
	for(i=0;i<64;i++){
		ret = pthread_create(&id[i],NULL,p,&id[i]);
		if(ret!=0)
			cout<<"nop"<<i<<" ";
	}
	
	pthread_mutex_unlock(&mutex_out);		//start
	while(sum>=0){
	Sleep(64);	
	}
	
	Sleep(16);
	pthread_mutex_lock(&mutex_out);			//destroy locks and quit
	pthread_mutex_destroy(&mutex_out);
	return 0;
}    
