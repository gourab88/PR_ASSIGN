 
// a1.c
// APR ASSIGNMENT 1
// GENETIC ALGORITHM
// AUTHOR : Gourab Saha
// To compile :gcc a1.c -lm
// change the value of N(Defined constant) to change
// the no of iteration.

  
  
  
   #include<stdio.h>
   #include<stdlib.h>
   #include<math.h>
   

   #define N 1000
   #define M 10
   #define p 0.9
   #define L 15
   #define val_max 0.45
   
      void initialize_population(char ** population);
      double get_real(char * element);
      int get_option(float f);
      void update_population(char **population,char **new_population);
      void shuffle(int *shuffle_array);
     
   
   
   int main()
   {
      
      
      char ** population,**new_population;
      float * roulette_wheel,*fitness_value;
      int i,j,option,k,x,position,track,track2;
      double real;
      char t;
      float sum,fitness;
      int *shuffle_array;
      double q;
      double r;
      double max,max2;
      char s[L];
      
      
      
      population=(char **)malloc(M*sizeof(char*));
      new_population=(char **)malloc(M*sizeof(char*));
      roulette_wheel=(float*)malloc((M+1)*sizeof(float));
      fitness_value=(float*)calloc(M,sizeof(float));
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
              
              real=get_real(population[j]);
              option=get_option(real);
   
               
               switch(option)
               {
                   case 1:
                       fitness_value[j]=sin(real)+6;
                       break;
                   
                   case 2:
                       fitness_value[j]=2*sin(real)+6;
                       break;
                       
                   case 3:
                       fitness_value[j]=3*sin(real)+6;
                       break;
                       
                   case 4:
                       fitness_value[j]=4*sin(real)+6;
                       break;
                       
                   case 5:
                       fitness_value[j]=5*sin(real)+6;
                       break;
                       
                   case 6:
                       fitness_value[j]=sin(real)+6;
                       break;
                   
               }
               if (fitness_value[j]>max)
               {
                  track=j;
                  max=fitness_value[j];
               }
               
               roulette_wheel[0]=0.0;
               roulette_wheel[j+1]=fitness_value[j];
               sum=sum+ fitness_value[j];
           
           }
           
 
           
           for(j=0;j<L;j++)
           {
              
               s[j]=population[track][j];
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
                
               for(x=0;x<L;x++)
                   new_population[j][x]=population[k][x];
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
               real=get_real(population[j]);
               option=get_option(real);
               
               switch(option)
               {
                   case 1:
                       fitness_value[j]=sin(real)+6;
                       break;
                   
                   case 2:
                       fitness_value[j]=2*sin(real)+6;
                       break;
                       
                   case 3:
                       fitness_value[j]=3*sin(real)+6;
                       break;
                       
                   case 4:
                       fitness_value[j]=4*sin(real)+6;
                       break;
                       
                   case 5:
                       fitness_value[j]=5*sin(real)+6;
                       break;
                       
                   case 6:
                       fitness_value[j]=sin(real)+6;
                       break;
                   
               }
               if (fitness_value[j]>max2)
               {
                  track2=j;
                  max2=fitness_value[j];
               }
           }
               
               
               
            if(max>max2)
            {
               x=rand()%M;
               for(j=0;j<L;j++)
               {
                  population[x][j]=s[j];
               }
               fitness_value[x]=max;
       
           } 
      
   }
    
    x=N;
 
     printf("\n\n");
     printf("*********************************************\n");
     printf("**  FINAL POPULATION AFTER %d ITERATION  **\n",x);
     printf("*********************************************\n\n");
     for(i=0;i<M;i++)
     {
         printf("element %d\t:\t",(i+1));
        for(j=0;j<L;j++){
            printf("%c",population[i][j]);
        }           
        printf("      with fitness value  : %f  ",fitness_value[i]);
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
   
    
   
   
   
   
   
   void initialize_population(char ** population){
       
       int i,j,r;
       
       for(i=0;i<M;i++){
           for(j=0;j<L;j++){
               population[i][j]=(char)((int)(rand()%2)+(int)'0');
           }
       }
   }
   
   void update_population(char **population,char **new_population){
      
      int i,j;
      
       for(i=0;i<M;i++)
          for(j=0;j<L;j++)
            population[i][j]=new_population[i][j];
   }
   
   
   
   double get_real(char * element)
   {
       int i;
       double f=0.0;
       
       for(i=0;i<L;i++)
       {
          if(element[i]=='1')
           f=f+pow(2,(5-(i+1)));
       }
       return f;
   }
   
   
   int get_option(float f){
       
       if(f>10*M_PI) { return 6; }
       if(f>8*M_PI) { return 5; }
       if(f>6*M_PI) { return 4; }
       if(f>4*M_PI) { return 3; }
       if(f>2*M_PI) { return 2; }
       return 1;
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