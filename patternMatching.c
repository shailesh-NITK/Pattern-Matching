# Pattern-Matching

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#define size 200
FILE *fp;
int count1=1;
//CAATTGTAAACCATATTGGTATAATAACATGATATTGTTAAAAGAAAAGTTGAAGTATGC
//char Text[30]={'C','A','A','T','T','G','T','A','A','A','C','C','A','T','A','T','T','G','G','T','A','T','A','A','T','A','A','C','A','T'};
char Pat[200];//={'E','D','C'};
char protein[]={'A','C','D','E','F','G','H','I','K','L','M','N','P','Q','R','S','T','V','W','Y'};

//	<----------------- Approximate Matching Start ------------------->

void apx(int pat_len,int text_len,int x){ 
 int TextIx=0;
 char c;
 int PatIx,k,out;
 for(TextIx=0;TextIx<=(text_len-pat_len);TextIx++){
	PatIx=0;
	k=1;
	out=1;
	rewind(fp);
	fseek(fp,(TextIx+PatIx),SEEK_CUR);
	c=fgetc(fp);
	if(c=='\n'){c=fgetc(fp);}
next1:	while(c==Pat[PatIx]){
		//printf(" %d %c %d ",PatIx,c,pat_len);
		if(PatIx==(pat_len + x) && out==1 || PatIx==pat_len-1 && out==1){
			printf("\n %d. An occurnce of %s at Location %d to %d \n",count1,Pat,TextIx,TextIx+PatIx);
			out=0;count1++;
			break;
		}
		else {  
			PatIx++;
			rewind(fp);
			fseek(fp,(TextIx+PatIx),SEEK_CUR);
			c=fgetc(fp);
			//printf(" %d %c %d ",PatIx,c,pat_len);
			if(c=='\n'){c=fgetc(fp);TextIx++;}
		}
	
	}
	c=Pat[PatIx]; 
	if(k<=x){
		k++;
		goto next1;
	}
 }

}


//	<----------------- Approximate Matching End ----------------------->

//	<----------------- Multiple Reference Character Algorithm Start ----------------------->
int pos[200];
int skip[200],count2=0;

void PreProcess_Pat(int PatLength){
char c;
int j;
for(j=0;j<size;j++){
	pos[j]=0;
	skip[j]=2*PatLength;
}

for(j=0;j<PatLength;j++){
	c=Pat[j];
	//printf(" c=%c ",c);
	pos[c]=j+1;
	skip[c]=2*PatLength-j-1;
}

}


void Pos_Skip(int PatLength, int TextLength){

 int TextIx,PatIx,last_mismatch,z;
 int pt,pt1,ref,ref1,ref_ref1,ref2,ref_ref2;
 int infix[size]={0};
 infix[Pat[0]]=1;
 last_mismatch=0;
 TextIx=PatLength;
 char c;

while(TextIx<=TextLength+1){
	rewind(fp);
	fseek(fp,(TextIx-PatLength+last_mismatch),SEEK_CUR);
	c=fgetc(fp);
	if(c==EOF){return;}
	if(c==Pat[last_mismatch]){
		{	//printf(" %c ",c);
			rewind(fp);
			fseek(fp,(TextIx-PatLength),SEEK_CUR);
			c=fgetc(fp);
			if(infix[c]){
				for(z=0,PatIx=PatLength-1;PatIx;PatIx--)  {
					rewind(fp);
					fseek(fp,(TextIx-(++z)),SEEK_CUR);
					c=fgetc(fp);
					if(c==EOF){return;}
					//printf(" %d %c ",TextIx,c);
					if(c!=Pat[PatIx]){
						last_mismatch=PatIx;
						goto next;
					}
				}
			count2++;
			printf("\n An occurnce of %s at Location %d to %d \n",Pat,TextIx-PatLength,TextIx-1);
			}
		}
	}
	//start skipping part
	next:
		ref1=TextIx;
		rewind(fp);
		fseek(fp,ref1,SEEK_CUR);
		c=fgetc(fp);
		if(c==EOF){return;}
		pt=pos[c];
		if(!pt){
			ref2=ref1+PatLength;
			rewind(fp);
			fseek(fp,ref2,SEEK_CUR);
			c=fgetc(fp);
			if(c==EOF){return;}
			pt1=pos[c];
		
			if(!pt1){
				ref_ref2 = ref2 + PatLength - pt1;
					rewind(fp);
					fseek(fp,ref_ref2,SEEK_CUR);
					c=fgetc(fp);
					if(c==EOF){return;}
				TextIx = TextIx + 3*PatLength - pt1 + 1 - pos[c];
			} 
			else{
				TextIx=TextIx + 2 *PatLength - pt1 + 1;
			}
		}
		else{
			ref_ref1=ref1+PatLength -pt;
					rewind(fp);
					fseek(fp,ref_ref1,SEEK_CUR);
					c=fgetc(fp);
					if(c==EOF){return;}
			TextIx= TextIx + skip[c] - pt + 1; 
		}
}
return; 
}

//	<----------------- Multiple Reference Character Algorithm End ----------------------->

//	<-------------------------------- SSABC Algorithm Start ----------------------------->

char x[200];//,y[30]={'C','A','A','T','T','G','T','A','A','A','C','C','A','T','A','T','T','G','G','T','A','T','A','A','T','A','A','C','A','T'};
char firstCh,lastCh;
int qsBc[200];

void preQsBc(char *x,int m,int qsBc[]){
 int i;
	for(i=0;i<size;++i){
			qsBc[i]=m+1;
			//printf(" %d ",qsBc[i]);
		}
	//printf("\n");
	for(i=0;i<m;++i){
			qsBc[x[i]]=m-i;
			//printf(" %d %d",qsBc[x[i]],x[i]);
		}
	//printf("\n");
}

////--- Pre-Processing Phase ---

void Preprocess(char *x,int m, int n){
preQsBc(x,m,qsBc);
firstCh=x[0];
lastCh=x[m-1];
}

void output(int j,int m){
	printf("\n An occurnce of %s at Location %d to %d \n",x,j,j+m-1);
}

void SSABS(char *x,int m,int n){
int j=0,i,flag=1,count=0;
char c1,c2,c3,c4;
	while(j<=n-m){
		rewind(fp);
		fseek(fp,j,SEEK_CUR);
		c1=fgetc(fp);
		rewind(fp);
		fseek(fp,(j+m-1),SEEK_CUR);
		c2=fgetc(fp);
		if(lastCh==c2 && firstCh==c1){
			//printf(" %c %c ",c1,c2);
			rewind(fp);
			fseek(fp,(j+m-2),SEEK_CUR);
			c3=fgetc(fp);
			for(i=m-2;i>0 && x[i]==c3;i--){
				if(flag==1){flag=0;
					rewind(fp);
					fseek(fp,(j+i),SEEK_CUR);
					c3=fgetc(fp);
				//continue;
				}
				rewind(fp);
				fseek(fp,(j+i-1),SEEK_CUR);
				c3=fgetc(fp);//printf(" %c ",c3);
			}
			if(i<=0){
				count++;
				output(j,m);
			}
		}
		rewind(fp);
		fseek(fp,j+m,SEEK_CUR);
		c4=fgetc(fp);
		j=j+qsBc[c4];
		//printf(" %d%c ",j,c4);
	}

printf("\n No. of Occurances %d ",count);
}

//	<---------------------------- SSABC Algorithm End ---------------------------->

//	<--------------------------Brute Force Algorithm start ----------------------->

void brute(int pat_len,int text_len){
 int TextIx=0,count=0;
 char c;
 for(TextIx=0;TextIx<=(text_len-pat_len);TextIx++){
	int PatIx=0;
	rewind(fp);
	fseek(fp,(TextIx+PatIx),SEEK_CUR);
	c=fgetc(fp);
	//if(c=='\n'){c=fgetc(fp);}
	while(c==Pat[PatIx]){
		//printf(" %d %c %d ",PatIx,c,pat_len);
		if(PatIx==pat_len-1){
			count++;
			printf("\n An occurnce of %s at Location %d to %d \n",Pat,TextIx,TextIx+PatIx);
			break;
		}
		else {  
			PatIx++;
			rewind(fp);
			fseek(fp,(TextIx+PatIx),SEEK_CUR);
			c=fgetc(fp);
			//printf(" %d %c %d ",PatIx,c,pat_len);
			//if(c=='\n'){c=fgetc(fp);TextIx++;}
		}
	}
 }
printf("\n No. of Occurances %d ",count);
}

//	<--------------------------Brute Force Algorithm End ---------------------------->

//	<------------------- Check Function Start------------------------->

int check(char *pat,int pat_size){
 int i,j,pr_count=0;
 //printf("%d %d",strlen(protein),strlen(pat));
 for(i=0;i<pat_size;i++){
	for(j=0;j<strlen(protein);j++){
		if(protein[j]==*pat){
			pat++;
			pr_count++;
			//printf(" %d %c %c",pr_count,*pat,protein[i]);
		}		
	}
 }

//printf(" %d",pr_count);
 if(pr_count==pat_size){
	return 1;
 }
else{return 0;}
}
//	<------------------- Check Function End ------------------------->

//	<------------------- Main Function ------------------------->

int main(){
 clock_t begin,end;
 double time_req;
int count=0,d;
char c;
fp=fopen("/home/shailesh/Desktop/intern/datasets/all_protein.fa","r");
 if(fp==NULL){
	printf("\nfile cant open");
	return;
 }

while(1)
{
	c=fgetc(fp);
	if(c==EOF){break;}
	count++;
}
 rewind(fp);
 printf("\n \t1.Approximate Matching Algorithm \n\t2.Multiple Reference Character Algorithm \n\t3.A FAST Pattren Matching Algorithm \n\t4.Brute Force Algorithm\n");
 int ch,correct;
 printf("\n  Enter Pattern Matchng Algorithm Choice ---> ");
 scanf("%d",&ch);
 switch(ch){
 case 1:
	 printf("\n <---- Approximare Matching Algorithm ------>\n");
	 printf("\n Enter DNA/PROTEIN Pattern ----> ");
	 scanf("%s",Pat);
	 correct=check(Pat,strlen(Pat));
	 if(correct!=1){printf("\n Wrong DNA/PROTEIN Pattern\n");return 0;}
	 printf("\n Enter Distance --> ");
	 scanf("%d",&d);
	 begin=clock();
	 apx(strlen(Pat),count,d);
	 printf("\n No. of Occurances %d ",count1-1);
	 end=clock();
	 time_req=(double)(end-begin)/CLOCKS_PER_SEC;
	 printf("\n Time Required ----> %f s\n\n",time_req);

  	break;
 case 2:
	 printf("\n <------ Multiple Reference Character Algorithm ------>\n");
	 printf("\n Enter DNA/PROTEIN Pattern ----> ");
	 scanf("%s",Pat);
	 correct=check(Pat,strlen(Pat));
	 if(correct!=1){printf("\n Wrong DNA/PROTEIN Pattern\n");return 0;}
	 begin=clock();
	 PreProcess_Pat(strlen(Pat));
	 Pos_Skip(strlen(Pat),count);
	 printf("\n No. of Occurances %d ",count2);
	 end=clock();
	 time_req=(double)(end-begin)/CLOCKS_PER_SEC;
	 printf("\n Time Required ----> %f s\n\n",time_req);

	break;
 case 3:
	 printf("\n <---- A FAST Pattren Matching Algorithm ------>\n");
	 printf("\n Enter DNA/PROTEIN Pattern ----> ");
	 scanf("%s",x);
	 correct=check(Pat,strlen(Pat));
	 if(correct!=1){printf("\n Wrong DNA/PROTEIN Pattern\n");return 0;}
	 begin=clock();
	 Preprocess(x,strlen(x),count);
	 rewind(fp);
	 SSABS(x,strlen(x),count);
	 end=clock();
	 time_req=(double)(end-begin)/CLOCKS_PER_SEC;
	 printf("\n Time Required ----> %f s\n\n",time_req);

	break;
 case 4:
	 printf("\n <---- Brute Force Algorithm ------>\n");
	 printf("\n Enter DNA/PROTEIN Pattern ----> ");
	 scanf("%s",Pat);
	 correct=check(Pat,strlen(Pat));
	 if(correct!=1){printf("\n Wrong DNA/PROTEIN Pattern\n");return 0;}
	 begin=clock();
	 brute(strlen(Pat),count);
	 end=clock();
	 time_req=(double)(end-begin)/CLOCKS_PER_SEC;
	 printf("\n Time Required ----> %f s\n\n",time_req);

	break;
 default : printf("Wrong Choice\n");
 }

return 0;
}

