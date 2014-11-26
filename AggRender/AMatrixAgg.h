#ifndef AMATRIXAGG_H
#define AMATRIXAGG_H

class AMatrixAgg : public AMatrix
{
	friend class ACanvasAgg;
	CLASS_INFO(MatrixAgg,Matrix);
public:
	AMatrixAgg();
	virtual ~AMatrixAgg();

	virtual void Reset();
	virtual void Translate(REAL offsetX, REAL offsetY, MatrixOrder order=MatrixOrderPrepend);
	virtual void Scale(REAL scaleX, REAL scaleY, MatrixOrder order=MatrixOrderPrepend);
	virtual void Shear(REAL shearX, REAL shearY, MatrixOrder order=MatrixOrderPrepend);
	virtual void TransformVectors(APointF* pts, int count);
	virtual void TransformPoints(APointF* pts, int count);
	virtual void Rotate(REAL angle, MatrixOrder order=MatrixOrderPrepend);
	virtual void RotateAt(REAL angle, const APointF &center, MatrixOrder order=MatrixOrderPrepend);
	virtual void Multiply(const AMatrix* matrix, MatrixOrder order=MatrixOrderPrepend);
protected:
	agg::trans_affine m_agg_mtx;
};

#endif//AMATRIXAGG_H