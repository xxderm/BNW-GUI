#ifndef BNW_GUI_H
#define BNW_GUI_H

#include <BnwGUI/UI/WindowElement.h>
#include <GL/glew.h>
#include  <gl/GL.h>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtc/matrix_transform.inl>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>
#include <string>
#include <freetype/freetype.h>
#include <map>
#include <SDL.h>
#include <codecvt>
#include <cstdlib>
#include <sstream>

namespace BnwGUI
{

	enum Event
	{
		MOUSE_LEFT_PRESS,
		MOUSE_RIGHT_PRESS,
		MOUSE_LEFT_RELEASE,
		MOUSE_RIGHT_RELEASE
	};	

	template<class T>
	T* GetElement(UI::BaseElement* base)
	{
		return static_cast<T*>(base);
	}

	class EventListener
	{
	public:

		virtual void HandleEvent(std::string, Event e) = 0;

	};	

	struct Character final
	{
		GLuint Texture;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		GLuint Advance;
		FT_BBox BBox;
		FT_UShort units_per_EM;
		FT_Short Ascent;
		FT_Short Descent;
	};

	class Render final
	{
		std::string VertexShaderUI = {
			"#version 450\n"
			"layout(location = 0) in vec2 position;\n"
			"layout(location = 1) in vec2 texCoord;\n"
			"out vec2 textureCoords;\n"
			"uniform mat4 transformationMatrix;\n"
			"uniform mat4 mvp;\n"
			"void main(void)\n"
			"{\n"
			"gl_Position = transformationMatrix * vec4(position, 0.0, 1.0);\n"
			"textureCoords = vec2((position.x + 1.0) / 2.0, 1 - (position.y + 1.0) / 2.0);\n"
			"}"
		};
		std::string FragmentShaderUI = {
			"#version 450\n"
			"in vec2 textureCoords;\n"
			"out vec4 out_Color;\n"
			"uniform sampler2D guiTexture;\n"
			"uniform vec4 guiColor;\n"
			"uniform bool TextureMode = false;\n"
			"uniform float HighlightIntensity = 1.0;\n"
			"void main()\n"
			"{\n"
			"if(TextureMode)\n"
			"out_Color = texture(guiTexture,textureCoords) * HighlightIntensity;\n"
			"else\n"
			"out_Color = guiColor;\n"
			"}"
		};
		std::string VertexShaderFont = {
			"#version 410 core\n"
			"layout (location = 0) in vec4 vertex;\n"
			"out vec2 TexCoords;\n"
			"uniform mat4 transform;\n"
			"void main()\n"
			"{\n"
			"gl_Position = transform * vec4(vertex.xy, 0.0, 1.0);\n"
			"TexCoords = vertex.zw;\n"
			"}"
		};
		std::string FragmentShaderFont = {
			"#version 330 core\n"
			"in vec2 TexCoords;\n"
			"out vec4 color;\n"
			"uniform sampler2D text;\n"
			"uniform vec4 textColor;\n"
			"const float width = 0.8;\n"
			"const float edge = 0.19;\n"
			"void main()\n"
			"{\n"
			"//float distance = 1.0 - texture(text, TexCoords).a;\n"
			"//float alpha = 1.0 - smoothstep(width, width + edge, distance);\n"			
			"//color = vec4(textColor.rgb, alpha);\n"	
			"color = vec4(textColor.rgb, texture(text, TexCoords).a);\n"
			"}"
		};
		GLuint VertexArray;
		GLuint VertexBuffer[2];
		GLuint ShaderUI;
		GLuint ShaderFont;
		GLuint TextVBO;
		GLuint TextVAO;
		std::map<char32_t, Character> mCharacters;
		glm::mat4x4 TextProjection;
		
	private:

		int MaxFontSize;

		void CreateShader();

		glm::mat4 TransMat(glm::vec2 trans, glm::vec2 scale);

		void AddUnicodeRange(FT_Face& face, unsigned int begin, unsigned int end);

	public:

		const int GetMaxFontSize() const;
		
		glm::vec2 GetTextSize(std::wstring Text, float Scale, bool maxHeightSym, float* textHeight = nullptr);

		void Init(const char* fontPath, glm::vec2 screenSize, GLuint maxFontSize = 72);

		void Resize(glm::vec2 screenSize);

		void RenderRect(glm::vec2 pos, glm::vec2 scale,
			glm::vec4 color = glm::vec4(0.8f), int texture = -1);

		int RenderText(std::wstring Text, glm::vec4 Color, float Scale, int x, int y);

		int RenderText(std::wstring Text, glm::vec4 Color, float Scale, glm::vec2 pos);

	};
	
	class GuiManager final
	{
		Render *Renderer;
		std::vector<UI::BaseElement *> Elements;
		std::vector<EventListener *> EventNotify;
		glm::vec2 ScreenSize;
	public:

		void Init(const char* fontPath, glm::vec2 screenSize, GLuint maxFontSize = 72);

		void Render();

		void Resize(glm::vec2 ss);

		void HandleEvent(SDL_Event* e);

		void Subscribe(EventListener* listener);

		void operator+=(UI::BaseElement*element);

		~GuiManager();

	private:
		void Update();

	};	

}

#endif // !BNW_GUI_H
