#include <Adafruit_AHTX0.h>


// Adafruit IO Publish Example
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"

/************************ Example Starts Here *******************************/



// set up the AdaIO feed
AdafruitIO_Feed *temperatureFeed = io.feed("temperature");
AdafruitIO_Feed *humidityFeed = io.feed("humidity");
AdafruitIO_Feed *BatCurrentFeed = io.feed("Bat_Current");
AdafruitIO_Feed *BatPercentFeed = io.feed("Bat_Percent");

Adafruit_AHTX0 aht;

void setup() {
  // start the serial connection
  Serial.begin(115200);

  // wait for serial monitor to open
  while(! Serial);

  Serial.print("Connecting to Adafruit IO");

  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

  if (! aht.begin()) {
    Serial.println("Could not find AHT? Check wiring");
  while (1) delay(10);
  }
  Serial.println("AHT10 or AHT20 found");



  
  while (!Serial)
  delay(10);     // will pause Zero, Leonardo, etc until serial console opens

  

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  io.run();
  Serial.println("sending");
    
  sensors_event_t humidity, temp;

  float middle_temp = 0;
  float middle_humidity = 0;
  float max_temp;
  float min_temp;
  float max_humidity;
  float min_humidity;
  float devide_temp = 0;
  float devide_humidity = 0;
  int count = 0;
  for(int i = 0; i < 10; i++)
  {

    count++;
    aht.getEvent(&humidity, &temp);// populate temp and humidity objects with fresh data

    if(i==0)
    {
      max_temp = temp.temperature;
      min_temp = temp.temperature-0.001;
      max_humidity = humidity.relative_humidity;
      min_humidity = humidity.relative_humidity-0.001;
    }

    Serial.print("Temperature "); Serial.print(count); Serial.print(": "); Serial.print(temp.temperature); Serial.println(" degrees C");
    Serial.print("Humidity "); Serial.print(count); Serial.print(": ");  Serial.print(humidity.relative_humidity); Serial.println("% rH"); Serial.println("");

    
    Serial.print("Devide_temp "); Serial.print(count); Serial.print(": "); Serial.print(devide_temp); Serial.println("Count");
    Serial.print("Devide_humidity "); Serial.print(count); Serial.print(": "); Serial.print(devide_humidity); Serial.println("Count"); Serial.println("");

    Serial.print("Max_Temp "); Serial.print(count); Serial.print(": ") ; Serial.print(max_temp); Serial.println(" degrees C");
    Serial.print("Min_Temp "); Serial.print(count); Serial.print(": ") ; Serial.print(min_temp); Serial.println(" degrees C"); Serial.println("");

    Serial.print("Max_Humidity "); Serial.print(count); Serial.print(": "); Serial.print(max_humidity); Serial.println(" % rH");
    Serial.print("Min_Humidity "); Serial.print(count); Serial.print(": "); Serial.print(min_humidity); Serial.println(" % rH"); Serial.println("");

    Serial.print("Middle_Temp "); Serial.print(count); Serial.print(": ") ; Serial.print(middle_temp); Serial.println(" degrees C");
    Serial.print("Middle "); Serial.print(count); Serial.print(": "); Serial.print(middle_humidity); Serial.println(" % rH"); Serial.println("");

    if(temp.temperature < max_temp && temp.temperature > min_temp || temp.temperature == max_temp || temp.temperature == min_temp)
    {
      middle_temp += temp.temperature;
      devide_temp++;
    }
    else if(temp.temperature >= max_temp)
    {
      max_temp = temp.temperature;
    }
    else if(temp.temperature <= min_temp)
    {
      min_temp = temp.temperature;
    }



    if(humidity.relative_humidity < max_humidity&& humidity.relative_humidity > min_humidity)
    {
      middle_humidity += humidity.relative_humidity;
      devide_humidity++;
    }
    else if(humidity.relative_humidity >= max_humidity && humidity.relative_humidity > min_humidity)
    {
      max_humidity = humidity.relative_humidity;      
    }
    else if(humidity.relative_humidity <= min_humidity && humidity.relative_humidity < max_humidity)
    {
      min_humidity = humidity.relative_humidity;
    }
    
    delay(5000); //60s Pause zwischen den Messungen
  }  

  middle_temp = middle_temp/devide_temp;
  middle_humidity = middle_humidity/devide_humidity;
  
  float bat_current = analogRead(A13)*2; //Aktuelle Akku Spannung auslesen
  float bat_min = 3000.0, bat_max = 4200.0; //Min und Max Spannung des Akkus
  float bat_percent = (bat_current - bat_min) / (bat_max - bat_min) * 100; //Berechnung des Akkustandes in Prozent

  Serial.print("Battery Current: "); Serial.print(bat_current); Serial.println(" V");
  Serial.print("Battery: "); Serial.print(bat_percent); Serial.println("%");
  Serial.print("Temperature: "); Serial.print(middle_temp); Serial.println(" degrees C");
  Serial.print("Humidity: "); Serial.print(middle_humidity); Serial.println("% rH");

  BatPercentFeed->save(bat_percent);  
  BatCurrentFeed->save(bat_current);
  temperatureFeed->save(middle_temp);
  humidityFeed->save(middle_humidity);


  

  // Adafruit IO is rate limited for publishing, so a delay is required in
  // between feed->save events. In this example, we will wait three seconds
  // (1000 milliseconds == 1 second) during each loop.
  delay(3000000); // 30s
}
