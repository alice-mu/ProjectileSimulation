#include <iostream>
#include <cmath>
#include <math.h>
#include <stdlib.h>

using namespace std;

bool projectileDestination(const float h, const float v, const float m,
                           const float theta, const float d,
                           const float t, const float b, const float w,
                           float& destX, float& destY) {
  
  if (h < 0 || v < 0 || m <= 0 || theta < 0 || d <= 0 || t < 0 || b < 0 || w <= 0)
	  return false;
  
  if (theta == 90 || v == 0){
	  
	  destX = 0;
	  
	  destY = h;
	  
	  return true;
  }
  
  float angle = theta * M_PI / 180;
  
  float vx = v * cos(angle);
  
  float vy = v * sin(angle);
  
  float time = b / vx;
  
  destY = h + v * sin(angle) * time + 0.5 * -9.8 * time * time;

  if (destY <= 0 || theta > 90){
	  
	  destY = 0;
	  
	  time = (sqrt(vy*vy + 2*9.8*h) + vy) / 9.8;
	  
	  destX = time * vx;
	  
	  return true;
  }
  
  if (destY > 0 && destY <= t){
	  
	  destX = b;
	  
	  return true;
  }
  
  float time1 = 0;
  
  float time2 = 0;
  
  if (destY > t){
	
	float vfy = sqrt(vy*vy + 2*9.8*t);
	
	time1 = (vfy - vy) / -9.8;
	
	time2 = 2*vfy / 9.8;
	
	destX = vx * (time1 + time2);
	
	if (destX > b && destX < (b + w)){
		
		destY = t;
		
		return true;
	}

	if (destX > (b + w)){
		
		destY = 0;
		
		time = (sqrt(vy*vy + 2*9.8*h) + vy) / 9.8;
		
		destX = vx * time;
		
		return true;
	}
	
	else
		return false;
  }
}

#define isNaN(X) (X != X)

int main(const int argc, const char* const argv[]) {

  float h = 50;
  float v = 25;
  float m = 1;
  float theta = 64.4;
  float d = 60;
  float t = 30;
  float b = 40;
  float w = 30;

  float hitsAtX;
  float hitsAtY;

  if (projectileDestination(h,v,m,theta,d,t,b,w,hitsAtX,hitsAtY))
    cout << "Projectile hit at (" << hitsAtX << ", " << hitsAtY << ")" <<endl;
  else
    cout << "Unable to calculate where projectile hits." << endl;
  return 0;
}
