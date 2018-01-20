#include <NTPClient.h>
// change next line to use with another board/shield
#include <ESP8266WiFi.h>
//#include <WiFi.h> // for WiFi shield
//#include <WiFi101.h> // for WiFi 101 shield or MKR1000
#include <WiFiUdp.h>


extern "C"{
  #include "display.h"
}

const char *ssid     = "";
const char *password = "";

WiFiUDP ntpUDP;

// By default 'time.nist.gov' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP);

// You can specify the time server pool and the offset, (in seconds)
// additionaly you can specify the update interval (in milliseconds).
// NTPClient timeClient(ntpUDP, "europe.pool.ntp.org", 3600, 60000);

void setup() {
  //Serial.begin(115200);
  WiFi.begin(ssid, password);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    //Serial.print ( "." );
  }

  timeClient.begin();
  timeClient.setTimeOffset(-3 * 3600);
  displayinit();
}

int hours = 0, minutes = 0;
void loop() {
  timeClient.update();

  //Serial.println(timeClient.getFormattedTime());
  hours = (byte)timeClient.getHours();
  minutes = (byte)timeClient.getMinutes();
  //Serial.print("horas ");
  //Serial.println(hours);
  //Serial.print("minutos ");
  //Serial.println(minutes);
  displayWrite(hours, minutes);

  delay(1000);
}
