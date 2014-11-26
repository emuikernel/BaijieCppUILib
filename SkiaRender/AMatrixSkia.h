#ifndef AMATRIXSKIA_H
#define AMATRIXSKIA_H

class AMatrixSkia : public AMatrix
{
	friend class ACanvasSkia;
	CLASS_INFO(MatrixSkia,Matrix);
public:
	AMatrixSkia();
	virtual ~AMatrixSkia();

	virtual void Reset(){}
	virtual void Translate(REAL offsetX, REAL offsetY, MatrixOrder order=MatrixOrderPrepend){}
	virtual void Scale(REAL scaleX, REAL scaleY, MatrixOrder order=MatrixOrderPrepend){}
	virtual void Shear(REAL shearX, REAL shearY, MatrixOrder order=MatrixOrderPrepend){}
	virtual void TransformVectors(APointF* pts, int count){}
	virtual void TransformPoints(APointF* pts, int count){}
	virtual void Rotate(REAL angle, MatrixOrder order=MatrixOrderPrepend){}
	virtual void RotateAt(REAL angle, const APointF &center, MatrixOrder order=MatrixOrderPrepend){}
	virtual void Multiply(const AMatrix* matrix, MatrixOrder order=MatrixOrderPrepend){}
protected:
	
};

#endif//AMATRIXSKIA_H