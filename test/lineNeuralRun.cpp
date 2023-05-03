#include <Arduino.h>
#include <math.h>
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
double inputSensor[5];
double outputPWM[2];
double w1[5][5];
double w2[2][5];
double bias1 = 1.00;
double bias2 = 1.06;
double Z[5];
double A[5];
double ZZ[2];
double AA[2]; 
int thamchieu = 350;
int map2(float x, float in_min, float in_max, int out_min, int out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
double tanh2(double x)
{
  double x0 = exp(x);
  double x1 = 1.0 / x0;

  return ((x0 - x1) / (x0 + x1));
}

double Sigmoid(double x)
{
    return 1 / (1 + exp(-x));
}
void prediction()
  {
    Z[0]=((inputSensor[0]*w1[0][0] + inputSensor[1]*w1[0][1] + inputSensor[2]*w1[0][2] + inputSensor[3]*w1[0][3] + inputSensor[4]*w1[0][4]) + bias1);
    A[0]=tanh2(Z[0]);
    Z[1]=((inputSensor[0]*w1[1][0] + inputSensor[1]*w1[1][1] + inputSensor[2]*w1[1][2] + inputSensor[3]*w1[1][3] + inputSensor[4]*w1[1][4]) + bias1);
    A[1]=tanh2(Z[1]);
    Z[2]=((inputSensor[0]*w1[2][0] + inputSensor[1]*w1[2][1] + inputSensor[2]*w1[2][2] + inputSensor[3]*w1[2][3] + inputSensor[4]*w1[2][4]) + bias1);
    A[2]=tanh2(Z[2]);
    Z[3]=((inputSensor[0]*w1[3][0] + inputSensor[1]*w1[3][1] + inputSensor[2]*w1[3][2] + inputSensor[3]*w1[3][3] + inputSensor[4]*w1[3][4]) + bias1);
    A[3]=tanh2(Z[3]);
    Z[4]=((inputSensor[0]*w1[4][0] + inputSensor[1]*w1[4][1] + inputSensor[2]*w1[4][2] + inputSensor[3]*w1[4][3] + inputSensor[4]*w1[4][4]) + bias1);
    A[4]=tanh2(Z[4]);

    ZZ[0]=((w2[0][0]*A[0] + w2[0][1]*A[1] + w2[0][2]*A[2] + w2[0][3]*A[3] + w2[0][4]*A[4] + bias2));
    ZZ[1]=((w2[1][0]*A[0] + w2[1][1]*A[1] + w2[1][2]*A[2] + w2[1][3]*A[3] + w2[1][4]*A[4] + bias2));

    AA[0]= Sigmoid(ZZ[0]);
    AA[1]= Sigmoid(ZZ[1]);

    outputPWM[0]=AA[0];
    outputPWM[1]=AA[1];
  }
void setup(){
    //Hidden layer 
    w1[0][0] = -2.7218410;w1[0][1] = -1.7322509;w1[0][2] = -0.0630482;w1[0][3] = 1.1421115 ;w1[0][4] = 1.8137315 ;
    w1[1][0] = 1.2863220 ;w1[1][1] = 1.2715729 ;w1[1][2] = -1.7594139;w1[1][3] = -1.4040303;w1[1][4] = -0.9387921;
    w1[2][0] = -0.7887248;w1[2][1] = 0.4232717; w1[2][2] = 0.5688917; w1[2][3] = -0.0022334;w1[2][4] = 0.7640701;
    w1[3][0] = 0.8476304; w1[3][1] = -0.6896469;w1[3][2] = 0.0485468; w1[3][3] = -0.9427285;w1[3][4] = -0.3839036;
    w1[4][0] = 2.0674223; w1[4][1] = 1.7499374; w1[4][2] = 0.3059014; w1[4][3] = -2.1621260;w1[4][4] = -3.7013397;
    // Output layer
    w2[0][0] = 4.3702783 ;w2[0][1] = -2.8619201;w2[0][2] = 1.1892703;w2[0][3] = -0.9473322;w2[0][4] = -2.1684892;
    w2[1][0] = -1.7207040;w2[1][1] = -0.3556673;w2[1][2] = -0.3422399;w2[1][3] = -0.1001826;w2[1][4] = 5.6455578;

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

}
int convert(int a, int b){
    if (a > b){
      return 1;} else {
        return 0;}
  }
void controlMotor()
{
  int leftPWM=map2(outputPWM[0],0,1,0,150);
  int rightPWM=map2(outputPWM[1],0,1,0,150);
  analogWrite(TT,leftPWM);
  digitalWrite(NT,LOW);
  analogWrite(TP,rightPWM);
  digitalWrite(NP,LOW);
  Serial.print("PWM Left: "); Serial.print(leftPWM); 
  Serial.print(" PWM Right: "); Serial.println(rightPWM); 
}
void loop(){
    inputSensor[0] = convert(analogRead(Left),thamchieu);
    inputSensor[1] = convert(analogRead(M_Left),thamchieu);
    inputSensor[2] = convert(analogRead(Middle),thamchieu);
    inputSensor[3] = convert(analogRead(M_Right),thamchieu);
    inputSensor[4] = convert(analogRead(Right),thamchieu);
    Serial.print(" Left: "); Serial.print(inputSensor[0]);
    Serial.print(" M_Left: "); Serial.print(inputSensor[1]);
    Serial.print(" Middle: "); Serial.print(inputSensor[2]);
    Serial.print(" M_Right: "); Serial.print(inputSensor[3]);
    Serial.print(" Right: "); Serial.println(inputSensor[4]);
    prediction();
    controlMotor();
}
