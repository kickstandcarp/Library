#include <glm/gtx/quaternion.hpp>

#include "math/integration.hpp"



template <>
void accumulate<glm::quat>(glm::quat &v, const glm::quat &dv_dt, const float dt)
{
	v *= glm::exp(dt*0.5f*dv_dt);
}
