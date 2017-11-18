#include <glm/gtx/quaternion.hpp>

#include "physics/integration.hpp"



template <>
glm::quat ingetrate<glm::quat>(const float dt, const glm::quat &x, const glm::quat &dx_dt)
{
    return x*glm::exp(dt*0.5f*dx_dt);
}
