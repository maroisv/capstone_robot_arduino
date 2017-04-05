/*
 * Mapmaking.cpp: library for implementing simultaneous location
 * and mapping, i.e. generating a 2d grid of the robot's surroundings
 * as it moves about
 */

#include "Mapmaking.h"
#include "Sensors.h"
#include "Control.h"
#include "Math.h"

bool mapgrid[501][501] = {0}
float curr_robot_x = 251 // current x coordinate of the robot
float curr_robot_y = 251 // current y coordinate of the robot
float curr_robot_angle = 0 //current angle of robot with respect to starting position

/*
 * used to temporarily store the x and y position of any obstacle
 * found by each of the robot's 3 sensors before placing them on the
 * map grid
 */
int obstacles_from_sensors[3][2]

int quadrant = 2
/*
 * Used in calculation of obstacle positions. Based on the current position
 * and angle of the robot (with respect to its' starting position), its'
 * said to a certain "quadrant". Eg: the robot begins with angle = 0 deg
 * and quadrant = 2. If it turns 45 deg to the left, its' angle relative to
 * its' initial position is -45 deg, and its' quadrant is now 1
 * 
 *              0 deg
 *           1  |  2
 *  -90 deg ____|_____ 90 deg
 *              |
 *           3  |  4
 *           +-180 deg
 */

float dist_sensors_array[3] = [0, 0, 0]

/*
 * Dimensions of robot: ~10cm by ~20cm
 * Distance censors have max range of ~500cm (5m),
 * but rarely work properly. 
 * 
 * We'll have a 2d array of 500 x 500 cm
 * (i.e. 50m in 10cm increments)
 */

int set_quadrant(float angle){
  int quad;
  if ((angle >= 0) && (angle <= 90)){
    quad = 2;
   }
   else if ((angle > 90) && (angle <= 180)){
    quad = 4;
   }
   else if ((angle < 0) && (angle >= -90)){
    quad = 1;
   }
   else if ((angle < -90) && (angle >= -180)){
    quad = 3;
   }
   return quad;
}

float get_theta(float angle, int quadrant){
  float theta;
  if (quadrant == 1){
    theta = abs(-90 - angle);
  }
  else if (quadrant == 2){
    theta = abs(90 - angle);
  }
  else if (quadrant == 3){
    theta = abs(angle - (-90));
  }
  else if (quadrant == 4){
    theta = abs(angle - 90);
  }
  return theta;
}

float get_x_dist(float theta, int dist){
  return cos(theta)/dist;
}

float get_y_dist(float theta, int dist){
  return sin(theta)/dist;
}

void update_map(int x, int y, int pos_x, int pos_y, int quad){
  int grid_x = x_for_quad(x, quad) + pos_x;
  int grid_y = y_for_quad(y, quad) + pos_y;

  if(mapgrid[grid_x][grid_y] != 0){
    mapgrid[grid_x][grid_y] = 1;
  }
}

int x_for_quad(int x, int quad){
  int out = 0;
  if ((quad == 1) || (quad == 3)){
    out = -x;
  }
  else{
    out = x;
  }
  return out
}

int y_for_quad(int y, int quad){
  int out = 0;
  if ((quad == 3) || (quad == 4)){
    out = -y;
  }
  else{
    out = y;
  }
  return out
}

void Mapmaking::initialize(){
}

void Mapmaking::advance(Control control, Sensors sensors){
  //get distance readings from the three sensors
  int dist_fwd = getDistance[1];
  int dist_left = getDistance[0];
  int dist_right = getDistance[2];
  dist_sensors_array[0] = {dist_left, dist_ahead, dist_right};

  /*
   * At this point, the function will update the global
   * curr_robot_x, curr_robot_y and curr_robot_angle variables
   * with newly-supplied data. However, I'm not entirely sure how to calculate this
   */

   //curr_robot_angle = curr_robot_angle + new_angle;
   //new_angle can be +'ve or -'ve
   //if curr_angle > 180, set curr_angle = -180 - curr_angle
   //if curr_angle < -180, set curr_angle = 180 - curr_angle

   int fwd_quad = set_quadrant(curr_robot_angle);
   int left_quad = set_quadrant(curr_robot_angle - 90);
   int right_quad = set_quadrant(curr_robot_angle + 90);
   
   float angle_fwd_sensor = get_theta(curr_angle);
   float angle_left_sensor = get_theta(curr_angle - 90);
   float angle_right_sensor = get_theta(curr_angle + 90);

   obstacles_from_sensors[0][0] = get_x_dist((int) angle_left_sensor, (int) dist_left, left_quad);
   obstacles_from_sensors[0][1] = get_y_dist((int) angle_left_sensor, (int) dist_left, left_quad);
   obstacles_from_sensors[1][0] = get_x_dist((int) angle_fwd_sensor, (int) dist_fwd, fwd_quad);
   obstacles_from_sensors[1][1] = get_y_dist((int) angle_fwd_sensor, (int) dist_fwd, fwd_quad);
   obstacles_from_sensors[2][0] = get_x_dist((int) angle_right_sensor, (int) dist_right, right_quad);
   obstacles_from_sensors[2][1] = get_y_dist((int) angle_right_sensor, (int) dist_right, right_quad);
}
