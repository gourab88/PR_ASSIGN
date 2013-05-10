#include<malloc.h>
#include <cstdlib>
#include<cmath>
#include"Neuron.h"
#define theta 0.2
#define TrainingSet "IRISDATA.txt"
#define TestSet "IRISDATA_TEST.txt"
#define step_value 2.0
#define Num_Hidden_Nodes 6
#define Epsilon 0.000001
using namespace std;


class Mlp_model
{
	int trainingpoints, Num_Inputs, Classes;
	float **data_values;
	int *class_label;
	int Hidden_Nodes;
	float Learning_rate ;
	Neuron **nd;
	public:
		bool Initialization(FILE *);
		void Calculate_Active_values(float *, float);
		bool MLP_Training(long int , float);
		int MLP_Weight_modification(long int );
		void Mlp_Test(float );
		float Activation_func_value(float *, int ,float );
		
};


bool Mlp_model::Initialization(FILE *ip)
{

	ip=fopen(TrainingSet,"r");
	fscanf(ip,"%d %d %d",&trainingpoints, &Num_Inputs, &Classes);
	Num_Inputs++;
	if(trainingpoints < Classes)
		return(false);
	//Read File For Input Data
	data_values = new float *[trainingpoints];
	class_label = new int [trainingpoints];
	for(int i=0; i<trainingpoints; i++)
	{
		data_values[i] = new float [Num_Inputs];
		for(int j=0; j<Num_Inputs - 1; j++) {
			fscanf(ip, "%f ", &data_values[i][j]);
		}
		data_values[i][Num_Inputs - 1] = 1.0;
		fscanf(ip, "%d", &class_label[i]);
		
	}
	
	//Input layer 
	nd = new Neuron *[3];
	nd[0] = new Neuron [Num_Inputs];
	for(int h=0; h<Num_Inputs; h++)
		nd[0][h].SetParametre(0, h);

	//case 2 : Hidden layer 
	Hidden_Nodes = Num_Hidden_Nodes;
	nd[1] = new Neuron [Hidden_Nodes];
	for(int i = 0; i < Hidden_Nodes; i++)
	{
		nd[1][i].Initialize_wt(Num_Inputs);
		nd[1][i].SetParametre(1, i);
	}

	// Output Layer
	nd[2] = new Neuron [Classes];
	for(int i = 0; i < Classes; i++)
	{
		nd[2][i].Initialize_wt(Hidden_Nodes);
		nd[2][i].SetParametre(2, i);
	}
	Learning_rate = theta;
	fclose(ip);
	return(true);
}

void Mlp_model::Calculate_Active_values(float *data_pts, float sigma)
{
	
	// Output of nodes at Input layer  
	for(int i=0;i < Num_Inputs;i++) 
		nd[0][i].Active_value = data_pts[i];
	
	//Output of nodes at Hidden layer
	float *out_values;
	out_values = new float [Num_Inputs];
	for(int r=0;r < Num_Inputs;r++)
		out_values[r] = nd[0][r].Active_value;
	for(int j = 0;j < Hidden_Nodes;j++)
	{
		float *inp_vec;
		inp_vec = new float [Num_Inputs];
		for(int r=0;r < Num_Inputs;r++)
			inp_vec[r] = out_values[r] * nd[1][j].weights[r];
		nd[1][j].Active_value = Activation_func_value(inp_vec, Num_Inputs, sigma);
		delete inp_vec;
	}
	
	//Output of nodes at Output layer
	out_values = new float [Hidden_Nodes];
	for(int r=0;r < Hidden_Nodes;r++)
		out_values[r] = nd[1][r].Active_value;
	for(int j = 0;j < Classes;j++)
	{
		float *inp_vec;
		inp_vec = new float [Hidden_Nodes];
		for(int r=0;r < Hidden_Nodes;r++)
			inp_vec[r] = out_values[r] * nd[2][j].weights[r];
		nd[2][j].Active_value = Activation_func_value(inp_vec, Hidden_Nodes, sigma);
		delete inp_vec;
	}

	delete out_values;
	return;
	
}


float Mlp_model::Activation_func_value(float *ip_wht,int no_fet,float sigma)
{
	float value = 0.0;
	for(int i = 0;i < no_fet;i++)
		value = value + ip_wht[i];
	value = 1.0 / (1.0 + exp(-sigma * value));

	return(value);
	
}

int Mlp_model::MLP_Weight_modification(long int Node_Index)
{
	int ind = Node_Index % trainingpoints;
	int cl = class_label[ind];
	float temp1 =0;
	int flag = 0;
	float *d;
	d = new float [Classes];
	for(int i=0;i < Classes; i++)
		d[i] = 0.0;
	d[cl - 1] = 1.0;
	
	for(int i = 0;i < Classes; i++) {
		float yj = nd[2][i].Active_value;
		nd[2][i].delta = (d[i] - yj)* yj * (1 - yj);
		for(int j = 0;j < Hidden_Nodes; j++)
		{	
			temp1 = nd[2][i].weights[j] + Learning_rate * nd[2][i].delta * nd[1][j].Active_value;
			if( (temp1 - nd[2][i].weights[j] > Epsilon) || (temp1 - nd[2][i].weights[j] < -Epsilon)) flag = 1;
			nd[2][i].weights[j] = temp1;
			//nd[2][i].weights[j] = nd[2][i].weights[j] + Learning_rate * nd[2][i].delta * nd[1][j].Active_value;
		}
	}

	for(int i = 0;i < Hidden_Nodes; i++) {
		float yj = nd[1][i].Active_value;
		float temp = 0.0;
		for(int k=0;k < Classes;k++)
			temp = temp + nd[2][k].delta * nd[2][k].weights[i];
		nd[1][i].delta = temp * yj * (1 - yj);
		for(int j = 0;j < Num_Inputs; j++)
		{
			temp1 = nd[1][i].weights[j] + Learning_rate * nd[1][i].delta * nd[0][j].Active_value;
			if( (temp1 - nd[1][i].weights[j] > Epsilon) || (temp1 - nd[1][i].weights[j] < -Epsilon)) flag = 1;
			nd[1][i].weights[j] = temp1;
			//nd[1][i].weights[j] = nd[1][i].weights[j] + Learning_rate * nd[1][i].delta * nd[0][j].Active_value;
		}
			
	}
	
	return flag;
}

bool Mlp_model::MLP_Training(long int MAX_ITER, float sigma)
{
	bool flag = false;
	int flag1 = 1; 
	long int iter = 0;
	while(iter < MAX_ITER || flag == true)
	{
		
		Calculate_Active_values(data_values[iter % trainingpoints], sigma);
		flag1 = MLP_Weight_modification(iter);
		if(flag1 ==0) break;
		iter++;
	}
	printf("No Of Iterations taken are:%d", iter);
	return true;
}


void Mlp_model::Mlp_Test(float sigma)
{

	float temp;
	FILE *ipt, *opt;

	int cnfusion[Classes][Classes];
	for(int i =0; i< Classes; i++)
	{
		for(int j =0;j<Classes; j++)
		{
			cnfusion[i][j]=0;
		}
		
	}
	ipt = fopen(TestSet,"r");

	int no_sample_pts;
	fscanf(ipt, "%d", &no_sample_pts);
	for(int t = 0; t < no_sample_pts; t++)
	{
		float *inp_data;
		int class_lab;
		inp_data = new float [Num_Inputs];
		for(int i = 0;i < Num_Inputs - 1; i++){
			fscanf(ipt, "%f ", &temp);
			inp_data[i] = temp;
		}
		
		fscanf(ipt, "%d ", &class_lab);
		inp_data[Num_Inputs - 1] = 1.0;
		Calculate_Active_values(inp_data, sigma);
		float max = 0.0;
		int max_Node_Index;
		for(int i = 0; i < Classes;i++)
		{
			if(max < nd[2][i].Active_value)
			{
				max = nd[2][i].Active_value;
				max_Node_Index = i + 1;
				
			}
		}
		cnfusion[class_lab-1][max_Node_Index-1]++;
		
		delete inp_data;
		
	}
	printf("\nConfusion Matrix\n");
	for(int i =0; i< Classes; i++)
	{
		for(int j =0;j<Classes; j++)
		{
				printf("%d\t", cnfusion[i][j]);
		}

		printf("\n");
	}
	int error = 0;
	for(int i =0; i< Classes; i++)
	{
		for(int j =0;j<Classes; j++)
		{
			if(i != j)
			{
				error += cnfusion[i][j];
			}
		}

	}
	//cout<<"Error Rate = "<<error/no_sample_pts<<endl;
	fclose(ipt);
}


int main()
{
	Mlp_model mlp;
	FILE *fl;
	if(!mlp.Initialization(fl))
	{
		cout<<"The Data-Set is invalid ";
		return 0;
	}
	cout<<"Enter number of Iteration to Train MLP :";
	long int iteration_no;
	cin>>iteration_no;
	//cout<<"Enter the value of sigma for Activation function :";
	float sigma = step_value;

	if(!mlp.MLP_Training(iteration_no, sigma))
		return 0;

	mlp.Mlp_Test(sigma);
	
	
	
}


