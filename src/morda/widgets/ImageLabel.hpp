/* The MIT License:

Copyright (c) 2014 Ivan Gagis

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE. */

// Home page: http://morda.googlecode.com

/**
 * @author Ivan Gagis <igagis@gmail.com>
 */


#pragma once

#include "Widget.hpp"
#include "../resources/ResTexture.hpp"


namespace morda{

class ImageLabel : public virtual Widget{
	ImageLabel(const ImageLabel&);
	ImageLabel& operator=(const ImageLabel&);
	
	std::shared_ptr<morda::ResTexture> tex;
	
	bool keepAspectRatio = false;
	
public:
	ImageLabel();
	
	ImageLabel(const stob::Node& desc);
public:
	virtual ~ImageLabel()noexcept{}
	
	void Render(const morda::Matr4f& matrix)const override;

	morda::Vec2f ComputeMinDim()const override;
	
	morda::Vec2f Measure(const Vec2f& offer)const override;
	
private:

};

}
