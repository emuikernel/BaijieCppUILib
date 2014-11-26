#include "stdafx.h"
#include "agg.h"
#include "AMatrixAgg.h"

AMatrixAgg::AMatrixAgg():AMatrix()
{
	//
}

AMatrixAgg::~AMatrixAgg()
{
	//
}

void AMatrixAgg::Reset()
{
	m_agg_mtx.reset();
}

void AMatrixAgg::Translate(REAL offsetX, REAL offsetY, MatrixOrder order/*=MatrixOrderPrepend*/)
{
// 	if(order==MatrixOrderPrepend) { m_agg_mtx.premultiply(agg::trans_affine_translation(offsetX, offsetY)); }
// 	else                            { m_agg_mtx.multiply(agg::trans_affine_translation(offsetX, offsetY)); }
	m_agg_mtx.translate(offsetX,offsetY);
}
void AMatrixAgg::Scale(REAL scaleX, REAL scaleY, MatrixOrder order/*=MatrixOrderPrepend*/)
{
	if(order==MatrixOrderPrepend) { m_agg_mtx.premultiply(agg::trans_affine_scaling(scaleX, scaleY)); }
	else                            { m_agg_mtx.multiply(agg::trans_affine_scaling(scaleX, scaleY)); }
}
void AMatrixAgg::Shear(REAL shearX, REAL shearY, MatrixOrder order/*=MatrixOrderPrepend*/)
{
	if(order==MatrixOrderPrepend) { m_agg_mtx.premultiply(agg::trans_affine_skewing(shearX, shearY)); }
	else                            { m_agg_mtx.multiply(agg::trans_affine_skewing(shearX, shearY)); }
}
void AMatrixAgg::TransformVectors(APointF* pts, int count)
{
	// Store matrix to an array [6] of double
	double M[6]; m_agg_mtx.store_to(M);
	//TRACE("Matrix dump=%.2f %.2f %.2f %.2f %.2f %.2f\n", M[0], M[1], M[2], M[3], M[4], M[5]);

	for(int i=0;i<count;i++)
	{
		double x=pts[i].x;
		double y=pts[i].y;
		m_agg_mtx.transform(&x, &y);
		pts[i].x=(float)(x-M[4]);
		pts[i].y=(float)(y-M[5]);
	}
}

void AMatrixAgg::TransformPoints(APointF* pts, int count)
{
	for(int i=0;i<count;i++)
	{
		double x=pts[i].x;
		double y=pts[i].y;
		m_agg_mtx.transform(&x, &y);
		pts[i].x=(float)x;
		pts[i].y=(float)y;
	}
}

void AMatrixAgg::Rotate(REAL angle, MatrixOrder order/*=MatrixOrderPrepend*/)
{
	if(order==MatrixOrderPrepend) { m_agg_mtx.premultiply(agg::trans_affine_rotation(agg::deg2rad(angle))); }
	else                            { m_agg_mtx.multiply(agg::trans_affine_rotation(agg::deg2rad(angle))); }
}

void AMatrixAgg::RotateAt(REAL angle, const APointF &center, MatrixOrder order/*=MatrixOrderPrepend*/)
{
	Translate(center.x, center.y, order);
	if(order==MatrixOrderPrepend) { m_agg_mtx.premultiply(agg::trans_affine_rotation(agg::deg2rad(angle))); }
	else                            { m_agg_mtx.multiply(agg::trans_affine_rotation(agg::deg2rad(angle))); }
	Translate(-center.x, -center.y, order);
}

void AMatrixAgg::Multiply(const AMatrix* matrix, MatrixOrder order/*=MatrixOrderPrepend*/)
{
	if(order==MatrixOrderPrepend) { m_agg_mtx.premultiply(((AMatrixAgg *)matrix)->m_agg_mtx); }
	else                            { m_agg_mtx.multiply(((AMatrixAgg *)matrix)->m_agg_mtx); }
}