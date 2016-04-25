#include "../../Common.h"
#include "Model.h"
#include "../../Math/Math.h"

namespace Bitz
{
	using namespace Math;

	namespace GFX
	{
		namespace Drawables
		{
			const int32_t DIMENTIONS = 3;

			Model::Model()
			{
				_RenderMode = ThreeD;
				_Position = Vector3F(0);
				_Size = Vector3F(1);
				SetColour(Vector4F(1.0f, 1.0f, 1.0f, 1.0f));
			}

			Model::~Model()
			{
			}

			uint32_t Model::GetVertCount() const
			{
				return _VertCount;
			}

			void Model::PopulateVertArray(float_t * vertArray, int32_t * startPosition)
			{
				assert(_VertArray != nullptr);
				int size = _VertCount * DIMENTIONS * sizeof(float_t);
				Memcpy(vertArray + (*startPosition), size, _VertArray, size);
				(*startPosition) += (_VertCount * DIMENTIONS);
			}

			void Model::PopulateColArray(float_t* colArray, int32_t* startPosition)
			{
				assert(_ColArray != nullptr);
				int size = _VertCount * 4 * sizeof(float_t);
				Memcpy(colArray + (*startPosition), size, _ColArray, size);
				(*startPosition) += (_VertCount * 4);
			}

			void Model::PopulateTexArray(float_t* texArray, int32_t* startPosition)
			{
				assert(_TexArray != nullptr);
				int size = _VertCount * 2 * sizeof(float);
				Memcpy(texArray + (*startPosition), size, _TexArray, size);
				(*startPosition) += (_VertCount * 2);
			}

			void Model::PopulateNormArray(float_t* normArray, int32_t* startPosition)
			{
				assert(_NormArray != nullptr);
				int size = _VertCount * 3 * sizeof(float);
				Memcpy(normArray + (*startPosition), size, _NormArray, size);
				(*startPosition) += (_VertCount * 3);
			}

			void Model::SetNormalTexture(const Texture_Ptr newTexture)
			{
				_NormalTexture = newTexture;
			}

			void Model::SetSpecularTexture(const Texture_Ptr newTexture)
			{
				_SpecularTexture = newTexture;
			}

			Texture_Ptr Model::GetNormalTexture() const
			{
				return _NormalTexture;
			}

			Texture_Ptr Model::GetSpecularTexture() const
			{
				return _SpecularTexture;
			}

			void Model::SetVertArray(const float_t * verts, const  int32_t count)
			{
				_VertCount = count;
				int32_t dataSize = _VertCount * DIMENTIONS;

				if (_VertArray != nullptr)delete[] _VertArray;

				_VertArray = new float[dataSize];

				if (_NormArray == nullptr)
				{
					_NormArray = new float[dataSize] {0};
				}

				if (_TexArray == nullptr)
				{
					_TexArray = new float[_VertCount * 2]{ 0 };
				}
				SetColour(_Colour);
				Memcpy(_VertArray, dataSize*sizeof(float_t), verts, dataSize*sizeof(float_t));
			}

			void Model::SetNormalArray(const float_t * norms, const  int32_t count)
			{
				assert(_VertCount == count && "Number of normals set must not be less than or exceed the number of verts present in the model");
				int32_t dataSize = count * DIMENTIONS;

				if (_NormArray != nullptr)delete[] _NormArray;

				_NormArray = new float[dataSize];
				
				Memcpy(_NormArray, dataSize*sizeof(float_t), norms, dataSize*sizeof(float_t));
			}

			void Model::SetTexCordArray(const float_t * texcords, const  int32_t count)
			{
				assert(_VertCount == count && "Number of TexCorrds set must not be less than or exceed the number of verts present in the model");
				int32_t dataSize = count * 2;

				if (_TexArray != nullptr)delete[] _TexArray;

				_TexArray = new float[dataSize];

				Memcpy(_TexArray, dataSize*sizeof(float_t), texcords, dataSize*sizeof(float_t));
			}

			void Model::SetColourArray(const float_t * colours, const  int32_t count)
			{
				assert(_VertCount == count && "Number of Colours set must not be less than or exceed the number of verts present in the model");
				int32_t dataSize = count * 4;

				if (_ColArray != nullptr)delete[] _ColArray;

				_ColArray = new float[dataSize];

				Memcpy(_ColArray, dataSize*sizeof(float_t), colours, dataSize*sizeof(float_t));
			}

			void Model::SetColour(const Vector4F newColour)
			{
				if (_VertCount == 0)return;
				bool arrayUpdated = false;
				if (_ColArray == nullptr)
				{
					_ColArray = new float_t[12 * _VertCount];
					arrayUpdated = true;
				}
				else if (_ColArray != nullptr)
				{
					delete _ColArray;
					_ColArray = new float_t[12 * _VertCount];
					arrayUpdated = true;
				}

				if (_Colour == newColour && !arrayUpdated)return;

				_Colour = newColour;
				float_t tempArray[12] = { _Colour.X ,_Colour.Y,_Colour.Z,_Colour.W ,_Colour.X ,_Colour.Y,_Colour.Z,_Colour.W ,_Colour.X ,_Colour.Y,_Colour.Z,_Colour.W};
				for (auto i = 0; i < _VertCount; i++)
				{
					Memcpy(&_ColArray[12 * i], 12 * sizeof(float_t), tempArray, 12 * sizeof(float_t));
				}
			}

			void Model::SetAlpha(const float newAlpha)
			{
				_Colour.W = newAlpha;
			}

			glm::mat4 Model::GetTransformation() const
			{
				glm::mat4 matrix;

				if (_Rotation.X != 0)matrix = glm::rotate(matrix, _Rotation.X, glm::vec3(1, 0, 0));
				if (_Rotation.Y != 0)matrix = glm::rotate(matrix, _Rotation.Y, glm::vec3(0, 1, 0));
				if (_Rotation.Z != 0)matrix = glm::rotate(matrix, _Rotation.Z, glm::vec3(0, 0, 1));
				matrix = glm::translate(matrix, glm::vec3(-_Position.X, -_Position.Y, -_Position.Z));
				matrix = glm::scale(matrix, glm::vec3(_Size.X, _Size.Y, _Size.Z));
				return matrix;
			}
		}
	}
}