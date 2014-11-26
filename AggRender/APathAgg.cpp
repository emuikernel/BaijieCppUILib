#include "stdafx.h"
#include "agg.h" 
#include "APathAgg.h"

APathAgg::APathAgg():APath()
{
	//
}

APathAgg::~APathAgg()
{
	//
}

void APathAgg::Reset()
{
	m_agg_ps.remove_all();
}

void APathAgg::Start()
{
	m_agg_ps.start_new_path();
}

void APathAgg::End()
{
	m_agg_ps.close_polygon();
}

bool APathAgg::IsClosed()
{
	if(!m_agg_ps.total_vertices()) return(true);

	double x, y;
	unsigned int nTip=m_agg_ps.last_vertex(&x, &y);
	if(nTip&agg::path_flags_close) return true;
	return false;
}

void APathAgg::AddLine(APointF pt1,APointF pt2)
{
	if(IsClosed()) m_agg_ps.move_to(pt1.x, pt1.y);
	else           m_agg_ps.line_to(pt1.x, pt1.y);
	m_agg_ps.line_to(pt2.x,pt2.y);
}

void APathAgg::AddRect(ARectF r)
{
	m_agg_ps.move_to(r.left,r.top);
	m_agg_ps.line_to(r.right,r.top);
	m_agg_ps.line_to(r.right,r.bottom);
	m_agg_ps.line_to(r.left,r.bottom);
	m_agg_ps.close_polygon();
}

void APathAgg::AddEllipse(ARectF r)
{
	REAL x = r.left,y = r.top;
	REAL width = r.right - r.left;
	REAL height = r.bottom - r.top;
	agg::bezier_arc arc(x+width/2.00, y+height/2.00, width/2.00, height/2.00, 0.00, 2.00*agg::pi);
	//2.3 m_agg_ps.add_path(arc, 0, true);
	m_agg_ps.join_path(arc, 0);
}

void APathAgg::AddBezier(APointF pt1,APointF pt2,APointF pt3,APointF pt4)
{
	if(IsClosed()) m_agg_ps.move_to(pt1.x,pt1.y);
	else                   m_agg_ps.line_to(pt1.x,pt1.y);
	m_agg_ps.curve4(pt2.x,pt2.y,pt3.x,pt3.y,pt4.x,pt4.y);
}

void APathAgg::AddArc(ARectF r,REAL startAngle,REAL sweepAngle)
{
	REAL x = r.left,y = r.top;
	REAL width = r.right - r.left;
	REAL height = r.bottom - r.top;
	agg::bezier_arc arc(x+width/2.00, y+height/2.00, width/2.00, height/2.00, agg::deg2rad(startAngle), agg::deg2rad(sweepAngle));

	//2.3 m_agg_ps.add_path(arc, 0,  !z_is_poly_closed());

	if(IsClosed()) m_agg_ps.concat_path(arc, 0);
	else                   m_agg_ps.join_path(arc, 0);
}

