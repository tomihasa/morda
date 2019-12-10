#include "../../Morda.hpp"

#include "../../util/util.hpp"
#include "CollapseArea.hpp"
#include "../button/ToggleButton.hpp"

using namespace morda;

namespace{
const char* layout_c = R"qwertyuiop(
	Pile{
		layout{dx{max}}
		Color{
			layout{dx{fill}dy{fill}}
			color{@{morda_color_mg}}
		}
		Margins{
			layout{dx{max}dy{max}}

			defs{
				marHor{4dp}
				marVer{1dp}
			}
			left{@{marHor}}
			top{@{marVer}}
			right{@{marHor}}
			bottom{@{marVer}}

			Row{
				layout{dx{max}}
				ImageToggle{
					id{switch}
					look{
						unpressed{morda_img_dropdown_arrow}
						pressed{morda_img_dropright_arrow}
					}
				}
				Widget{
					layout{dx{@{marHor}}}
				}
				Pile{
					id{title}
				}
			}
		}
	}
	Pile{
		id{content}
	}
)qwertyuiop";
}

CollapseArea::CollapseArea(const stob::Node* chain) :
		Widget(chain),
		Column(stob::parse(layout_c).get())
{
	this->contentArea = this->try_get_widget_as<Pile>("content");
	ASSERT(this->contentArea)

	this->title_v = this->try_get_widget_as<Pile>("title");
	ASSERT(this->title_v)

	if(auto p = getProperty(chain, "title")){
		this->title_v->add(*p);
	}

	if(chain){
		this->contentArea->add(*chain);
	}

	{
		auto sw = this->try_get_widget_as<ToggleButton>("switch");
		ASSERT(sw)
		sw->pressedChanged = [this](Button& tb){
			if(tb.isPressed()){
				this->contentArea->get_layout_params().dim.y = 0;
			}else{
				this->contentArea->get_layout_params().dim.y = Widget::LayoutParams::min_c;
			}
		};
	}
}
