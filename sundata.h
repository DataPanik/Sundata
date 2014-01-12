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


#ifndef sundata_h
#define sundata_h

#include "Arduino.h"
#include <math.h>
	
class sundata
{
  
  public:
        
        sundata(float lat_deg,float long_deg, int DT);                                              //constructor with input variables lattitude and longitude in degrees and also the time difference from Greenwich
        
        void time (int year,int month, int day,int hour, int minute, int sec);                      //function for refreshing current time
        
        void calculations();                                                                        //function for updating the results following the time function
        
        float elevation_rad();                                                                      //returns sun's elevation in rads
	   
	float elevation_deg();                                                                      //returns sun's elevation in degrees
        
        float azimuth_rad();                                                                        //returns sun's azimuth in rads
        
	float azimuth_deg();                                                                        //returns sun's azimuth in degrees

        float sunrise_time();                                                                       //returns time of sunrise in decimal time                                                       
        
        float sunset_time();                                                                        //retuns time of sunset in decimal time
        
        
  private:
  
//input variables
//----------------------------------------------------------------
        float _lat_deg;                                                                             //lattitude in degrees
        float _lat_rad;                                                                             //lattitude in rads
        float _long_deg;                                                                            //longtitude in degrees
        float _long_rad;                                                                            //longtitude in rads
        int _DT;                                                                                    //time difference from Greenwich
        
        int _year;              
        int _month;
        int _day;
        int _hour;
        int _minute;
        int _sec;
        
//calcuations variables
//----------------------------------------------------------------

        float _LT;                                                                                  //local time in minutes
        int leapyear;                                                                               //indicator for leapyears
        float tss;                                                                                  //time of sunset
        float tsr;                                                                                  //time of sunrise
        
        float LSTM;                                                                                 //local standard time meridian
        float EOT;                                                                                  //equation of time
        float B;                                                                                    //value needed for calculation of equation of time 
        
        float TC;                                                                                   //time correction factor
        
        float LST;                                                                                  //local solar time
        
        float HRA;                                                                                  //hour angle
        
        float delta;                                                                                //declination
        
        float elev_deg;                                                                             //elevation in degrees
        float elev_rad;                                                                             //elavation in rad
        float azimth_deg;                                                                           //azimou8io in degrees
        float azimth_rad;                                                                           //azimou8io in rad

};

#endif
