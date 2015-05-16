#pragma once
#include "TextureData.h"

namespace Bitz
{
	namespace Content
	{
		class ContentManager
		{
		public:
			static void Init(std::wstring graphicsRoot, std::wstring audioRoot, std::wstring dataRoot);
			static void UnloadUnused();
			static std::wstring GetGraphicsRoot();
			static std::wstring GetAudioRoot();
			static std::wstring GetDataRoot();

			template <class T>
			static T* Load(std::wstring fileName);

		private:
			static std::wstring _GraphicsRoot;
			static std::wstring _AudioRoot;
			static std::wstring _DataRoot;
			static bool _HasInit;
		};
	}
}