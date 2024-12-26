#include "Utils.hpp"

#include "Camera.hpp"
#include "Hittable.hpp"
#include "HittableList.hpp"
#include "Material.hpp"
#include "Sphere.hpp"


int main(void) {
    HittableList world;

    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left = make_shared<Dielectric>(1.50);
    auto material_bubble = make_shared<Dielectric>(1.00 / 1.50);
    auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<Sphere>(Point3(0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(Point3(0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(Point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<Sphere>(Point3(1.0, 0.0, -1.0), 0.5, material_right));

	Camera camera; 
	camera.setRatio(16.0/9.0);
	camera.setWidth(400);
	camera.setMaxDepth(50);
	camera.setSamplesPerPixel(100);

	camera.setFov(20);
	camera.setLookfrom(Point3(-2, 2, 1));
	camera.setLookat(Point3(0, 0, -1));
	camera.setVup(Vec3(0, 1, 0));

	camera.setFocusDist(3.4);
	camera.setDefocusAngle(10.0);

	camera.render(world);
}