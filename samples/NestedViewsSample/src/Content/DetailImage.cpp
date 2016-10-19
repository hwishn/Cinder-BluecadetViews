#include "DetailImage.h"

#include <ImageManager.h>

using namespace ci;
using namespace ci::app;
using namespace std;
using namespace bluecadet::views;
using namespace bluecadet::utils;

//==================================================
// Class Lifecycle
// 

DetailImage::DetailImage() : TouchView() {
}

DetailImage::~DetailImage(){
}

void DetailImage::setup(int id) {
	
	mBgImageContainer = BaseViewRef( new BaseView() );

	// Create bg
	mBgImage = ImageViewRef(new ImageView());
	string iconFileName = "bluecadetIcon.png";
	mBgImage->setup(ImageManager::getInstance()->getTexture(iconFileName));

	// Center the bgImage within the container
	mBgImage->setPosition(-0.5f * mBgImage->getSize());
	
	// Move the container back
	mBgImageContainer->setPosition(vec2(mBgImage->getWidth()*0.5f, mBgImage->getHeight()*0.5f));

	// Add views
	mBgImageContainer->addChild(mBgImage);
	addChild(mBgImageContainer);

	// Setup touchable area, rectangle based on icon size
	TouchView::setup(vec2(mBgImage->getWidth(), mBgImage->getHeight()));
	setTouchEnabled(true);

	// Set the position of the entire detailImage (touchable area + bgImageContainer)
	float xPos = mBgImage->getWidth()*(float)id;
	float yPos = 0.0f;
	int maxColumns = 5;
	if (id > maxColumns){
		xPos -= maxColumns*mBgImage->getWidth();
		yPos += mBgImage->getHeight();
	}

	// Set the alpha of every other image
	if (id % 2 == 0)
		setAlpha(0.5f);

	setPosition(vec2(xPos, yPos));
}

void DetailImage::update() {
	// Drag this view to match the location of the current touch if it has moving touches
	// *Another way to do this could be to make a handleMovedTouch() signal callback instead of having this check in the update
	if (hasMovingTouches()) {
		setPosition(getCurTouchPosition());
	}
}

//==================================================
// Callbacks
// 

void DetailImage::handleTouchBegan(const bluecadet::touch::TouchEvent& touchEvent) {
	// Set hit state immediately on touch down
	mBgImageContainer->setScale(vec2(1.25f));
}

void DetailImage::handleTouchEnded(const bluecadet::touch::TouchEvent& touchEvent) {
	if (touchEvent.isCanceled) {
		// Remove hit state immediately on cancel
		getTimeline()->removeTarget(&mBgImageContainer->getScale());
		mBgImage->setScale(vec2(1.0f));
	} else {
		// Remove hit state gradually on touch up
		getTimeline()->apply(&mBgImageContainer->getScale(), vec2(1), 0.25f, easeOutQuad);
	}
}