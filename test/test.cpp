#include <Arduino.h>
#define NumberOf(arg) ((unsigned int) (sizeof (arg) / sizeof (arg [0]))) // calculates the number of layers (in this case 4)
#define _1_OPTIMIZE B11010110 // https://github.com/GiorgosXou/NeuralNetworks#macro-properties
#define Tanh
#include <NeuralNetwork.h>
      NeuralNetwork *NN;
//Motor
#define TT 9
#define NT 3
#define TP 10
#define NP 11
//Sensor
#define Left 20
#define M_Left 17
#define Middle 16
#define M_Right 15
#define Right 14
//
int VLeft;
int VM_Left;
int VMiddle;
int VM_Right;
int VRight;
int thamchieu = 350;
float sensor[1][5];
int pwmleft;
int pwmright;
const unsigned int layers[] = {5, 6, 6, 2};
float *output; // 4th layer's output(s)

// It is 1 for each layer  [Pretrained Biases ]
const PROGMEM float biases[] = {1,1,0.6364130};

// It is 784*7 + 7*9 + 9*1 [Pretrained weights] (81% Accuracy) | Possibly overtrained too lol [...]
const PROGMEM float weights[] = {
-1.8037566 ,-1.2396941 , 0.4800380 , 0.8286207 ,-0.4101121,
-0.1182027 ,-0.1652946 ,-1.1175197 ,-0.3640673 ,-0.5265324, 
-1.0775654 , 0.6625021 , 1.1846766 ,-0.2893540 , 0.0106971, 
0.2208483 ,-0.6149857 , 0.8021431 , 0.0478915 , 0.1732091, 
1.0991920 , 0.8253026 , 0.2847719 ,-1.4397324 ,-2.0069627, 
0.5356242 ,-0.4119368 ,-0.1662195 ,-1.6357931 ,-1.1053336, 

-0.0981182 ,-0.8577623 ,-1.0082198 ,-0.3198337 , 1.4856368 , 1.0048000, 
0.2279774 ,-0.5043206 ,-0.6509188 , 0.2550355 ,-0.4963381 ,-0.8147675 ,
0.1040516 , 0.7254746 , 0.5454139 ,-0.7968187 , 0.5222654 , 1.0549180 ,
0.3854138 , 0.6768500 ,-0.1395342 , 0.6559056 , 0.4616637 , 0.2182665 ,
-1.4634702 ,-0.5878224 ,-0.4777312 ,-0.8392019 ,-0.2607377 , 0.1622213 ,
1.3625763 ,-0.6635051 , 0.5878581 ,-0.5536258 ,-1.6941379 ,-0.5207168 ,


-0.5274420 ,-0.7338503 ,-0.0763561 , 1.5313644 , 0.2981606 , 1.6368551 ,
1.4578479 , 0.3601732 ,-0.2319831 , 1.1912211 ,-0.4899840 ,-0.4403914 
};


void setup()
{
  Serial.begin(9600);
  pinMode(Left, INPUT);
  pinMode(M_Left, INPUT);
  pinMode(Middle, INPUT);
  pinMode(M_Right, INPUT);
  pinMode(Right, INPUT);
  pinMode(TT, OUTPUT);
  pinMode(NT, OUTPUT);
  pinMode(TP, OUTPUT);
  pinMode(NP, OUTPUT);
//   NN = new NeuralNetwork(layers, weights, biases, NumberOf(layers)); // Creating a NeuralNetwork with pretrained Weights and Biases

  //Goes through all the input arrays
  NN->print();                           // Prints the weights & biases of each layer
}
int convert(int a, int b){
    if (a > b){
      return 1;} else {
        return 0;}
  }
void loop(){
    // hienthi();
  VLeft=convert(analogRead(Left),thamchieu);
  VM_Left=convert(analogRead(M_Left),thamchieu);
  VMiddle=convert(analogRead(Middle),thamchieu);
  VM_Right=convert(analogRead(M_Right),thamchieu);
  VRight=convert(analogRead(Right),thamchieu);
  sensor[0][0] = VLeft;
  sensor[0][1] = VM_Left;
  sensor[0][2] = VMiddle;
  sensor[0][3] = VM_Right;
  sensor[0][4] = VRight;
  Serial.print(VLeft); Serial.print(" - ");
  Serial.print(VM_Left); Serial.print(" - ");
  Serial.print(VMiddle); Serial.print(" - ");
  Serial.print(VM_Right); Serial.print(" - ");
  Serial.print(VRight); Serial.println("");
  // for (int i = 0; i < 1; i++)
  // {
  output = NN->FeedForward(sensor[0]); // FeedForwards the input[i]-array through the NN | returns the predicted output(s)
  pwmleft = output[0] * 100.0;
  pwmright = output[1] * 100.0;

  analogWrite(TT, pwmleft);
  Serial.print("speed_left:  ");
  Serial.println(pwmleft); 
  analogWrite(TP, pwmright);
  Serial.print("speed_right:  ");
  Serial.println(pwmright);

}
