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
	opacity = 1.0;
	anchor.set(0.5, 0.5);
}

Sprite::~Sprite()
{
	this->StopAllAction();
	SpriteImage.get()->unref();
	SpriteImage.release();
}

void Sprite::Draw(SkCanvas* canvas)
{
	if (IsVisible() == false)
		return;
	SkPaint paint;
	paint.setAlpha(opacity*255);

	/*SkColorMatrix matrix;
	matrix.setRotate(SkColorMatrix::kG_Axis, 180);
	paint.setColorFilter(SkColorFilter::MakeMatrixFilterRowMajor255(matrix.fMat));*/

	canvas->save();
	canvas->rotate(GetDegress(), GetBound().left()+GetWidth()*anchor.fX, GetBound().top()+GetHeight()*anchor.fY);
	canvas->drawImageRect(SpriteImage.get(), GetBound(), &paint);
	canvas->restore();
}



void Sprite::SetScale(SkScalar sx, SkScalar sy)
{
	SkImageInfo info = SkImageInfo::MakeN32Premul(SpriteImage->width() * sx, SpriteImage->height() * sy);
	SkAutoPixmapStorage scaled;
	scaled.alloc(info);

	SpriteImage->scalePixels(scaled, kHigh_SkFilterQuality, SkImage::kDisallow_CachingHint);
	SpriteImage.get()->unref();
	SpriteImage.release();
	
	SpriteImage = SkImage::MakeRasterCopy(scaled);
	SetSize(SpriteImage->width(), SpriteImage->height());
}

void Sprite::SetScale(SkScalar scale)
{
	SetScale(scale, scale);
}

void Sprite::SetScaleX(SkScalar sx)
{
	SetScale(sx, 1.0);
}

void Sprite::SetScaleY(SkScalar sy)
{
	SetScale(1.0,sy);
}

void Sprite::SetAnchorPoint(SkPoint po)
{
	anchor = po;
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