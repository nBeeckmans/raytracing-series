#include "Utils.hpp"

#include "BVHNode.hpp"
#include "Camera.hpp"
#include "Hittable.hpp"
#include "HittableList.hpp"
#include "Material.hpp"
#include "Sphere.hpp"
#include "Texture.hpp"

void bouncingSpheres() {
    HittableList world;

    auto checker = make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(.9, .9, .9));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(checker)));

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = randomDouble();
            Point3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    auto centerTo = center + Vec3(0, randomDouble(0, 0.5), 0);
                    world.add(make_shared<Sphere>(center, centerTo, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = randomDouble(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    world = HittableList(make_shared<BVHNode>(world));

	Camera camera; 
	camera.setRatio(16.0/9.0);
	camera.setWidth(400);
	camera.setSamplesPerPixel(100);
	camera.setMaxDepth(50);

	camera.setFov(20);
	camera.setLookfrom(Point3(13, 2, 3));
	camera.setLookat(Point3(0, 0, 0));
	camera.setVup(Vec3(0, 1, 0));

	camera.setDefocusAngle(0.6);
	camera.setFocusDist(10.0);

	camera.render(world);
}

void checkeredSpheres() {
    HittableList world;

    auto checker = make_shared<CheckerTexture>(0.32, Color(.2, .3, .1), Color(0.9, 0.9, 0.9));
    world.add(make_shared<Sphere>(Point3(0, -10, 0), 10, make_shared<Lambertian>(checker)));
    world.add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<Lambertian>(checker)));
    
    Camera camera;
    camera.setRatio(16.0 / 9.0);
    camera.setWidth(400);
    camera.setSamplesPerPixel(100);
    camera.setMaxDepth(50);

    camera.setFov(20);
    camera.setLookfrom(Point3(13, 2, 3));
    camera.setLookat(Point3(0, 0, 0));
    camera.setVup(Vec3(0, 1, 0));

    camera.setDefocusAngle(0);

    camera.render(world);
}

int main(void) {
    switch (2) {
    case 1: bouncingSpheres(); break;
    case 2: checkeredSpheres(); break;
    }

}