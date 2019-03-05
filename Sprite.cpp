#include "Sprite.h"

//SkPoint
Sprite::Sprite(const char *pImagePath)
{
	sk_sp<SkData> blob = SkData::MakeFromFileName(pImagePath);
	if (blob == NULL) return;
	SpriteImage = SkImage::MakeFromEncoded(blob);
	SetPosition(0, 0);
	SetSize(SpriteImage->width(), SpriteImage->height());
}

void Sprite::Draw(SkCanvas* canvas)
{
	if (GetSkRect().right() == 0 || GetSkRect().bottom() == 0)
		SetSize(SpriteImage->width(), SpriteImage->height());
	canvas->drawImageRect(SpriteImage.get(), GetSkRect(), 0);
	//canvas->drawImage(SpriteImage.get(), GetSkRect().left(),GetSkRect().top(), 0);
}


void Sprite::OnMouseMove(int x, int y)
{
}

void  Sprite::OnMouseDown(int x, int y)
{
}
