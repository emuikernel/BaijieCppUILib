#ifndef APATHAGG_H
#define APATHAGG_H

class APathAgg: public APath
{
	CLASS_INFO(PathAgg,Path);
public:
	friend class ACanvasAgg;

	APathAgg();
	virtual ~APathAgg();

	virtual void Reset();
	virtual void Start();
	virtual void End();

	virtual void AddLine(APointF pt1,APointF pt2);
	virtual void AddRect(ARectF r);
	virtual void AddEllipse(ARectF r);
	virtual void AddBezier(APointF pt1,APointF pt2,APointF pt3,APointF pt4);
	virtual void AddArc(ARectF r,REAL startAngle,REAL sweepAngle);
protected:
	agg::path_storage m_agg_ps;

	bool IsClosed();
};

#endif//APATHAGG_H


