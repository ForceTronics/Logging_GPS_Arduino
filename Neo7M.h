#pragma once
/*
 
Neo7M.h - Library for reading basic GPS data (date, time, long, lat) from Neo-7M from ublox 
it was created for a video tutorial on the ForceTronics YouTube Channel that shows how to log GPS data onto a SD card
The library reads NMEA $GPRMC data, below is the format of the NMEA data string:
RMC - NMEA has its own version of essential gps pvt (position, velocity, time) data. 
It is called RMC, The Recommended Minimum, which will look similar to:
$GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W*6A
Where:
     RMC          Recommended Minimum sentence C
     123519       Fix taken at 12:35:19 UTC
     A            Status A=active or V=Void.
     4807.038,N   Latitude 48 deg 07.038' N
     01131.000,E  Longitude 11 deg 31.000' E
     022.4        Speed over the ground in knots
     084.4        Track angle in degrees True
     230394       Date - 23rd of March 1994
     003.1,W      Magnetic Variation
     *6A          The checksum data, always begins with *
Information on GPS data in the NMEA standard refer to this website: http://www.gpsinformation.org/dale/nmea.htm

Note that some of the code in this library was leveraged from a tutorial by David Watts on NEO-6M GPS module: https://www.youtube.com/watch?v=bgOZLgaLa0g&t=107s

*/

#ifndef Neo7M_h
#define Neo7M_h
#include "Arduino.h" 


class Neo7M
{
public:
   Neo7M();
  ~Neo7M();
  bool getNMEAData(HardwareSerial &sComm);
  String nmea[15]; //holds GPS data like date, time, lat, long, etc

private:
 String ConvertLat();
 String ConvertLng();
};

#endif
