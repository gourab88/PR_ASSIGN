 
// a3.c
// APR ASSIGNMENT 3
// FUZZY C MEANS CLUSTERING ALGORITHM
// AUTHOR : Gourab Saha
// To compile :gcc a3.c -lm
// To rum :./a.out < iris.data
 




#define NO_OF_DATA_POINTS 150
#define NO_OF_CLASS 3
#define NO_OF_FEATURES 4
#define r 1.2
#define ERROR 0.0001
  
  
  
#include<stdio.h>
#include<stdlib.h>
#include<math.h>


void initialize(double ** u);

int main(){
    
    
    double **u,**new_u,**features,**distance;
    int i,j,k,label;
    int * actual_class_label,*predicted_class_label;
    double **y;
    double sum,p,diff,max;
    
    int no_of_iterations=0;
    int flag=1;
    int count;
    int x;
    int cluster_class_correspondance[NO_OF_CLASS][NO_OF_CLASS];
    int mapping[NO_OF_CLASS];
    int misclassified[NO_OF_CLASS];
    
     for(i=0;i<NO_OF_CLASS;i++)
     {
         misclassified[i]=0;
         for(j=0;j<NO_OF_CLASS;j++)
            cluster_class_correspondance[i][j]=0;
     }
    
    
    u=(double **)calloc(NO_OF_DATA_POINTS,sizeof(double*));
    distance=(double **)calloc(NO_OF_DATA_POINTS,sizeof(double*));
    new_u=(double **)calloc(NO_OF_DATA_POINTS,sizeof(double*));
    features=(double **)calloc(NO_OF_DATA_POINTS,sizeof(double*));
    
    
     for(i=0;i<NO_OF_DATA_POINTS;i++)
     {
       u[i]=(double*)malloc(NO_OF_CLASS*sizeof(double));
       distance[i]=(double*)malloc(NO_OF_CLASS*sizeof(double));
       new_u[i]=(double*)malloc(NO_OF_CLASS*sizeof(double));
       features[i]=(double*)malloc(NO_OF_FEATURES*sizeof(double));
     }
       

       
    y=(double **)calloc(NO_OF_CLASS,sizeof(double*));
    for(i=0;i<NO_OF_CLASS;i++)
       y[i]=(double*)malloc(NO_OF_FEATURES*sizeof(double));
        
    
   
    actual_class_label=(int*)calloc(NO_OF_DATA_POINTS,sizeof(int*));
    predicted_class_label=(int*)calloc(NO_OF_DATA_POINTS,sizeof(int*));
    
   
  
    
    for(i=0;i<NO_OF_DATA_POINTS;i++)
    {
        for(j=0;j<NO_OF_FEATURES;j++)
            scanf("%lf", &features[i][j]);
        scanf("%d", &actual_class_label[i]);
    }
    
    
    initialize(u);
    
   
    
    while(flag)
    {
    
        no_of_iterations++;
        
        
        // Calculate mean;
    
         for(j=0;j<NO_OF_CLASS;j++)
          {
              sum=0.0;
              for(k=0;k<NO_OF_FEATURES;k++)
                  y[j][k]=0;
              for(i=0;i<NO_OF_DATA_POINTS;i++)
              {
                
                  p=pow(u[i][j],r);
                  sum=sum+p;
                  for(k=0;k<NO_OF_FEATURES;k++)
                  {
                     y[j][k]= y[j][k]+p*features[i][k];
                  }
                  
              }
              
               for(k=0;k<NO_OF_FEATURES;k++)
              {
                     y[j][k]= y[j][k]/sum;
                  
              }
          }
          
      
          
          // Calculate distance
          
          for(i=0;i<NO_OF_DATA_POINTS;i++)
          {
            for(j=0;j<NO_OF_CLASS;j++)
            {
                 distance[i][j]=0.0;
                for(k=0;k<NO_OF_FEATURES;k++)
                {
                    distance[i][j]=distance[i][j]+pow((features[i][k]-y[j][k]),2);
                  
                } 
            }
          }
          
          
         //Calculate new u-matrix
         
         
          for(i=0;i<NO_OF_DATA_POINTS;i++)
          {
            for(j=0;j<NO_OF_CLASS;j++)
            {
                 new_u[i][j]=0.0;
                for(k=0;k<NO_OF_CLASS;k++)
                {
                     new_u[i][j]= new_u[i][j]+pow((distance[i][j]/distance[i][k]),(1/(r-1)));
                }
                
                   new_u[i][j]=1/new_u[i][j];
            }
          }
          
          
          
          //Check stoppping condition
          
          flag=0;
          
          for(i=0;i<NO_OF_DATA_POINTS;i++)
          {
            for(j=0;j<NO_OF_CLASS;j++)
            {
                diff=new_u[i][j]-u[i][j];
                if(diff<0)
                    diff=-1*diff;
                if(diff>ERROR)
                {
                    flag=1;
                    break;
                }
            }
            if(flag==1)
                break;
          }
          
          
          if(flag==1)
          {
            for(i=0;i<NO_OF_DATA_POINTS;i++)
            {
                  for(j=0;j<NO_OF_CLASS;j++)
                  {
                    u[i][j]=new_u[i][j];
                    new_u[i][j]=0.0;
                  }
            
            }
          }
      
      
      
          
          
    }
        
    
    for(i=0;i<NO_OF_DATA_POINTS;i++)
    {
        max=0.0;
        label=-1;
        for(j=0;j<NO_OF_CLASS;j++)
        {
            if(new_u[i][j]>max)
            {
                max=new_u[i][j];
                label=j+1;
            }
            
            predicted_class_label[i]=label;
            
          
            
      
        }
        
    }
    
    
    
    
     
       //correspondance  of cluster no and class label
       

     for(i=0;i<NO_OF_DATA_POINTS;i++)
     {
        cluster_class_correspondance[predicted_class_label[i]-1][actual_class_label[i]-1]++;
     }
        
    for(i=0;i<NO_OF_CLASS;i++)
    {
        x=0;
        k=0;
        for(j=0;j<NO_OF_CLASS;j++)
        {
            if(cluster_class_correspondance[i][j]>x)
            {
                x=cluster_class_correspondance[i][j];
                k=j;
            }
        }
        mapping[i]=k;
    
    }
  
    count =0;
    
    for(i=0;i<NO_OF_DATA_POINTS;i++)
    {
        x=mapping[predicted_class_label[i]-1]+1;
        predicted_class_label[i]=x;
        if(actual_class_label[i]!=predicted_class_label[i])
        {
           misclassified[predicted_class_label[i]-1]++;
            count++;
        }
    }
         
            
    
    printf("\n\nNo of iterations to converge :%d\n",no_of_iterations);
     for(j=0;j<NO_OF_CLASS;j++)
         printf("\nNo of misclassified points for class %d  : %d\n",j+1, misclassified[j]);
    printf("\nTotal No of misclassified points :%d\n",count);
    printf("\nMisclassification rate :%f %% \n\n\n",((double)count/NO_OF_DATA_POINTS)*100.0);
            
    
    
    
    
    for(i=0;i<NO_OF_DATA_POINTS;i++)
    {
       
        free(u[i]);
        free(distance[i]);
        free(new_u[i]);
        free(features[i]);
    }
    
    for(i=0;i<NO_OF_CLASS;i++)
        free(y[i]);
       
    
    
    free(u);
    free(distance);
    free(new_u);
    free(features);
    free(y);
    free(actual_class_label);
    free(predicted_class_label);
    
    
}



void initialize(double ** u)
{
    
    int i,j;
    double sum;
    for(i=0;i<NO_OF_DATA_POINTS;i++)
    {
        sum=0.0;
        for(j=0;j<NO_OF_CLASS;j++)
        {
            u[i][j]=(double)rand()/(double)RAND_MAX;
            sum=sum+u[i][j];
        }
        
         for(j=0;j<NO_OF_CLASS;j++)
            u[i][j]=u[i][j]/sum;
    }
    
    
}