#ifndef AMVCBASE_H
#define AMVCBASE_H

class AFC_API AMVCBase : public AObject
{
	CLASS_INFO(MVCBase,Object);
public:
	AMVCBase();
	virtual ~AMVCBase();

	virtual bool GetModified();
	virtual void SetModified(bool v);
protected:
	bool	m_bModified;
};
#endif//AMVCBASE_H