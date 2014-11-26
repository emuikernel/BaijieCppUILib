#ifndef AENUMERATOR_H
#define AENUMERATOR_H

/* 枚举器类
 * 该类不可实例化，仅用于定义一种规范
 * 枚举器必须有个宿主（一般是容器）
 */
class AFC_API AEnumerator 
{
protected:
	AEnumerator(){}
public:
	virtual ~AEnumerator(){}
	virtual bool Next(){ return false; }//进行枚举
	virtual void* Cur(){ return NULL; }//得到当前枚举值
	virtual int Index(){ return 0; }//得到当前枚举所对应的下标 (对于Map来说就是Key）
};


#endif//AENUMERATOR_H