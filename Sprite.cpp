#include "Sprite.h"



SkAutoPixmapStorage::SkAutoPixmapStorage() : fStorage(nullptr) {}

SkAutoPixmapStorage::~SkAutoPixmapStorage() {
	this->freeStorage();
}

SkAutoPixmapStorage& SkAutoPixmapStorage::operator=(SkAutoPixmapStorage&& other) {
	this->fStorage = other.fStorage;
	this->INHERITED::reset(other.info(), this->fStorage, other.rowBytes());

	other.fStorage = nullptr;
	other.INHERITED::reset();

	return *this;
}

size_t SkAutoPixmapStorage::AllocSize(const SkImageInfo& info, size_t* rowBytes) {
	size_t rb = info.minRowBytes();
	if (rowBytes) {
		*rowBytes = rb;
	}
	return info.computeByteSize(rb);
}

bool SkAutoPixmapStorage::tryAlloc(const SkImageInfo& info) {
	this->freeStorage();

	size_t rb;
	size_t size = AllocSize(info, &rb);
	if (SkImageInfo::ByteSizeOverflowed(size)) {
		return false;
	}
	void* pixels = sk_malloc_canfail(size);
	if (nullptr == pixels) {
		return false;
	}
	this->reset(info, pixels, rb);
	fStorage = pixels;
	return true;
}

void SkAutoPixmapStorage::alloc(const SkImageInfo& info) {
	SkASSERT_RELEASE(this->tryAlloc(info));
}

sk_sp<SkData> SkAutoPixmapStorage::detachPixelsAsData() {
	if (!fStorage) {
		return nullptr;
	}

	sk_sp<SkData> data = SkData::MakeFromMalloc(fStorage, this->computeByteSize());
	fStorage = nullptr;
	this->INHERITED::reset();

	return data;
}


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


void Sprite::SetScale(SkScalar scale)
{
	SkImageInfo info = SkImageInfo::MakeN32Premul(SpriteImage->width() * scale, SpriteImage->height() * scale);
	SkAutoPixmapStorage scaled;
	scaled.alloc(info);
	
	SpriteImage->scalePixels(scaled, kHigh_SkFilterQuality, SkImage::kDisallow_CachingHint);

	delete SpriteImage.get();
	//不知道会不会内存泄漏
    SpriteImage =SkImage::MakeRasterCopy(scaled);
	SetSize(SpriteImage->width() , SpriteImage->height() );
}

void Sprite::SetScaleX(SkScalar sx)
{
	SetSize(SpriteImage->width() * sx, SpriteImage->height());
	//SpriteImage.get().
}

void Sprite::SetScaleY(SkScalar sy)
{
	SetSize(SpriteImage->width(),SpriteImage->height() * sy);
}


void Sprite::OnMouseMove(int x, int y)
{
}

void  Sprite::OnMouseDown(int x, int y)
{
}

//void draw(SkCanvas* canvas) {
//	std::vector<int32_t> srcPixels;
//	int quarterWidth = image->width() / 16;
//	int rowBytes = quarterWidth * 4;
//	int quarterHeight = image->height() / 16;
//	srcPixels.resize(quarterHeight * rowBytes);
//	SkPixmap pixmap(SkImageInfo::MakeN32Premul(quarterWidth, quarterHeight),
//		&srcPixels.front(), rowBytes);
//	canvas->scale(4, 4);
//	SkFilterQuality qualities[] = { kNone_SkFilterQuality, kLow_SkFilterQuality,
//					 kMedium_SkFilterQuality, kHigh_SkFilterQuality };
//	for (unsigned index = 0; index < SK_ARRAY_COUNT(qualities); ++index) {
//		image->scalePixels(pixmap, qualities[index]);
//		sk_sp<SkImage> filtered = SkImage::MakeFromRaster(pixmap, nullptr, nullptr);
//		canvas->drawImage(filtered, 16 * index, 0);
//	}
//}
//void draw(SkCanvas* canvas) {
//	SkPaint paint;
//	SkPictureRecorder recorder;
//	SkCanvas* recordingCanvas = recorder.beginRecording(50, 50);
//	for (auto color : { SK_ColorRED, SK_ColorBLUE, 0xff007f00 }) {
//		paint.setColor(color);
//		recordingCanvas->drawRect({ 10, 10, 30, 40 }, paint);
//		recordingCanvas->translate(10, 10);
//		recordingCanvas->scale(1.2f, 1.4f);
//	}
//	sk_sp<SkPicture> playback = recorder.finishRecordingAsPicture();
//	int x = 0, y = 0;
//	for (auto alpha : { 70, 140, 210 }) {
//		paint.setAlpha(alpha);
//		auto srgbColorSpace = SkColorSpace::MakeSRGB();
//		sk_sp<SkImage> image = SkImage::MakeFromPicture(playback, { 50, 50 }, nullptr, &paint,
//			SkImage::BitDepth::kU8, srgbColorSpace);
//		canvas->drawImage(image, x, y);
//		x += 70; y += 70;
//	}
//}