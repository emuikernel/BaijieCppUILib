#ifndef AVIEWBASE_H
#define AVIEWBASE_H


class AFC_API AViewBase : public AObject
{
	CLASS_INFO(ViewBase,Object);
public:
	AViewBase(void);
	virtual ~AViewBase(void);

	//-- 完整同步
	virtual void SyncFromModel()=0;//将模型数据同步到视图
	virtual void SyncToModel()=0;//将视图的数据或变化同步到模型
};

#endif
