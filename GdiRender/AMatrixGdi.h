#ifndef AMATRIXGDI_H
#define AMATRIXGDI_H

class AMatrixGdi : public AMatrix
{
	friend class ACanvasGdi;
	CLASS_INFO(MatrixGdi,Matrix);
public:
	AMatrixGdi();
	virtual ~AMatrixGdi();

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

#endif//AMATRIXGDI_H