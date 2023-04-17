#pragma once

#include "Math/Math.h"

namespace Alexio
{
	class Camera
	{
	public:
		Camera() = default;
		Camera(const Mat4x4& proj)
			: mProjection(proj) {}

		void SetProjection(Mat4x4 projection) { mProjection = projection; }
		Mat4x4 GetProjection() { return mProjection; }
	protected:
		Mat4x4 mProjection;
	};
}