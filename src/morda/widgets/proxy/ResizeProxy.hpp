#pragma once

#include "../widget.hpp"


namespace morda{

/**
 * @brief Widget for intercepting resize events.
 * From GUI script it can be instantiated as "ResizeProxy".
 */
class ResizeProxy : virtual public Widget{
public:
	ResizeProxy(const puu::forest& desc) :
			widget(desc)
	{}

	ResizeProxy(const ResizeProxy&) = delete;
	ResizeProxy& operator=(const ResizeProxy&) = delete;

	void on_resize() override;

	/**
	 * @brief Resize event signal.
	 * Emitted when this widget is resized.
	 */
	std::function<void(const Vec2r&)> resized;
};

}
