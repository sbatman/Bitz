#include "../../Common.h"
#include "../../Math/Math.h"
#include "Sprite.h"

namespace Bitz
{
	using namespace Math;

	namespace GFX
	{
		namespace Drawables
		{

			const int32_t DIMENTIONS = 3;
			const float_t QUADDEFINITION[]{ 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 1, 0 };
			const uint32_t VERTCOUNT = 6;

			Sprite::Sprite()
			{
				_RenderMode = TwoD;
				FlagQuadDirty();
				_VertCount = 6;
				_VertArray = new float_t[_VertCount*DIMENTIONS];
				_TexArray = new float_t[_VertCount * 2];
				_Position = Vector2F(0);
				_Rotation = 0;
				_Size = Vector2F(1);
			}

			Sprite::~Sprite()
			{

			}

			void Sprite::SetPosition(const Vector2F newPosition)
			{
				Bitz::Interfaces::IPositionable2DF::SetPosition(newPosition);
				FlagQuadDirty();
			}

			void Sprite::SetPosition(const float x, const float y)
			{
				this->_Position.X = x;
				this->_Position.Y = y;
				FlagQuadDirty();
			}

			void Sprite::SetSize(const Vector2F newSize)
			{
				Bitz::Interfaces::ISizeable2DF::SetSize(newSize);
				FlagQuadDirty();
			}

			void Sprite::SetTexture(Texture* texture)
			{
				IDrawable::SetTexture(texture);
				if (_Texture != nullptr)
				{
					_TexArray[0] = _Texture->_UV_TL.X;
					_TexArray[1] = _Texture->_UV_TL.Y;
					_TexArray[2] = _Texture->_UV_TR.X;
					_TexArray[3] = _Texture->_UV_TR.Y;
					_TexArray[4] = _Texture->_UV_BR.X;
					_TexArray[5] = _Texture->_UV_BR.Y;
					_TexArray[6] = _Texture->_UV_TL.X;
					_TexArray[7] = _Texture->_UV_TL.Y;
					_TexArray[8] = _Texture->_UV_BR.X;
					_TexArray[9] = _Texture->_UV_BR.Y;
					_TexArray[10] = _Texture->_UV_BL.X;
					_TexArray[11] = _Texture->_UV_BL.Y;
				}
			}

			void Sprite::SetRotation(const float_t newRotation)
			{
				Bitz::Interfaces::IRotateable2DF::SetRotation(newRotation);
				FlagQuadDirty();
			}

			void Sprite::PopulateVertArray(float_t * vertArray, int32_t * startPosition)
			{
				if (_QuadDirty)UpdateQuad();

				uint32_t dataLength = _VertCount * DIMENTIONS * sizeof(float_t);

				memcpy_s(vertArray + (*startPosition), dataLength, _VertArray, dataLength);
				(*startPosition) += _VertCount * DIMENTIONS;
			}

			void Sprite::PopulateTexArray(float_t * texArray, int32_t * startPosition)
			{
				if (_QuadDirty)UpdateQuad();

				uint32_t dataLength = _VertCount * 2 * sizeof(float_t);

				memcpy_s(texArray + (*startPosition), dataLength, _TexArray, dataLength);
				(*startPosition) += _VertCount * 2;
			}

			void Sprite::PopulateColArray(float_t* colArray, int32_t* startPosition)
			{
				for (auto i = 0; i < _VertCount * 4; i += 4)
				{
					colArray[(*startPosition)++] = _Colour.X;
					colArray[(*startPosition)++] = _Colour.Y;
					colArray[(*startPosition)++] = _Colour.Z;
					colArray[(*startPosition)++] = _Colour.W;
				}
			}

			void Sprite::PopulateNormArray(float_t * normArray, int32_t * startPosition)
			{
				return;
			}

			void Sprite::UpdateQuad()
			{
				memcpy_s(_VertArray, _VertCount*DIMENTIONS*sizeof(uint32_t), QUADDEFINITION, _VertCount*DIMENTIONS*sizeof(uint32_t));

				if (abs(_Rotation) <= 0.001f)
				{
					for (auto i = 0; i < _VertCount*DIMENTIONS; i += 3)
					{
						_VertArray[i] *= _Size.X;
						_VertArray[i + 1] *= _Size.Y;
						_VertArray[i] += _Position.X;
						_VertArray[i + 1] += _Position.Y;
					}
				}
				else
				{
					glm::vec4 topLeft(_VertArray[0], _VertArray[1], _VertArray[2], 1);
					glm::vec4 topRight(_VertArray[3], _VertArray[4], _VertArray[5], 1);
					glm::vec4 bottomRight(_VertArray[6], _VertArray[7], _VertArray[8], 1);
					glm::vec4 bottomLeft(_VertArray[15], _VertArray[16], _VertArray[17], 1);

					glm::mat4 transformMatrix = glm::mat4(1.0f);
					transformMatrix = glm::translate(transformMatrix, glm::vec3(_Position.X, _Position.Y, 0.0f));
					transformMatrix = glm::translate(transformMatrix, glm::vec3(_Size.X *0.5f, _Size.X*0.5f, 0.0f));
					transformMatrix = glm::rotate(transformMatrix, _Rotation, glm::vec3(0.0f, 0.0f, 1.0f));
					transformMatrix = glm::translate(transformMatrix, glm::vec3(_Size.X *-0.5f, _Size.X*-0.5f, 0.0f));
					transformMatrix = glm::scale(transformMatrix, glm::vec3(_Size.X, _Size.Y, 1.0f));

					topLeft = transformMatrix* topLeft;
					topRight = transformMatrix*topRight;
					bottomRight = transformMatrix*bottomRight;
					bottomLeft = transformMatrix*bottomLeft;

					delete _VertArray;

					_VertArray = new float_t[18]
					{
						topLeft.x, topLeft.y, topLeft.z,
							topRight.x, topRight.y, topRight.z,
							bottomRight.x, bottomRight.y, bottomRight.z,
							topLeft.x, topLeft.y, topLeft.z,
							bottomRight.x, bottomRight.y, bottomRight.z,
							bottomLeft.x, bottomLeft.y, bottomLeft.z
					};
				}

				_QuadDirty = false;
			}

			void Sprite::FlagQuadDirty()
			{
				_QuadDirty = true;
			}

			uint32_t Sprite::GetVertCount() const
			{
				return _VertCount;
			}
		}
	}
}