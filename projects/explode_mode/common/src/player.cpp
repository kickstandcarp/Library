#include "player.hpp"
#include "square.hpp"
// #include "background.hpp"



Player::Player(const std::shared_ptr<CoordinateTransform> &move_coordinate_transform, const std::shared_ptr<CoordinateTransform> &aim_coordinate_transform, const float radius, const std::shared_ptr<Clock> &clock)
:   move_radius(move_radius),
	is_firing(false),
	is_slowing(false),
    move_coordinate_transform(move_coordinate_transform),
    aim_coordinate_transform(aim_coordinate_transform),
    clock(clock)
{
    this->coordinate_transform = this->move_coordinate_transform;
    this->move_coordinate_transform->set_rotation_axis(glm::vec3(0.0f, 1.0f, 0.0f));
    this->move_coordinate_transform->set_origin_translation(glm::vec3(0.0, 0.0, this->radius));

    this->move_horizontal_kinetics = std::unique_ptr<AccelerationKinetics<float> >(new AccelerationKinetics<float>(this->move_coordinate_transform, rotation_angle_parameter));
    this->move_horizontal_kinetics->max_velocity = 5.0f;

    this->move_vertical_kinetics = std::unique_ptr<AccelerationKinetics<float> >(new AccelerationKinetics<float>(this->move_coordinate_transform, translation_y_parameter));
    this->move_vertical_kinetics->max_velocity = 10.0f;

    this->aim_coordinate_transform->set_parent(this->move_coordinate_transform);

    this->aim_kinetics = std::unique_ptr<AccelerationKinetics<glm::vec2> >(new AccelerationKinetics<glm::vec2>(glm::vec2(0.0f)));
    this->aim_kinetics->value = std::unique_ptr<ValueAccessor<glm::vec2> >(new MemberValueAccessor<Player, glm::vec2>(this, &Player::get_aim_angles, &Player::set_aim_angles));
    this->aim_kinetics->friction = glm::vec2(10.0f);
    this->aim_kinetics->max_velocity = glm::vec2(3.0f);

	this->reticle_coordinate_transform = std::make_shared<CoordinateTransform>();
	this->reticle_coordinate_transform->set_scaling(glm::vec3(0.01f));

    std::vector<glm::vec3> vertices;
    render_astroids_text("~reticle~", vertices);
    offset_vertices(glm::vec3(-0.5f*static_cast<float>(astroids_font_char_size.x), 0.5f*static_cast<float>(astroids_font_char_size.y), 0.0f), vertices);
    this->reticle_line = std::unique_ptr<Line>(new Line(vertices, this->reticle_coordinate_transform, clock));
	this->reticle_point = std::unique_ptr<Line>(new Line({ glm::vec3(0.0f) }, this->reticle_coordinate_transform, clock));
}

Player::~Player()
{

}

glm::vec2 Player::get_aim_angles() const
{
    return this->aim_angles;
}

void Player::set_aim_angles(const glm::vec2& aim_angles)
{
    this->aim_angles = glm::clamp(aim_angles, glm::vec2(-0.5f*PI, 0.0f), glm::vec2(0.5f*PI, 0.5f*PI - FLT_EPSILON));

    glm::vec3 direction = std::cos(-this->aim_angles.y)*glm::vec3(std::sin(this->aim_angles.x), std::sin(-this->aim_angles.y), std::cos(this->aim_angles.x));
    glm::vec4 axis_angle = rotation_axis_angle(glm::vec3(0.0f, 0.0f, 1.0f), direction);

    this->aim_coordinate_transform->set_rotation_axis(glm::vec3(axis_angle));
    this->aim_coordinate_transform->set_rotation_angle(axis_angle.w);
}

void Player::reset(const float aim_at_height)
{
    this->move_coordinate_transform->set_translation(glm::vec3(0.0f, 1.0f, 0.0f));

    float angle = std::atan2(aim_at_height - 1.0f, this->radius);
    this->set_aim_angles(glm::vec2(0.0f, angle));

	this->is_firing = false;
	this->is_slowing = false;
}

void Player::move()
{
	if (std::abs(this->move_direction->get_x()) > 1.0e-3f)
	{
		this->move_horizontal_kinetics->acceleration = (200.0f / this->radius)*this->move_direction->get_x();
		this->move_horizontal_kinetics->friction = 0.0f;
	}
	else
	{
		this->move_horizontal_kinetics->acceleration = 0.0f;
		this->move_horizontal_kinetics->friction = 20.0f;
	}

	/* if (std::abs(this->move_direction->get_y()) > 1.0e-3f)
	{
		this->move_vertical_kinetics->acceleration = 300.0f*this->move_direction->get_y();
		this->move_vertical_kinetics->friction = 0.0f;
	}
	else
	{
		this->move_vertical_kinetics->acceleration = 0.0f;
		this->move_vertical_kinetics->friction = 20.0f;
	} */

	this->move_horizontal_kinetics->step(this->clock->get_delta_time());
	// this->move_vertical_kinetics->step(this->clock->get_delta_time());
}

void Player::aim()
{
	if (this->aim_direction->get_magnitude() > 1.0e-3f)
		this->aim_kinetics->acceleration = -20.0f*glm::vec2(this->aim_direction->get_x(), this->aim_direction->get_y());
	else
		this->aim_kinetics->acceleration = glm::vec2(0.0f);

	this->aim_kinetics->step(this->clock->get_delta_time());
}

void Player::playing()
{
    this->move();
    this->aim();

	this->is_firing = this->fire_button->get_moved_down();
	if (this->is_firing)
	{
		this->reticle_line->glow_color = 3.0f*glm::vec3(0.7f, 0.7f, 1.0f);
		this->reticle_line->finish_scan(true);
	}

	this->is_slowing = this->slow_button->get_is_down();
}

void Player::update(const EventHandler &event_handler)
{

}

/* void Player::collide(const std::list<std::shared_ptr<Entity> > &entities)
{
	glm::vec3 position = this->aim_coordinate_transform->transform_position(glm::vec3(0.0f), true);
	glm::vec3 direction = this->aim_coordinate_transform->transform_direction(glm::vec3(0.0f, 0.0f, -1.0f), true);
	
	Background* background = nullptr;

	float interected_distance = std::numeric_limits<float>::infinity();
	Square* intersected_square = nullptr;
	for (auto entity = entities.begin(); entity != entities.end(); ++entity)
	{
		if (background == nullptr)
			background = dynamic_cast<Background*>(entity->get());

		Square* square = dynamic_cast<Square*>(entity->get());
		if (square != nullptr)
		{
			if (this->is_firing)
			{
				float distance = square->ray_intersection_distance(position, direction);
				if (distance < interected_distance)
				{
					interected_distance = distance;
					intersected_square = square;
				}
			}

			square->time_kinetics->acceleration = this->is_slowing ? -3.0f : 10.0f;
		}
	}

	if (intersected_square != nullptr)
	{
		intersected_square->state->transition(intersected_square, "explode", 10, direction);
		if (background != nullptr)
			background->start_oscillation(3.0f, 0.666f);
	}

	if (background != nullptr)
		background->time_kinetics->acceleration = this->is_slowing ? -3.0f : 10.0f;
} */

void Player::step(const Clock &clock)
{

}

void Player::draw(const Camera &camera, Window &window)
{

}
