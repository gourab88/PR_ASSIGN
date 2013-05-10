#include<iostream>
#include<string.h>

class Neuron
{
	int Layer;
	int Node_Index;

	public:
		float Active_value;
		float *weights;
		float delta;
		void Initialize_wt(int );
		void SetParametre(int ,int );
};


void Neuron::Initialize_wt(int inputs)
{
	weights = new float[inputs];
	for(int i=0; i<inputs; i++) 
	{
		weights[i] = rand() /(RAND_MAX + 1.0);
	}
}


void Neuron::SetParametre(int i, int j)
{
	Layer = i;
	Node_Index = j;
}
