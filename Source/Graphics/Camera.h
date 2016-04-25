#pragma once
#include "../Common.h"
#include "../Math/Math.h"
#include "../Interfaces/Core.h"

namespace Bitz
{
	namespace GFX
	{
		class GraphicsManager;

		/// <summary>
		/// The camera class controls the view of the current viewport
		/// allowing it to be quickly switched as required.
		/// </summary>
		class Camera : public Bitz::Interfaces::IPositionable3DF,
			public Bitz::Interfaces::IRotateable3DF
		{
			friend class GraphicsManager;
		public:

			enum CameraMode
			{
				Perspective,
				Ortho
			};

			Camera();
			~Camera();

			/// <summary>
			/// Sets the new fov for the camera, only applicable wehen in perspective mode
			/// </summary>
			/// <param name='newPosition'>The new fov, will be clamped between 1 and 360</param>
			void SetFOV(const float_t newValue);
			/// <summary>
			/// Sets the new mode for the camera
			/// </summary>
			/// <param name='newPosition'>The new mode which will be applied on the next draw this camera is used</param>
			void SetMode(const CameraMode newMode);

			/// <summary>
			/// Gets the current fov
			/// </summary>
			float_t GetFOV() const;
			/// <summary>
			/// Gets the current camera mode
			/// </summary>
			CameraMode GetMode() const;
			/// <summary>
			/// Gets the current zoom multiplier
			/// </summary>
			float_t GetZoom() const;
			/// <summary>
			/// Gets the current cached projection matrix
			/// </summary>
			glm::mat4 GetProjectionMatrix() const;
			/// <summary>
			/// Gets the current cached view matrix
			/// </summary>
			glm::mat4 GetViewMatrix() const;

			/// <summary>
			/// Gets the current zoom level (its a scaling factor >1.0 to zoom in <1.0 to zoom out) default : 1.0f
			/// </summary>
			void SetZoom(const float_t newZoom);

		protected:
			/// <summary>
			/// Applies this cameras position and rotation from the identity to the gl_modelview
			/// </summary>
			void Apply();
			/// <summary>
			/// This logic applies when this camera whasnt the last camera used, allows the seperation of
			/// some of the apply logic to reduce opwngl calls
			/// </summary>
			void MakeActive();


		private:
			/// <summary>
			/// Used for float comparisons
			/// </summary>
			const float_t EPSILON = 0.001f;
			/// <summary>
			/// The current camera mode
			/// </summary>
			CameraMode _CurrentMode;
			/// <summary>
			/// The current fov
			/// </summary>
			float_t _FOV;
			/// <summary>
			/// The current zoom Level
			/// </summary>
			float_t _Zoom;
			/// <summary>
			/// Whether or not the next apply should force a call to make active,
			/// Important if the internal camera state/mode has changed.
			/// </summary>
			bool _ForceMakeActiveOnApply;
			/// <summary>
			/// Cooked Projection Matrix
			/// </summary>
			glm::mat4 _ProjectionMatrix;
			/// <summary>
			/// Cooked View Matrix
			/// </summary>
			glm::mat4 _ViewMatrix;
		};
	}
}