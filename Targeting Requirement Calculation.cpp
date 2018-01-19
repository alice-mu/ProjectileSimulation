#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;

#define NaN std::numeric_limits<float>::quiet_NaN() 
#define isNaN(X) (X != X)

float ylocation (const float angle, const float v);

float ylocation (const float h, const float vtotal, const float angle, const float t) {
	
	float yloc = h + ((vtotal * sin(angle)) * t) + (0.5 * -9.8 * t * t);
	
	return yloc;
}

bool hitTargetGivenVelocity (const float h, const float v, const float m,
							 const float d, const float t, const float b, 
							 const float w, float& theta) {
	
	if (h < 0 || v < 0 || m <= 0|| d < 0 || b < 0 || w <= 0 || theta > 270 || theta < -90)
	  return false;
  
	float maxang = (M_PI / 2)-0.01;
	
	float minang = M_PI / 4;
	
	int counter = 0;
	
	float midpoint;
	
	float time_minang;
	
	float time_midpoint;
	
	float dest_minang;
	
	float dest_midpoint;
	
	bool done = false;
	
	do{
		midpoint = (maxang + minang) /2;
		
		time_minang = d / (v * cos(minang));
	
		time_midpoint = d / (v * cos(midpoint));
		
		dest_minang = ylocation((h-t), v, minang, time_minang);
	
		dest_midpoint = ylocation((h-t), v, midpoint, time_midpoint);

		if (fabs(dest_midpoint) < 0.000001){
			
			theta = midpoint;
			
			done = true;
		}
		
		else if (fabs(dest_minang) < 0.000001){
			
			theta = minang;
			
			done = true;
		}
		
		else if (dest_minang * dest_midpoint < 0)
			maxang = midpoint;

		else
			minang = midpoint;
		
	} while ((maxang - minang) > 0.000001 && !done);
		
	if (!done)
		theta = midpoint;
	
	theta = 180 * theta / M_PI;
	
	return true;
}

bool hitTargetGivenAngle (const float h, const float m, const float theta, 
							const float d, const float t, const float b, 
							const float w, float& v) {
	
	if (h < 0 || v < 0 || m <= 0|| d < 0 || b < 0 || w <= 0 || theta > 270 || theta < -90)
	  return false;
  
	v = 0.01;
	
	float angle = theta * M_PI / 180;
	
	float desty;
	
	float time;
	
	do{
		v += 10;
		
		time = d / (v * cos(angle));
		
		desty = ylocation((h - t), v, angle, time);
		
	} while (desty < 0);
	
	float vmin = v - 10;
	
	float vmax = v;
	
	float midpoint;

	float time_vmin;
	
	float time_midpoint;
	
	float dest_vmin;
	
	float dest_midpoint;
	
	bool done = false;

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
	
  float d = 60;
  float h = 70;
  float b = 40;
  float t = 70;
  float w = 30;
  float m = 0.5;
  float theta = 77.6;
  float v = 25.5;

  cout << "Given initial velocity: " << v << endl;
  
  cout << "Target at (" << d << "," << t << ")" << endl;
  
  if (hitTargetGivenVelocity (h, v, m, d, t, b, w, theta)) {
    cout << "required angle: " << theta << endl << endl;
  }
  
  else {
    cout << "cannot calculate the angle\n\n";  
  }
  
  cout << "Given angle: " << theta << endl;
  
  cout << "Target at (" << d << "," << t << ")" << endl;
  
  if (hitTargetGivenAngle (h, m, theta, d, t, b, w, v)) {
    cout << "required initial velocity: " << v << endl << endl;
  }
  
  else {
    cout << "cannot calculate the velocity\n\n";
  }

  return 0;
}
