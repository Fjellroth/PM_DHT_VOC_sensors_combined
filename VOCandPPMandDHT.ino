 //Christer- This sketch is a combination of various github resources, 
    //many lines are commented out, this is because we needed the data presented in a simple way for input into a CSV file.
    //Hardware used in this case is an arduino mega as I was unable to get the software serial to function on two different serial inputs with the arduino uno.
    //The sample rate for the PM sensors is as fast as the sketch will run, when adding a 3 second delay for example the serial monitor would display "checksum error" after each data set.
    //This error comes from the last few lines of code but I do not know why.
 
    
#include <MICS-VZ-89TE.h>
#include <DHT.h>
#include <Wire.h>
//DHT Setup
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

int period = 797;// for some reason this delay works perfectly

unsigned long time_now = 0;
//int data [7];//commented out as already defined in PM code
bool MICS_status;
int MICS_voc;
int MICS_eqco2;

    // uncomment this line:
    #define pmsSerialA Serial1
    #define pmsSerialB Serial2
     
    // For UNO and others without hardware serial, we must use software serial...
    // pin #2 is IN from sensor (TX pin on sensor), leave pin #3 disconnected
    // comment these two lines if using hardware serial
 //   #include <SoftwareSerial.h>
 // SoftwareSerial pmsSerial(2, 3);
     
    void setup() {
      // our debugging output
      Serial.begin(9600);
      dht.begin();
      // sensor baud rate is 9600
      pmsSerialA.begin(9600);
      pmsSerialB.begin(9600);
    }
     
    struct pms5003data {
      uint16_t framelen;
      uint16_t pm10_standard, pm25_standard, pm100_standard;
      uint16_t pm10_env, pm25_env, pm100_env;
      uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
      uint16_t unused;
      uint16_t checksum;
    };
     
    struct pms5003data data;

    void loop() {    
if(millis() > time_now + period){ //note millis function will only work for 50 days before crashing. I used this millis function so that all data points would arrive about the same time and write as frequently as each other
  time_now = millis();
//DHT loop
    hum = dht.readHumidity();
    temp= dht.readTemperature();
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity:, ");
    Serial.print(hum);
    Serial.print(", %, Temp:, ");
    Serial.print(temp);
    Serial.print(", Celsius,");
 //VOC sensor loop   
    MICS_VZ_89TE myMICS;
    MICS_status = myMICS.begin();
    myMICS.readSensor();
    MICS_eqco2 = myMICS.getCO2();
    MICS_voc = myMICS.getVOC();
    Serial.print("MICS VOC PPB:, ");
    Serial.print(MICS_voc);
    Serial.print(" ,MICS CO2 PPM:, ");
    Serial.print(MICS_eqco2);
    Serial.println(",");
}

      if (readPMSdata(&pmsSerialA)) {
    //    delay(500); // delay function will not work with this PM sensors
        // reading data was successful!
       // Serial.println();
        //Serial.println("-------------------A--------------------");
        //Serial.println("Concentration Units (standard)");
        Serial.print(",PMA 1.0:, "); Serial.print(data.pm10_standard);
        Serial.print("\t\t,PMA 2.5:, "); Serial.print(data.pm25_standard);
        Serial.print("\t\t,PMA 10:, "); Serial.print(data.pm100_standard);Serial.print(",");
        //Serial.println("------------------A---------------------");
        //Serial.println("Concentration Units (environmental)");
       // Serial.print("PM 1.0: "); Serial.print(data.pm10_env);
       // Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_env);
       // Serial.print("\t\tPM 10: "); Serial.println(data.pm100_env);
       // Serial.println("------------------A---------------------");
       // Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(data.particles_03um);
       //Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(data.particles_05um);
       //Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(data.particles_10um);
       //Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(data.particles_25um);
       //Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(data.particles_50um);
       //Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(data.particles_100um);
       //Serial.println("---------------------------------------");
      }
      
      
        if (readPMSdata(&pmsSerialB)) {
   //     delay(500);
        // reading data was successful!
      //  Serial.println();
        //Serial.println("-------------------B--------------------");
       // Serial.println("Concentration Units (standard)");
        Serial.print(",PMB 1.0:, "); Serial.print(data.pm10_standard);
        Serial.print("\t\t,PMB 2.5:, "); Serial.print(data.pm25_standard);
        Serial.print("\t\t,PMB 10:, "); Serial.print(data.pm100_standard); Serial.print(",");
        //Serial.println("------------------B---------------------");
        //Serial.println("Concentration Units (environmental)");
        //Serial.print("PM 1.0: "); Serial.print(data.pm10_env);
       // Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_env);
       // Serial.print("\t\tPM 10: "); Serial.println(data.pm100_env);
       // Serial.println("------------------B---------------------");
       // Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(data.particles_03um);
       // Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(data.particles_05um);
       // Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(data.particles_10um);
       // Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(data.particles_25um);
       //Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(data.particles_50um);
       // Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(data.particles_100um);
       // Serial.println("---------------------------------------");
      }
    }
     
    boolean readPMSdata(Stream *s) {
      if (! s->available()) {
        return false;
      }
      
      // Read a byte at a time until we get to the special '0x42' start-byte
      if (s->peek() != 0x42) {
        s->read();
        return false;
      }
     
      // Now read all 32 bytes
      if (s->available() < 32) {
        return false;
      }
        
      uint8_t buffer[32];    
      uint16_t sum = 0;
      s->readBytes(buffer, 32);
     
      // get checksum ready
      for (uint8_t i=0; i<30; i++) {
        sum += buffer[i];
      }
     
      /* debuggin
      for (uint8_t i=2; i<32; i++) {
        Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
      }
      Serial.println();*/
      
      
      // The data comes in endian'd, this solves it so it works on all platforms
      uint16_t buffer_u16[15];
      for (uint8_t i=0; i<15; i++) {
        buffer_u16[i] = buffer[2 + i*2 + 1];
        buffer_u16[i] += (buffer[2 + i*2] << 8);
      }
     
      // put it into a nice struct :)
     memcpy((void *)&data, (void *)buffer_u16, 30);
     
      if (sum != data.checksum) {
        Serial.println("Checksum failure");
        return false;
      }
      // success!
      return true;
    
    }
