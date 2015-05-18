
//ethernet weather station by Sukhmanbir Kaur
//This sketch hits a php script (weatherstation.php) on my server which parses json returned from yahoo weather API
#include <SPI.h>
#include <Ethernet.h>
#include <LiquidCrystal.h>
#include <Wire.h>
#include <LCD.h>
#include <LiquidCrystal_I2C.h>

const byte thermistorPin = 8;
const unsigned int thermistorNominal = 50000;
const byte temperatureNominal = 25;
const byte numSamples = 5;
const int bCoefficient = 3950;
const unsigned int seriesResistor = 51000;
int samples[numSamples];            

boolean readingdata = false;
byte upButton = 2;
byte downButton = 3;
int i = 0;
int level = 0;f7 
int menu = 1;
char title[50];
char lat[30];
char longt[30];
char date[30];
char day[30];
char high[30];
char low[30];
char text[30];
char dateTom[30];
char dayTom[30];
char highTom[30];
char lowTom[30];
char textTom[30];
long lastDebounceTime = 0;  
long debounceDelay = 250;   
unsigned long lastAttemptTime;
int temper = 0;
#define I2C_ADDR    0x27 // <<----- Add your address here.  Find it from I2C Scanner
#define BACKLIGHT_PIN     3
#define En_pin  2
#define Rw_pin  1
#define Rs_pin  0
#define D4_pin  4
#define D5_pin  5
#define D6_pin  6
#define D7_pin  7
LiquidCrystal_I2C	lcd(I2C_ADDR,En_pin,Rw_pin,Rs_pin,D4_pin,D5_pin,D6_pin,D7_pin);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0x12, 0x34, 0x5D }; 
char serverName[] = "gc200303856.computerstudi.es"; 
EthernetClient client;

///////////////////////////////////////

void setup()
{   
  analogReference(EXTERNAL);
  
  pinMode(upButton, INPUT);
  pinMode(downButton, INPUT); 
  
  lcd.begin(16, 2);
  lcd.setBacklightPin(BACKLIGHT_PIN,POSITIVE);
  lcd.setBacklight(HIGH);
  lcd.home(); // go home

  if (Ethernet.begin(mac) == 0)
  {
    // no point in carrying on, so do nothing forevermore:
    //while(true);
  }
  
  //get weather first timeF
  sendGET();
}

////////////////////////////////////////

void loop(){
  
  //update weather info from internet
  if (millis() - lastAttemptTime > 1800000)
  {
    sendGET();
    lastAttemptTime = millis();
  }
if(digitalRead(upButton) == HIGH && menu <= 7)
  {
    menu++;
    lastDebounceTime = millis();
  }
  
  //check for down presses
  if(digitalRead(downButton) == HIGH  && menu >= 2)
  {
    menu--;
    lastDebounceTime = millis();
  }    

  //menu selection  
  switch (menu) {
  case 1:
    //Title 
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(title);
    for (int positionCounter = 0; positionCounter < 24; positionCounter++) {
      if(positionCounter == 0)
      {
        delay(1000);
      }
      lcd.scrollDisplayLeft(); 
      delay(500);
    }
    delay(3000);
    break;
  case 2:
    //Latitude and Logitude
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(lat);
    lcd.setCursor(0, 1);
    lcd.print(longt);
    delay(1000);
    break;
  case 3:
    //Today's day and date
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(date);
    lcd.setCursor(0, 1);
    lcd.print(day);
    delay(1000);
    break;
  case 4:
    //Today's Temp
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(high);
    lcd.setCursor(0, 1);
    lcd.print(low);
    delay(1000);
  break;  
  case 5:
    //Forecast
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(text);
    delay(1000);
    break;
   case 6:
    //Tomorrow's day and date
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(dateTom);
    lcd.setCursor(0, 1);
    lcd.print(dayTom);
    delay(1000);
    break;
  case 7:
    //Tomorrow's Temp
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(highTom);
    lcd.setCursor(0, 1);
    lcd.print(lowTom);
    delay(1000);
  break;  
  case 8:
    //Forecast
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(textTom);
    delay(1000);
    break;
  }
} 

//////////////////////////////////////////

void sendGET() //client function to send/receive GET request data.
{
  if (client.connect(serverName, 80))
  { 
    client.println("GET /arduino-weather/weatherstation.php HTTP/1.0");
    client.println("Host: gc200303856.computerstudi.es");
    client.println(); //end of get request
  } 
 
  while(client.connected() && !client.available()) delay(1); //waits for data
  while (client.connected() || client.available()) { //connected or data available  
    char c = client.read(); //gets byte from ethernet buffer
//    Serial.print(c); //prints byte to serial monitor 
    //look for $
    if(c == '$')
    {
      readingdata = true;
    }
    
    //save to output
    if (readingdata == true && c != '$')
    {
        if(c != '|') //if not a comma fill variables
        {
           if(level == 0)
           {
             title[i] = c;
             //temper = temper + 1;
           }
           else if(level == 1)
           {
             lat[i] = c;
           }
           else if(level == 2)
           {
             longt[i] = c;
           }
           else if(level == 3)
           {
             date[i] = c;
           }   
           else if(level == 4)
           {
             day[i] = c;
           }  
           else if(level == 5)
           {
             high[i] = c;
           }
           else if(level == 6)
           {
              low[i] = c;
           }
           else if(level == 7)
           {
             text[i] = c;
           }
           else if(level == 8)
           {
             dateTom[i] = c;
           }   
           else if(level == 9)
           {
             dayTom[i] = c;
           }  
           else if(level == 10)
           {
             highTom[i] = c;
           }
           else if(level == 11)
           {
              lowTom[i] = c;
           }
           else if(level == 12)
           {
             textTom[i] = c;
           }    
         i++; 
       }
       else //skip comma
       {
         level = level + 1;
         i = 0;
       }    
    }
  }

i = 0;
level = 0;
readingdata = false;
client.stop(); //stop client

}

///////////////////////////////////////////

int temperature(){
    uint8_t i;
    float average;
    // take N samples in a row, with a slight delay
    for (i=0; i< numSamples; i++) {
    samples[i] = analogRead(thermistorPin);
    delay(10);
    }
    // average all the samples out
    average = 0;
    for (i=0; i< numSamples; i++) {
    average += samples[i];
    }
    average /= numSamples;
    // convert the value to resistance
    average = 1023 / average - 1;
    average = seriesResistor / average;
    float steinhart;
    steinhart = average / thermistorNominal; // (R/Ro)
    steinhart = log(steinhart); // ln(R/Ro)
    steinhart /= bCoefficient; // 1/B * ln(R/Ro)
    steinhart += 1.0 / (temperatureNominal + 273.15); // + (1/To)
    steinhart = 1.0 / steinhart; // Invert
    steinhart -= 273.15; // convert to C
    steinhart = (steinhart * 1.8) + 32;
    
    return steinhart;
}
