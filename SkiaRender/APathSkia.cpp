#include "stdafx.h"
#include "APathSkia.h"

APathSkia::APathSkia():APath()
{

}

APathSkia::~APathSkia()
{
	//
}

void APathSkia::Reset()
{
	m_Path.reset();
}

void APathSkia::Start()
{

}
void APathSkia::End()
{
	m_Path.close();
}

void APathSkia::AddLine(APointF pt1,APointF pt2)
{
	m_Path.moveTo(SkScalar(pt1.x),SkScalar(pt1.y));
	m_Path.lineTo(SkScalar(pt2.x),SkScalar(pt2.y));
}

void APathSkia::AddRect(ARectF r)
{
	SkRect rx;
	rx.set( SkScalar(r.left),SkScalar(r.top),SkScalar(r.right),SkScalar(r.bottom));
	m_Path.addRect(rx);
}
void APathSkia::AddEllipse(ARectF r)
{
	SkRect rx;
	rx.set( SkScalar(r.left),SkScalar(r.top),SkScalar(r.right),SkScalar(r.bottom));
	m_Path.addArc(rx,SkScalar(0),SkScalar(360));
}
void APathSkia::AddBezier(APointF pt1,APointF pt2,APointF pt3,APointF pt4)
{
	//
}
void APathSkia::AddArc(ARectF r,REAL startAngle,REAL sweepAngle)
{
	SkRect rx;
	rx.set( SkScalar(r.left),SkScalar(r.top),SkScalar(r.right),SkScalar(r.bottom));
	m_Path.addArc(rx,SkScalar(startAngle),SkScalar(sweepAngle));
}