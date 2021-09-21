#ifndef BASE_ELEMENT_H
#define BASE_ELEMENT_H

#include <vector>
#include <glm/glm.hpp>
#include <functional>
#include <SDL.h>

namespace BnwGUI
{

	class Render;

	namespace UI
	{

		enum Anchor : int
		{
			LEFT,
			RIGHT,
			TOP,
			BOTTOM,
			TOP_LEFT,
			TOP_RIGHT,
			TOP_CENTER,
			BOTTOM_LEFT,
			BOTTOM_RIGHT,
			BOTTOM_CENTER,
			CENTER,
			CENTER_LEFT,
			CENTER_RIGHT
		};

		enum EventType : int
		{
			ON_CLICK_BUTTON,
			ON_VOID
		};

		enum ElementParameter : int
		{
			WITH_TITLE = 0,
			WITH_EXIT_BUTTON,
			UNTITLED_NO_EXIT_BUTTON,
			TITLE_WITH_EXIT_BUTTON
		};

		class WindowElement;		

		class BaseElement 
		{			
		protected:
			BaseElement *Parent = nullptr;
			Render *Renderer;
			bool Active = false;
			glm::vec2 Size = { 0.1, 0.05 };
			glm::vec2 Position = { 0, 0 };
			glm::vec2 ScreenSize;
		protected:
			std::function<void(void)> OnClickFunc;
		public:

			const glm::vec2 GetNormalizedDeviceCoords(int mouseX, int mouseY, glm::vec2 ScrSize)
			{
				float x = (2.0f * mouseX) / ScrSize.x - 1.f;
				float y = 1.f - (2.0f * mouseY) / ScrSize.y;
				return glm::vec2(x, y);
			}

			/**
			* @return Range [0, ScrSize]
			*/
			const glm::vec2 NormalizedDevCoordsToWindowsCoords(float x, float y, glm::vec2 SrcSize)
			{
				int xwin = (x + 1) * SrcSize.x * 0.5 + (-1);
				int ywin = (y + 1) * SrcSize.y * 0.5 + (-1);
				return glm::vec2(xwin, ywin);
			}

			const bool Hover(glm::vec2 mouse, glm::vec2 pos);

			const glm::vec2 GetResultPos();

			virtual void Render() = 0;		

			virtual EventType HandleEvent(SDL_Event* e);

			virtual void SetScreenSize(glm::vec2 ss) { ScreenSize = ss; }

			void BeginScissor(const glm::vec2& pos, const glm::vec2& size);

			void EndScissor();

			void SetRenderer(BnwGUI::Render *Renderer);

			void SetParent(BaseElement*parent);

			void SetPosition(glm::vec2 pos) { Position = pos; }

			void SetSize(glm::vec2 size) { Size = size; }

			void OnClickEvent(std::function<void(void)> func);

		};

	}
}

#endif // !BASE_ELEMENT_H