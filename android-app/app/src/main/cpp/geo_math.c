#include "geo_math.h"

#include <math.h>

static double to_rad(double deg) {
    return deg * 0.017453292519943295;
}

double distance_km(double lat1, double lon1, double lat2, double lon2) {
    const double earth_km = 6371.0;
    double d_lat = to_rad(lat2 - lat1);
    double d_lon = to_rad(lon2 - lon1);
    double a = sin(d_lat / 2.0) * sin(d_lat / 2.0)
             + cos(to_rad(lat1)) * cos(to_rad(lat2))
             * sin(d_lon / 2.0) * sin(d_lon / 2.0);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));
    return earth_km * c;
}
