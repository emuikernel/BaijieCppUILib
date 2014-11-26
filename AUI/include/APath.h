#ifndef APATH_H
#define APATH_H

class AUI_API APath : public AObject
{
	CLASS_INFO(Path,Object);
public:
	APath();
	virtual ~APath();

	virtual void Reset()=0;
	virtual void Start()=0;
	virtual void End()=0;
	virtual void AddLine(APointF pt1,APointF pt2)=0;
	virtual void AddRect(ARectF r)=0;
	virtual void AddEllipse(ARectF r)=0;
	virtual void AddBezier(APointF pt1,APointF pt2,APointF pt3,APointF pt4)=0;
	virtual void AddArc(ARectF r,REAL startAngle,REAL sweepAngle)=0;
};

#endif//APATH_H