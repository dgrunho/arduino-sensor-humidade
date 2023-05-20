#include "DHT.h" // Adafruit DHT library has to be imported
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#define DHTPIN 4    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11
LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DHT dht(DHTPIN, DHTTYPE);
bool switchState = false;
bool state = true;
bool lastLcdState = false;
float humidityTarget = 85.00;
float humidityCurrent;
int onDurationInSeconds = 1800;
int remainingOnDurationInSeconds = 0;
int delayTime = 1000;
// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(2, OUTPUT);
  pinMode(3, INPUT_PULLUP);
  dht.begin();
  digitalWrite(2, HIGH);
  Wire.begin();
  lcd.init();        
  lcd.backlight();              // initialize the lcd 
}

// the loop function runs over and over again forever
void loop() {
  CheckLCD();
  delay(delayTime);
  
  PrintDataToLCD();
  
  if (remainingOnDurationInSeconds > 0)  {
    remainingOnDurationInSeconds = remainingOnDurationInSeconds - (delayTime/1000);
    if (humidityCurrent <= humidityTarget - 10){
      
    }
    digitalWrite(LED_BUILTIN, HIGH); 
    digitalWrite(2, LOW);
  } else {
    humidityCurrent = ReadHumidity();
      if (isnan(humidityCurrent)) {
        return;
      }

      digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
      digitalWrite(2, HIGH);

      if(humidityCurrent > humidityTarget){
        remainingOnDurationInSeconds = onDurationInSeconds;
      }
  }
}

void PrintDataToLCD(){
  lcd.clear();  
   lcd.setBacklight(5);
  if (remainingOnDurationInSeconds > 0)  {
    humidityCurrent = ReadHumidity();
    int tme = remainingOnDurationInSeconds;                                                           //Time we are converting. This can be passed from another function.
    //int hr = tme/3600;                                                        //Number of seconds in an hour
    int mins = (tme)/60;                                              //Remove the number of hours and calculate the minutes.
    int sec = tme-mins*60;                                            //Remove the number of hours and minutes, leaving only seconds.
    String hrMinSec = (String(mins) + ":" + String(sec));  //Converts to HH:MM:SS string. This can be returned to the calling function.
    lcd.setCursor(0,0);
    lcd.print("Remaining: ");
    lcd.setCursor(11,0);
    lcd.print(hrMinSec);
    lcd.setCursor(0,1);
    lcd.print("Humidity: ");
    lcd.setCursor(10,1);
if(isnan(humidityCurrent)){
  
}
    lcd.print(humidityCurrent);
   
  }
  else {
    lcd.setCursor(0,0);
    lcd.print("Humidity: ");
    lcd.setCursor(10,0);
    lcd.print(humidityCurrent);
    lcd.setCursor(0,1);
    if (switchState == true){
      lcd.print("Fan On");
    }else{
      lcd.print("Fan Off");
    }
    lcd.setCursor(8,1);
    lcd.print("T:");
    lcd.setCursor(10,1);
    lcd.print(humidityTarget);
  }
  
  
}

float ReadHumidity(){
// Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Failed to read");
    lcd.setCursor(0,1);
    lcd.print("from DHT sensor!");
    return h;
  }

   // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);


  return h;
}

void CheckLCD(){
  byte err, adr;       /*variable error is defined with address of I2C*/
  bool hasLcdConnected = false;
  int number_of_devices;
  number_of_devices = 0;
  for (adr = 1; adr < 127; adr++ )
  {
    Wire.beginTransmission(adr);
    err = Wire.endTransmission();

    if (err == 0)
    {
      number_of_devices++;
    }
  }
  if (number_of_devices == 0)
    hasLcdConnected = false;
  else
    hasLcdConnected = true;
   Serial.println(number_of_devices);
  if(hasLcdConnected != lastLcdState)
  {
    lastLcdState = hasLcdConnected;
    if  (hasLcdConnected == true)
    { 
      Serial.println("Initializing LCD");
      lcd.init();        
      lcd.backlight();
    }
  }
}
