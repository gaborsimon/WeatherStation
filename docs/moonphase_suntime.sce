// 2013.01.03 19:24

clc;
DEG_TO_RAD = %pi / 180.0;
RAD_TO_DEG = 180.0 / %pi;

Y = 2014
M = 2
D = 15

//**************************************************************************
//**************************************************************************
//******
//****** Moon Phase
//******
//**************************************************************************
//**************************************************************************

//--------------------------------
//-- Time scale
//--------------------------------
JDE = int(365.25 * (Y + 4716)) + int(30.6001 * (M + 1)) + D - 13 - 1524.5;

t = (JDE - 2451545.0) / 36525.0;

//--------------------------------
//-- Parameters
//--------------------------------
t2 = t * t;
t3 = t2 * t;
t4 = t3 * t;

d  = 297.850192 + t * 445267.111403 + t2 * -0.001882 + t3 / 545868.0 - t4 / 113065000.0;
m = 357.529109 + t * 35999.050290 + t2 * -0.000154 + t3 / 24490000.0;
mPrime = 134.963396 + t * 477198.867505 + t2 * 0.008742 + t3 / 69699.0 - t4 / 14712000.0;

//--------------------------------
//-- Phase angle
//--------------------------------
i = 180.0 - d - 6.289 * sin(mPrime * DEG_TO_RAD) + 2.100 * sin(m * DEG_TO_RAD) - 1.274 * sin((2.0 * d - mPrime) * DEG_TO_RAD) - 0.658 * sin((2.0 * d) * DEG_TO_RAD) - 0.214 * sin((2.0 * mPrime) * DEG_TO_RAD) - 0.110 * sin(d * DEG_TO_RAD);
i = i - floor(i / 360.0) * 360.0;

//--------------------------------
//-- Moon fraction
//--------------------------------
fraction = (1 + cos(i * DEG_TO_RAD)) * 0.5;

percentage = 100 * fraction;

//**************************************************************************
//**************************************************************************
//******
//****** Sun set/rise time
//******
//**************************************************************************
//**************************************************************************

//--------------------------------
//-- Time scale
//--------------------------------
t = (367 * Y) - int((7 * (Y + int((M + 9) / 12))) / 4) + int((275 * M) / 9) + D - 730530;

//--------------------------------
//-- Orbital elements of the Sun
//--------------------------------
w = 282.9404 + 4.70935E-5   * t;     // [deg] longitude of periheion
a = 1.000000;                        // [AU] mean distance
e = 0.016709 - 1.151E-9     * t;     // eccentricity
M = 356.0470 + 0.9856002585 * t;     // [deg] mean anomaly
M = M - floor(M/360.0) * 360.0;      // reduces angle between 0-360 degree 

//--------------------------------
//-- Parameters
//--------------------------------
oblecl = 23.4393 - 3.563E-7 * t;     // [deg] Obliquity of the ecliptic
L = M + w;                           // [deg] Mean longitude of the Sun
L = L - floor(L/360.0) * 360.0;      // reduces angle between 0-360 degree
//E = M + (180.0 / %pi) * e * sin(M) * (1.0 + e * cos(M)) // [deg] Eccentric anomaly
E = M + (180.0 / %pi) * e * sin(M * DEG_TO_RAD) * (1.0 + e * cos(M * DEG_TO_RAD)); // [deg] Eccentric anomaly

// Ecliptic rectangular coordinates of the Sun
x = cos(E * DEG_TO_RAD) - e;
y = sin(E * DEG_TO_RAD) * sqrt(1 - (e*e));

// Distance (r) and true anomaly (v)
r = sqrt(x*x + y*y);
v = atan(y,x) * RAD_TO_DEG;

// Longitude of the Sun
lon = v + w;
lon = lon - floor(lon/360.0) * 360.0;

// Ecliptic rectangular coordinates of the Sun
x = r * cos(lon * DEG_TO_RAD);
y = r * sin(lon * DEG_TO_RAD);
z = 0.0;

// Equatorial coordinates
xe = x;
ye = y * cos(oblecl * DEG_TO_RAD);
ze = y * sin(oblecl * DEG_TO_RAD);

//--------------------------------
//-- Sun Position
//--------------------------------
r;
RA_deg  = atan(ye,xe) * RAD_TO_DEG;
RA_h    = RA_deg / 15.0;
DEC_deg = atan(ze,sqrt(xe*xe+ye*ye)) * RAD_TO_DEG;

//--------------------------------
//-- Sun rise/set times
//--------------------------------

long = 19.0;
lat  = 47.5;
h    = -0.833;
GMST0_deg = (L + 180.0);
GMST0_deg = GMST0_deg - floor(GMST0_deg / 360.0) * 360.0;
GMST0_h = GMST0_deg / 15.0;
UTs_h = (RA_h - GMST0_h - (long / 15.0));
UTs_h = UTs_h - floor(UTs_h / 24.0) * 24.0;

X = (sin(h * DEG_TO_RAD) - sin(lat * DEG_TO_RAD) * sin(DEC_deg * DEG_TO_RAD) ) / (cos(lat * DEG_TO_RAD) * cos(DEC_deg * DEG_TO_RAD));
LHA = acos(X) * RAD_TO_DEG;
LHA_h = LHA / 15.0;

SUNRISE = UTs_h - LHA_h;
SUNRISE_H = floor(SUNRISE)
SUNRISE_M = 60 * (SUNRISE - floor(SUNRISE))
SUNSET  = UTs_h + LHA_h;
SUNSET_H = floor(SUNSET)
SUNSET_M = 60 * (SUNSET - floor(SUNSET))
