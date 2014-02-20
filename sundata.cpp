/*  
********************************
*Copyright 2013, Nikos Zempilis*
********************************

    ***********************************************************************
    * This file is part of Sundata, Arduino Library.                      *
    *                                                                     *
    * Sundata is free software: you can redistribute it and/or modify     *
    * it under the terms of the GNU General Public License as published by*
    * the Free Software Foundation, either version 3 of the License, or   *
    * (at your option) any later version.                                 *
    *                                                                     *
    * Sundata is distributed in the hope that it will be useful,          *
    * but WITHOUT ANY WARRANTY; without even the implied warranty of      *
    * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
    * GNU General Public License for more details.                        *
    *                                                                     *
    * You should have received a copy of the GNU General Public License   *
    * along with Sundata.  If not, see <http://www.gnu.org/licenses/>.     *
    ***********************************************************************

This is a library for calculating the basic attributes needed for solar position, sun azimuth, 
sun elevation, sunrise and sunset time. It was created to be used in solar panel trackers. The 
equations were all found at http://pveducation.org/ and tested with various online solar position
calculators. 

Author: Nikos Zempilis 
Email: nzebilis@yahoo.com

gr.linkedin.com/pub/nikos-zempilis/75/844/a90/
*/

#include "sundata.h"
#include "Arduino.h"
#include <math.h>

#define ONE_DEGREE_RADIAN 0.0174532925 
#define ONE_RADIAN        57.2957795

sundata::sundata(float lat_deg,float long_deg, int DT){                                           //constructor
  
  _lat_deg=lat_deg;                                                                               
  _lat_rad=_lat_deg * ONE_DEGREE_RADIAN;                                                          //transformation from degrees to rads
  _long_deg=long_deg;
  _long_rad=_long_deg * ONE_DEGREE_RADIAN;                                                        //transformation from degrees to rads
  
  _DT=DT;
}

void sundata::time(int year,int month, int day,int hour, int minute, int sec){                    //function refreshing current time
    _year=year;
    _month=month;
    _day=day;
    _hour=hour;
    _minute=minute;
    _sec=sec;
    leapyear=0;
    
    
    if((_year-2000)%4 == 0){                                                                      //checking if the year is a leapyear and setting accordingly the leapyear indicator 
      leapyear=1;                                                                                 //in order to add a day at calculation of days from the start of the year
    }                                                                                             //if not the indicator is left at 0
    
    switch (_month){                                                                              //calculation of days from start of the year
      case 12:
        _day += 30;
      case 11:
        _day += 31;
      case 10:
        _day += 30;
      case 9:
        _day += 31;
      case 8:
        _day += 31;
      case 7:
        _day += 30;
      case 6:
        _day += 31;
      case 5:
        _day += 30;
      case 4:
        _day += 31;
      case 3:
        _day += 28 + leapyear;                                                                   //if this year is a leapyear there is an extra day after february
      case 2:
        _day += 31;
        break;
   }
   
  
}
void sundata::calculations(){                                                                                                                                 //function for calculations update
   float tiltOfEarth = 23.45;
   int degreePerHour = 15;

   _LT=( (_hour*(long)3600) + (_minute*60) + _sec ) / (float)3600;                                                                                            //local time in decimal form
   
   LSTM= degreePerHour * _DT;                                                                                                                                 //local standard time meridian 
  
   B=0.986*(_day-81);                                                                                                                                         //value needed for equation of time
  
   EOT= 9.87 * sin( (B*ONE_DEGREE_RADIAN) *2) - ( 7.53 * cos(B*ONE_DEGREE_RADIAN) ) - ( 1.5 * sin(B*ONE_DEGREE_RADIAN) ) ;                                    //equation of time
  
   TC=4*(_long_deg-LSTM)+EOT;                                                                                                                                 //time correction factor in minutes 
    
   LST=_LT+(TC/60);                                                                                                                                           //local solar time
  
   HRA= degreePerHour * (LST-12);                                                                                                                             //hour angle
  
   delta= tiltOfEarth * sin(B*ONE_DEGREE_RADIAN) ;                                                                                                            //declination
 
   elev_rad=asin( ( sin(delta*ONE_DEGREE_RADIAN) * sin(_lat_rad) )  +  ( cos(delta*ONE_DEGREE_RADIAN) * cos(_lat_rad) * cos(HRA*ONE_DEGREE_RADIAN) ) );       //elevation in rads
  
   elev_deg=ONE_RADIAN*(elev_rad);                                                                                                                            //elevation in degrees
  
   azimth_rad=acos( ( ( sin(delta*ONE_DEGREE_RADIAN) * cos(_lat_rad) ) - ( cos(delta*ONE_DEGREE_RADIAN) * sin(_lat_rad) * cos(HRA*ONE_DEGREE_RADIAN) ) ) / cos(elev_rad) ) ; //azimuth in rads if it is morning
   
   azimth_deg=azimth_rad * ONE_RADIAN;                                                                                                                        //azimuth in degrees if it is morning
  
   if (LST>12 || HRA>0) {                                                                                                          
     azimth_deg=360 - azimth_deg;                                                                                                                             //azimuth in degrees if it is afternoon
     azimth_rad=azimth_deg * ONE_DEGREE_RADIAN;                                                                                                               //azimuth in rads if it is afternoon
   }
}

float sundata::elevation_rad(){                                                                    //returns elavation in rads
 return elev_rad;
}


float sundata::elevation_deg(){                                                                    //returns elevation in degrees
 return elev_deg;
}

float sundata::azimuth_rad(){                                                                      //returns azimuth in rads
return azimth_rad;
}

float sundata::azimuth_deg(){                                                                      //returns azimuth in degrees
  return azimth_deg;
}
 
float sundata::sunrise_time(){                                                                     //calculates sunrise time and returns it
  tsr= 12 - (ONE_RADIAN * 0.0666 * acos( -tan(_lat_rad)*tan(delta*ONE_DEGREE_RADIAN) ) ) - (TC/60);
  return tsr;
 }
 
float sundata::sunset_time(){                                                                      //calculates sunset time and returns it
  tss= 12 + (ONE_RADIAN * 0.0666 * acos( -tan(_lat_rad)*tan(delta*ONE_DEGREE_RADIAN) ) ) - (TC/60);
  return tss;
}
 
 
 
 
