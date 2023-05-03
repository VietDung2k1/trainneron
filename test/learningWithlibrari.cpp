#include <Arduino.h>

#define NumberOf(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0]))) // calculates the number of layers (in this case 3)
#define _1_OPTIMIZE B00001000 // https://github.com/GiorgosXou/NeuralNetworks#macro-properties
#define ACTIVATION__PER_LAYER // DEFAULT KEYWORD for allowing the use of any Activation-Function per "Layer-to-Layer".
        #define Sigmoid // 0     Says to the compiler to compile the Sigmoid Activation-Function 
        #define Tanh    // 1     Says to the compiler to compile the Tanh    Activation-Function 

#include <NeuralNetwork.h>
          NeuralNetwork *NN;
unsigned long countlearning = 0;
const unsigned int layers[] = {5, 2, 2}; // 3 layers (1st)layer with 3 input neurons (2nd)layer 5 hidden neurons each and (3rd)layer with 1 output neuron
byte Actv_Functions[] = {1, 0}; // 1 = Tanh and 0 = Sigmoid (just as a proof of consept)
float *output; // 4th layer's output(s) 
//Default Inputs/Training-Data
const float inputs[22][5] = {
{0,1,0,1,1},
{0,1,0,0,1},
{1,0,1,1,1},
{1,0,0,1,1},
{0,1,1,1,1},
{0,0,1,1,1},
{0,0,0,0,1},
{0,0,0,1,1},
{0,0,0,1,0},
{0,0,1,1,0},
{0,0,1,0,0},
{0,1,1,1,0},
{0,1,1,0,0},
{0,1,0,0,0},
{1,1,0,0,0},
{1,0,0,0,0},
{1,1,1,0,0},
{1,1,1,1,0},
{1,1,0,0,1},
{1,1,1,0,1},
{1,0,0,1,0},
{1,1,0,1,0}
};

const float expectedOutput[22][2] = {
{1,0},
{1,0},
{1,0},
{1,0},
{1,0},
{1,0},
{1,0},
{1,0},
{1,0},
{1,0},
{1,1},
{1,1},
{0,1},
{0,1},
{0,1},
{0,1},
{0,1},
{0,1},
{0,1},
{0,1},
{0,1},
{0,1}
}; // values that we are expecting to get from the 3rd/(output)layer of Neural-Network, in other words something like a feedback to the Neural-network.

void setup()
{
  Serial.begin(9600);
  Serial.println("---");
  NN = new NeuralNetwork(layers, NumberOf(layers), Actv_Functions); // Creating a Neural-Network with default learning-rates
  Serial.println("Setupdone");
  do{ 
    for (int j = 0; j < NumberOf(inputs); j++) // Epoch
    {
      NN->FeedForward(inputs[j]);      // FeedForwards the input arrays through the NN | stores the output array internally
      NN->BackProp(expectedOutput[j]); // "Tells" to the NN if the output was the-expected-correct one | then, "teaches" it
    }
    
    // Prints the Error.
    Serial.print("MSE: "); 
    Serial.println(NN->MeanSqrdError,9);
    countlearning ++;
    // Loops through each epoch Until MSE goes < 0.003
  }while(NN->getMeanSqrdError(NumberOf(inputs)) > 0.02);

  Serial.print("Number of times to study: "); 
  Serial.println(countlearning);
  Serial.println("\n =-[OUTPUTS]-=");


  //Goes through all the input arrays
  for (int i = 0; i < NumberOf(inputs); i++)
  {
    output = NN->FeedForward(inputs[i]); // FeedForwards the input[i]-array through the NN | returns the predicted output(s)
    Serial.print(output[0], 2);
    Serial.print("  -  ");
    Serial.println(output[1], 2);       
  }
  NN->print();                           // Prints the weights and biases of each layer
}
void loop() {
}


