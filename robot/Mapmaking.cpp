/*
   Mapmaking.cpp: library for implementing simultaneous location
   and mapping, i.e. generating a 2d grid of the robot's surroundings
   as it moves about
*/

#include "Mapmaking.h"
#include "Sensors.h"
#include "Control.h"
#include "Math.h"

bool mapgrid[181][181] = {0};
/*
   This array represents the map. For reasons defined below, it
   represents an area with width 9m and length 9m, with a resolution
   of 5cm (i.e. each [x][y] represents a 5x5cm square). The value of
   each square [x][y] is either 0 (no obstacle there) or 1 (there is
   an obstacle)
*/
int curr_robot_x = 91; // current x coordinate of the robot
int curr_robot_y = 91; // current y coordinate of the robot
float curr_robot_angle = 0; //current angle of robot with respect to starting position

/*
 * the robot's previous position and angle
 */
int prev_robot_x = 91;
int prev_robot_y = 91;
float prev_robot_angle = 0;
/*
   used to temporarily store the x and y position of any obstacle
   found by each of the robot's 3 sensors before placing them on the
   map grid
*/
int obstacles_from_sensors[3][2];

int quadrant = 2;
/*
   Used in calculation of obstacle positions. Based on the current position
   and angle of the robot (with respect to its' starting position), its'
   said to a certain "quadrant". Eg: the robot begins with angle = 0 deg
   and quadrant = 2. If it turns 45 deg to the left, its' angle relative to
   its' initial position is -45 deg, and its' quadrant is now 1

                0 deg
             1  |  2
    -90 deg ____|_____ 90 deg
                |
             3  |  4
             +-180 deg
*/

//int dist_sensors_array[3] = {0, 0, 0};

/*
   Dimensions of robot: ~10cm by ~20cm
   Distance censors have max range of ~500cm (5m),
   but rarely work properly. However, the maximum
   effective range of the robot's Bluetooth module
   is ~30m

   Arduino does not allow 2D arrays larger than
   [180][180], which will allow our map to capture
   an area that's 9 x 9 meters, with a 5cm resolution
*/

int set_quadrant(float angle) {
  int quad;
  if ((angle >= 0) && (angle <= 90)) {
    quad = 2;
  }
  else if ((angle > 90) && (angle <= 180)) {
    quad = 4;
  }
  else if ((angle < 0) && (angle >= -90)) {
    quad = 1;
  }
  else if ((angle < -90) && (angle >= -180)) {
    quad = 3;
  }
  return quad;
}

float get_theta(float angle, int quadrant) {
  float theta;
  if (quadrant == 1) {
    theta = abs(-90 - angle);
  }
  else if (quadrant == 2) {
    theta = abs(90 - angle);
  }
  else if (quadrant == 3) {
    theta = abs(angle - (-90));
  }
  else if (quadrant == 4) {
    theta = abs(angle - 90);
  }
  return theta;
}

int get_x_dist(float theta, int dist) {
  return cos(theta) / dist;
}

int get_y_dist(float theta, int dist) {
  return sin(theta) / dist;
}

int x_for_quad(int x, int quad) {
  int out = 0;
  if ((quad == 1) || (quad == 3)) {
    out = -x;
  }
  else {
    out = x;
  }
  return out;
}

int y_for_quad(int y, int quad) {
  int out = 0;
  if ((quad == 3) || (quad == 4)) {
    out = -y;
  }
  else {
    out = y;
  }
  return out;
}

void update_map(int x, int y, int pos_x, int pos_y) {
  int grid_x = round((x + curr_robot_x)/5);
  int grid_y = round((y + curr_robot_y)/5);

  if (mapgrid[grid_x][grid_y] != 0) {
    mapgrid[grid_x][grid_y] = 1;
  }
}

void Mapmaking::initialize() {
  curr_robot_x = 91; // current x coordinate of the robot
  curr_robot_y = 91; // current y coordinate of the robot
  curr_robot_angle = 0; //current angle of robot with respect to starting position

  prev_robot_x = 91;
  prev_robot_y = 91;
  prev_robot_angle = 0;
}

void Mapmaking::advance(Control control, Sensors sensors) {
  //get distance readings from the three sensors

  prev_robot_angle = curr_robot_angle;
  curr_robot_angle = control.getAngleRotation() - prev_robot_angle;
  if (curr_robot_angle > 180){
    curr_robot_angle = -180 - curr_robot_angle;
  }
  else if (curr_robot_angle < -180){
    curr_robot_angle = 180 - curr_robot_angle;
  }
  
  int fwd_quad = set_quadrant(curr_robot_angle);

  float new_dist = control.getDistanceTravelled();
  prev_robot_x = curr_robot_x;
  prev_robot_y = curr_robot_y;
  curr_robot_x = prev_robot_x + x_for_quad(get_x_dist(curr_robot_angle, new_dist), fwd_quad);
  curr_robot_y = prev_robot_y + y_for_quad(get_y_dist(curr_robot_angle, new_dist), fwd_quad);
  
  int dist_fwd = sensors.getDistance(1);
  int dist_left = sensors.getDistance(0);
  int dist_right = sensors.getDistance(2);
  int dist_sensors_array[3] = {dist_left, dist_fwd, dist_right};

  /*
     At this point, the function will update the global
     curr_robot_x, curr_robot_y and curr_robot_angle variables
     with newly-supplied data. However, I'm not entirely sure how to calculate this
  */
  /*
    curr_robot_angle = curr_robot_angle + new_angle;
    new_angle can be +'ve or -'ve
    if curr_angle > 180, set curr_angle = -180 - curr_angle
    if curr_angle < -180, set curr_angle = 180 - curr_angle
  */
  
  int left_quad = set_quadrant(curr_robot_angle - 90);
  int right_quad = set_quadrant(curr_robot_angle + 90);

  float angle_fwd_sensor = get_theta(curr_robot_angle, fwd_quad);
  float angle_left_sensor = get_theta(curr_robot_angle - 90, left_quad);
  float angle_right_sensor = get_theta(curr_robot_angle + 90, right_quad);

  obstacles_from_sensors[0][0] = get_x_dist((float) angle_left_sensor, (int) dist_left); //TODO: add quadrants
  obstacles_from_sensors[0][1] = get_y_dist((float) angle_left_sensor, (int) dist_left);
  obstacles_from_sensors[1][0] = get_x_dist((float) angle_fwd_sensor, (int) dist_fwd);
  obstacles_from_sensors[1][1] = get_y_dist((float) angle_fwd_sensor, (int) dist_fwd);
  obstacles_from_sensors[2][0] = get_x_dist((float) angle_right_sensor, (int) dist_right);
  obstacles_from_sensors[2][1] = get_y_dist((float) angle_right_sensor, (int) dist_right);

  update_map(x_for_quad(obstacles_from_sensors[0][0], left_quad), y_for_quad(obstacles_from_sensors[0][1], left_quad), curr_robot_x, curr_robot_y);
  update_map(x_for_quad(obstacles_from_sensors[1][0], fwd_quad), y_for_quad(obstacles_from_sensors[1][1], fwd_quad), curr_robot_x, curr_robot_y);
  update_map(x_for_quad(obstacles_from_sensors[2][0], right_quad), y_for_quad(obstacles_from_sensors[2][1], right_quad), curr_robot_x, curr_robot_y);
}
