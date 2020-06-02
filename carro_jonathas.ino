#include <SoftwareSerial.h>
#include <Servo.h>

const int pinoLedVerde = 8;
const int pinoLedVermelho = 7;

const int pinoServo = 9; //PINO DIGITAL UTILIZADO PELO SERVO  
const int pinoRX = 3; //PINO DIGITAL 2 (RX)
const int pinoTX = 2; //PINO DIGITAL 3 (TX)
const int pinoLed = 13; //PINO DIGITAL UTILIZADO PELO LED
char dadoBluetooth = '#'; //VARIÁVEL QUE ARMAZENA O VALOR ENVIADO PELO BLUETOOTH
boolean loopLED = false; //VARIÁVEL BOOLEANA QUE FAZ O CONTROLE DE ATIVAÇÃO DO LOOP DO LED

const int pinIn1 = 4;
const int pinIn2 = 5;
const int pinVelocidade = 11;
int sentido = 1; //1-frente, 0-re

boolean estaAcelerando = false;
int velocidade = 80;
const int velocidadeInicial = 80;

Servo s; //OBJETO DO TIPO SERVO
int pos; //POSIÇÃO DO SERVO

boolean ledVerde = false;
boolean ledVermelho = false;


SoftwareSerial bluetooth(pinoRX, pinoTX);

void setup() {
  //L298
  pinMode(pinIn1, OUTPUT);
  pinMode(pinIn2, OUTPUT);
  pinMode(pinoLedVerde, OUTPUT);
  pinMode(pinoLedVermelho, OUTPUT);
  pinMode(pinVelocidade, OUTPUT);
  
  //SERVO
  s.attach(pinoServo); //ASSOCIAÇÃO DO PINO DIGITAL AO OBJETO DO TIPO SERVO
  s.write(136); //INICIA O MOTOR NA POSIÇÃO

  //BLUETOOTH
  Serial.begin(9600); //INICIALIZA A SERIAL
  bluetooth.begin(9600); //INICIALIZA A SERIAL DO BLUETOOTH
  //bluetooth.print("$"); //IMPRIME O CARACTERE
  delay(100); //INTERVALO DE 100 MILISSEGUNDOS
  pinMode(pinoLed, OUTPUT); //DEFINE O PINO COMO SAÍDA
}

void loop() {
  
  if(bluetooth.available()){ //SE O BLUETOOTH ESTIVER HABILITADO, FAZ
    //Serial.println("BLUETOOTH LIGADO");
    dadoBluetooth = bluetooth.read(); 
    //Serial.println(dadoBluetooth);
     
    if(dadoBluetooth == '1'){       
      //Serial.println("ESQUERDA");
      s.write(161);
      estaAcelerando = true;
      if(sentido == 1){        
        frente();
      }else {
        re();
      }            
    }else if(dadoBluetooth == '3'){ 
      //Serial.println("DIREIRA");
      estaAcelerando = true;
      s.write(90);      
      if(sentido == 1){
        frente();
      }else {
        re();
      }             
    }
    if(dadoBluetooth =='0'|| dadoBluetooth=='2'){
      s.write(136);      
      parar();
      estaAcelerando = false;
      velocidade = velocidadeInicial;
    }
    
    if(dadoBluetooth == '5'){      
      sentido = 1;
      frente();
      estaAcelerando = true;
    }else if(dadoBluetooth == '7'){      
      sentido = 0;
      re();
      estaAcelerando = true;
    }else if(dadoBluetooth == '4' || dadoBluetooth == '6'){       
       parar();
       estaAcelerando = false;
       velocidade = velocidadeInicial;
    }
  }
  //Serial.println(velocidade);
  if(estaAcelerando && velocidade<=255){    
    //Serial.println(velocidade);
    analogWrite(pinVelocidade, velocidade);
    velocidade = velocidade + 3;
  }
  
  if(dadoBluetooth == '9'){
    ledVermelho = !ledVermelho;
    delay(100);
  }
  if(ledVermelho == true){
    digitalWrite(pinoLedVermelho, HIGH);
  }else{
    digitalWrite(pinoLedVermelho, LOW);
  }

  if(dadoBluetooth == 'f'){
    ledVerde = !ledVerde;
    delay(100);
  }
  if(ledVerde == true){
    digitalWrite(pinoLedVerde, HIGH);
  }else{
    digitalWrite(pinoLedVerde, LOW);
  }
  
  delay(50);
}

void frente() {
  digitalWrite(pinIn1, HIGH);
  digitalWrite(pinIn2, LOW);   
}

void re() {
  digitalWrite(pinIn1, LOW);
  digitalWrite(pinIn2, HIGH);  
}

void parar() {
  analogWrite(pinVelocidade, 0);
  digitalWrite(pinIn1, LOW);
  digitalWrite(pinIn2, HIGH);
}
