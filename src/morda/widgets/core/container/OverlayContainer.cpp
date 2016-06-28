#include "OverlayContainer.hpp"
#include "../proxy/MouseProxy.hpp"


using namespace morda;

namespace{

const char* DContextMenuLayout = R"qwertyuiop(
	layout{
		dimX{max} dimY{max}
	}
	MouseProxy{
		name{morda_overlay_mouseproxy}
		layout{
			dimX{max} dimY{max}
		}
	}
	Container{
		name{morda_overlay_container}
		layout{
			dimX{max} dimY{max}
		}
	}
)qwertyuiop";

}

OverlayContainer::OverlayContainer(const stob::Node* chain) :
		Widget(chain),
		Frame(chain)
{
	this->onChildrenListChanged();
}

void OverlayContainer::onChildrenListChanged(){
	if(!this->overlayLayer || !this->overlayLayer->parent()){
		this->overlayLayer = utki::makeShared<Frame>(stob::parse(DContextMenuLayout).get());
		this->add(this->overlayLayer);
		
		this->overlayContainer = this->overlayLayer->findChildByNameAs<Container>("morda_overlay_container");
		ASSERT(this->overlayContainer)
		
		auto mp = this->overlayLayer->findChildByNameAs<MouseProxy>("morda_overlay_mouseproxy");
		ASSERT(mp)
		
		mp->mouseButton = [this](Widget& w, bool isDown, const Vec2r& pos, Widget::EMouseButton button, unsigned id) -> bool{
			this->hideContextMenu();
			return false;
		};
	}
	
	ASSERT(this->overlayLayer)
	ASSERT(this->children().size() >= 1)
	
	if(this->children().back() != this->overlayLayer){
		auto w = this->overlayLayer->removeFromParent();
		this->add(w);
	}
}



void OverlayContainer::showContextMenu(std::shared_ptr<Widget> w, Vec2r anchor){
	auto& lp = this->getLayoutParamsDuringLayoutAs<LayoutParams>(*w);
	
	Vec2r dim = this->dimForWidget(*w, lp);

	for(unsigned i = 0; i != 2; ++i){
		utki::clampTop(dim[i], this->overlay().rect().d[i]);
	}
	
	this->overlay().add(w);
	
	w->resize(dim);
	
	anchor.y -= dim.y;
	
	for(unsigned i = 0; i != 2; ++i){
		utki::clampRange(anchor[i], 0.0f, this->overlay().rect().d[i] - w->rect().d[i]);
	}
	
	w->moveTo(anchor);
}

void OverlayContainer::hideContextMenu(){
	if(this->overlayContainer->children().size() == 0){
		return;
	}
	ASSERT(this->overlayContainer->children().size() > 0)
	
	this->overlayContainer->remove(*this->overlayContainer->children().back());
}