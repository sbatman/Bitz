#pragma once
#include "TextureData.h"

namespace Bitz
{
	namespace Content
	{
		class ContentManager
		{
		public:
			static void Init(std::string graphicsRoot, std::string audioRoot, std::string dataRoot);
			static void UnloadUnused();
			static std::string GetGraphicsRoot();
			static std::string GetAudioRoot();
			static std::string GetDataRoot();

			template <class T>
			static T* Load(std::string fileName);

		private:
			static std::string _GraphicsRoot;
			static std::string _AudioRoot;
			static std::string _DataRoot;
			static bool _HasInit;
		};
	}
}