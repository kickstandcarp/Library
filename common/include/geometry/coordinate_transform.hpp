#ifndef GEOMETRY_COORDINATE_TRANSFORM_HPP
#define GEOMETRY_COORDINATE_TRANSFORM_HPP

#include <memory>
#include <set>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class CoordinateTransform: public std::enable_shared_from_this<CoordinateTransform>
{
	public:
		CoordinateTransform(const glm::vec3 &translation=glm::vec3(0.0), const glm::quat &rotation=glm::quat(1.0f, 0.0f, 0.0f, 0.0f), const glm::vec3 &scaling=glm::vec3(1.0), const glm::vec3 &origin_translation=glm::vec3(0.0));
		virtual ~CoordinateTransform();

		glm::vec3									get_translation() const;
		glm::quat									get_rotation() const;
		glm::vec3									get_scaling() const;
		glm::vec3									get_origin_translation() const;

		const std::shared_ptr<CoordinateTransform>	get_parent() const;

		const glm::mat4x4&							get_local_matrix();
		const glm::mat4x4&							get_global_matrix();
		const glm::mat4x4&							get_inverse_global_matrix();
		bool										get_updated() const;

		void										set_translation(const glm::vec3 &translation);
		void										set_rotation(const glm::quat &rotation);
		void										set_scaling(const glm::vec3 &scaling);
		void										set_origin_translation(const glm::vec3 &origin_translation);

		void		                                set_parent(const std::shared_ptr<CoordinateTransform> &coordinate_transform);

		void                                        translate(const glm::vec3 &translation);
		void										rotate(const glm::quat &rotation);
		void										scale(const glm::vec3 &scaling);
		void										translate_origin(const glm::vec3 &translation);

		glm::vec3									transform_position(const glm::vec3 &position, const bool to_global);
		glm::vec3									transform_direction(const glm::vec3 &direction, const bool to_global);

	private:
		void                                        set_matrices();
		void										set_updated(const bool is_updated);

		glm::vec3									translation, scaling, origin_translation;
		glm::quat                                   rotation;

		std::shared_ptr<CoordinateTransform>		parent;
		std::set<std::weak_ptr<CoordinateTransform>, std::owner_less<std::weak_ptr<CoordinateTransform> > > children;

		bool										is_updated;
		glm::mat4x4									local_matrix, global_matrix, inverse_global_matrix;
};

#endif
