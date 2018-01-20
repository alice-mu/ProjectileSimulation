//Calculates the required angle for a projectile under the force of drag to hit a given target

//Uses discrete-time simulation with user-inputted differential element magnitude and drag constant to do a step-by-step
//simulation of a projectile firing under the effects of air resistance

//Because drag is determined by the total velocity and affects, in turn, the total velocity, the problem cannot
//be solved without a discrete-time silmulation


#include <stdlib.h>
#include <limits>
#include <math.h>
#include <iostream>
#define NaN std::numeric_limits<float>::quiet_NaN() 
#define isNaN(X) (X != X)

using namespace std;

//calculates a miniscule change in time and its resulting effects on acceleration, velocity, and position
void stepcalc(const float k, const float step, const float m, float& x_velocity, float& y_velocity, float& x_position, float& y_position){
	
	float velocity = sqrt(x_velocity*x_velocity + y_velocity*y_velocity);
	
	//x acceleration is always in the opposite direction of velocity
	float x_acceleartion = -k*velocity*x_velocity/m;
	
	//y acceleration can be positive or negative relative to its velocity
	float y_acceleration = -9.8-k*velocity*y_velocity/m;
	
	x_velocity += step*x_acceleartion;
	
	//set limit according to terminal velocity
	//(drag force cannot overcome gravity, velocity will cease to change once y_acceleration mathematically works out to be upwards)
	if (y_acceleration < 0)
		y_velocity += step*y_acceleration;
	
	x_position += step*x_velocity;
	
	y_position += step*y_velocity;
	
	return;
}
	
//function to be called in main to calculate the required angle for a projectile under the force of drag to hit a given target
bool hitTargetGivenAngle (const float h, const float m, const float theta, 
							const float d, const float step, const float k,
              const float t, const float b, const float w, float& v) {
  
	//error-checking for invalid parameters such as negative mass
  	if ( m <= 0 || h < 0 || b < 0 || theta > 90 || theta < -90 || d < 0 || step <= 0 || k < 0 || w < 0)
		return false;
	
	float angle = theta * M_PI / 180;
	
	//starting velocity small, but not 0 (infinite loop)
	float vel = 0.001;
	
	float x_velocity;
	float y_velocity;
	float x_position;
	float y_position;
	//true if precision is acheived through bisection
	bool done1 = false;
	
	//PART ONE: FINDING INTERVAL FOR BISECTION
	//do-while loop that increments vel by 10 until the projectile overshoots the target
	//then the (max of the bisection interval) = vel; (the min) = vel-10
	
	//more efficient than setting bisection range to (0, FLT_MAX) as I am working with the assumption
	//of real-life values (velocity < 100m/s)
	do{
		vel += 10;
		
		x_velocity = vel*cos(angle);
		y_velocity = vel*sin(angle);
		
		x_position = 0;
		y_position = 0;
		
		//true if either x or y position reaches their respective target distances
		bool done2 = false;
		
		//true if counter hits 1000000; angle too steep, projectile "never" reaches x-position needed
		//counter is essentially a limit on the time; protect from infinite loop
		bool done3 = false;
		int counter = 0;
		
		do{
			//discrete-time simulation step
			stepcalc(k, step, m, x_velocity, y_velocity, x_position, y_position);
			
			counter++;
			
			//if firing height > target height, and y position is below the target height
			//(if y position reaches its required distance)
			if (h > t && y_position < (t-h))
				done2 = true;
			
			//if firing height < target height, and x position is further than the target distance
			//did not check for y position like last if statement because there are 2 roots if target height > firing height
			if (t >= h && x_position > d)
				done2 = true;
			
			if (t >= h && counter == 1000000)
				done3 = true;
			
		} while (!done2 && !done3);
		
		//projectile overshot the target; upper bound of velocity interval found for bisection
		if (h > t && x_position > d)
			done1 = true;
		
		//projectile overshot the target and not because of my counter restriction on the time
		//upper bound of velocity interval found for bisection
		if (t >= h && y_position > (t-h) && !done3)
			done1 = true;
		
		//if the velocity used in these calculations don't meet requirements, increment by 10 and simulate again
	} while (!done1);
	
	float vmax = vel;
	float vmin = vel-10;
	
	//PART TWO: BISECTION ON THE FOUND INTERVAL
	do{
		v = (vmin + vmax) / 2;
		
		//resetting position and velocity for new discrete-time calculations
		x_position = 0;
		y_position = 0;
		
		x_velocity = v*cos(angle);
		y_velocity = v*sin(angle);
		
		//true if either x or y position reaches their respective target distances
		bool done = false;
		
		do{
			stepcalc(k, step, m, x_velocity, y_velocity, x_position, y_position);
			
			if (h > t && y_position < (t-h))
				done = true;
			
			if (t >= h && x_position > d)
				done = true;
			
		} while (!done);
		
		//replacing vmax or vmin with the midpoint velocity
		if ((h > t && x_position < d) || (t >= h && y_position < (t - h)))
			vmin = v;
		else
			vmax = v;
	
	//until precision achieved
	} while (vmax - vmin > 0.0001);
	
	
	//PART THREE: RE-TESTING FINAL ANSWER
	//must check if projectile would hit the corner of the target building before it reaches target
	float vtest = v;
	
	x_position = 0;
	y_position = 0;
		
	x_velocity = vtest*cos(angle);
	y_velocity = vtest*sin(angle);
		
	bool done = false;
	
	do{
	
		stepcalc(k, step, m, x_velocity, y_velocity, x_position, y_position);
		
		//when firing height > target height
		//if y position reaches ground level and x position has not reached the target building x distance
		//(if the building height<0 and projectile hits the ground instead of entering the indent the negative building makes)
		if (h > t && fabs(y_position + h) < 0.1 && x_position < b)
			return false;
		
		//when firing height < target height
		//if x position reaches the edge of the building and y position is below the height of target building
		//(if projectile clips edge of building)
		if (t > h && fabs(x_position - b) < 0.1 && y_position < (t-h))
			return false;
				
		if (h > t && y_position < (t-h))
			done = true;
			
		if (t >= h && x_position > d)
			done = true;
			
	} while (!done);
	
	return true;
}
		
int main() {
	
	//taking user inputs for parameters
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
 
 	cout << "what is the initial angle?" << endl;
	float theta;
	cin >> theta;
	
	cout << "what is the drag constant?" << endl;
	float k;
	cin >> k;
	
	cout << "what is the differential time magnitude?" << endl;
	float step;
	cin >> step;

	float v;	

	cout << endl << "Given angle: " << theta << endl;
	cout << "Target at (" << d << "," << t << ")" << endl;
	
	if (hitTargetGivenAngle (h, m, theta, d, step, k, t, b, w, v)) {
		cout << "required initial velocity: " << v << endl << endl;
	}
	
	else {
		cout << "cannot calculate the velocity" << endl << endl;
	}
    
	return 0;
}
