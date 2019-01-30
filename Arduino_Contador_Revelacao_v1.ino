#include <EEPROM.h>
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define btnBack 4
#define btnNext 5
#define btnEnter 6
#define led 13
#define rele 12
#define apito 3

#define nove 9
#define onze 11
#define doze 12

long int menuLCD;
int back;
int next;
int enter;
  
unsigned long startMillis = 0;

int revelador;
int pausa;
int fixador;
int bleatch;

int menu01[nove] = {180, 120, 195, 120, 270, 120, 78, 180, 300};
int menu02[nove] = {180, 120, 210, 120, 270, 120, 78, 180, 300};
int menu03[nove] = {180, 120, 225, 120, 270, 120, 78, 180, 300};
int menu04[nove] = {180, 120, 240, 120, 270, 120, 78, 180, 300};

int menu05[nove] = {300, 120, 195, 120, 240, 120, 180, 60, 300};
int menu06[nove] = {300, 120, 210, 120, 360, 120, 180, 60, 300};
int menu07[nove] = {300, 120, 225, 120, 600, 120, 180, 60, 300};
int menu08[nove] = {300, 120, 240, 120, 900, 120, 180, 60, 300};

int menu09[nove] = {300, 120, 195, 120, 180, 120, 252, 60, 300};
int menu10[nove] = {300, 120, 195, 120, 380, 120, 378, 60, 300};

int menu11[doze] = {180, 120, 360, 120, 120, 360, 120, 360, 240, 120, 120, 300};
int menu12[doze] = {180, 120, 378, 120, 120, 378, 120, 360, 240, 120, 120, 300};

int menu13[doze] = {300, 120, 369, 138, 360, 138, 360, 240, 120};
int menu14[nove] = {300, 120, 378, 138, 420, 138, 480, 240, 120};
int menu15[nove] = {300, 120, 387, 138, 480, 138, 480, 240, 120};

int menu16[onze] = {360, 360, 120, 120, 240, 120, 360, 240, 120, 300, 180};
int menu17[onze] = {360, 498, 120, 180, 360, 180, 480, 480, 120, 300, 180};

String rev1[nove] = {"Prelavagem","Lavagem","Revelador","Lavagem","Bleatch","Lavagem","Fixador","Estabilizador","Lavagem Final"};
String rev2[doze] = {"Prelavagem","Lavagem","Revelador","Lavagem","Inversão","2 revelador","Condicionador","Bleatch","Fixador","Lavagem","Lavagem","Lavagem final"};
String rev3[nove] = {"Prelavagem","Lavagem","Revelador","Lavagem","Color developer","Lavagem","Bleatch Fix","Lavagem","Lavagem final"};
String rev4[onze] = {"Prelavagem","First Developer","Lavagem","Reversal","Color Developer","Pre Bletch","Bleatch","Fixer","Lavagem","Lavagem Final","Rinse"};

  /*****************************************************************************************************
  *****     SETUP
  *****************************************************************************************************/

  void setup() {
    Serial.begin(38400);
    Serial.println("Inicio");

    lcd.begin(16, 2);
    lcd.print("   Maquinas de  ");
    lcd.setCursor(0, 1);
    lcd.print("  Outros Tempos ");
    delay(3000);
    lcd.clear();
    
    pinMode(btnBack,INPUT_PULLUP);
    pinMode(btnNext,INPUT_PULLUP);
    pinMode(btnEnter,INPUT_PULLUP);
      
    pinMode(led, OUTPUT);
    pinMode(rele, OUTPUT);
      
    menuLCD = back = enter = next = 0;
    menuLCD = 1;

//    EEPROMWriteInt(0,300);
//    EEPROMWriteInt(2,300);
//    EEPROMWriteInt(4,300);
//    EEPROMWriteInt(6,300);

    revelador = EEPROMReadInt(0);
    pausa     = EEPROMReadInt(2);
    fixador   = EEPROMReadInt(4);
    bleatch   = EEPROMReadInt(6);
    Serial.print("Fim de setup");

    beep(50);
    beep(50);
    beep(50);
  }
   
  /*****************************************************************************************************
  *****     ESCREVE NA EEPROM UM INT
  *****************************************************************************************************/
  
  void EEPROMWriteInt(int address, int value) {
    byte two = (value & 0xFF);
    byte one = ((value >> 8) & 0xFF);  
    EEPROM.update(address, two);
    EEPROM.update(address + 1, one);
  }
  
  /*****************************************************************************************************
  *****     LE DA EEPROM UM INT
  *****************************************************************************************************/
  
  int EEPROMReadInt(int address) {
    long two = EEPROM.read(address);
    long one = EEPROM.read(address + 1);
    return ((two << 0) & 0xFFFFFF) + ((one << 8) & 0xFFFFFFFF);
  }

  /*****************************************************************************************************
  *****     LIGA AS SAIDAS
  *****************************************************************************************************/
  
  void ligaSaidas() { 
    digitalWrite(led, HIGH);
    digitalWrite(rele, HIGH);
    lcd.setCursor(14, 1);
    lcd.print("ON");
  }
  
  /*****************************************************************************************************
  *****     DESLIGA AS SAIDAS
  *****************************************************************************************************/
  
  void desligaSaidas() { 
    digitalWrite(led, LOW);
    digitalWrite(rele, LOW);
    lcd.setCursor(0, 1);
    lcd.print("   TERMINADO    ");
  }
  
  /*****************************************************************************************************
  *****     LIGA APITO
  *****************************************************************************************************/
  
  void ligaApito() {
    digitalWrite(apito,HIGH);
  }

  /*****************************************************************************************************
  *****     DESLIGA APITO
  *****************************************************************************************************/

  void desligaApito() {
    digitalWrite(apito,LOW);    
  }
  /*****************************************************************************************************
  *****     sec2min CONVERTE SEGUNDOS EM MM:SS PARA APRESENTAR NO DISPLAY
  *****************************************************************************************************/
  
  void sec2min(int sec) {
    int m = int(sec / 60);
    int s = int(sec % 60);
    lcd.setCursor(5, 1);
    if(m<10)
      lcd.print("0");
    lcd.print(m);
    lcd.print(":");
    if(s<10)
      lcd.print("0");
    lcd.print(s);
  }

  /*****************************************************************************************************
  *****     BEEP
  *****************************************************************************************************/

  void beep(unsigned char delayms) {
    analogWrite(apito, 20);
    delay(delayms); //Delaying
    analogWrite(apito ,0);
    delay(delayms); //Delaying    
  }
  
  /*****************************************************************************************************
  *****     CICLO DE REVELACAO
  *****************************************************************************************************/

  void cicloRevelacao(int tempos[], String titulo[], int n){
    Serial.println("ciclo revelacao");
    int i;
    for(i=0;i<n;i++){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(i+1);
      lcd.print("/");
      lcd.print(n);
      lcd.print(" ");
      lcd.print(titulo[i]);
//      Serial.print(i);
//      Serial.print("/");
//      Serial.print(n);
//      Serial.print(" ");
//      Serial.print(titulo[i]);
//      Serial.print(" : ");    
//      Serial.println(tempos[i]);
      ligaSaidas();  
      startMillis = millis();
      while((millis()-startMillis) / 1000 < tempos[i]) {
        sec2min(tempos[i] - int((millis()-startMillis)/1000));
        delay(1000);
      };
      desligaSaidas();
      ligaApito();
      while(digitalRead(btnNext)==1) {
        beep(50);
      }
      desligaApito();
      lcd.clear();
    }
  }
  
  /*****************************************************************************************************
  *****     REVELACAO P/B
  *****************************************************************************************************/
  
  void pb(){
    int i=0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PB 1/4 REVELADOR");
    ligaSaidas();
    startMillis = millis();
    while((millis()-startMillis) / 1000 < revelador) {
      sec2min(revelador - int((millis()-startMillis)/1000));
      delay(1000);
    };
    desligaSaidas();
    ligaApito();
    while(digitalRead(btnNext)==1) {
      beep(50);
    }
    desligaApito();

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PB 2/4 PAUSA");
    ligaSaidas();
    startMillis = millis();
    while((millis()-startMillis) / 1000 < pausa) {
      sec2min(pausa - int((millis()-startMillis)/1000));
      delay(1000);
    };
    desligaSaidas();
    ligaApito();
    while(digitalRead(btnNext)==1) {
      beep(50);
    }
    desligaApito();
  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PB 3/4 FIXADOR");
    ligaSaidas();
    startMillis = millis();
    while((millis()-startMillis) / 1000 < fixador) {
      sec2min(fixador - int((millis()-startMillis)/1000));
      delay(1000);
    };
    desligaSaidas();
    ligaApito();
    while(digitalRead(btnNext)==1) {
      beep(50);
    }
    desligaApito();
  
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("PB 4/4 BLEATCH");
    ligaSaidas();
    startMillis = millis();
    while((millis()-startMillis) / 1000 < bleatch) {
      sec2min(bleatch - int((millis()-startMillis)/1000));
      delay(1000);
    };
    desligaSaidas();
    ligaApito();
    while(digitalRead(btnNext)==1) {
      beep(50);
    }
    desligaApito();
    lcd.clear();
  }
  
  /*****************************************************************************************************
  *****     LOOP
  *****************************************************************************************************/
  
  void loop() {
  
  while(1){    
    switch(menuLCD) {
  
      /*****************************************************************************************************
      *****     NIVEL 1
      *****************************************************************************************************/

      case 1: {
        lcd.clear();
        lcd.setCursor(6, 0);
        lcd.print("COR");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 2;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 11;
            break;
          }
          delay(1);
        }
      }
      break;
     
      case 2:{
        lcd.setCursor(6,0);
        lcd.print("P&B");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 21;
            break;
          }
          delay(1);
        }
      }
      break;
  
      /*****************************************************************************************************
      *****     NIVEL 2
      *****************************************************************************************************/
  
      case 11: {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("COR/C41");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 12;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 111;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 1;
            break;
          }
          delay(1);
        }
      }
      break;
  
      case 12: {
        lcd.clear();
        lcd.setCursor(5, 0);
        lcd.print("COR/E6");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 11;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 121;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 1;
            break;
          }
          delay(1);
        }
      }
      break;
  
      case 21: {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("PB/REVELADOR");
        lcd.setCursor(5, 1);
        lcd.print(revelador);
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            revelador-=5;
            menuLCD = 21;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            EEPROMWriteInt(0, revelador);
            menuLCD = 22;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            revelador+=5;
            menuLCD = 21;
            break;
          }
          delay(1);
        }
      }
      break;
  
      case 22: {
        lcd.clear();
        lcd.setCursor(4, 0);
        lcd.print("PB/PAUSA");
        lcd.setCursor(5, 1);
        lcd.print(pausa);
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            pausa-=5;
            menuLCD = 22;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            EEPROMWriteInt(2,pausa);
            menuLCD = 23;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            pausa+=5;
            menuLCD = 22;
            break;
          }
          delay(1);
        }
      }
      break;
  
      case 23: {
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("PB/FIXADOR");
        lcd.setCursor(5, 1);
        lcd.print(fixador);
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            fixador-=5;
            menuLCD = 23;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            EEPROMWriteInt(4,fixador);
            menuLCD = 24;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            fixador+=5;
            menuLCD = 23;
            break;
          }
          delay(1);
        }
      }
      break;
  
      case 24: {
        lcd.clear();
        lcd.setCursor(3, 0);
        lcd.print("PB/BLEATCH");
        lcd.setCursor(5, 1);
        lcd.print(bleatch);
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            bleatch-=5;
            menuLCD = 24;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            EEPROMWriteInt(6,bleatch);
            menuLCD = 25;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            bleatch+=5;
            menuLCD = 24;
            break;
          }
          delay(1);
        }
      }
      break;
  
      case 25: {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("REVELACAO P&B");
        lcd.setCursor(3, 1);
        lcd.print("AVANCAR ?");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 22;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            pb();
            menuLCD = 2;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 21;
            break;
          }
          delay(1);
        }
      }
      break;
  
  
      /*****************************************************************************************************
      *****     NIVEL 3
      *****************************************************************************************************/
  
      case 111: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COR/C41/BELLINI");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 112;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 1111;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 11;
            break;
          }
          delay(1);
        }
      }
      break;
      case 112: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COR/C41/TETENAL");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 113;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 1121;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 11;
            break;
          }
          delay(1);
        }
      }
      break;
      case 113: {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("COR/C41/FUJI");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 111;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 1131;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 11;
            break;
          }
          delay(1);
        }
      }
      break;     
      case 121: {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("COR/E6/BELLINI");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 122;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 1211;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 11;
            break;
          }
          delay(1);
        }
      }
      break;
      case 122: {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("COR/E6/TETENAL");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 123;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 1221;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 11;
            break;
          }
          delay(1);
        }
      }
      break;     
      case 123: {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("COR/E6/FUJI");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 121;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 1231;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 11;
            break;
          }
          delay(1);
        }
      }
      break;
  
      /*****************************************************************************************************
      *****     NIVEL 4
      *****************************************************************************************************/
  
      case 1111: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COR/C41/BELLINI");
        lcd.setCursor(0, 1);
        lcd.print("1 A 4 ROLOS");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1112;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 11111;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 111;
            break;
          }
          delay(1);
        }
      }
      break;
      case 1112: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COR/C41/BELLINI");
        lcd.setCursor(0, 1);
        lcd.print("5 A 8 ROLOS");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1113;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 11121;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 111;
            break;
          }
          delay(1);
        }
      }
      break;
      case 1113: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COR/C41/BELLINI");
        lcd.setCursor(0, 1);
        lcd.print("9 A 12 ROLOS");
//        Serial.println("COR.C41.BELLINI.9-12");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1114;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 11131;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 111;
            break;
          }
          delay(1);
        }
      }
      break;
      case 1114: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COR/C41/BELLINI");
        lcd.setCursor(0, 1);
        lcd.print("12 A 16 ROLOS");
//        Serial.println("COR.C41.BELLINI.12-16");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1111;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 11141;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 111;
            break;
          }
          delay(1);
        }
      }
      break;
      case 1121: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COR/C41/TETENAL");
        lcd.setCursor(0, 1);
        lcd.print("1 A 4 ROLOS");
//        Serial.println("COR.C41.TETENAL.1-4");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1122;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 11211;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 111;
            break;
          }
          delay(1);
        }
      }
      break;
      case 1122: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COR/C41/TETENAL");
        lcd.setCursor(0, 1);
        lcd.print("5 A 8 ROLOS");
//        Serial.println("COR.C41.TETENAL.5-8");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1123;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 11221;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 111;
            break;
          }
          delay(1);
        }
      }
      break;    
      case 1123: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COR/C41/TETENAL");
        lcd.setCursor(0, 1);
        lcd.print("9 A 12 ROLOS");
//        Serial.println("COR.C41.TETENAL.9-12");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1124;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 11231;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 111;
            break;
          }
          delay(1);
        }
      }
      break;    
      case 1124: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COR/C41/TETENAL");
        lcd.setCursor(0, 1);
        lcd.print("12 A 16 ROLOS");
//        Serial.println("COR.C41.TETENAL.12-16");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1121;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 11241;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 111;
            break;
          }
          delay(1);
        }
      }
      break;    
      case 1131: {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("COR/C41/FUJI");
        lcd.setCursor(0, 1);
        lcd.print("5 A 9 ROLOS");
//        Serial.println("COR.C41.FUJI.5-9");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1132;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 11311;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 111;
            break;
          }
          delay(1);
        }
      }
      break; 
      case 1132: {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("COR/C41/FUJI");
        lcd.setCursor(0, 1);
        lcd.print("9 A 16 ROLOS");
//        Serial.println("COR.C41.FUJI.9-16");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1131;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 11321;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 111;
            break;
          }
          delay(1);
        }
      }
      break; 
      case 1211: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COR/E6/BELLINNI");
        lcd.setCursor(0, 1);
        lcd.print("1 A 4 ROLOS");
//        Serial.println("COR.E6.BELLINNI.1-4");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 121;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 1212;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 121;
            break;
          }
        }
      }
      break;
      case 1212: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COR/E6/BELLINNI");
        lcd.setCursor(0, 1);
        lcd.print("5 A 8 ROLOS");
//        Serial.println("COR.E6.BELLINI.5-8");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1211;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 12121;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 121;
            break;
          }
        }
      }
      break;
      case 1221: {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("COR/E6/TETENAL");
        lcd.setCursor(0, 1);
        lcd.print("1 A 4 ROLOS");
//        Serial.println("COR.E6.TETENAL.1-4");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1222;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 12211;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 121;
            break;
          }
        }
      }
      break;
      case 1222: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COR/E6/TETENAL");
        lcd.setCursor(0, 1);
        lcd.print("5 A 8 ROLOS");
//        Serial.println("COR.E6.TETENAL.5-8");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1223;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 12221;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 121;
            break;
          }
        }
      }
      break;
      case 1223: {
        lcd.clear();
        lcd.setCursor(1, 0);
        lcd.print("COR/E6/TETENAL");
        lcd.setCursor(0, 1);
        lcd.print("9 A 12 ROLOS");
//        Serial.println("COR.E6.TETENAL.9-12");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1221;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 12231;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 121;
            break;
          }
        }
      }
      break;
      case 1231: {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("COR/E6/FUJI");
        lcd.setCursor(0, 1);
        lcd.print("6 A 9 ROLOS");
//        Serial.println("COR.E6.FUJI.6-9");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1232;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 12311;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 121;
            break;
          }
        }
      }
      break;
      case 1232: {
        lcd.clear();
        lcd.setCursor(2, 0);
        lcd.print("COR/E6/FUJI");
        lcd.setCursor(0, 1);
        lcd.print("9 A 18 ROLOS");
//        Serial.println("COR.E6.FUJI.9-18");
        while (1){
          if(digitalRead(btnNext)==0){
            delay(20);
            while(digitalRead(btnNext)==0);
            delay(20);
            menuLCD = 1231;
            break;
          }
          else if(digitalRead(btnEnter)==0){
            delay(20);
            while(digitalRead(btnEnter)==0);
            delay(20);
            menuLCD = 12321;
            break;
          }
          else if(digitalRead(btnBack)==0){
            delay(20);
            while(digitalRead(btnBack)==0);
            delay(20);
            menuLCD = 121;
            break;
          }
        }
      }
      break;

      /*****************************************************************************************************
      *****     NIVEL 5
      *****************************************************************************************************/

      case 11111: {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("COR/QQ COISA");
        Serial.println("Contador 11111");
  //    tipo1(menu01);
        cicloRevelacao(menu01, rev1, 9);
        menuLCD = 1111;
        break;
      }
      break;
  
      case 11121: {
        Serial.println("Contador 11121");
   //     tipo1(menu02);
        cicloRevelacao(menu02, rev1, 9);
        menuLCD = 1111;
        break;
      }
      break;
  
      case 11131: {
        Serial.println("Contador 11131");
   //     tipo1(menu03);
        cicloRevelacao(menu03, rev1, 9);
        menuLCD = 1111;
        break;
      }
      break;
  
      case 11141: {
        Serial.println("Contador 11141");
   //     tipo1(menu04);
        cicloRevelacao(menu04, rev1, 9);
        menuLCD = 1111;
        break;
      }
      break;
  
      case 11211: {
        Serial.println("Contador 11211");
  //      tipo1(menu05);
        cicloRevelacao(menu05, rev1, 9);
        menuLCD = 1121;
        break;
      }
      break;
  
      case 11221: {
        Serial.println("Contador 11221");
  //      tipo1(menu06);
        cicloRevelacao(menu06, rev1, 9);
        menuLCD = 1121;
        break;
      }
      break;
  
      case 11231: {
        Serial.println("Contador 11231");
        cicloRevelacao(menu07, rev1, 9);
  //      tipo1(menu07);
        menuLCD = 1121;
        break;
      }
      break;
  
      case 11241: {
        Serial.println("Contador 11241");
  //      tipo1(menu08);
        cicloRevelacao(menu08, rev1, 9);
        menuLCD = 1121;
        break;
      }
      break;
  
      case 11311: {
        Serial.println("Contador 11311");
  //      tipo1(menu09);
        cicloRevelacao(menu09, rev1, 9);
        menuLCD = 1131;
        break;
      }
      break;
  
      case 11321: {
        Serial.println("Contador 11321");
   //     tipo1(menu10);
        cicloRevelacao(menu10, rev1, 9);
        menuLCD = 1131;
        break;
      }
      break;
  
      case 12111: {
        Serial.println("Contador 12111");
//        tipo1(menu11);
        cicloRevelacao(menu11, rev2, 12);
        menuLCD = 1211;
        break;
      }
      break;
  
      case 12121: {
        Serial.println("Contador 12121");
  //      tipo1(menu12);
        cicloRevelacao(menu12, rev2, 12);
        menuLCD = 1211;
        break;
      }
      break;
  
      case 12211: {
        Serial.println("Contador 12211");
  //      tipo1(*menu13);
        cicloRevelacao(menu13, rev3, 9);
        menuLCD = 1221;
        break;
      }
      break;
  
      case 12221: {
        Serial.println("Contador 12221");
//        tipo1(*menu14);
        cicloRevelacao(menu14, rev3, 9);
        menuLCD = 1221;
        break;
      }
      break;
  
      case 12231: {
        Serial.println("Contador 12231");
  //      tipo1(*menu15);
        cicloRevelacao(menu15, rev3, 9);
        menuLCD = 1221;
        break;
      }
      break;
  
      case 12311: {
        Serial.println("Contador 12311");
  //      tipo1(*menu16);
        cicloRevelacao(menu16, rev4, 11);
        menuLCD = 1231;
        break;
      }
      break;
  
      case 12321: {
        Serial.println("Contador 12231");
//        tipo1(*menu17);
        cicloRevelacao(menu17, rev4, 11);
        menuLCD = 1231;
        break;
      }
      break;
  
    }     // fim do state machine
  }       // fim do while infinito
}         // fim do loop
