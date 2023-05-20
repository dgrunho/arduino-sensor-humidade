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

  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  float hc = 78.00;

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    lcd.setCursor(0,0);
  lcd.print("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
  
  if(h > hc){
    switchState = true;
  }else{
    switchState = false;
  }
 
  lcd.setCursor(0,0);
  lcd.print("Humidity: ");
  lcd.setCursor(10,0);
  lcd.print(h);
  lcd.setCursor(0,1);
  lcd.print("Fan On: ");
  lcd.setCursor(10,1);
  lcd.print(switchState);
  Serial.println(switchState);
  //switchState = digitalRead(3);
  if(switchState == true){
     digitalWrite(LED_BUILTIN, HIGH);  // turn the LED off by making the voltage LOW
      digitalWrite(2, LOW);
  }
  else
  {
      digitalWrite(LED_BUILTIN, LOW);  // turn the LED on (HIGH is the voltage level)
      digitalWrite(2, HIGH);
  }
                     // wait for a second

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
      lcd.backlight();              // initialize the lcd 
    }
  }
}
