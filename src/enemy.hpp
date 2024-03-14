#pragma once
#include <cmath>
#include "utils.hpp"
#include "entity.hpp"
#include "gameState.hpp"
#include "intersection.hpp"

class Enemy :
    public Entity 
{
private:
    sf::CircleShape detection;
    Entity *target;

    // light
    std::vector<Line> rays;
    sf::Shader lightShader;
    sf::RenderTexture lightTexture;

    Intersection getIntersection(const Line ray, const Line segment);
public:
    Enemy(const float radius, const float maxVelocity, Entity *target, sf::Vector2f windowSize);

    void update(const float& dt, std::vector<Object> objects);
    void render(sf::RenderTarget* target = nullptr);
};

Enemy::Enemy(
    const float radius,
    const float maxVelocity,
    Entity *target,
    sf::Vector2f windowSize
) :
    Entity(radius, maxVelocity), 
    detection(radius * 4.2f),
    target(target)
{
    this->lightShader.loadFromFile("resources/light.vert", "resources/light.frag");
    this->lightShader.setUniform("u_resolution", windowSize);

    this->lightTexture.create((unsigned int)windowSize.x, (unsigned int)windowSize.y);
    this->lightTexture.setSmooth(true);
}

Intersection Enemy::getIntersection(const Line ray, const Line segment) {
    float r_px = ray.a.x; // ray x
	float r_py = ray.a.y; // ray y
	float r_dx = ray.b.x-ray.a.x; // direction of ray x
	float r_dy = ray.b.y-ray.a.y; // direction of ray y

	float s_px = segment.a.x; // segment x
	float s_py = segment.a.y; // segment y
	float s_dx = segment.b.x-segment.a.x; // direction of segment x
	float s_dy = segment.b.y-segment.a.y; // direction of segment y

    // finding unit vector of both ray and segment to check if they are parallel
	float r_mag = std::sqrt(r_dx*r_dx+r_dy*r_dy);
	float s_mag = std::sqrt(s_dx*s_dx+s_dy*s_dy);
    // in case they are, return a false status
	if(r_dx/r_mag==s_dx/s_mag && r_dy/r_mag==s_dy/s_mag)
		return { false };

    // SOLVE FOR T1 & T2
	// r_px+r_dx*T1 = s_px+s_dx*T2 && r_py+r_dy*T1 = s_py+s_dy*T2
	// ==> T1 = (s_px+s_dx*T2-r_px)/r_dx = (s_py+s_dy*T2-r_py)/r_dy
	// ==> s_px*r_dy + s_dx*T2*r_dy - r_px*r_dy = s_py*r_dx + s_dy*T2*r_dx - r_py*r_dx
	// ==> T2 = (r_dx*(s_py-r_py) + r_dy*(r_px-s_px))/(s_dx*r_dy - s_dy*r_dx)
	float T2 = (r_dx*(s_py-r_py) + r_dy*(r_px-s_px))/(s_dx*r_dy - s_dy*r_dx);
	float T1 = (s_px+s_dx*T2-r_px)/r_dx;

    // conditions for intersection
	if(T1<0) return { false };
	if(T2<0 || T2>1) return { false };

    // if all conditions satisfy, return an intersection with true status, point and the value T
	return {
        true,
		r_px+r_dx*T1,
		r_py+r_dy*T1,
		T1
	};
}

void Enemy::update(const float& dt, std::vector<Object> objects) {
    // managing light
    std::vector<float> angles;
    std::vector<Intersection> intersections;
    this->rays.clear();
    this->lightTexture.clear();

    sf::Vector2f entityPos = this->entity.getPosition() + this->entity.getRadius() * sf::Vector2f{ 1.f, 1.f };
    this->lightShader.setUniform("u_entity", entityPos);

    for (Object object : objects) {
        for (Point point : object.getVerticies()) {
            float angle = std::atan2(point.y-entityPos.y,point.x-entityPos.x);
            angles.push_back(angle-0.0001f);
            angles.push_back(angle);
            angles.push_back(angle+0.0001f);
        }
    }

    for (float angle : angles) {
        float dx = std::cos(angle);
        float dy = std::sin(angle);

        Line ray{ { entityPos.x, entityPos.y }, { entityPos.x+dx, entityPos.y+dy } };

        Intersection closest{ false };
        for (Object object : objects) {
            for (Line segment : object.getSegments()) {
                Intersection intersect = this->getIntersection(ray, segment);
                intersect.angle = angle; // store angle too for later use
                if (!intersect.status) continue; // if doesn't intersect then pass
                // if no previous intersection OR current intersection closer to previous closest
                if (!closest.status || intersect.T1 < closest.T1) 
                    closest = intersect;
            }
        }
        intersections.push_back(closest);
    }

    std::sort(intersections.begin(), intersections.end(), [&](const Intersection& a, const Intersection& b) { return a.angle < b.angle; });
    intersections.emplace_back(intersections[0]); // this completes the triangle fan from all directions

    // storing vertices for the triangle fan
    std::vector<sf::Vertex> vertices;
    // initial point for the triangle fan
    vertices.emplace_back(sf::Vertex(entityPos));

    // storing every intersection point in the vertices
    for (Intersection intersect : intersections) {
        // Line ray{ { entityPos.x, entityPos.y }, { intersect.point.x, intersect.point.y } };
        // rays.push_back(ray);
        vertices.emplace_back(intersect.point.vertex());
    }

    this->lightTexture.draw(vertices.data(), vertices.size(), sf::TriangleFan);
    this->lightTexture.display();

    // managing velocity and chasing player
    this->velocity *= 0.85f;

    float detectionRadius = this->detection.getRadius(), radius = this->entity.getRadius();
    this->detection.setPosition(
        this->entity.getPosition() - sf::Vector2f{ detectionRadius - radius, detectionRadius - radius }
    );

    if (this->detection.getGlobalBounds().intersects(this->target->getGlobalBounds())) {
        sf::Vector2f direction = Utils::normalize(this->target->getPosition() - this->entity.getPosition());

        if (!this->entity.getGlobalBounds().intersects(this->target->getGlobalBounds()))
            this->velocity = direction * this->maxVelocity;
    }
    this->move(this->velocity, dt);
}

void Enemy::render(sf::RenderTarget* target) {
    // for (Line ray : rays)
    //     target->draw(ray.vertexArray(sf::Color::Red));
    target->draw(sf::Sprite(this->lightTexture.getTexture()), &this->lightShader);
    target->draw(this->entity);
}