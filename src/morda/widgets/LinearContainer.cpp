#include "LinearContainer.hpp"

#include "../util/Gravity.hpp"
#include "../util/LayoutDim.hpp"
#include "../util/util.hpp"

#include <cmath>



using namespace morda;


//TODO: add edge margins


namespace{
	
const char* D_Margins = "margins";
const char* D_Weight = "weight";
const char* D_Band = "band";

class Info{
public:
	real weight;
	Vec2r dim;
	real margin;//actual margin between child widgets
	Gravity gravity;
	LeftBottomRightTop margins;
};

}//~namespace



LinearContainer::LinearContainer(const stob::Node* desc) :
		Widget(desc),
		Container(desc),
		PaddedWidget(desc),
		LinearWidget(desc),
		GravitatingWidget(desc)
{}



//override
void LinearContainer::OnResize(){
	unsigned longIndex = this->GetLongIndex();
	unsigned transIndex = this->GetTransIndex();
	
	std::vector<Info> infoArray(this->Children().size());
	
	//Calculate rigid size, net weight and store weights and margins
	real rigid = this->Padding()[longIndex] + this->Padding()[2 + longIndex];
	real netWeight = 0;
	
	{
		auto info = infoArray.begin();
		for(auto i = this->Children().cbegin(); i != this->Children().cend(); ++i, ++info){
			ASSERT(info != infoArray.end())
			if(const stob::Node* layout = (*i)->GetPropertyNode(Container::D_Layout())){
				if(const stob::Node* weight = layout->GetProperty(D_Weight)){
					info->weight = weight->AsFloat();
					netWeight += info->weight;
				}else{
					info->weight = 0;
				}

				LayoutDim dim = LayoutDim::FromLayout(*layout);
				info->dim = (*i)->Measure(dim.ForWidget(*this, *(*i)));
				
				if(const stob::Node* n = layout->GetProperty(D_Band)){
					if(NodeHoldsFractionValue(*n)){
						real band = n->AsFloat() / 100;
						ting::util::ClampBottom(info->dim[transIndex], this->Rect().d[transIndex] * band);
					}else{
						ting::util::ClampBottom(info->dim[transIndex], std::min(DimValue(*n), this->Rect().d[transIndex]));
					}
				}
				
				if(const stob::Node* g = layout->Child(Gravity::D_Gravity()).node()){
					info->gravity = Gravity::FromSTOB(*g);
				}else{
					info->gravity = this->gravity();
				}
				
				if(const stob::Node* margins = layout->Child(D_Margins).node()){
					info->margins = LeftBottomRightTop::FromSTOB(*margins);
				}else{
					info->margins = LeftBottomRightTop::Default();
				}
			}else{
				info->dim = (*i)->GetMinDim();
				info->weight = 0;
				info->gravity = this->gravity();
				info->margins = LeftBottomRightTop::Default();
			}
			
			if(info != infoArray.begin()){//if not first child
				info->margin = std::max(
						(info - 1)->margins[this->IsReverse() ? longIndex : (longIndex + 2)],
						info->margins[this->IsReverse() ? (longIndex + 2) : longIndex]
					);
			}else{
				info->margin = 0;
			}
			
			rigid += info->dim[longIndex] + info->margin;
		}
	}
	
	//arrange widgets
	{
		real flexible = this->Rect().d[longIndex] - rigid;
		ting::util::ClampBottom(flexible, 0.0f);
		if(!std::isfinite(flexible)){
			flexible = 0;
		}
		ASSERT_INFO(flexible >= 0, "flexible = " << flexible)
		
		real pos = this->Padding()[this->IsReverse() ? (longIndex + 2) : longIndex];//start arranging widgets from padding
		auto info = infoArray.begin();
		for(auto i = this->Children().begin(); i != this->Children().end(); ++i, ++info){
			Vec2r newSize(info->dim);
			
			if(netWeight > 0){
				newSize[longIndex] += (info->weight / netWeight) * flexible;
			}

			Vec2r newPos;
			if((this->IsVertical() && !this->IsReverse()) || (!this->IsVertical() && this->IsReverse())){
				newPos[longIndex] = this->Rect().d[longIndex] - pos - info->margin - newSize[longIndex];
			}else{
				newPos[longIndex] = pos + info->margin;
			}
			pos += info->margin + newSize[longIndex];

			(*i)->Resize(RoundVec(newSize));
			
			newPos[transIndex] = info->gravity.PosForRect(*this, (*i)->Rect().d)[transIndex];
			
			(*i)->MoveTo(newPos);
		}
	}
}



//override
morda::Vec2r LinearContainer::ComputeMinDim()const NOEXCEPT{
	unsigned longIndex = this->GetLongIndex();
	unsigned transIndex = this->GetTransIndex();
	
	morda::Vec2r minDim(0);
	
	float prevMargin = 0;
	
	for(Widget::T_ChildrenList::const_iterator i = this->Children().begin(); i != this->Children().end(); ++i){
		LeftBottomRightTop margins = LeftBottomRightTop::Default();
		morda::Vec2r dim = (*i)->GetMinDim();
		if((*i)->Prop()){
			if(const stob::Node* layout = (*i)->Prop()->Child(Container::D_Layout()).node()){
				if(const stob::Node* m = layout->Child(D_Margins).node()){
					margins = LeftBottomRightTop::FromSTOB(*m);
				}
				
				{
					LayoutDim ld = LayoutDim::FromLayout(*layout);
					
					//FRACTION min size is 0
					for(unsigned j = 0; j != 2; ++j){
						if(ld[j].unit == LayoutDim::FRACTION){
							ld[j].unit = LayoutDim::PIXEL;
							ld[j].value = 0;
						}
					}
					
					dim = (*i)->Measure(ld.ForWidget(*this, *(*i)));
				}
			}
		}
		
		if(minDim[transIndex] < dim[transIndex]){
			minDim[transIndex] = dim[transIndex];
		}
		minDim[longIndex] += dim[longIndex];
		
		//margin works for non-first children only
		if(i != this->Children().begin()){//if not first child
			minDim[longIndex] += std::max(
					prevMargin,
					margins[this->IsReverse() ? (longIndex + 2) : longIndex]
				);
		}
		
		prevMargin = margins[this->IsReverse() ? longIndex : (longIndex + 2)];
	}
	
	minDim[0] += this->Padding()[0] + this->Padding()[2];
	minDim[1] += this->Padding()[1] + this->Padding()[3];
	
	return minDim;
}


