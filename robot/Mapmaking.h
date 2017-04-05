/*
 * Mapmaking.h: Library for implementing simultaneous
 * location and mapping
 */
 
#ifndef mapmaking_h
#define mapmaking_h

#include "Sensors.h"
#include "Control.h"
#include "Math.h"

class Mapmaking
{
  public:
    Mapmaking();
    void initialize();
    int advance(advance(Control control, Sensors sensors));
  private:
    int set_quadrant(float angle);
    float get_theta(float angle, int quadrant);
    float get_x_dist(float theta, int dist);
    float get_y_dist(float theta, int dist);
    int x_for_quad(int x, int quad);
    int y_for_quad(int y, int quad);
    void update_map(int x, int y, int pos_x, int pos_y, int quad);
}
