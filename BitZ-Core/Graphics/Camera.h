#pragma once
#include "../Common.h"
#include "../Math/Math.h"
#include "../Interfaces.h"

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

		protected:
			/// <summary>
			/// Applies this cameras position and rotation from the identity to the gl_modelview
			/// </summary>
			void Apply();
			/// <summary>
			/// This logic applies when this camera whasnt the last camera used, allows the seperation of
			/// some of the apply logic to reduce opwngl calls
			/// </summary>
			void MakeActive() const;

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
			/// Whether or not the next apply should force a call to make active,
			/// Important if the internal camera state/mode has changed.
			/// </summary>
			bool _ForceMakeActiveOnApply;
		};
	}
}