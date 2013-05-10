
// a2.c
// APR ASSIGNMENT 1
// OPTIMAL PARTITION USING GENETIC ALGORITHM
// AUTHOR : Gourab Saha
// To compile :gcc a2.c -lm
// change the value of N(Defined constant) to change
// the no of iteration.
   
    
   
   
    #include<stdio.h>
    #include<stdlib.h>
    #include<math.h>
       
    #define N 50
    #define M 6
    #define p 0.9
    #define L 10
    #define val_max 0.45
       
       
      
    void generate_random(double *x,double *y);
    void int_to_bin(int i,char *str);
    void bruteforce_find(double *x,double *y,double *m,int *t);
    void initialize_population(char ** population);
    double  get_value(char *str,double* x,double *y);
    void update_population(char **population,char **new_population);
    void shuffle(int *shuffle_array);
     
    
    int main()
     {
          
        int i,n,j;
        double x[L],y[L];
        char str[L];
        double l,min;
        int track;
        char ** population,**new_population;
        double * roulette_wheel;
        double *fitness_value;
        int *shuffle_array;
        int option,k,position,track1,track2,real;
        char t;
        double sum;
        double q;
        double r;
        int x1;
        double max,max2;
        char s[L];
      
        
      
          
       generate_random(x,y);
       
       bruteforce_find(x,y,&min,&track);
       
       
       
     printf("\n\n");
     printf("*********************************************\n");
     printf("**OPTIMAL PARTITION FOUND USING BRUTE-FORCE**\n",x1);
     printf("*********************************************\n\n");
     
     int_to_bin(track,str);
       
       printf("\n\nOptimal partion is\t");
              
      for(i=0;i<L;i++)
         printf("%c",str[i]);
         
      printf("  with the minimization function value :   %f\n\n",min);
       
       
       
      population=(char **)malloc(M*sizeof(char*));
      new_population=(char **)malloc(M*sizeof(char*));
      roulette_wheel=(double*)malloc((M+1)*sizeof(double));
      fitness_value=(double*)calloc(M,sizeof(double));
      shuffle_array=(int*)malloc(M*sizeof(int));
       
     for(i=0;i<L;i++)
       population[i]=(char*)malloc(L*sizeof(char));
     for(i=0;i<L;i++)
       new_population[i]=(char*)malloc(L*sizeof(char));
       
       initialize_population(population);
        
        
        
        for(i=0;i<N;i++)
      {
         
         // Step 2 : Selection
          
           sum=0;
           max =0.0;
           track=-1;
           for(j=0;j<M;j++)
           {
              
              fitness_value[j]=get_value(population[j],x,y);
               
               if (fitness_value[j]>max)
               {
                  track1=j;
                  max=fitness_value[j];
               }
               
               roulette_wheel[0]=0.0;
               roulette_wheel[j+1]=fitness_value[j];
               sum=sum+ fitness_value[j];
           
           }
            for(j=0;j<L;j++)
           {
              
               s[j]=population[track1][j];
           }
           
            
             for(j=1;j<=M;j++)
           {
                  
                 roulette_wheel[j]=roulette_wheel[j]/sum;
                 roulette_wheel[j]=roulette_wheel[j-1]+ roulette_wheel[j];
                 
           }
           roulette_wheel[0]=0.0;
           
              for(j=0;j<M;j++)
           {
               r=(double)rand()/(double)RAND_MAX;
               k=0;
               while(1)
               {
                  if(roulette_wheel[k]>r)
                     break;
                  k++;
               }
                k--;
                
               for(x1=0;x1<L;x1++)
                   new_population[j][x1]=population[k][x1];
           }
           
           update_population(population,new_population);
           
           // Step 3: Crossover
           
           
           for(j=0;j<M;j++)
               shuffle_array[j]=j;
 
               
           shuffle(shuffle_array);
           
           for(j=0;j<M;j=j+2)
           {
                r=(double)rand()/(double)RAND_MAX;
         
                if(r<p)
                {
                   position=rand()%(L-1);
                   for(k=position+1;k<L;k++)
                   {
                       t=population[shuffle_array[j]][k];
                       population[shuffle_array[j]][k]=population[shuffle_array[j+1]][k];
                       population[shuffle_array[j+1]][k]=t;
                   }
                   
                }
                
           }
           
           //Step 4 : Mutation
           
           
           for(j=0;j<M;j++)
           {
               for(k=0;k<L;k++)
               {
                   q=(cos(i*3*M_PI/N)+1)*((0.45-(1.0/(double)L))/2)+(1.0/(double)L);
                   r=(double)rand()/(double)RAND_MAX;
                   if(r<q)
                   {
                       if(population[j][k]=='1')
                           population[j][k]='0';
                       else
                            population[j][k]='1';
                   }
               }
           }
           
           //Step 5: Elitiest model
           
           max2 =0.0;
           track2=-1;
           
           for(j=0;j<M;j++)
           {
                fitness_value[j]=get_value(population[j],x,y);
                 if (fitness_value[j]>max2)
               {
                  track2=j;
                  max2=fitness_value[j];
               }
           }
           
           
           if(max>max2)
            {
               x1=(int)rand()%M;
               for(j=0;j<L;j++)
               {
                  population[x1][j]=s[j];
               }
               fitness_value[x1]=max;
       
           } 
        
        
      }
      
      
      x1=N;
 
     printf("\n\n");
     printf("*********************************************\n");
     printf("**  FINAL POPULATION AFTER %d ITERATION  **\n",x1);
     printf("*********************************************\n\n");
     for(i=0;i<M;i++)
     {
         printf("element %d\t:\t",(i+1));
        for(j=0;j<L;j++){
            printf("%c",population[i][j]);
        }           
        printf("      with minimization function value  : %f  ",(1/fitness_value[i]));
        printf("\n");
     }
     
     printf("\n\n");
     
     
     
        
     for(i=1;i<M;i++)
     {
          free(population[i]);
          free(new_population[i]);
     }
     
     
      free(population);
      free(new_population);
      free(roulette_wheel);
      free(fitness_value);
      free(shuffle_array);
     
     
   }
     
     
     
    
    
    void generate_random(double *x,double *y)
    {
        int i;
        for(i=0;i<L;i++)
        {
            x[i]=(double)rand()/(double)RAND_MAX;
            y[i]=(double)rand()/(double)RAND_MAX;
            
        }
    }
    
    
    void  bruteforce_find(double *x,double *y,double *m,int *t)
     {
         
        double mean1[2],mean2[2];
        char str[L];
        int c1,c2;
        int i,n,j;
        double min;
        int track;
        double l;
        
        
        
        n=((int)pow(2,L)-2)/2;
        min=10.0;
        track -1;
        
        for(i=1;i<n;i++)
        {
          int_to_bin(i,str);
          for(j=0;j<2;j++)
          {
             mean1[j]=0.0;
             mean2[j]=0.0;
          }
          l=0.0;
          c1=0;
          c2=0;
          
          for(j=0;j<L;j++)
          {
            if(str[j]=='0')
            {
                mean1[0]= mean1[0]+x[j];
                mean1[1]= mean1[1]+y[j];
                c1++;
            }
            else
            {
                mean2[0]= mean2[0]+x[j];
                mean2[1]= mean2[1]+y[j];
                c2++;
            }
          }
            
           mean1[0]= mean1[0]/c1;
           mean1[1]= mean1[1]/c1;
           mean2[0]= mean2[0]/c2;
           mean2[1]= mean2[1]/c2;
           
           
          for(j=0;j<L;j++)
          {
             if(str[j]=='0')
                l=l+(x[j]-mean1[0])*(x[j]-mean1[0])+(y[j]-mean1[1])*(y[j]-mean1[1]);
             else
                l=l+(x[j]-mean2[0])*(x[j]-mean2[0])+(y[j]-mean2[1])*(y[j]-mean2[1]);
          }
          
          if(l<min)
          {
            min=l;
            track=i;
           }
        }
        
        *m=min;
        *t=track;
        
     }
    
    
    
    
    
    
    
    
   void int_to_bin(int i,char *str){
        int j,c,n;
        
        for(j=0;j<L;j++)
            str[j]='0';
        
        c=L-1;
        n=i;
        while(i>=2)
        {
           str[c]=(char)((int)(i%2)+(int)'0');
           i=i/2;
           c--;
        }
        str[c]=(char)((int)(i)+(int)'0');
   }
   
   
   
   
   
   
     void initialize_population(char ** population){
       
       int i,j,r;
       char str[L];
       
       for(i=0;i<M;i++){
            r=(rand()%510)+1;
            int_to_bin(r,str);
             for(j=0;j<L;j++)
                population[i][j]=str[j];
       }
   }
   
   void update_population(char **population,char **new_population){
      
      int i,j;
      
       for(i=0;i<M;i++)
          for(j=0;j<L;j++)
            population[i][j]=new_population[i][j];
   }
   
   
   
   double get_value(char *str,double* x,double *y)
   {
        double mean1[2],mean2[2];
        int c1,c2;
        int i,n,j;
        double min;
        int track;
        double l;
        
          
         for(j=0;j<2;j++)
          {
             mean1[j]=0.0;
             mean2[j]=0.0;
          }
          l=0.0;
          c1=0;
          c2=0;
          
          for(j=0;j<L;j++)
          {
            if(str[j]=='0')
            {
                mean1[0]= mean1[0]+x[j];
                mean1[1]= mean1[1]+y[j];
                c1++;
            }
            else
            {
                mean2[0]= mean2[0]+x[j];
                mean2[1]= mean2[1]+y[j];
                c2++;
            }
          }
            
           mean1[0]= mean1[0]/c1;
           mean1[1]= mean1[1]/c1;
           mean2[0]= mean2[0]/c2;
           mean2[1]= mean2[1]/c2;
           
           
          for(j=0;j<L;j++)
          {
             if(str[j]=='0')
                l=l+(x[j]-mean1[0])*(x[j]-mean1[0])+(y[j]-mean1[1])*(y[j]-mean1[1]);
             else
                l=l+(x[j]-mean2[0])*(x[j]-mean2[0])+(y[j]-mean2[1])*(y[j]-mean2[1]);
          }
          
          return (1/l);
          
   }
   
   
   

   
   void shuffle(int *shuffle_array)
   {
       int i,j,t;
       
       for (i = 0; i < M - 1; i++) {
             srand(rand());
             j = i + rand() / (RAND_MAX / (M - i) + 1);
             t =shuffle_array[j];
             shuffle_array[j] =shuffle_array[i];
             shuffle_array[i] = t;
           }
   }