#ifndef __E_TEXTUREPANEL_H__
#define __E_TEXTUREPANEL_H__

#include "Panel.h"

class R_Texture;

class E_TexturePanel : public Panel
{
public:
	E_TexturePanel();
	virtual ~E_TexturePanel();

	void Display() override;

private:
	void TextureInfo(R_Texture* tex);
	void TexturePanelHeader();
};

#endif // !__E_TEXTUREPANEL_H__