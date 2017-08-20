#include <glm/gtx/quaternion.hpp>

#include "math/integration.hpp"



template <>
void accumulate<glm::quat, glm::vec3>(glm::quat &v, const glm::vec3 &dv_dt, const float dt)
{
    v *= glm::exp(dt*0.5f*v*glm::quat(0.0f, dv_dt.x, dv_dt.y, dv_dt.z));
}
