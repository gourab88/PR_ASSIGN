// a4.c
// APR ASSIGNMENT 4
// MULTI LAYER PERCEPTRON
// AUTHOR : Gourab Saha
// To compile :gcc a4.c -lm
// To rum :./a.out < mlp.data


  
#include<stdio.h>
#include<stdlib.h>
#include<math.h>

#define NO_OF_TRAINING_DATA_POINTS 75
#define NO_OF_TEST_DATA_POINTS 75
#define NO_OF_CLASS 3
#define NO_OF_FEATURES 4
#define LEARNING_RATE 0.2
#define HIDDEN_LAYER_NODES 8
#define ERROR_THRESOLD 0.0001



void initialize(double ** weight_hidden_layer,double ** weight_output_layer,int n1,int n2,int n3);
void standardisation(double **train_features,double **test_features);


int main(){
    
    double  **train_features, **test_features,**weight_hidden_layer,**weight_output_layer;
    int * train_class_label,*test_class_label,*predicted_class_label;
    double *op_hidden_layer,*op_result_layer;
    double error,difference,max;
    int track,count;
    
    double **change_weight_hidden_layer,**change_weight_output_layer;

    
    int n1,n2,n3;
    int flag=1;
    int no_of_iteration=0;
    int i,j,k,x;
    double **delta2;
    double **delta1;
    
    
    n1=NO_OF_FEATURES+1;
    n2=HIDDEN_LAYER_NODES;
    n3=NO_OF_CLASS;
    
    
    op_hidden_layer=(double *)calloc(n2,sizeof(double));
    op_result_layer=(double *)calloc(n3,sizeof(double));
    delta2=(double **)calloc(NO_OF_TRAINING_DATA_POINTS,sizeof(double*));
    delta1=(double **)calloc(NO_OF_TRAINING_DATA_POINTS,sizeof(double*));
    
     for(i=0;i<NO_OF_TRAINING_DATA_POINTS;i++)
     {
        delta2[i]=(double *)calloc(n3,sizeof(double));
     }
     
     for(i=0;i<NO_OF_TRAINING_DATA_POINTS;i++)
     {
        delta1[i]=(double *)calloc(n2,sizeof(double));
     }
 
    train_class_label=(int*)malloc(NO_OF_TRAINING_DATA_POINTS*sizeof(int));
    test_class_label=(int*)malloc(NO_OF_TEST_DATA_POINTS*sizeof(int));
    predicted_class_label=(int*)malloc(NO_OF_TEST_DATA_POINTS*sizeof(int));
    
    train_features=(double **)calloc(NO_OF_TRAINING_DATA_POINTS,sizeof(double*));
    
    for(i=0;i<NO_OF_TRAINING_DATA_POINTS;i++)
     {
        train_features[i]=(double *)calloc(n1,sizeof(double));
     }
     
     
     test_features=(double **)calloc(NO_OF_TEST_DATA_POINTS,sizeof(double*));
    
    for(i=0;i<NO_OF_TEST_DATA_POINTS;i++)
     {
        test_features[i]=(double *)calloc(n1,sizeof(double));
     }
    
    
    weight_hidden_layer=(double **)calloc(n1,sizeof(double*));
    change_weight_hidden_layer=(double **)calloc(n1,sizeof(double*));
    
    for(i=0;i<n1;i++)
     {
        weight_hidden_layer[i]=(double *)calloc(n2,sizeof(double));
        change_weight_hidden_layer[i]=(double *)calloc(n2,sizeof(double));
     }
     
    
    weight_output_layer=(double **)calloc(n2,sizeof(double*));
    change_weight_output_layer=(double **)calloc(n2,sizeof(double*));
    
    for(i=0;i<n2;i++)
     {
        weight_output_layer[i]=(double *)calloc(n3,sizeof(double));
        change_weight_output_layer[i]=(double *)calloc(n3,sizeof(double));
     }
     
     
  
   
     
    

     for(i=0;i<NO_OF_TRAINING_DATA_POINTS;i++)
    {
        for(j=0;j<NO_OF_FEATURES;j++)
            scanf("%lf", &train_features[i][j]);
        scanf("%d", &train_class_label[i]);
    }
    
    
    for(i=0;i<NO_OF_TEST_DATA_POINTS;i++)
    {
        for(j=0;j<NO_OF_FEATURES;j++)
            scanf("%lf", &test_features[i][j]);
        scanf("%d", &test_class_label[i]);
    }
      
      
      
 //for(i=0;i<NO_OF_TRAINING_DATA_POINTS;i++)
 //   {
 //       for(j=0;j<NO_OF_FEATURES;j++)
 //           printf("%f ",train_features[i][j]);
 //        printf(" %d\n", train_class_label[i]);
 //   }
    
 

    
    //Step 1: Initialization
    
    
    initialize(weight_hidden_layer,weight_output_layer,n1,n2,n3);
    
    
    
    
     //Step 2: Standardisation
     
    
    standardisation(train_features,test_features);
    
  
    
    
    //Step 3: Training
   
    
    printf("\n\nTRAINING PHASE\n\n");
    
    while(1)
    {
       no_of_iteration++;
       error=0.0;
       flag=1;
     
     
     
     
       
       for(i=0;i<n1;i++)
       {
        for(j=0;j<n2;j++)
        {
            change_weight_hidden_layer[i][j]=0.0;
        }
       }
       
       
        for(i=0;i<n2;i++)
       {
        for(j=0;j<n3;j++)
        {
            change_weight_output_layer[i][j]=0.0;
        }
       }
       
       
       
       
       
       
       
       for(x=0;x<NO_OF_TRAINING_DATA_POINTS;x++)
        {
		    
            for(i=0;i<n2;i++)
	    {
		op_hidden_layer[i]=0;
                for(j=0;j<n1;j++)
		    op_hidden_layer[i]+=weight_hidden_layer[j][i]*train_features[x][j];
		op_hidden_layer[i]=1/(1+exp(-1*op_hidden_layer[i]));
	    }
				
				 
                                
	    for(i=0;i<n3;i++)
	    {
		op_result_layer[i]=0;
		for(j=0;j<n2;j++)
                      op_result_layer[i]+=weight_output_layer[j][i]*op_hidden_layer[j];
		op_result_layer[i]=1/(1+exp(-1*op_result_layer[i]));
                                        
                 if(i==(train_class_label[x]-1))
                 {
                      
                    delta2[x][i]=1-op_result_layer[i];
                    error=error+pow(delta2[x][i],2);
                }
                                         
                else
                                        
                {
                    delta2[x][i]=-1*op_result_layer[i];
                    error=error+pow(delta2[x][i],2);
                }
                                          
                delta2[x][i]=delta2[x][i]*op_result_layer[i]*(1-op_result_layer[i]);
	    }
				
                                
                                
                            
                                        
	    for(i=0;i<n2;i++)
	    {
		for(j=0;j<n3;j++)
		{
						 
		    change_weight_output_layer[i][j]+=LEARNING_RATE*delta2[x][j]*op_hidden_layer[i];
						 
		}
	    }
				
                                
				
	    for(i=0;i<n2;i++)
	    {
		delta1[x][i]=0.0;
		for(j=0;j<n3;j++)
                    delta1[x][i]=delta1[x][i]+(delta2[x][j]*weight_output_layer[i][j]);
                delta1[x][i]=delta1[x][i]*op_hidden_layer[i]*(1-op_hidden_layer[i]);
            }
                                
                                
					
	    for(i=0;i<n1;i++)
	    {	
		for(j=0;j<n2;j++)
		{		
						 
                    change_weight_hidden_layer[i][j]+=LEARNING_RATE*delta1[x][j]*train_features[x][i];
						 
		}
            }	
				
                                
                                				
				 
				 
        }
       
         
      
        error=error/2;
        
        if(error< ERROR_THRESOLD)
                flag=0;
                
      printf("Iteration no :%d\tTotal error = %f\n",no_of_iteration,error); 
                    
       
        
        if(flag==0)
            break;
        else
        {
            for(i=0;i<n2;i++)
            {
               for(j=0;j<n3;j++)
               {
                weight_output_layer[i][j]=weight_output_layer[i][j]+change_weight_output_layer[i][j];
                   
               }
            }
          
           
            for(i=0;i<n1;i++)
            {
               for(j=0;j<n2;j++)
               {
                weight_hidden_layer[i][j]=weight_hidden_layer[i][j]+ change_weight_hidden_layer[i][j];
               }
            }
            
        }
      
      
        
            
    }
    
    
    
    
    
    printf("\n\nTESTING PHASE\n\n");
    
    //Step 4: Testing Phase
    
    
      count=0;
    
      for(x=0;x<NO_OF_TEST_DATA_POINTS;x++)
        {
               
               max=0.0;
               track=-1;
               
                for(j=0;j<n2;j++)
               {
                     op_hidden_layer[j]=0.0;
                 }
        
                    
            for(i=0;i<n2;i++)
	    {
		op_hidden_layer[i]=0;
                for(j=0;j<n1;j++)
		    op_hidden_layer[i]+=weight_hidden_layer[j][i]*train_features[x][j];
		op_hidden_layer[i]=1/(1+exp(-1*op_hidden_layer[i]));
	    }
            
            printf("Test Point no :%d\t op_layer values : ",x+1);
            
             for(i=0;i<n3;i++)
	    {
		op_result_layer[i]=0;
		for(j=0;j<n2;j++)
                      op_result_layer[i]+=weight_output_layer[j][i]*op_hidden_layer[j];
		op_result_layer[i]=1/(1+exp(-1*op_result_layer[i]));
                printf("  %f",op_result_layer[i]);
                
                if(op_result_layer[i]>max)
                {
                     max=op_result_layer[i];
                    track=i+1;
                }
            
            }
        
             printf("\tclass label : %d\n",track);
                
            predicted_class_label[x]=track;
        
                
            if(test_class_label[x]!=predicted_class_label[x])
                    count++;
        
        }
        
   
    
         printf("\n\nNo of iteration to converge :  %d\n\n",no_of_iteration);
         printf("\n\nNo of misclassified points :  %d\n\n",count);
         printf("\n\nMisclassification rate:  %f %%\n\n",((double)count/(double)NO_OF_TEST_DATA_POINTS)*100);
         
         
        for(i=0;i<NO_OF_TRAINING_DATA_POINTS;i++)
        {
    
            free(train_features[i]);
            free(test_features[i]);
            free(delta2[i]);
            free(delta1[i]);
        }

        for(i=0;i<n1;i++)
        {
            free( weight_hidden_layer[i]);
            free(change_weight_hidden_layer[i]);
        }
        
        for(i=0;i<n2;i++)
        {
            free( weight_output_layer[i]);
            free(change_weight_output_layer[i]);
    
        }


        free(train_features);
        free(test_features);
        free(weight_hidden_layer);
        free(change_weight_hidden_layer);
        free( weight_hidden_layer);
        free(change_weight_output_layer);
        free(train_class_label);
        free(test_class_label);
        free(predicted_class_label);
        free(delta2);
        free(delta1);
        free(op_hidden_layer);
        free(op_result_layer);
        
        return 1;
    
    

}








void initialize(double ** weight_hidden_layer,double ** weight_output_layer,int n1,int n2,int n3)

{
    int i,j;
    
    for(i=0;i<n1;i++)
    {
        for(j=0;j<n2;j++)
        {
            weight_hidden_layer[i][j]=(double)rand()/(double)RAND_MAX;
        }
    }
    
    for(i=0;i<n2;i++)
    {
        for(j=0;j<n3;j++)
        {
            weight_output_layer[i][j]=(double)rand()/(double)RAND_MAX;
        }
    }
    
    
    
}







void standardisation(double **train_features,double **test_features)
{
    
    
    double mean_train[NO_OF_FEATURES];
    double standard_deviation_train[NO_OF_FEATURES];
    double mean_test[NO_OF_FEATURES];
    double standard_deviation_test[NO_OF_FEATURES];
    int i,j;
    
    for(i=0;i<NO_OF_FEATURES;i++)
    {
        mean_train[i]=0.0;
        standard_deviation_train[i]=0.0;
        mean_test[i]=0.0;
        standard_deviation_test[i]=0.0;
         
    }
    
    for(i=0;i<NO_OF_TRAINING_DATA_POINTS;i++)
    {
        for(j=0;j<NO_OF_FEATURES;j++)
        {
            mean_train[j]=mean_train[j]+train_features[i][j];
           
        }
       
    }
    
    for(i=0;i<NO_OF_TEST_DATA_POINTS;i++)
    {
        for(j=0;j<NO_OF_FEATURES;j++)
        {
            mean_test[j]=mean_test[j]+test_features[i][j];
        }
    }
    
    
     for(j=0;j<NO_OF_FEATURES;j++)
    {
            mean_train[j]=mean_train[j]/NO_OF_TRAINING_DATA_POINTS;
            mean_test[j]=mean_test[j]/NO_OF_TEST_DATA_POINTS;
    }
    
    
    
    for(i=0;i<NO_OF_TRAINING_DATA_POINTS;i++)
    {
        for(j=0;j<NO_OF_FEATURES;j++)
        {
            standard_deviation_train[j]= standard_deviation_train[j]+pow((train_features[i][j]-mean_train[j]),2);
        }
    }
    
     for(i=0;i<NO_OF_TEST_DATA_POINTS;i++)
    {
        for(j=0;j<NO_OF_FEATURES;j++)
        {
            standard_deviation_test[j]= standard_deviation_test[j]+pow((test_features[i][j]-mean_test[j]),2);
        }
    }
    
     for(j=0;j<NO_OF_FEATURES;j++)
    {
            standard_deviation_train[j]=sqrt(standard_deviation_train[j]/NO_OF_TRAINING_DATA_POINTS);
            standard_deviation_test[j]=sqrt(standard_deviation_test[j]/NO_OF_TEST_DATA_POINTS);
    }
    
    
     for(i=0;i<NO_OF_TRAINING_DATA_POINTS;i++)
    {
        for(j=0;j<NO_OF_FEATURES;j++)
        {
            train_features[i][j]=(train_features[i][j]-mean_train[j])/ standard_deviation_train[j];
        }
         train_features[i][j]=1.0;
    }
    
    
     for(i=0;i<NO_OF_TEST_DATA_POINTS;i++)
    {
        for(j=0;j<NO_OF_FEATURES;j++)
        {
            test_features[i][j]=(test_features[i][j]-mean_test[j])/ standard_deviation_test[j];
        }
        test_features[i][j]=1.0;
    }
    
}