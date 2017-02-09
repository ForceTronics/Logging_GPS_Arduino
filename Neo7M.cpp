#include "Neo7M.h"
#include "Arduino.h"
#include <avr/dtostrf.h>        // dtostrf missing in Arduino Zero/Due, need to comment this out for AVR Arduinos

//constructor function for class
Neo7M::Neo7M() {}

//destructor function for class
Neo7M::~Neo7M() {}

//this function gets basic NMEA data from Neo-7M GPS chip and parses it into array
//it uses Serial1 comm to read data. Data it returns: date, time, lat, long, hem
//The argument is a hardware serial port that will be used to communicate with the GPS module
bool Neo7M::getNMEAData(HardwareSerial &sComm) {
  bool worked = false; //tracks if we got the GPS data we wanted or not
  int pos = 0; //used to track where we are in the array
  int stringplace = 0; //used to track where we are reading the NMEA string
  sComm.begin(9600);

    //Serial1.flush();
    while (sComm.available() > 0) //While data is available read it in
    {
      sComm.read();
  
    }
    if (sComm.find("$GPRMC,")) { //This is a header for a standard NMEA string that contains GPS data like long, lat, date, time, etc
      String tempMsg = sComm.readStringUntil('\n'); //look for endline, means we are at the end of the string
      for (int i = 0; i < tempMsg.length(); i++) {
        if (tempMsg.substring(i, i + 1) == ",") { //the data in the string is separated by commas, if a comma is detected then you reached the end of a string and you can read it
          nmea[pos] = tempMsg.substring(stringplace, i); //once a string has been found read it into array
          stringplace = i + 1; //update place holder for start of next string
          pos++; //iterate array place holder
        }
        if (i == tempMsg.length() - 1) { //reads in last string which won't end in a comma
          nmea[pos] = tempMsg.substring(stringplace, i); //read in last string
        }
      }
      nmea[2] = ConvertLat(); //convert lat reading to easy to read form
      nmea[4] = ConvertLng();
      worked = true; 
    }
    sComm.end(); //end serial comm
    return worked;
}

//This function takes the NMEA latitude string and coverts it to a readable string form 
//Part of this process depends on the hemisphere, for instance if in Southern hemisphere then make value neg
//Hem is 3 position in array (starting from zero) and lat is 2
String Neo7M::ConvertLat() {
  String posneg = "";
  if (nmea[3] == "S") { //if in southern hem add make value negative
    posneg = "-";
  }
  String latfirst;
  float latsecond;
  for (int i = 0; i < nmea[2].length(); i++) {
    if (nmea[2].substring(i, i + 1) == ".") {
      latfirst =nmea[2].substring(0, i - 2);
      latsecond = nmea[2].substring(i - 2).toFloat();
    }
  }
  latsecond = latsecond / 60;
  String CalcLat = "";

  char charVal[9];
  dtostrf(latsecond, 4, 6, charVal);
  for (int i = 0; i < sizeof(charVal); i++)
  {
    CalcLat += charVal[i];
  }
  latfirst += CalcLat.substring(1);
  latfirst = posneg += latfirst;
  return latfirst;
}

//converts NMEA longitude value into a more readable form
//if in western hem then value becomes neg
//Hem is 5 position in array (starting from zero) and lat is 4
String Neo7M::ConvertLng() {
  String posneg = "";
  if (nmea[5] == "W") {
    posneg = "-";
  }

  String lngfirst;
  float lngsecond;
  for (int i = 0; i < nmea[4].length(); i++) {
    if (nmea[4].substring(i, i + 1) == ".") {
      lngfirst = nmea[4].substring(0, i - 2);
      lngsecond = nmea[4].substring(i - 2).toFloat();
    }
  }
  lngsecond = lngsecond / 60;
  String CalcLng = "";
  char charVal[9];
  dtostrf(lngsecond, 4, 6, charVal);
  for (int i = 0; i < sizeof(charVal); i++)
  {
    CalcLng += charVal[i];
  }
  lngfirst += CalcLng.substring(1);
  lngfirst = posneg += lngfirst;
  return lngfirst;
}
