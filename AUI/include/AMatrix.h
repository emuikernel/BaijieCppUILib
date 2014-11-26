#ifndef AMATRIX_H
#define AMATRIX_H

class AUI_API AMatrix : public AObject
{
	CLASS_INFO(Matrix,Object);
public:
	AMatrix();
	virtual ~AMatrix();

	virtual void Reset()=0;
	virtual void Translate(REAL offsetX, REAL offsetY, MatrixOrder order=MatrixOrderPrepend)=0;
	virtual void Scale(REAL scaleX, REAL scaleY, MatrixOrder order=MatrixOrderPrepend)=0;
	virtual void Shear(REAL shearX, REAL shearY, MatrixOrder order=MatrixOrderPrepend)=0;
	virtual void TransformVectors(APointF* pts, int count)=0;
	virtual void TransformPoints(APointF* pts, int count)=0;
	virtual void Rotate(REAL angle, MatrixOrder order=MatrixOrderPrepend)=0;
	virtual void RotateAt(REAL angle, const APointF &center, MatrixOrder order=MatrixOrderPrepend)=0;
	virtual void Multiply(const AMatrix* matrix, MatrixOrder order=MatrixOrderPrepend)=0;
};

#endif//AMATRIX_H