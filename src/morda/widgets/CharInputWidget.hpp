#pragma once

#include <utki/Buf.hpp>

#include "widget.hpp"

#include "../util/key.hpp"

namespace morda{

/**
 * @brief Base class for widgets which want to get character input events.
 * When the widget has keyboard focus it will also get character input events.
 */
class CharInputWidget : virtual public widget{

protected:
	CharInputWidget() :
			widget(puu::forest())
	{}

public:
	/**
	 * @brief Handler for character input.
     * @param unicode - unicode string of entered characters in UTF-32.
	 * @param key - key code associated with character input, can be UNKNOWN.
     */
	virtual void onCharacterInput(const std::u32string& unicode, key key) = 0;

	virtual ~CharInputWidget()noexcept{}
};


}
