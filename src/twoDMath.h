#ifndef twoDMath_h
#define twoDMath_h

#include <arduino.h>


class twoDPt;

extern	const double	piOTwo;
extern	const double	twoPi;
extern	const double	threePiOtwo;

extern	double	degToRad(double deg);
extern	double	radToDeg(double rad);
extern	double	angle(twoDPt* endPt,twoDPt* startPt=NULL);



enum quadrant {

	origin,
	xAxis,
	yAxis,
	quad1,
	quad2,
	quad3,
	quad4,
	quadError
};



class twoDPt {

	public:
				twoDPt(void);
	virtual	~twoDPt(void);
	
				void		setPtRec(double inX,double inY);
				void		setPtPolar(double inMag,double inRad);
				void		setPt(twoDPt* inPt);
				double	getX(void);
				double	getY(void);
				double	getMag(void);
				double	getRad(void);
				quadrant	getQuad(void);
				bool		getIsPolar(void);
				
				void		toRect(void);
				void		toPolar(void);
				
				void		addPt(twoDPt* inPt);
				void		subPt(twoDPt* inPt);
				
	protected:
				bool		isPolar;		// Are these variables holding rectangular or polar values?
				double	magX;			// x or magnitude.
				double	radY;			// y or radians.
};

#endif