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
				int size = _VertCount * DIMENTIONS * sizeof(float);
				Memcpy(vertArray + (*startPosition), size, _VertArray, size);
				(*startPosition) += (_VertCount * DIMENTIONS);
			}

			void Model::PopulateColArray(float_t* colArray, int32_t* startPosition)
			{
				for (auto i = 0; i < (_VertCount * 4); i += 4)
				{
					colArray[(*startPosition)++] = _Colour.X;
					colArray[(*startPosition)++] = _Colour.Y;
					colArray[(*startPosition)++] = _Colour.Z;
					colArray[(*startPosition)++] = _Colour.W;
				}
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

			void Model::SetVerts(const float_t * verts, const  int32_t count)
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

				Memcpy(_VertArray, dataSize*sizeof(float_t), verts, dataSize*sizeof(float_t));
			}

			void Model::SetNormals(const float_t * norms, const  int32_t count)
			{
				assert(_VertCount == count && "Number of normals set must not be less than or exceed the number of verts present in the model");
				int32_t dataSize = count * DIMENTIONS;

				if (_NormArray != nullptr)delete[] _NormArray;

				_NormArray = new float[dataSize];

				Memcpy(_NormArray, dataSize*sizeof(float_t), norms, dataSize*sizeof(float_t));
			}

			void Model::SetTexCords(const float_t * texcords, const  int32_t count)
			{
				assert(_VertCount == count && "Number of TexCorrds set must not be less than or exceed the number of verts present in the model");
				int32_t dataSize = count * 2;

				if (_TexArray != nullptr)delete[] _TexArray;

				_TexArray = new float[dataSize];

				Memcpy(_TexArray, dataSize*sizeof(float_t), texcords, dataSize*sizeof(float_t));
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