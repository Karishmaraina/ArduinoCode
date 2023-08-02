#define MAX 1000000
#include <TinyGPSPlus.h>


// The TinyGPSPlus object


TinyGPSPlus gps;

#include <Arduino.h>
#include "HX711.h"
#include "soc/rtc.h"

// HX711 circuit wiring
const int LOADCELL_DOUT_PIN = 15;
const int LOADCELL_SCK_PIN = 4;

HX711 scale;


#include <stdlib.h>
#include <string.h>
//Include libraries
#include <HTTPClient.h>               //Download: https://electronoobs.com/eng_arduino_httpclient.php
#include <WiFi.h>                     //Download: https://electronoobs.com/eng_arduino_wifi.php


//Add WIFI data
//const char* ssid = "HONS3G";              //Add your WIFI network name 
//const char* password =  "HONStech22";           //Add WIFI password

//const char* ssid = "System Out";              //Add your WIFI network name 
//const char* password =  "Vishal@1";           //Add WIFI password

const char* ssid = "TruckTest";              //Add your WIFI network name 
const char* password =  "ybbn7501";           //Add WIFI password


//Variables used in the code
String LED_id = "1";                  //Just in case you control more than 1 LED
//bool toggle_pressed = false;          //Each time we press the push button    
String data_to_send = "";             //Text data to send to the server
unsigned int Actual_Millis, Previous_Millis;
int refresh_time = 2000;               //Refresh rate of connection to website (recommended more than 1s)

char Truck_number2[] = "JK02CS1234";
char Truck_number1[] = "HR09CG3792";
char Truck_number3[] = "UP35CG66";
char Truck_number[] = "HR09CG3792";

int S_no = 0 , Truck_weight = 200 , Truck_value = 300;
//Inputs/outputs
//int button1 = 13;                     //Connect push button on this pin
int LED = 2;                          //Connect LED on this pin (add 150ohm resistor)
int LED_stat = 0 ;

char msg_buf_1;

// defines pins numbers
const int trigPin = 12;
const int echoPin = 13;
// defines variables
long duration;
int distance;

// Potentiometer is connected to GPIO 34 (Analog ADC1_CH6) 
const int IR = 34;

// variable for storing the potentiometer value
int IR_level = 0;

int Capacity = 500  , Heap_Status;
//char Load_status[5] = "NLOAD";
//char Heap_status[5] = "NHEEP" ;

char Load_status[] = "NOLOAD"; 
char Long[] = "74.8914261"; 
char Latt[] = "32.688177"; 
char Long1[] = "74.8914261"; 
char Latt1[] = "32.688177"; 

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);



struct Button {
    const uint8_t PIN;
    uint32_t numberKeyPresses;
    bool pressed;
};

Button button1 = {18, 0, false};
Button button2 = {19, 0, false};

//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 
int Dumper_Capacity = 0 ;



void IRAM_ATTR Key1() {
    button_time = millis();
if (button_time - last_button_time > 250)
{
        button1.numberKeyPresses++;
        button1.pressed = true;
       last_button_time = button_time;
       strcpy(Truck_number,Truck_number1);
}
}
void IRAM_ATTR Key2() {
    button_time = millis();
if (button_time - last_button_time > 250)
{
        button2.numberKeyPresses++;
        button2.pressed = true;
       last_button_time = button_time;
       strcpy(Truck_number,Truck_number2);
}
}

void loadcell_init()
{
  rtc_clk_cpu_freq_set(RTC_CPU_FREQ_80M);
  Serial.println("HX711 Demo");

  Serial.println("Initializing the scale");

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  //scale.set_scale(INSERT YOUR CALIBRATION FACTOR);
  scale.set_scale(444.5);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  //scale.set_scale(443.0); 
  scale.tare();               // reset the scale to 0
}
int weight = 0 ;
void loadcell_value()
{
  //Serial.print("one reading:\t");
  //weight = (round)(scale.get_units(), 1);
  weight = round(scale.get_units());
  //Serial.print(weight);
  /*Serial.print("one reading:\t");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| average:\t");
  Serial.println(scale.get_units(10), 5);*/

  scale.power_down();             // put the ADC in sleep mode
  delay(20);
  scale.power_up();
  if(weight < 10) { weight = 0 ;}
  else if(weight >=90 && weight <=180) { Serial.print("Wright: "); Serial.println(weight); weight = 150 ;}
  else if(weight >=250 && weight <=350) { Serial.print("Wright: "); Serial.println(weight); weight = 300 ;}
  else if(weight >= 390 && weight <=480) { Serial.print("Wright: "); Serial.println(weight); weight = 450 ;}
  else if(weight >= 550 && weight <=650) { Serial.print("Wright: "); Serial.println(weight); weight = 600 ;}
  else if(weight > 700 && weight <=790) { Serial.print("Wright: "); Serial.println(weight); weight = 750 ;}
  else if(weight > 850 && weight <=960) { Serial.print("Wright: "); Serial.println(weight); weight = 900 ;}
}

void US_value()
{
  // Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);
}

void IR_value()
{
    IR_level = analogRead(IR);
    Serial.print("\nIR_level =");Serial.print(IR_level);Serial.print("\n");
}

void LCD_init()
{

  // initialize the LCD
  lcd.begin();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  //lcd.print("HONS....");
  lcd.setCursor(0,0);
  lcd.print("   Project by    ");
  lcd.setCursor(0,1);
  lcd.print("  HEXQUAD GCET   ");
  delay(3000);
  lcd.setCursor(0,0);
  lcd.print("CP=      WT=     ");
  lcd.setCursor(0,1);
  lcd.print("LS=      HS=     ");
  //delay(500);
}

void update_LCD()
{
    //lcd.setCursor(4,0);
    //lcd.print(Capacity);

    lcd.setCursor(12,0);
    lcd.print(weight);

    if(weight < 50)
      {  lcd.setCursor(3,1);  lcd.print("NLOAD");  strcpy(Load_status,"NOLOAD");}
    else if((weight > 50) && (weight < Dumper_Capacity)) 
      {  lcd.setCursor(3,1);  lcd.print("NORML");  strcpy(Load_status,"NORMAL");}  
    else if(weight > Dumper_Capacity)
      {  lcd.setCursor(3,1);  lcd.print("OLOAD");  strcpy(Load_status,"OVERLOADED");}  

    if ((distance > 6) && (IR_level > 200))
      {  lcd.setCursor(12,1);  lcd.print("B-US");  Heap_Status = 0; }
    else if((distance < 6) && (IR_level > 200))
      {  lcd.setCursor(12,1);  lcd.print("NHEAP");  Heap_Status = 0;}  
    else if((distance < 6) && (IR_level < 200))
      {  lcd.setCursor(12,1);  lcd.print("HEAP");  Heap_Status = 1;}    
   
    
}

void setup() {
  delay(10);
  Serial.begin(9600);                   //Start monitor
  Serial2.begin(9600);
  /*pinMode(LED, OUTPUT);                   //Set pin 2 as OUTPUT
  //pinMode(button1, INPUT_PULLDOWN);       //Set pin 13 as INPUT with pulldown
  //attachInterrupt(button1, isr, RISING);  //Create interruption on pin 13
*/
  WiFi.begin(ssid, password);             //Start wifi connection
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    Serial.print(".");
  }

  Serial.print("Connected, my IP: ");
  Serial.println(WiFi.localIP());
  Actual_Millis = millis();               //Save time for refresh loop
  Previous_Millis = Actual_Millis; 
  loadcell_init();
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an 
  LCD_init();
  Serial.print("\nStarted....");
  delay(2000);
  pinMode(button1.PIN, INPUT_PULLUP);
    attachInterrupt(button1.PIN, Key1, FALLING);
    pinMode(button2.PIN, INPUT_PULLUP);
    attachInterrupt(button2.PIN, Key2, FALLING);
}

int t_count = 0 , response_code = 0; 
void loop() {  
  //delay(1000);
  Serial.print("\n------------------------------------------\n");
  //weight = 0 ;
  loadcell_value();
  Serial.print("----Result-----");
  Serial.print("\nweight = ");Serial.print(weight);Serial.print("\n");
  US_value();
  IR_value();
  update_LCD();
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
       displayInfo();

  //We make the refresh loop using millis() so we don't have to sue delay();
  
  Actual_Millis = millis();
  if(Actual_Millis - Previous_Millis > refresh_time){
    Previous_Millis = Actual_Millis;  
    if(WiFi.status()== WL_CONNECTED){                   //Check WiFi connection status  
      HTTPClient http;                                  //Create new client
      Serial.print("\nt_count ="); Serial.print(t_count);
      if(t_count == 0)
      {
          //t_count = 0 ; 
          S_no++;
          //if(toggle_pressed){                               //If button was pressed we send text: "toggle_LED"
            //data_to_send = "toggle_LED=" + LED_id;  
            //data_to_send = "qst=" + ((String)S_no) + "&qst1=" + (Truck_number) +  "&qst2=" + ((String)(Truck_weight)) + +  "&qst3=" + ((String)(Truck_value));  
            data_to_send = "GET_weight=" + ((String)(Truck_number));
            //toggle_pressed = false;                         //Also equal this variable back to false 
          //}
          /*else{
            data_to_send = "check_LED_status=" + LED_id;    //If button wasn't pressed we send text: "check_LED_status"
          }*/
          
          //Begin new connection to website       
          http.begin("http://127.0.0.1/test/esp32_update8.php");   //Indicate the destination webpage 
          http.addHeader("Content-Type", "application/x-www-form-urlencoded");         //Prepare the header
      }    
          
      else if ( t_count == 1)
      {
        //t_count = 0; 
        Serial.print("\nSend data function...");
        //toggle_pressed = true;
        //if(LED_stat==0) {toggle_pressed = true;} 
        //else if(LED_stat==1) {toggle_pressed = false;} 
        //sprintf(msg_buf_1, "%d", t_count);
        //itoa(t_count,msg_buf_1,10);
        //data_to_send = "check_LED_status=" + ((String)t_count)  ;
        //data_to_send = "qst=" + ((String)S_no) + "&qst1=" + (Truck_number) +  "&qst2=" + ((String)(Truck_weight)) + +  "&qst3=" + ((String)(Truck_value));  
        data_to_send = "qst10=" + ((String)Truck_number) + "&qst11=" + ((String)Long) +  "&qst12=" + ((String)Latt) +   "&qst13=" + ((String)(weight)) +    "&qst14=" + ((String)(Load_status))  +   "&qst17=" + ((String)(Dumper_Capacity)) +   "&qst15=" + ((String)(Heap_Status)) ;  
        
        http.begin("http://127.0.0.1/test/esp32_update8.php");   //Indicate the destination webpage 
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");         //Prepare the header
      
      }

      else if ( t_count == 2)
      {
        //t_count = 0; 
        Serial.print("\nSend data function...");
        //toggle_pressed = true;
        //if(LED_stat==0) {toggle_pressed = true;} 
        //else if(LED_stat==1) {toggle_pressed = false;} 
        //sprintf(msg_buf_1, "%d", t_count);
        //itoa(t_count,msg_buf_1,10);
        //data_to_send = "check_LED_status=" + ((String)t_count)  ;
        //data_to_send = "qst=" + ((String)S_no) + "&qst1=" + (Truck_number) +  "&qst2=" + ((String)(Truck_weight)) + +  "&qst3=" + ((String)(Truck_value));  
        data_to_send = "qst10=" + ((String)Truck_number) + "&qst11=" + ((String)Long) +  "&qst12=" + ((String)Latt) +   "&qst13=" + ((String)(weight)) +    "&qst14=" + ((String)(Load_status)) +   "&qst17=" + ((String)(Dumper_Capacity)) +   "&qst16=" + ((String)(Heap_Status)) ;  
        
        http.begin("http://127.0.0.1/test/esp32_update8.php");   //Indicate the destination webpage 
        http.addHeader("Content-Type", "application/x-www-form-urlencoded");         //Prepare the header
      
      }

      response_code = http.POST(data_to_send);                                //Send the POST. This will giveg us a response code
      
      //If the code is higher than 0, it means we received a response
      if(response_code > 0){
        Serial.println("HTTP code " + String(response_code));                     //Print return code
  
        if(response_code == 200){                                                 //If code is 200, we received a good response and we can read the echo data
          String response_body = http.getString();  
          
          
          char arr[response_body.length() + 1]; 
          strcpy(arr, response_body.c_str()); 
          Serial.print("Server reply: ");                                         //Print data to the monitor for debug
          Serial.println(response_body);
          char str1[10] = "0" ; 
          int val = strcmp(arr , str1);
          int x = atoi(arr);
          
          //int val1 = strcmp(response_body , "200");
          Serial.print("\nval = ");Serial.print(val);
          //Serial.print("\nval 1= ");Serial.print(val1);
          if( (t_count == 0 ) && (val != 0 ))
             {  t_count = 1 ; Serial.print("Send data selected..."); Dumper_Capacity = x; lcd.setCursor(4,0);lcd.print(Dumper_Capacity); Serial.print("\nDumper_Capacity="); Serial.print(Dumper_Capacity);}
          else if(t_count == 1 )
             {  t_count = 2 ; Serial.print("Update data in Dumpers Records");} 
          else if(t_count == 2 )
             {  t_count = 0 ; Serial.print("Insert in Records");}    
          //If the received data is LED_is_off, we set LOW the LED pin
          if(response_body == "LED_is_off"){
            digitalWrite(LED, LOW);
          }
          //If the received data is LED_is_on, we set HIGH the LED pin
          else if(response_body == "LED_is_on"){
            digitalWrite(LED, HIGH);
          }  
        }//End of response_code = 200
      }//END of response_code > 0
      
      else{
       Serial.print("Error sending POST, code: ");
       Serial.println(response_code);
      }
      http.end();                                                                 //End the connection
    }//END of WIFI connected
    else{
      Serial.println("WIFI connection error");
    }
  }
  if (button1.pressed) {
        Serial.printf("Button has been A pressed %u times\n", button1.numberKeyPresses);
        button1.pressed = false;
    }
    else if (button2.pressed) {
        Serial.printf("Button has been B pressed %u times\n", button2.numberKeyPresses);
        button2.pressed = false;
    }
}

void displayInfo()


{


  Serial.print(F("Location: "));


  if (gps.location.isValid()){


    //Serial.print("Lat: ");


    //Serial.print(gps.location.lat(), 6);
    //strcpy(Latt,(gps.location.lat(), 6)); 
    //sprintf(Latt, "%d.%02u", (gps.location.lat(), 6));
    //float tempnumber = (gps.location.lat(), 6) + 0.0000005;  // round it up.
    //Serial.print("\nLong AAAA=");Serial.print(tempnumber);
    //sprintf (Latt,"%d.%02u", (int) tempnumber, (int) ((tempnumber - (int) tempnumber ) * 1000000) );
    //gcvt((gps.location.lat(), 6), 6, Latt);
    //ftoa((gps.location.lat(),6), Latt, 2);
    sprintf(Latt, "%d",(int)(gps.location.lat()) );
    sprintf(Latt1, "%g",(gps.location.lat(),6));
    //Serial.print(F(","));
    //char *result = Latt1+1;
    
    memmove(Latt1, Latt1+1, strlen(Latt1));
    strcat(Latt, Latt1);

    //Serial.print("Lng: ");


    //Serial.print(gps.location.lng(), 6);
    //strcpy(Long,((String)(gps.location.lat(), 6))); 
    //sprintf(Long, "%d.%02u", (gps.location.lng(), 6));
     //tempnumber = (gps.location.lng(), 6) + 0.0000005;  // round it up.
    //sprintf (Long,"%d.%02u", (int) tempnumber, (int) ((tempnumber - (int) tempnumber ) * 1000000) );
    //gcvt((gps.location.lng(), 6), 6, Long);
    //ftoa((gps.location.lng(),6), Long, 2);
    sprintf(Long, "%d",(int)(gps.location.lng()) );
    sprintf(Long1, "%g",(gps.location.lng(),6));
    //Serial.print(F(","));
    //Serial.print("\nLong CCCC=");Serial.print(Long1);
    memmove(Long1, Long1+1, strlen(Long1));
    //Serial.print("\nLong BBBB=");Serial.print(Long);
    //Serial.print("\nLong BBBB=");Serial.print(Long1);
    strcat(Long, Long1);

    Serial.print("\nLong AAAA=");Serial.print(Long);
    Serial.print("\nLatt AAAA=");Serial.print(Latt);

    Serial.println();


  }  


  else


  {


    Serial.print(F("INVALID"));


  }


}


void updateSerial()


{


  delay(500);


  while (Serial.available())


  {


    Serial2.write(Serial.read());//Forward what Serial received to Software Serial Port


  }


  while (Serial2.available())


  {


    Serial.write(Serial2.read());//Forward what Software Serial received to Serial Port


  }


}

void reverse(char* str, int len)
{
    int i = 0, j = len - 1, temp;
    while (i < j) {
        temp = str[i];
        str[i] = str[j];
        str[j] = temp;
        i++;
        j--;
    }
}
int intToStr(int x, char str[], int d)
{
    int i = 0;
    while (x) {
        str[i++] = (x % 10) + '0';
        x = x / 10;
    }
 
    // If number of digits required is more, then
    // add 0s at the beginning
    while (i < d)
        str[i++] = '0';
 
    reverse(str, i);
    str[i] = '\0';
    return i;
}
 
void ftoa(float n, char* res, int afterpoint)
{
    // Extract integer part
    int ipart = (int)n;
 
    // Extract floating part
    float fpart = n - (float)ipart;
 
    // convert integer part to string
    int i = intToStr(ipart, res, 0);
 
    // check for display option after point
    if (afterpoint != 0) {
        res[i] = '.'; // add dot
 
        // Get the value of fraction part upto given no.
        // of points after dot. The third parameter
        // is needed to handle cases like 233.007
        fpart = fpart * pow(10, afterpoint);
 
        intToStr((int)fpart, res + i + 1, afterpoint);
    }
}
