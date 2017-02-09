#include "Neo7M.h" //call GPS library
#include <SPI.h>
#include <SD.h>

const int chipSelect = 10; //pin used for chip select on SD card SPI comm
String labels = "Time, Status, Latitude, Hemisphere, Longitude, Hemisphere, Speed, Track Angle, Date,"; //labels for fields in SD CSV file
String wFile = "WDATA"; //used as file name, need to follow 8.3 naming convention
Neo7M neo;

void setup() {
  if (!SD.begin(chipSelect)) {
    // don't do anything more:
    return;
  }
  else {
    //Serial.println("card initialized.");
    bool file = false; //tracks when we found a file name we can use
    int count = 0; //used for naming the file
    while(!file){
      if(SD.exists(wFile + ".csv")) {
        count++; //up the count for naming the file
        wFile = "WDATA" + (String)count; //make a new name
      }
      else file = true; //we found a new name so we can exit loop
    } 
    File myFile = SD.open((wFile + ".csv"), FILE_WRITE); //open file to write labels to CSV
    if (myFile) {
       myFile.println(labels);
       myFile.close(); //we are done with file so close it
    }
    else { //if file doesn't open alert user
     Serial.begin(9600);
    Serial.println("error opening file..."); 
    Serial.println("");
    Serial.end();
    }
  }
  
}

void loop() {

  delay(2000);

  //if you are using an AVR arduino then use "Serial" for argument, the Arduino Zero has more than one hardware serial port so we use "Serial1"
 if(!neo.getNMEAData(Serial1)) { //if this is true failed to get GPS data
    Serial.begin(9600);
    Serial.println("failed to get data.....");
    Serial.end();
  }
  else { //if this is true we got GPS data so let's print it to SD card
    File dataFile = SD.open((wFile+".csv"), FILE_WRITE); //create file object and open csv created in setup
    // if the file is available, write to it:
    if (dataFile) {
      for (int i = 0; i < 9; i++) { //print out the lables and matching NMEA data
         if(i<8) dataFile.print(neo.nmea[i]+","); //print GPS data to SD card and append comma
         else dataFile.println(neo.nmea[i]+",");
       }
      dataFile.close();  //close file since we are finished
      // print to the serial port too:
      Serial.begin(9600);
      for (int i = 0; i < 15; i++) { //print out the lables and matching NMEA data
        Serial.println(neo.nmea[i]);
        //Serial.println("");
      }
     Serial.end();
    }
  // if the file isn't open, pop up an error:
    else { 
      Serial.begin(9600);
      Serial.println("error opening file..."); 
      Serial.println("");
      Serial.end();
    }
  }
}

