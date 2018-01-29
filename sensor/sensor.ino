//Programa: Conectando Sensor Ultrassonico HC-SR04 ao Arduino
//Autor: FILIPEFLOP

//Carrega a biblioteca do sensor ultrassonico
#include <Ultrasonic.h>

//Define os pinos para o trigger e echo
#define pino_trigger1 4
#define pino_echo1 5
#define pino_trigger2 6
#define pino_echo2 7
#define sensorPin1 A3
#define sensorPin2 A4
#define sensorPin3 A5
float limiar_sonic=20.0;
float cmMsec;
long microsec;
float estado_inicial1;
float estado_inicial2;
float estado_inicial3;
float estado_atual;
//Inicializa o sensor nos pinos definidos acima
Ultrasonic ultrasonic1(pino_trigger1, pino_echo1);
Ultrasonic ultrasonic2(pino_trigger2, pino_echo2);

void setup()
{
  Serial.begin(9600);
  pinMode(sensorPin1,INPUT);
  pinMode(sensorPin2,INPUT);
  pinMode(sensorPin3,INPUT);
  estado_inicial1=analogRead(sensorPin1);
  estado_inicial2=analogRead(sensorPin2);
  estado_inicial3=analogRead(sensorPin3);
}

void loop(){

  if(sonic_frente()){
  
    Serial.println("<a>"); //avançar
  }

  if(sonic_tras()){
  
    Serial.println("<g>"); //girar 360
  }

  if(detecta_linha_frenteE()){

    Serial.println("<e>"); //girar sentido anti-horario motor esquerdo e sentido horario direito
  }

  if(detecta_linha_frenteD()){

    Serial.println("<d>"); //girar sentido anti-horario motor direito e sentido horario esquerdo
  }

  if(detecta_linha_tras()){

    Serial.println("<a>"); //avançar
  }
}





bool sonic_frente()
{
  microsec = ultrasonic1.timing();
  cmMsec = ultrasonic1.convert(microsec, Ultrasonic::CM);
  if(cmMsec<limiar_sonic){
    return true;
    }
  else{
    return false;
    }
}
bool sonic_tras()
{
  microsec = ultrasonic2.timing();
  cmMsec = ultrasonic2.convert(microsec, Ultrasonic::CM);
  if(cmMsec<limiar_sonic){
    return true;
    }
  else{
    return false;
    }
}
bool detecta_linha_frenteE()
{
  estado_atual=analogRead(sensorPin1);
  if(estado_atual<(estado_inicial1-100) || estado_atual>(estado_inicial1+100)){
    return true;
  }
  else{
    return false;
    }
}
bool detecta_linha_frenteD()
{
  estado_atual=analogRead(sensorPin2);
  if(estado_atual<(estado_inicial2-100) || estado_atual>(estado_inicial2+100)){
    return true;
  }
  else{
    return false;
    }
}
bool detecta_linha_tras()
{
  estado_atual=analogRead(sensorPin3);
  if(estado_atual<(estado_inicial3-100) || estado_atual>(estado_inicial3+100)){
    return true;
  }
  else{
    return false;
    }
}
