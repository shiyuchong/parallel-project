#include<stdio.h>

int main(){
	FILE* out; 	
	out = fopen("input.txt","w");
	for(int i=0;i<1000;i++){
		fprintf(out,"%d ",i);
	}
	
	return 0;
}

