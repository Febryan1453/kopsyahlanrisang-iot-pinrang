#include <Arduino.h>

#include <LiquidCrystal_I2C.h>

#include <Wire.h>

//Sensor DO
#define DO_PIN A0
 
#define VREF 5000    //VREF (mv)
#define ADC_RES 1024 //ADC Resolution
 
//Single-point calibration Mode=0
//Two-point calibration Mode=1
#define TWO_POINT_CALIBRATION 1
 
#define READ_TEMP (25) //Current water temperature ℃, Or temperature sensor function
 
//Single point calibration needs to be filled CAL1_V and CAL1_T
#define CAL1_V (43) //mv
#define CAL1_T (25)   //℃

//Two-point calibration needs to be filled CAL2_V and CAL2_T
//CAL1 High temperature point, CAL2 Low temperature point

#define CAL2_V (14) //mv
#define CAL2_T (15)   //℃
 
const uint16_t DO_Table[41] = {
    14460, 14220, 13820, 13440, 13090, 12740, 12420, 12110, 11810, 11530,
    11260, 11010, 10770, 10530, 10300, 10080, 9860, 9660, 9460, 9270,
    9080, 8900, 8730, 8570, 8410, 8250, 8110, 7960, 7820, 7690,
    7560, 7430, 7300, 7180, 7070, 6950, 6840, 6730, 6630, 6530, 6410};
 
uint8_t Temperaturet;
uint16_t ADC_Raw;
uint16_t ADC_Voltage;
float DO;
 
int16_t readDO(uint32_t voltage_mv, uint8_t temperature_c)
{
#if TWO_POINT_CALIBRATION == 1
  uint16_t V_saturation = (uint32_t)CAL1_V + (uint32_t)35 * temperature_c - (uint32_t)CAL1_T * 35;
  return (voltage_mv * DO_Table[temperature_c] / V_saturation);
#else
  uint16_t V_saturation = (int16_t)((int8_t)temperature_c - CAL2_T) * ((uint16_t)CAL1_V - CAL2_V) / ((uint8_t)CAL1_T - CAL2_T) + CAL2_V;
  return (voltage_mv * DO_Table[temperature_c] / V_saturation);
#endif
}


//Firebase
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>


//Provide the token generation process info.
#include "addons/TokenHelper.h"
//Provide the RTDB payload printing info and other helper functions.
#include "addons/RTDBHelper.h"

// Insert your network credentials
//#define WIFI_SSID "PDTI"
//#define WIFI_PASSWORD "pdti1234"

#define WIFI_SSID "GATEWAY-LANRISANG"
#define WIFI_PASSWORD "MF2022pinrang"

// #define WIFI_SSID "realme 6"
// #define WIFI_PASSWORD "abcd12345"

// Insert Firebase project API Key
//#define API_KEY "3kP2SUYg14SufFclb7lYPdX8TycG19xz4ppBPcm0"
#define API_KEY "AIzaSyD26QdvEJBFCO2-CG3PKtvdRCbMKD3nkN0"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://coba-esp32-39fd9-default-rtdb.asia-southeast1.firebasedatabase.app/" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

#define trigPin 13  //Pin D7 ESP8266 Wemos D1 R2 WiFi
#define echoPin 12  //Pin D6 ESP8266  Wemos D1 R2 WiFi
#define relay1 16 //Pin D2 ESP8266  Wemos D1 R2 WiFi
#define relay2 1 //Pin D1 ESP8266  Wemos D1 R2 WiFi
//const int relay2 = D10;

LiquidCrystal_I2C lcd(0x27 ,16,2);

int jarak = 135;
int tinggi;
float suhua;
float suhub;
float ph;


void setup() {
  //Serial.begin(9600);
  Serial.begin(115200);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  /* Assign the api key (required) */
  config.api_key = API_KEY;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;

  /* Sign up */
  if (Firebase.signUp(&config, &auth, "", "")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  /* Assign the callback function for the long running token generation task */
  config.token_status_callback = tokenStatusCallback; //see addons/TokenHelper.h
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode (relay1, OUTPUT);
  pinMode (relay2, OUTPUT);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  lcd.begin();                      // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
    lcd.print("ALAT PENGUKUR");
    delay (1000);
    lcd.setCursor(0,1);
    lcd.print("KUALITAS AIR");
    delay(1000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TAMBAK UDANG");
    delay(1000);
    lcd.setCursor(0,1);
    lcd.print("WINDU LANRISANG");
    delay(1000);
    lcd.clear();
    //lcd.setCursor(0,0);
    //lcd.print("TINGGI AIR");
    //lcd.print("=");
}

void loop() {

  //Sensor DO
  Temperaturet = (uint8_t)READ_TEMP;
  ADC_Raw = analogRead(DO_PIN);
  ADC_Voltage = uint32_t(VREF) * ADC_Raw / ADC_RES;
  DO = ((readDO(ADC_Voltage, Temperaturet))/1000);

  //Serial.print("Temperaturet:\t" + String(Temperaturet) + "\t");
  //Serial.print("ADC RAW:\t" + String(ADC_Raw) + "\t");
  //Serial.print("ADC Voltage:\t" + String(ADC_Voltage) + "\t");
  //Serial.println("DO:\t" + String(readDO(ADC_Voltage, Temperaturet)) + "\t");
  //Serial.println("DO:\t" + String(DO) + "\t");


  //Ketinggian Air
  long duration, gape;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, HIGH);
  duration = pulseIn(echoPin, HIGH);
  gape = (duration/2) / 29.1;
  tinggi = jarak - gape;

  //Firebase
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 1000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    //Pembacaan dat Tinggi Air
    //Firebase.RTDB.setInt(&fbdo, "TinggiAir", tinggi)
    if (Firebase.RTDB.setInt(&fbdo, "TinggiAir", tinggi)){
      //Serial.println("PASSED");
      //Serial.println("PATH: " + fbdo.dataPath());
      //Serial.println("TYPE: " + fbdo.dataType());
      // lcd.setCursor(0,0);
      // lcd.print("PASSED");
      // lcd.setCursor(0,1);
      // lcd.print("TYPE: " + fbdo.dataType());
      // delay (1000);
      //Serial.print("Tinggi Air =");
      //Serial.print(tinggi);
      //Serial.println(" cm.........");
      //Logika untuk pengukuran ketinggian Air
      if (tinggi < 50 )
      {
        digitalWrite(relay1, HIGH);
        //lcd.setCursor(11,0);
        lcd.setCursor(0,0);
        lcd.print("TINGGI AIR");
        lcd.print("=");
        lcd.print(tinggi);
        lcd.print("cm...");
        lcd.setCursor(0,1);
        lcd.print("MESIN AIR ON ....");
        delay (1000);
        //lcd.clear();
      }
    else if (tinggi >= 70 && tinggi <= 80)
    {
        digitalWrite(relay1, LOW);
        //lcd.setCursor(11,0);
        lcd.setCursor(0,0);
        lcd.print("TINGGI AIR");
        lcd.print("=");
        lcd.print(tinggi);
        lcd.print("cm...");
        lcd.setCursor(0,1);
        lcd.print("MESIN AIR OFF ...");
        delay (1000);
        //lcd.clear();
        
      }
        else if (tinggi >= 50 && tinggi < 70)
      {
        //lcd.setCursor(11,0);
        lcd.setCursor(0,0);
        lcd.print("TINGGI AIR");
        lcd.print("=");
        lcd.print(tinggi);
        lcd.print("cm ...");
        lcd.setCursor(0,1);
        lcd.print("TINGGI AIR AMAN..");
        delay (1000);
        //lcd.clear();
      }
      
    }
    else {
      lcd.setCursor(0,0);
      lcd.print("FAILED");
      lcd.setCursor(0,1);
      lcd.print("REASON: " + fbdo.errorReason());
      delay (1000);
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    //Pembacaan Data Kadar Oksigen Terlarut
    //Firebase.RTDB.setFloat(&fbdo, "DO", DO)
    if (Firebase.RTDB.setInt(&fbdo, "DO", DO)){
      //Serial.println("PASSED");
      //Serial.println("PATH: " + fbdo.dataPath());
      //Serial.println("TYPE: " + fbdo.dataType());
      // lcd.setCursor(0,0);
      // lcd.print("PASSED");
      // lcd.setCursor(0,1);
      // lcd.print("TYPE: " + fbdo.dataType());
      // delay (1000);
      //Serial.print("DO =");
      //Serial.print(DO);
      //Serial.println(" ...........");
      //Logika untuk pengukuran kadar oksigen terlarut
      if (DO < 3 )
      {
        digitalWrite(relay2, HIGH);
        //digitalWrite(relay2, LOW);
        //Serial.println("Oksigen Kurang");
        lcd.setCursor(0,0);
        lcd.print("DO = ");
        lcd.print(DO);
        lcd.print("ppm.......");
        lcd.setCursor(0,1);
        lcd.print("OKSIGEN KURANG..");
        delay (1000);
        lcd.setCursor(0,1);
        lcd.print("MESIN AIR ON ....");
        delay (1000);
      }
      else if (DO >= 15)
      {
        digitalWrite(relay2, LOW);
        //digitalWrite(relay2, HIGH);
        //Serial.println("Oksigen Berlebih");
        lcd.setCursor(0,0);
        lcd.print("DO = ");
        lcd.print(DO);
        lcd.print("ppm.......");
        lcd.setCursor(0,1);
        lcd.print("OKSIGEN BERLEBIH");
        delay (1000);
        lcd.setCursor(0,1);
        lcd.print("MESIN AIR OFF ...");
        delay (1000);
      }
      else if (DO >= 3 && DO < 15)
      {
        //Serial.println("Oksigen Aman");
        lcd.setCursor(0,0);
        lcd.print("DO = ");
        lcd.print(DO);
        lcd.print("ppm.......");
        lcd.setCursor(0,1);
        lcd.print("OKSIGEN AIR AMAN");
        delay (1000);
      }
    }
    else {
      lcd.setCursor(0,0);
      lcd.print("FAILED");
      lcd.setCursor(0,1);
      lcd.print("REASON: " + fbdo.errorReason());
      delay (1000);
      Serial.println("FAILED");
      Serial.println("REASON: " + fbdo.errorReason());
    }

    //Pembacaan Data Suhu Air Atas
    if (Firebase.RTDB.getFloat(&fbdo, "/SuhuA")){
      //Serial.println("PASSED");
      //Serial.println("PATH: " + fbdo.dataPath());
      //Serial.println("TYPE: " + fbdo.dataType());
      //int suhua = Firebase.RTDB.getFloat(&fbdo, "SuhuA");
      //if (fbdo.dataType() == "SuhuA") {
        suhua = fbdo.floatData();
        //Serial.print("Suhu Atas =");
        //Serial.print(suhua);
        //Serial.println(" celcius...........");
        lcd.setCursor(0,0);
        lcd.print("Suhu Air Atas...");
        lcd.setCursor(0,1);
        lcd.print(suhua);
        lcd.print(" celcius...........");
        delay (1000);
      //}
    }
    else {
      lcd.setCursor(0,0);
      lcd.print("FAILED");
      lcd.setCursor(0,1);
      lcd.print("REASON: " + fbdo.errorReason());
      delay (1000);
      //Serial.println("FAILED");
      //Serial.println("REASON: " + fbdo.errorReason());
    }

    //Pembacaan Data Suhu Air Bawah
    if (Firebase.RTDB.getFloat(&fbdo, "/SuhuB")){
      //Serial.println("PASSED");
      //Serial.println("PATH: " + fbdo.dataPath());
      //Serial.println("TYPE: " + fbdo.dataType());
      //int suhub = Firebase.RTDB.getFloat(&fbdo, "SuhuB");
      //if (fbdo.dataType() == "SuhuB") {
        suhub = fbdo.floatData();
        //Serial.print("Suhu Bawah =");
        //Serial.print(suhub);
        //Serial.println(" celcius...........");
        lcd.setCursor(0,0);
        lcd.print("Suhu Air Bawah...");
        lcd.setCursor(0,1);
        lcd.print(suhub);
        lcd.print(" celcius...........");
        delay (1000);
      //}
    }
    else {
      lcd.setCursor(0,0);
      lcd.print("FAILED");
      lcd.setCursor(0,1);
      lcd.print("REASON: " + fbdo.errorReason());
      delay (1000);
      //Serial.println("FAILED");
      //Serial.println("REASON: " + fbdo.errorReason());
    }

    //Pembacaan data pH Air
    if (Firebase.RTDB.getFloat(&fbdo, "/pH")){
      //Serial.println("PASSED");
      //Serial.println("PATH: " + fbdo.dataPath());
      //Serial.println("TYPE: " + fbdo.dataType());
      //int ph = Firebase.RTDB.getFloat(&fbdo, "pH");
      //if (fbdo.dataType() == "pH") {
        ph = fbdo.floatData();
        //Serial.print("pH Air =");
        //Serial.print(ph);
        //Serial.println(" ...............");
        lcd.setCursor(0,0);
        lcd.print("Kadar pH Air...");
        lcd.setCursor(0,1);
        lcd.print(ph);
        lcd.print(" .................");
        delay (1000);
      //}
      
    }
    else {
      lcd.setCursor(0,0);
      lcd.print("FAILED");
      lcd.setCursor(0,1);
      lcd.print("REASON: " + fbdo.errorReason());
      delay (1000);
      //Serial.println("FAILED");
      //Serial.println("REASON: " + fbdo.errorReason());
    }


  }

  

  

   
}
