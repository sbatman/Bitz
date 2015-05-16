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
				delete[] _VertArray;
			}

			uint32_t Model::GetVertCount() const
			{
				return _VertCount;
			}

			void Model::PopulateVertArray(float_t * vertArray, int32_t * startPosition)
			{
				_ASSERT(_VertArray != nullptr);
				int size = _VertCount * DIMENTIONS * sizeof(float);
				memcpy_s(vertArray, size, _VertArray, size);
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
				_ASSERT(_TexArray != nullptr);
				int size = _VertCount * 2 * sizeof(float);
				memcpy_s(texArray, size, _TexArray, size);
				(*startPosition) += (_VertCount * 2);
			}

			void Model::PopulateNormArray(float_t* normArray, int32_t* startPosition)
			{
				_ASSERT(_NormArray != nullptr);
				int size = _VertCount * 3 * sizeof(float);
				memcpy_s(normArray, size, _NormArray, size);
				(*startPosition) += (_VertCount * 3);
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

				memcpy_s(_VertArray, dataSize*sizeof(float_t), verts, dataSize*sizeof(float_t));
			}

			void Model::SetNormals(const float_t * norms, const  int32_t count)
			{
				_ASSERT_EXPR(_VertCount == count, "Number of normals set must not be less than or exceed the number of verts present in the model");
				int32_t dataSize = count * DIMENTIONS;

				if (_NormArray != nullptr)delete[] _NormArray;

				_NormArray = new float[dataSize];

				memcpy_s(_NormArray, dataSize*sizeof(float_t), norms, dataSize*sizeof(float_t));
			}

			void Model::SetTexCords(const float_t * texcords, const  int32_t count)
			{
				_ASSERT_EXPR(_VertCount == count, "Number of TexCorrds set must not be less than or exceed the number of verts present in the model");
				int32_t dataSize = count * 2;

				if (_TexArray != nullptr)delete[] _TexArray;

				_TexArray = new float[dataSize];

				memcpy_s(_TexArray, dataSize*sizeof(float_t), texcords, dataSize*sizeof(float_t));
			}

			void Model::ApplyTransformation()
			{
				if (_Rotation.X != 0)glRotatef(_Rotation.X, 1, 0, 0);
				if (_Rotation.Y != 0)glRotatef(_Rotation.Y, 0, 1, 0);
				if (_Rotation.Z != 0)glRotatef(_Rotation.Z, 0, 0, 1);

				glTranslatef(_Position.X, _Position.Y, _Position.Z);
			}
		}
	}
}