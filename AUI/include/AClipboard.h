#ifndef ACLIPBOARD_H
#define ACLIPBOARD_H


class AUI_API AClipboard : public AComponent
{
	CLASS_INFO(Clipboard,Component);
public:
	AClipboard(AComponent* pOwner);
	virtual ~AClipboard();

	void SetText(const AString& sText,bool bUnicode=true);
	AString GetText();
};

#endif//ACLIPBOARD_H