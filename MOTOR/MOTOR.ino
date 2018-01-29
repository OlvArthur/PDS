#include <TaskScheduler.h>
#include <Ultrasonic.h>

#define BRAKEVCC 0
#define CW 1
#define CCW 2
#define BRAKEGND 3
#define CS_THRESHOLD 15   // Definição da corrente de segurança (Consulte: "1.3) Monster Shield Exemplo").
#define direito 0
#define esquerdo 1
#define pino_trigger1 47
#define pino_echo1 49
#define pino_trigger2 51
#define pino_echo2 53
#define sensorPin1 A13
#define sensorPin2 A14
#define sensorPin3 A15

float limiar_sonic = 30;
float cmMsec;
long microsec;
float estado_inicial1;
float estado_inicial2;
float estado_inicial3;
float estado_atual;
//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic1(pino_trigger1, pino_echo1);
Ultrasonic ultrasonic2(pino_trigger2, pino_echo2);

char msg;
char receivedChars[50];
const byte numChars = 50;
bool newData = false; 

int inApin[2] = {7, 4}; // INA: Sentido Horário Motor0 e Motor1 (Consulte:"1.2) Hardware Monster Motor Shield").
int inBpin[2] = {8, 9}; // INB: Sentido Anti-Horário Motor0 e Motor1 (Consulte: "1.2) Hardware Monster Motor Shield").
int pwmpin[2] = {5, 6};            // Entrada do PWM
int cspin[2] = {2, 3};              // Entrada do Sensor de Corrente

int statpin = 13;       //pino motor
int i=0;

void setup()                         // Faz as configuração para a utilização das funções no Sketch
{
  Serial.begin(9600);              // Iniciar a serial para fazer o monitoramento
  pinMode(statpin, OUTPUT);
  pinMode(sensorPin1,INPUT);
  pinMode(sensorPin2,INPUT);
  pinMode(sensorPin3,INPUT);
  estado_inicial1=analogRead(sensorPin1);
  estado_inicial2=analogRead(sensorPin2);
  estado_inicial3=analogRead(sensorPin3);
  for (int i=0; i<2; i++)
  {
    pinMode(inApin[i], OUTPUT);
    pinMode(inBpin[i], OUTPUT);
    pinMode(pwmpin[i], OUTPUT);
  }
  
  for (int i=0; i<2; i++)
  {
    digitalWrite(inApin[i], LOW);
    digitalWrite(inBpin[i], LOW);
  }
}

void loop()                          // Programa roda dentro do loop
{
  Busca();
  if(sonic_frente()){
    paraFrente();
    
  }

  if(sonic_tras()){
    anti_horarioEsquerdo();
    
  }

  if(detecta_linha_frenteE()){
    anti_horarioEsquerdo();
    
  }

  if(detecta_linha_frenteD()){
    anti_horarioDireito();
    
  }

  if(detecta_linha_tras()){
    paraFrente();
   
  }
//  paraFrente();
//  delay(5000);
//  Stop();
//  paraTras();
//  delay(5000);
//  Stop();
}

//void motorGo(uint8_t motor, uint8_t direct, uint8_t pwm)         //Função que controla as variáveis: motor(0 ou 1), sentido (cw ou ccw) e pwm (entra 0 e 255);
//{
//  if (motor <= 1)
//  {
//    if (direct <=4)
//    {
//      if (direct <=1)
//        
//        digitalWrite(inApin[motor], HIGH);
//      else
//        digitalWrite(inApin[motor], LOW);
//
//      if ((direct==0)||(direct==2))
//        digitalWrite(inBpin[motor], HIGH);
//      else
//        digitalWrite(inBpin[motor], LOW);
//
//        analogWrite(pwmpin[motor], pwm);
//    }
//  }
//}

void paraFrente(){
  
  for(int i = 0; i < 2; i++){
  
    digitalWrite(inApin[i], HIGH);
    analogWrite(pwmpin[i], 255);
  }

}
void Stop(){
  
  for(int i = 0; i < 2; i++){
  
    digitalWrite(inApin[i], LOW);
    digitalWrite(inBpin[i], LOW);
    analogWrite(pwmpin[i], 255);
  }

}
void paraTras(){
 
  digitalWrite(inBpin[0], HIGH);
  analogWrite(pwmpin[0], 255);
  digitalWrite(inBpin[1], HIGH);
  analogWrite(pwmpin[1], 213);

}
void Busca(){
 
   anti_horarioEsquerdo(); 

}
void anti_horarioEsquerdo(){
  for(int i = 0; i < 2; i++){
  
    digitalWrite(inApin[i], LOW);
    analogWrite(pwmpin[i], 255);
  }

 
}

void anti_horarioDireito(){
  for(int i = 0; i < 2; i++){
  
    digitalWrite(inApin[i], LOW);
    analogWrite(pwmpin[i], 255);
  }

  digitalWrite(inBpin[direito], HIGH);
}

void serialRead() {
  static boolean recvInProgress = false;
  static byte ndx = 0;
  char startMarker = '<';
  char endMarker = '>';

  // if (Serial.available() > 0) {
  while (Serial.available() > 0 && newData == false) {
    msg = Serial.read();

    if (recvInProgress == true) {
      if (msg != endMarker) {
        receivedChars[ndx] = msg;
        ndx++;
          if (ndx >= numChars) {
            ndx = numChars - 1;
          }
          }
        else {
        receivedChars[ndx] = '\0'; // terminate the string
        recvInProgress = false;
        ndx = 0;
        newData = true;
        }
    }

    else if (msg == startMarker) {
      recvInProgress = true;
    }
  }
}
bool sonic_frente()//retorna true se for detectado algo em um raio de 30cm pelo sensor ultrassonico frontal
{
//  microsec = ultrasonic1.timing();
//  cmMsec = ultrasonic1.convert(microsec, Ultrasonic::CM);
  cmMsec = ultrasonic1.Ranging(CM);
  Serial.println(cmMsec);
  delay(10);
  if(cmMsec<limiar_sonic){
    return true;
    }
  else{
    return false;
    }
}
bool sonic_tras()//retorna true se for detectado algo em um raio de 30cm pelo sensor ultrassonico traseiro
{
  //microsec = ultrasonic2.timing();
  //cmMsec = ultrasonic2.convert(microsec, Ultrasonic::CM);
  //Serial.println(cmMsec);
  //delay(10);
  if(cmMsec<limiar_sonic){
    return true;
    }
  else{
    return false;
    }
}
bool detecta_linha_frenteE()//retorna true se houver detecção de linha no sensor frontal esquedo
{
  estado_atual=analogRead(sensorPin1);
  if(estado_inicial1 - estado_atual > 200){
    return true;
  }
  else{
    return false;
    }
}
bool detecta_linha_frenteD()//retorna true se houver detecção de linha no sensor frontal direito
{
  estado_atual=analogRead(sensorPin2);
  if(estado_inicial2 - estado_atual > 200){
    return true;
  }
  else{
    return false;
    }
}
bool detecta_linha_tras()//retorna true se houver detecção de linha no sensor traseiro
{
  estado_atual=analogRead(sensorPin3);
  if(estado_inicial3 - estado_atual > 200){
    return true;
  }
  else{
    return false;
    }
}
