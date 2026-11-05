#include <twoDMath.h>
#include <debug.h>


const double	piOTwo		= M_PI/2.0;
const double	twoPi			= 2 * M_PI;
const double	threePiOtwo	= 3 * piOTwo;

double	degToRad(double deg) {
	
	double	rad;
	
	if (deg==0) {
		rad = 0;
	} else {
		rad = deg * M_PI / 180.0;
	}
	return rad;
}


double	radToDeg(double rad) {

	double	deg;
	
	if (rad==0) {
		deg = 0;
	} else {
		deg = rad * 180 / M_PI;
	}
	return deg;
}


double	getAngle(twoDPt* endPt,twoDPt* startPt) {

	double	rad;
	
	if (endPt) {						// We have a point to work with.
		if (startPt) {					// Either we have a start point or we assume we're at the axis.
			endPt->subPt(startPt);	//	Normalize the end point to the axis.
		}									//
		endPt->toPolar();				// Ok, switch to polar.
		rad = endPt->getRad();		// Save off the angle (in radians)
		if (startPt) {					// If we had a start point..
			endPt->addPt(startPt);	//	Move the end point back.
		}									//
		return  rad;					// Here you go.
	}										//
	return NAN;							// No point? Fin! No value.
}


twoDPt::twoDPt(void) {

	isPolar = false;
	magX = 0;
	radY = 0;
}


twoDPt::~twoDPt(void) {  }
	
	
void twoDPt::setPtRec(double inX,double inY) {

	isPolar = false;
	magX = inX;
	radY = inY;
}


void twoDPt::setPtPolar(double inMag,double inRad) {

	isPolar = true;
	magX = inMag;
	while(inRad>twoPi) {
		inRad = inRad - twoPi;
	}
	radY = inRad;
}


void twoDPt::setPt(twoDPt* inPt) {

	if (inPt) {
		if (inPt->getIsPolar()) {
			isPolar = true;
			magX = inPt->getMag();
			radY = inPt->getRad();
		} else {
			isPolar = false;
			magX = inPt->getX();
			radY = inPt->getY();
		}
	}
}


double twoDPt::getX(void) {

	if (isPolar) {
		toRect();
	}
	return magX;
}


double twoDPt::getY(void) {

	if (isPolar) {
		toRect();
	}
	return radY;
}


double twoDPt::getMag(void) {

	if (!isPolar) {
		toPolar();
	}
	return magX;
}


double twoDPt::getRad(void) {

	if (!isPolar) {
		toPolar();
	}
	return radY;
}


bool twoDPt::getIsPolar(void) { return isPolar; }

/*
enum quadrant {

	origin,
	xAxis,
	yAxis,
	quad1,
	quad2,
	quad3,
	quad4
};
*/

quadrant twoDPt::getQuad(void) {

	if (isPolar) {																		// If we are currently polar..
		if (magX <= 0)											return origin;		// Zero mag means we are a point. Origin point.
		if (radY == 0 || radY== M_PI)						return xAxis;		// Angle is east or west? x axis.
		if (radY == piOTwo || radY == threePiOtwo)	return yAxis;		// Up or down? y axis.
		if (radY > 0 && radY < piOTwo)					return quad1;		// Between zero and 90? #1.
		if (radY > piOTwo && radY < twoPi)				return quad2;		// Between 90 and 180? #2.
		if (radY > twoPi && radY < threePiOtwo )		return quad3;		// Between 180 and down? #3.
		if (radY > threePiOtwo && radY < twoPi)		return quad4;		// Between down and 360? #4.
	} else {																				// Else we''re currently rectangular..
		if (magX==0 && radY == 0)	return origin;								// x,y zeroed out? Origin.
		if (radY==0) 					return xAxis;								// y = 0? x axis.
		if (magX==0)					return yAxis;								// x = 0? y axis
		if (magX > 0 && radY > 0)	return quad1;								// Both positive? #1.
		if (magX < 0 && radY > 0)	return quad2;								// x neg y pos? #2.
		if (magX < 0 && radY < 0)	return quad3;								// x neg y neg? #3.
		if (magX > 0 && radY < 0)	return quad4;								// x pos y neg? #4.
	}																						//
	return quadError;																	// Shuts up the compiler.
}


void twoDPt::toRect(void) {

	double	x;
	double	y;
	
	if (isPolar) {
		switch(getQuad()) {
			case origin		: break; 	
			case xAxis		:
				if (radY == 0) {
					x = magX;
				} else {
					x = -magX;
				}
				y = 0;
			break;
			case yAxis		:
				x = 0;
				if (radY == piOTwo) {
					y = magX;
				} else {
					y = -magX;
				}
			break;
			default			:
				x = magX * cos(radY); 
				y = magX * sin(radY);
				magX = x;
				radY = y;
			break;
		}
		magX = x;
		radY = y;
 		isPolar = false;	
 	}
}


void twoDPt::toPolar(void) {

 	double	mag;
 	
	if (!isPolar) {
		mag = sqrt((radY*radY)+(magX*magX));
		switch(getQuad()) {
			case origin		: break; 	
			case xAxis		:
				if (magX>0) {
					radY = 0;
				} else {
					radY = M_PI;
				}
				magX = abs(magX);
			break;
			case yAxis		:
				magX = abs(radY);
				if (radY>0) {
					radY = piOTwo;
				} else {
					radY = threePiOtwo;
				}
			break;
			case quad1		:
				radY = atan(radY/magX);					// Check.
				magX = mag;
			break;
			case quad2		:
				 radY = M_PI + atan(radY/magX);		// Check.
				 magX = mag;
			break;
			case quad3		:
				radY = M_PI + atan(radY/magX);		// Check.
				magX = mag;
			break;
			case quad4		:
				radY = twoPi + atan(radY/magX);	// Check.
				magX = mag;
			break;
		}
 		isPolar = true;
 	}
 }
				
				
void  twoDPt::addPt(twoDPt* inPt) {  

	if (inPt) {
		toRect();
		inPt->toRect();
		magX = magX + inPt->magX;
		radY = radY + inPt->radY;
	}
}


void  twoDPt::subPt(twoDPt* inPt) {

	if (inPt) {
		toRect();
		inPt->toRect();
		magX = magX - inPt->getX();
		radY = radY - inPt->getY();
	}
}
				
