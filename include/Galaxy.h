#ifndef _GALAXY_H
#define _GALAXY_H

//--- Standard includes --------------------------------------------------------
#include <vector>

//--- Implementation -----------------------------------------------------------
#include "Vector.h"
#include "Star.h"

#define BLACKHOLE_MASS 1000
#define STAR_MASS 10

//------------------------------------------------------------------------------
/** \brief Implementation of a single node of the barnes hut tree. */
class Galaxy{
private:
    double start_x, start_y;
    double start_vx, start_vy;
    double radius;
    int num_stars;
public:
    std::vector<Star> star_arr;
public:
    Galaxy(int num_stars, double radius, double start_x, double start_y, double start_vx, double start_vy);
    void initGalaxy(void);
};

#endif