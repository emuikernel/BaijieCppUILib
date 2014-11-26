#ifndef APATHGDI_H
#define APATHGDI_H


class APathGdi: public APath
{
	CLASS_INFO(PathGdi,Path);
public:
	friend class ACanvasGdi;

	APathGdi();
	virtual ~APathGdi();

	virtual void Reset();
	virtual void Start();
	virtual void End();

	virtual void AddLine(APointF pt1,APointF pt2);
	virtual void AddRect(ARectF r);
	virtual void AddEllipse(ARectF r);
	virtual void AddBezier(APointF pt1,APointF pt2,APointF pt3,APointF pt4);
	virtual void AddArc(ARectF r,REAL startAngle,REAL sweepAngle);
protected:

};

#endif//APATHGDI_H


