//Calculates the required angle (given velocity) and required velocity (given angle) for a projectile to hit given target

//Uses the intermediate value theorem and bisection along with recursive functions, user-inputted command-line parameters,
//and error checking to calculate the required angle and velocity for a projectile of mass m to hit location (d,t) from (0, h)
//(with other parameters taken into account such as target building width and height)


#include <stdlib.h>
#include <math.h>
#include <iostream>
#define NaN std::numeric_limits<float>::quiet_NaN() 
#define isNaN(X) (X != X)

using namespace std;

//function to be called to calculate y-location of projectile with a specified time
float ylocation (const float h, const float vtotal, const float angle, const float t) {
	
	//kinematics equation taking into account the effects of gravity on velocity
	float yloc = h + ((vtotal * sin(angle)) * t) + (0.5 * -9.8 * t * t);
	
	return yloc;
}

//function to be called in main for calculation of required angle (with a given velocity) to hit given target location
//returns true if able to calculate angle, false if unable (invalid parameters)
bool hitTargetGivenVelocity (const float h, const float v, const float m,
							 const float d, const float t, const float b, 
							 const float w, float& theta) {

	//error-checking for invalid parameters such as negative mass
	if (h < 0 || v < 0 || m <= 0|| d < 0 || b < 0 || w <= 0 || theta > 270 || theta < -90)
	  return false;
  
	//setting up max and min angle for bisection
	//no need for anything outside of 45 and 90 degrees towards the direction of the target
	//if maxang was 90 degrees program would be in infinite loop, so subtract 0.01
	float maxang = (M_PI / 2)-0.01;
	
	float minang = M_PI / 4;
	
	int counter = 0;
	
	//midpoint angle between max angle and min angle
	float midpoint;
	
	//time it takes for projectile to reach required x-location with the min agle and max angle, respectively
	float time_minang;
	
	float time_midpoint;
	
	//y-location of projectile with the time_minang and time_maxang, respectively
	float dest_minang;
	
	float dest_midpoint;
	
	bool done = false;
	
	//do-while loop check y-location of projectile in relation to required target
	//(negative value is undershooting--projectile below required location, positive is overshooting)
	//exits when projectile location is within 0.000001 of targetted location
	do{
		midpoint = (maxang + minang) /2;
		
		time_minang = d / (v * cos(minang));
	
		time_midpoint = d / (v * cos(midpoint));
		
		dest_minang = ylocation((h-t), v, minang, time_minang);
	
		dest_midpoint = ylocation((h-t), v, midpoint, time_midpoint);

		//checking if max and min of our angle range is the correct answer
		if (fabs(dest_midpoint) < 0.000001){
			
			theta = midpoint;
			
			done = true;
		}
		
		else if (fabs(dest_minang) < 0.000001){
			
			theta = minang;
			
			done = true;
		}
		
		//checking for a root between the first half of the bisection interval
		//if product is negative it tells us the one bound is overshooting and one is undershooting, correct answer is between
		//resetting max or min angle depending on root
		else if (dest_minang * dest_midpoint < 0)
			maxang = midpoint;

		else
			minang = midpoint;
	
	//stop when achieved precision
	} while ((maxang - minang) > 0.000001 && !done);
	
	if (!done)
		theta = midpoint;
	
	//return value from radians to degrees
	theta = 180 * theta / M_PI;
	
	return true;
}

//function to be called in main for calculation of required velocity (with a given angle) to hit given target location
//returns true if able to calculate angle, false if unable (invalid parameters)
bool hitTargetGivenAngle (const float h, const float m, const float theta, 
							const float d, const float t, const float b, 
							const float w, float& v) {
	
	//error-checking
	if (h < 0 || v < 0 || m <= 0|| d < 0 || b < 0 || w <= 0 || theta > 270 || theta < -90)
	  return false;
  
	//infinite loop if 0
	v = 0.01;
	
	//converting given angle from degrees to radians
	float angle = theta * M_PI / 180;
	
	float desty;
	
	float time;
	
	//loop that increments velocity by 10 until it finds a range the root is in
	//more effective than starting bisection of range 0 to FLT_MAX
	//(because I'm working under the assumption of real-life values for velocity and distance)
	do{
		v += 10;
		
		//time it takes for projectile to reach required x-position
		time = d / (v * cos(angle));
		
		//y-location of projectile using the calculated time
		desty = ylocation((h - t), v, angle, time);
		
	} while (desty < 0);
	
	//setting up bisection range
	//since v is the first known value to overshoot, v-10 is the last known value to undershoot => vmin=v-10
	float vmin = v - 10;
	
	float vmax = v;
	
	//midpoint velocity between vmin and vmax
	float midpoint;

	//time if takes for min velocity and midpoint velocity to reach x-position and their respective y-positions
	//no need to check for max velocity because if the root isn't between vmin and vmidpoint, then its between vmidpoint and vmax
	float time_vmin;
	
	float time_midpoint;
	
	float dest_vmin;
	
	float dest_midpoint;
	
	bool done = false;

	//do-while loop to do bisection on vmax and vmin bounds
	//exits when projectile location is within 0.00001 of targeted location
	do{
		midpoint = (vmin + vmax) / 2;
		
		time_vmin = d / (vmin * cos(angle));
		
		time_midpoint = d / (midpoint * cos(angle));
		
		dest_vmin = ylocation((h-t), vmin, angle, time_vmin);
		
		dest_midpoint = ylocation((h-t), midpoint, angle, time_midpoint);
		
		if (fabs(dest_vmin) < 0.00001){
			
			v = vmin;
			
			done = true;
		}
		
		if (fabs(dest_midpoint) < 0.00001){
			
			v = midpoint;
			
			done = true;
		}
		
		else if (dest_vmin * dest_midpoint < 0)
			vmax = midpoint;
		
		else
			vmin = midpoint;
		
	} while ((vmax - vmin) > 0.00001 && !done);
	
	if (!done)
		v = midpoint;
	
	return true;
}
		

int main() {
	
	//prompting user-inputted parameters for calculations
	
	cout << "what is the firing height?" << endl;
	float h;
	cin >> h;
	
	cout << "what is the object mass?" << endl;
	float m;
	cin >> m;
	
	cout << "what is the horizontal distance to the target?" << endl;
	float d;
	cin >> d;
	
	cout << "what is the target height?" << endl;
	float t;
	cin >> t;
	
	cout << "what is the distance to the target building?" << endl;
	float b;
	cin >> b;
	
	cout << "what is the width of the building?" << endl;
	float w;
	cin >> w;
	
	cout << "great! now we'll calculate the needed angle given a velocity. what is the initial velocity?" << endl;
	float v;
	cin >> v;
	
	float theta;

  	cout << "Given initial velocity: " << v << endl;
  
  	cout << "Target at (" << d << "," << t << ")" << endl;
  
	//calling function to calculate required angle with the user-inputted parameters
  	if (hitTargetGivenVelocity (h, v, m, d, t, b, w, theta)) {
    		cout << "required angle: " << theta << endl << endl;
 	}
  
  	else {
    		cout << "cannot calculate the angle\n\n";  
  	}
  
   	cout << "and now we'll calculate the needed velocity given an angle. what is the initial angle?" << endl;
	cin >> theta;
	
	cout << "Given angle: " << theta << endl;
  
	cout << "Target at (" << d << "," << t << ")" << endl;
  
	//calling function to calculate required velocity with the user-inputted parameters
	if (hitTargetGivenAngle (h, m, theta, d, t, b, w, v)) {
		cout << "required initial velocity: " << v << endl << endl;
	}
  
	else {
		cout << "cannot calculate the velocity\n\n";
	}

  	return 0;
}
