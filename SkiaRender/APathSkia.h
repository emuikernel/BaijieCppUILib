#ifndef APATHSKIA_H
#define APATHSKIA_H

#include "skpath.h"

class APathSkia: public APath
{
	CLASS_INFO(PathSkia,Path);
public:
	friend class ACanvasSkia;

	APathSkia();
	virtual ~APathSkia();

	virtual void Reset();
	virtual void Start();
	virtual void End();

	virtual void AddLine(APointF pt1,APointF pt2);
	virtual void AddRect(ARectF r);
	virtual void AddEllipse(ARectF r);
	virtual void AddBezier(APointF pt1,APointF pt2,APointF pt3,APointF pt4);
	virtual void AddArc(ARectF r,REAL startAngle,REAL sweepAngle);
protected:

	SkPath	m_Path;
};

#endif//APATHSKIA_H


