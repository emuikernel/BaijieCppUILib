#ifndef AEXCEPTION_H
#define AEXCEPTION_H

class AFC_API AException
{
public:
	AException(void);
	virtual ~AException(void);

	AException(const AString& sDesc);
	AException(const AException& e);
	AException& operator =(const AException& e);

	AString GetDesc();
protected:
	AString		m_sDesc;
};

#endif//AEXCEPTION_H