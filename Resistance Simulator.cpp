#include <stdlib.h>
#include <limits>
#include <math.h>
#include <iostream>
using namespace std;


#define NaN std::numeric_limits<float>::quiet_NaN() 
#define isNaN(X) (X != X)

void stepcalc(const float k, const float step, const float m, float& x_velocity, float& y_velocity, float& x_position, float& y_position){
	
	float velocity = sqrt(x_velocity*x_velocity + y_velocity*y_velocity);
	
	float x_acceleartion = -k*velocity*x_velocity/m;
	
	float y_acceleration = -9.8-k*velocity*y_velocity/m;
	
	x_velocity += step*x_acceleartion;
	
	if (y_acceleration < 0)
		y_velocity += step*y_acceleration;
	
	x_position += step*x_velocity;
	
	y_position += step*y_velocity;
	
	return;
}
	

bool hitTargetGivenAngle (const float h, const float m, const float theta, 
							const float d, const float step, const float k,
              const float t, const float b, const float w, float& v) {
  
  	if ( m <= 0 || h < 0 || b < 0 || theta > 90 || theta < -90 || d < 0 || step <= 0 || k < 0 || w < 0)
		return false;
	
	float angle = theta * M_PI / 180;
	
	float vel = 0.001;
	
	float x_velocity;
	float y_velocity;
	float x_position;
	float y_position;
	
	bool done1 = false;
	
	do{
		vel += 10;
		
		x_velocity = vel*cos(angle);
		y_velocity = vel*sin(angle);
		
		x_position = 0;
		y_position = 0;
		
		bool done2 = false;
		
		bool done3 = false;
		int counter = 0;
		
		do{
			stepcalc(k, step, m, x_velocity, y_velocity, x_position, y_position);
			
			counter++;
			
			if (h > t && y_position < (t-h))
				done2 = true;
			
			if (t >= h && x_position > d)
				done2 = true;
			
			if (t >= h && counter == 1000000)
				done3 = true;
			
		} while (!done2 && !done3);
		
		if (h > t && x_position > d)
			done1 = true;
		
		if (t >= h && y_position > (t-h) && !done3)
			done1 = true;
		
	} while (!done1);
	
	float vmax = vel;
	float vmin = vel-10;
	
	do{
		v = (vmin + vmax) / 2;
		
		x_position = 0;
		y_position = 0;
		
		x_velocity = v*cos(angle);
		y_velocity = v*sin(angle);
		
		bool done = false;
		
		do{
			stepcalc(k, step, m, x_velocity, y_velocity, x_position, y_position);
			
			if (h > t && y_position < (t-h))
				done = true;
			
			if (t >= h && x_position > d)
				done = true;
			
		} while (!done);
			
		if ((h > t && x_position < d) || (t >= h && y_position < (t - h)))
			vmin = v;
		
		else
			vmax = v;
	
	} while (vmax - vmin > 0.0001);
	
	float vtest = v;
	
	x_position = 0;
	y_position = 0;
		
	x_velocity = vtest*cos(angle);
	y_velocity = vtest*sin(angle);
		
	bool done = false;
	
	do{
	
		stepcalc(k, step, m, x_velocity, y_velocity, x_position, y_position);
			
		if (h > t && fabs(y_position + h) < 0.1 && x_position < b)
			return false;
		
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
	
	float step = 0.01;
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
