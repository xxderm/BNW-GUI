#include <BnwGUI/BnwGUI.h>

namespace BnwGUI
{

	void GuiManager::Init(const char* fontPath, glm::vec2 screenSize, GLuint maxFontSize)
	{
		ScreenSize = screenSize;
		Renderer = new BnwGUI::Render();
		Renderer->Init(fontPath, screenSize, maxFontSize);
	}

	void GuiManager::Render()
	{		
		//this->Update();
		for (auto& element : Elements)
		{
			element->Render();
		}
	}

	void GuiManager::Resize(glm::vec2 ss)
	{
		ScreenSize = ss;
		Renderer->Resize(ss);
		for (auto& element : Elements)
		{
			element->SetScreenSize(ss);
		}
	}

	void GuiManager::HandleEvent(SDL_Event* e)
	{
		for (auto& element : Elements)
		{
			element->HandleEvent(e);
		}
	}

	void GuiManager::Subscribe(EventListener* listener)
	{
		EventNotify.push_back(listener);
	}

	void GuiManager::operator+=(UI::BaseElement* element)
	{
		this->Elements.push_back(element);
		this->Elements.back()->SetRenderer(this->Renderer);
		this->Elements.back()->SetScreenSize(ScreenSize);
	}

	GuiManager::~GuiManager()
	{
		delete Renderer;
		for (auto& element : Elements)
		{
			delete element;
			element = nullptr;
		}
	}

	void GuiManager::Update()
	{
		for (auto& listener : EventNotify)
		{
		}
	}

	void Render::CreateShader()
	{
		unsigned int VertexUI, FragmentUI;
		const char* VertexShaderCodeUI = VertexShaderUI.c_str();
		const char* FragmentShaderCodeUI = FragmentShaderUI.c_str();
		const char* VertexShaderCodeFont = VertexShaderFont.c_str();
		const char* FragmentShaderCodeFont = FragmentShaderFont.c_str();
		unsigned int VertexFont, FragmentFont;

		VertexUI = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexUI, 1,  &VertexShaderCodeUI, NULL);
		glCompileShader(VertexUI);
		
		FragmentUI = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragmentUI, 1, &FragmentShaderCodeUI, NULL);
		glCompileShader(FragmentUI);

		ShaderUI = glCreateProgram();
		glAttachShader(ShaderUI, VertexUI);
		glAttachShader(ShaderUI, FragmentUI);
		glLinkProgram(ShaderUI);
		glDeleteShader(VertexUI);
		glDeleteShader(FragmentUI);

		VertexFont = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(VertexFont, 1, &VertexShaderCodeFont, NULL);
		glCompileShader(VertexFont);

		FragmentFont = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(FragmentFont, 1, &FragmentShaderCodeFont, NULL);
		glCompileShader(FragmentFont);

		ShaderFont = glCreateProgram();
		glAttachShader(ShaderFont, VertexFont);
		glAttachShader(ShaderFont, FragmentFont);
		glLinkProgram(ShaderFont);
		glDeleteShader(VertexFont);
		glDeleteShader(FragmentFont);
	}

	glm::mat4 Render::TransMat(glm::vec2 trans, glm::vec2 scale)
	{
		glm::mat4 matrix = glm::identity<glm::mat4>();
		matrix *= glm::translate(glm::vec3(trans.x, trans.y, 0));
		matrix *= glm::scale(glm::vec3(scale.x, scale.y, 1.f));
		return matrix;
	}

	void Render::AddUnicodeRange(FT_Face& face, unsigned int begin, unsigned int end)
	{
		for (unsigned long c = begin; c <= end; c++)
		{
			if (FT_Load_Char(face, c, FT_LOAD_DEFAULT | FT_LOAD_IGNORE_TRANSFORM | FT_LOAD_RENDER | FT_LOAD_FORCE_AUTOHINT))
			{
				printf("ERROR::FREETYPE: Failed to load Glyph");
				continue;
			}
			GLuint texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_ALPHA8,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_ALPHA,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			glGenerateMipmap(GL_TEXTURE_2D);
			//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_LOD_BIAS, 4);
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<GLuint>(face->glyph->advance.x),
				face->bbox,
				face->units_per_EM,
				face->ascender,
				face->descender
			};
			
			mCharacters.insert(std::pair<char32_t, Character>(c, character));
		}
	}

	const int Render::GetMaxFontSize() const
	{
		return MaxFontSize;
	}

	glm::vec2 Render::GetTextSize(std::wstring Text, float Scale, bool maxHeightSym, float* textHeight)
	{
		glm::ivec2 textSize = glm::vec2(0.);
		std::wstring::const_iterator c;
		Character* ch;
		int lines = 1;
		int currentWidth = 0;
		int maxWidth = 0;
		int next;

		for (c = Text.begin(); c != Text.end(); ++c)
		{		
			ch = &mCharacters[*c];
			
			if (*c == '\n')
			{
				++lines;
				if (currentWidth > maxWidth)
				{
					maxWidth = currentWidth;
					currentWidth = 0;
				}
				continue;
			}

			next = static_cast<int>((ch->Advance >> 6) * Scale);
			currentWidth += next;

			if (textSize.y == 0)
				textSize.y = static_cast<int>(ch->Size.y * Scale);

		}

		if (currentWidth > maxWidth)
			maxWidth = currentWidth;

		textSize.x = maxWidth;
		textSize.y *= lines;
		return textSize;
	}

	void Render::Init(const char* fontPath, glm::vec2 screenSize, GLuint fontSize)
	{
		MaxFontSize = fontSize;
		TextProjection = glm::ortho(0.0f, static_cast<GLfloat>(screenSize.x), 0.0f, static_cast<GLfloat>(screenSize.y));
		CreateShader();

		float Vertices[] = { -1, 1, -1, -1, 1, 1, 1, -1 };
		int Indices[] = { 0, 1, 2, 3, 4, 5, 6, 7 };
		
		glGenVertexArrays(1, &VertexArray);
		glBindVertexArray(VertexArray);

		glGenBuffers(2, VertexBuffer);
		
		glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer[0]);
		glBufferData(GL_ARRAY_BUFFER, 4 * 8, Vertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexBuffer[1]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, 4 * 8, Indices, GL_STATIC_DRAW);

		glEnable(GL_CULL_FACE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		FT_Face face;
		FT_Library ft;
		if (FT_Init_FreeType(&ft))
			printf("ERROR::FREETYPE: Could not init FreeType Library");
		if (FT_New_Face(ft, fontPath, 0, &face))
			printf("ERROR::FREETYPE: Failed to load font");
		FT_Set_Char_Size(face, fontSize * 72, fontSize * 72, 0, 0);
		FT_Set_Pixel_Sizes(face, 0, fontSize);
		FT_Select_Charmap(face, FT_Encoding_::FT_ENCODING_UNICODE);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		//AddUnicodeRange(face, 0x410, 0x042F);
		AddUnicodeRange(face, 0, 256);
		AddUnicodeRange(face, 0x400, 0x500);
		//AddUnicodeRange(face, 0x0430, 0x044F);
		
		glBindTexture(GL_TEXTURE_2D, 0);
		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		glUseProgram(ShaderFont);
		glGenVertexArrays(1, &TextVAO);
		glBindVertexArray(TextVAO);
		glGenBuffers(1, &TextVBO);
		glBindBuffer(GL_ARRAY_BUFFER, TextVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	void Render::Resize(glm::vec2 screenSize)
	{
		TextProjection = glm::ortho(0.0f, static_cast<GLfloat>(screenSize.x), 0.0f, static_cast<GLfloat>(screenSize.y));
	}

	void Render::RenderRect(glm::vec2 pos, glm::vec2 scale,
		glm::vec4 color, int texture)
	{
		glUseProgram(ShaderUI);
		glUniformMatrix4fv(glGetUniformLocation(ShaderUI, "transformationMatrix"), 1, GL_FALSE, &TransMat(pos, scale)[0][0]);
		if(texture == -1)
			glUniform1i(glGetUniformLocation(ShaderUI, "TextureMode"), (int)false);
		else
			glUniform1i(glGetUniformLocation(ShaderUI, "TextureMode"), (int)true);
		glUniform1i(glGetUniformLocation(ShaderUI, "guiTexture"), texture);
		glUniform4fv(glGetUniformLocation(ShaderUI, "guiColor"), 1, &color[0]);
		glBindVertexArray(VertexArray);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VertexBuffer[1]);
		glDrawElements(GL_TRIANGLE_STRIP, 8, GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
		glUseProgram(0);
	}

	int Render::RenderText(std::wstring Text, glm::vec4 Color, float Scale, int x, int y)
	{		
		glUseProgram(ShaderFont);
		glUniform4fv(glGetUniformLocation(ShaderFont, "textColor"), 1, &Color[0]);
		glUniformMatrix4fv(glGetUniformLocation(ShaderFont, "transform"), 1, GL_FALSE, &TextProjection[0][0]);
		glActiveTexture(GL_TEXTURE1);
		glUniform1i(glGetUniformLocation(ShaderFont, "text"), 1);
		glBindVertexArray(TextVAO);

		int xBeginCoord = x;
		int xLastCoord = x;
		int stride = 0;
		std::wstring::const_iterator c;
		int start = x;
		Character* ch;
		int next;

		for (c = Text.begin(); c != Text.end(); ++c)
		{	
			ch = &mCharacters[*c];
			if (*c == '\n')
			{
				x = start;
				y -= static_cast<int>(((ch->Advance >> 6) + 15) * Scale);
				stride = 0;
				continue;
			}

			GLfloat xpos = x + ch->Bearing.x * Scale;
			GLfloat ypos = y - (ch->Size.y - ch->Bearing.y) * Scale;
			GLfloat w = ((ch->Size.x) * Scale);
			GLfloat h = ((ch->Size.y) * Scale);
			GLfloat vertices[6][4] = {
				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos, ypos, 0.0, 1.0 },
				{ xpos + w, ypos, 1.0, 1.0 },
				{ xpos, ypos + h, 0.0, 0.0 },
				{ xpos + w, ypos, 1.0, 1.0 },
				{ xpos + w, ypos + h, 1.0, 0.0 }
			};
			glBindTexture(GL_TEXTURE_2D, ch->Texture);
			glBindBuffer(GL_ARRAY_BUFFER, TextVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindTexture(GL_ARRAY_BUFFER, 0);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			next = static_cast<int>((ch->Advance >> 6) * Scale);
			x += next;
			xLastCoord = x;
			stride += next;
		}

		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glUseProgram(0);

		return xLastCoord - xBeginCoord;
	}

	int Render::RenderText(std::wstring Text, glm::vec4 Color, float Scale, glm::vec2 pos)
	{
		return RenderText(Text, Color, Scale, 
			static_cast<int>(pos.x), 
			static_cast<int>(pos.y));
	}

}