#pragma once

#include <morda/render/ShaderColor.hpp>
#include "OpenGL2ShaderBase.hpp"

namespace mordaren{	

class OpenGL2ShaderColor :
		public morda::ShaderColor,
		public OpenGL2ShaderBase
{
	GLint colorUniform;
public:
	OpenGL2ShaderColor();
	
	OpenGL2ShaderColor(const OpenGL2ShaderColor&) = delete;
	OpenGL2ShaderColor& operator=(const OpenGL2ShaderColor&) = delete;
	

	void render(const r4::mat4f& m, const morda::VertexArray& va, r4::vec4f color)const override;
};

}
