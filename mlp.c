#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define e 2.718282
#define max_ite 500000

FILE *fpi;

int get_rand(int size)
{
	char ch;
	int n=0,loopctr;

	for(loopctr=0;loopctr<size;loopctr++)
	{
		ch=fgetc(fpi);
		if(ch==EOF)
			break;
		if(ch>='0'&&ch<='9')
			n=n*10+(int)(ch-'0');
		else
			loopctr--;
	}
	return n;
}

int main()
{
	FILE *fp,*fp1;
	int no_input_nodes,no_output_nodes,no_hidden_nodes,no_dim,no_points,no_classes;
	int loop1,loop2,index,count,pt,no_ite,point,flag,trng_type=0,max_node,no_miss;
	float lamda,eps,*mean,*std,*net1,*net2,*x_hidden,*x_out,*del1,*del2,**chng_w1,**chng_w2,sum,batch_error,error,max;
	float **w1,**w2,**x_trn,**x_test,**out_trn,**out_test;

	char ch;
	
	
	/*READING ARCH AND TRAINING/TEST SAMPLES*/
	
	fp=fopen("parameters.txt","r");
	if(fp==NULL)
	{
		printf("Parameters file cannot be opened");
		return 0;
	}
	
	fpi=fopen("pi.txt","r");
        if(fpi==NULL)
	{
		printf("Pi file cannot be opened");
		return 0;
	}
	srand(time(NULL));
        fseek(fpi,rand()%1000,SEEK_CUR); 	
        
        fp1=fopen("iris_data.txt","r");
	if(fp1==NULL)
	{
		printf("Training Sample file cannot be opened");
		return 0;
	}
        
        
        fscanf(fp,"%d%d%f%f%d",&no_dim,&no_classes,&lamda,&eps,&no_points);
        fclose(fp);
        
        while(1)
        {
		printf("\nPress 1 to have \"Online Learning\"\nPress 2 to have \"Batch-mode Learning\"\nEnter your choice:");
		scanf("%d",&trng_type);
		if(trng_type!=1&&trng_type!=2)
			printf("\nPlease Enter correctly");
		else
			break;
        }		
        
        no_input_nodes=no_dim+1;
        no_output_nodes=no_classes;
        no_hidden_nodes=no_input_nodes+no_output_nodes;
        
        x_trn=(float **)malloc(no_points/2*sizeof(float *));
        for(loop1=0;loop1<no_points/2;loop1++)
        	x_trn[loop1]=(float *)malloc(no_input_nodes*sizeof(float *));
        x_test=(float **)malloc(no_points/2*sizeof(float *));
        for(loop1=0;loop1<no_points/2;loop1++)
        	x_test[loop1]=(float *)malloc(no_input_nodes*sizeof(float *));
        	
        mean=(float *)malloc(no_dim*sizeof(float));	
        std=(float *)malloc(no_dim*sizeof(float));
	
        out_trn=(float **)malloc(no_points/2*sizeof(float *));
        for(loop1=0;loop1<no_points/2;loop1++)
        {
        	out_trn[loop1]=(float *)malloc(no_output_nodes*sizeof(float *));
        	for(loop2=0;loop2<no_output_nodes;loop2++)
        		out_trn[loop1][loop2]=0;
        }
        		
        out_test=(float **)malloc(no_points/2*sizeof(float *));
        for(loop1=0;loop1<no_points/2;loop1++)
        {
        	out_test[loop1]=(float *)malloc(no_output_nodes*sizeof(float *));
        	for(loop2=0;loop2<no_output_nodes;loop2++)
        		out_test[loop1][loop2]=0;
        }
        
        index=0;
        count=0;	
        for(loop1=0;loop1<no_points/2;loop1++)
        {
        	for(loop2=0;loop2<no_dim;loop2++)
        	{
        		fscanf(fp1,"%f",&x_trn[loop1][loop2]);
        		if(loop1==0)
        			mean[loop2]=x_trn[loop1][loop2];
        		else
        			mean[loop2]+=x_trn[loop1][loop2];
        		fgetc(fp1);
        	}	
        	while((ch=fgetc(fp1))!='\n')
		;	
        	x_trn[loop1][loop2]=1;	
        		
        	for(loop2=0;loop2<no_dim;loop2++)
        	{
        		fscanf(fp1,"%f",&x_test[loop1][loop2]);
        		mean[loop2]+=x_trn[loop1][loop2];
        		fgetc(fp1);
        	}	
        	while((ch=fgetc(fp1))!='\n')
		;	
        	x_test[loop1][loop2]=1;	
        		
        	if(count<no_points/(2*no_classes))
        	{
        		out_trn[loop1][index]=1;
        		out_test[loop1][index]=1;
        		count++;
        	}	
        	else
        	{
        		count=1;
        		index++;
        		out_trn[loop1][index]=1;
        		out_test[loop1][index]=1;
        	}	
        }	
        fclose(fp1);
        
        for(loop2=0;loop2<no_dim;loop2++)
	{
		mean[loop2]/=no_points;
		std[loop2]=0;
	}	
	//	printf("\nmean dimension %d: %f",loop2+1,mean[loop2]);
        
        for(loop1=0;loop1<no_points/2;loop1++)
        {
        	//printf("\n\nThe training input %d:",loop1+1);
        	for(loop2=0;loop2<no_input_nodes;loop2++)
        	{
        		//printf("%f ",x_trn[loop1][loop2]);
        		if(loop2<no_dim)
        			std[loop2]+=(x_trn[loop1][loop2]-mean[loop2])*(x_trn[loop1][loop2]-mean[loop2]);
        	}
        	//printf("\nTarget Output:");	
        	for(loop2=0;loop2<no_output_nodes;loop2++);
        		//printf("%f ",out_trn[loop1][loop2]);
        		
        	//printf("\nThe test input %d:",loop1+1);
        	for(loop2=0;loop2<no_input_nodes;loop2++)
        	{
        		//printf("%f ",x_test[loop1][loop2]);
        		if(loop2<no_dim)
        			std[loop2]+=(x_test[loop1][loop2]-mean[loop2])*(x_test[loop1][loop2]-mean[loop2]);
        	}
        	//printf("\nTarget Output:");	
        	for(loop2=0;loop2<no_output_nodes;loop2++);
        		//printf("%f ",out_test[loop1][loop2]);	
        }
        
        for(loop2=0;loop2<no_dim;loop2++)
	{
		std[loop2]=sqrt(std[loop2]/no_points);
		//printf("\ndimension %d mean: %f Std deviation :%f",loop2+1,mean[loop2],std[loop2]);
	}	
        
        /*STANDARDIZING INPUT DATA*/
        
	for(loop1=0;loop1<no_points/2;loop1++)
        {
        	//printf("\n\nAfter Standardization The training input %d:",loop1+1);
        	for(loop2=0;loop2<no_input_nodes;loop2++)
        	{
        		if(loop2<no_dim)
        			x_trn[loop1][loop2]=(x_trn[loop1][loop2]-mean[loop2])/std[loop2];
        		//printf("%f ",x_trn[loop1][loop2]);
        	}
        	//printf("\nTarget Output:");	
        	//for(loop2=0;loop2<no_output_nodes;loop2++);
        		//printf("%f ",out_trn[loop1][loop2]);
        		
        	//printf("\nAfter Standardization The test input %d:",loop1+1);
        	for(loop2=0;loop2<no_input_nodes;loop2++)
        	{
        		if(loop2<no_dim)
        			x_test[loop1][loop2]=(x_test[loop1][loop2]-mean[loop2])/std[loop2];
        		//printf("%f ",x_test[loop1][loop2]);
        	}
        	//printf("\nTarget Output:");	
        	//for(loop2=0;loop2<no_output_nodes;loop2++);
        		//printf("%f ",out_test[loop1][loop2]);	
        }	
        
        /*MLP TRAINING*/
        
        /*INITIALIZING WEIGHTS*/
        
        w1=(float **)malloc(no_input_nodes*sizeof(float *));
        for(loop1=0;loop1<no_input_nodes;loop1++) 
        {
        	w1[loop1]=(float *)malloc(no_hidden_nodes*sizeof(float));
        	for(loop2=0;loop2<no_hidden_nodes;loop2++)
        		{w1[loop1][loop2]=(float)(get_rand(4))/10000;
        		//printf("Initial weight w1 %d %d %f",loop1,loop2,w1[loop1][loop2]);	
        		}
        }
        
        w2=(float **)malloc(no_hidden_nodes*sizeof(float *));
        for(loop1=0;loop1<no_hidden_nodes;loop1++) 
        {
        	w2[loop1]=(float *)malloc(no_output_nodes*sizeof(float));
        	for(loop2=0;loop2<no_output_nodes;loop2++)
        		{w2[loop1][loop2]=(float)(get_rand(4))/10000;
        		//printf("Initial weight w2 %d %d %f",loop1,loop2,w2[loop1][loop2]);	
        		}
        }		
        chng_w1=(float **)malloc(no_input_nodes*sizeof(float *));
        for(loop1=0;loop1<no_input_nodes;loop1++) 
        {
        	chng_w1[loop1]=(float *)malloc(no_hidden_nodes*sizeof(float));
        	for(loop2=0;loop2<no_hidden_nodes;loop2++)
        		chng_w1[loop1][loop2]=0;
        }
        
        chng_w2=(float **)malloc(no_hidden_nodes*sizeof(float *));
        for(loop1=0;loop1<no_hidden_nodes;loop1++) 
        {
        	chng_w2[loop1]=(float *)malloc(no_output_nodes*sizeof(float));
        	for(loop2=0;loop2<no_output_nodes;loop2++)
        		chng_w2[loop1][loop2]=0;
        }		
        net1=(float *)malloc(no_hidden_nodes*sizeof(float));
        net2=(float *)malloc(no_output_nodes*sizeof(float));
        del1=(float *)malloc(no_hidden_nodes*sizeof(float));
        del2=(float *)malloc(no_output_nodes*sizeof(float));
        x_hidden=(float *)malloc(no_hidden_nodes*sizeof(float));
        x_out=(float *)malloc(no_output_nodes*sizeof(float));   
        
        
        
        for(loop1=0;loop1<no_hidden_nodes;loop1++)
        {
        	for(loop2=0;loop2<no_output_nodes;loop2++)
        		chng_w2[loop1][loop2]=0;
        }		
       	for(loop1=0;loop1<no_input_nodes;loop1++)
      	{
      		for(loop2=0;loop2<no_hidden_nodes;loop2++)
        		chng_w1[loop1][loop2]=0;
       	}
        
        		 
        for(no_ite=0;no_ite<max_ite;no_ite++)
        {
        	batch_error=0;
        	flag=0;
        	for(pt=0;pt<no_points/(2*no_classes);pt++)
        	{
        		for(point=pt;point<no_points/2;point=point+25)
        		{
				for(loop1=0;loop1<no_hidden_nodes;loop1++)
				{
					net1[loop1]=0;
					for(loop2=0;loop2<no_input_nodes;loop2++)
						net1[loop1]+=x_trn[point][loop2]*w1[loop2][loop1];
					x_hidden[loop1]=1/(1+pow(e,-net1[loop1]));	
				}
				
				error=0;
				for(loop1=0;loop1<no_output_nodes;loop1++)
				{
					net2[loop1]=0;
					for(loop2=0;loop2<no_hidden_nodes;loop2++)
						net2[loop1]+=x_hidden[loop2]*w2[loop2][loop1];
					x_out[loop1]=1/(1+pow(e,-net2[loop1]));
					del2[loop1]=x_out[loop1]*(1-x_out[loop1])*(out_trn[point][loop1]-x_out[loop1]);
					error+=(x_out[loop1]-out_trn[point][loop1])*(x_out[loop1]-out_trn[point][loop1]);
				}
				
				batch_error+=error/2;
				if(error/2>eps)
					flag=1;
				for(loop1=0;loop1<no_hidden_nodes;loop1++)
				{
					for(loop2=0;loop2<no_output_nodes;loop2++)
					{
						if(trng_type==1)
						{
							chng_w2[loop1][loop2]=lamda*del2[loop2]*x_hidden[loop1];
							//printf("change weight w2 %d %d %f",loop1,loop2,chng_w2[loop1][loop2]);
						}
						else
						{
							chng_w2[loop1][loop2]+=lamda*del2[loop2]*x_hidden[loop1];
						}
					}
				}
				
				
				for(loop1=0;loop1<no_hidden_nodes;loop1++)
				{
					sum=0;
					for(loop2=0;loop2<no_output_nodes;loop2++)
						sum=sum+del2[loop2]*w2[loop1][loop2];
					del1[loop1]=x_hidden[loop1]*(1-x_hidden[loop1])*sum;
				}
					
				for(loop1=0;loop1<no_input_nodes;loop1++)
				{	
					for(loop2=0;loop2<no_hidden_nodes;loop2++)
					{		
						if(trng_type==1)
						{
							chng_w1[loop1][loop2]=lamda*del1[loop2]*x_trn[point][loop1];
							//printf("change weight w1 %d %d %f",loop1,loop2,w1[loop1][loop2]);
						}
						else
						{
							chng_w1[loop1][loop2]+=lamda*del1[loop2]*x_trn[point][loop1];
						}
					}
				}	
								
				if(trng_type==1)
				{
					for(loop1=0;loop1<no_hidden_nodes;loop1++)
					{
						for(loop2=0;loop2<no_output_nodes;loop2++)
						{
							w2[loop1][loop2]+=chng_w2[loop1][loop2];
							chng_w2[loop1][loop2]=0;
						}
					}
					for(loop1=0;loop1<no_input_nodes;loop1++)
					{
						for(loop2=0;loop2<no_hidden_nodes;loop2++)
						{
							w1[loop1][loop2]+=chng_w1[loop1][loop2];
							chng_w1[loop1][loop2]=0;
						}
					}

				}
				//if(trng_type==1)	
				//	printf("\nPoint %d Error %f",point+1,error/2);
			}
		}	
		printf("\nIteration %d batch_error %f",no_ite+1,batch_error);
        	if(trng_type==2)
        	{
        		for(loop1=0;loop1<no_hidden_nodes;loop1++)
			{
				for(loop2=0;loop2<no_output_nodes;loop2++)
				{
					w2[loop1][loop2]+=chng_w2[loop1][loop2];
					chng_w2[loop1][loop2]=0;
				}
			}
			for(loop1=0;loop1<no_input_nodes;loop1++)
			{
				for(loop2=0;loop2<no_hidden_nodes;loop2++)
				{
					w1[loop1][loop2]+=chng_w1[loop1][loop2];
					chng_w1[loop1][loop2]=0;
				}
			}
		}	
        		
        	if((trng_type==2&&batch_error<eps) || ((trng_type==1)&&(flag==0)))
        		break;					
       	}
       	
       	
       	/*TESTING & FINDING NO. OF MISCLASSIFIED POINTS*/
       	
       	no_miss=0;
       	
       	for(point=0;point<no_points/2;point++)
        {
		for(loop1=0;loop1<no_hidden_nodes;loop1++)
		{
			net1[loop1]=0;
			for(loop2=0;loop2<no_input_nodes;loop2++)
				net1[loop1]+=x_trn[point][loop2]*w1[loop2][loop1];
			x_hidden[loop1]=1/(1+pow(e,-net1[loop1]));	
		}
				
		max=-99999;
		max_node=0;
		printf("\nTesting point %d: ",point+1);
		for(loop1=0;loop1<no_output_nodes;loop1++)
		{
			net2[loop1]=0;
			for(loop2=0;loop2<no_hidden_nodes;loop2++)
				net2[loop1]+=x_hidden[loop2]*w2[loop2][loop1];
			x_out[loop1]=1/(1+pow(e,-net2[loop1]));
			if(x_out[loop1]>max)
			{
				max=x_out[loop1];
				max_node=loop1;
			}	
			printf("%f ",x_out[loop1]);
		}
			
    		for(loop1=0;loop1<no_output_nodes;loop1++)
		{
			if(out_test[point][loop1]==1)
			{
				if(max_node!=loop1)
					no_miss++;
			}
		}
	}
	
	printf("\nNo.of missclassified points %d\n\n",no_miss);			
					
        				
        
        fclose(fpi);
        
	free(mean);
	free(std);
	free(net1);
	free(net2);
	free(x_hidden);
	free(x_out);
	free(del1);
	free(del2);
	for(loop1=0;loop1<no_points/2;loop1++)
	{
		free(x_trn[loop1]);
		free(x_test[loop1]);
		free(out_trn[loop1]);
		free(out_test[loop1]);
	}
	free(x_trn);
	free(x_test);
	free(out_trn);
	free(out_test);	
        for(loop1=0;loop1<no_input_nodes;loop1++) 
        {
        	free(w1[loop1]);
        	free(chng_w1[loop1]);
        }
        free(w1);
        free(chng_w1);
        for(loop1=0;loop1<no_hidden_nodes;loop1++) 
        {
        	free(w2[loop1]);
        	free(chng_w2[loop1]);
        }		
        free(w2);
        free(chng_w2);
        return 1;		
        				
}        	

