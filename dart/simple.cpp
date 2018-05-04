#include <dart/dart.hpp>
#include <iostream>

int main()
{
    constexpr double density = 1.0;
    constexpr double radius = 0.3;
    constexpr double restitution = 0.6;
    constexpr double starting_height = 10.0;

    // Create the sphere skeleton (returns a shared_ptr)
    dart::dynamics::SkeletonPtr sphere = dart::dynamics::Skeleton::create();

    // Create joint properties for a free joint that connects the
    // sphere to the world. This is a free joint since the body does
    // not need any constraints with respect to the world.
    dart::dynamics::FreeJoint::Properties sphere_joint_prop;

    // Create body properties for the sphere
    dart::dynamics::BodyNode::Properties sphere_body_prop;

    // Add the sphere body and connect to the world
    std::pair<dart::dynamics::FreeJoint*, dart::dynamics::BodyNode*> sphere_pair =
        sphere->createJointAndBodyNodePair<dart::dynamics::FreeJoint>(
            nullptr, sphere_joint_prop, sphere_body_prop);

    // Create a shape object to attach to the BodyNode (shared_ptr)
    dart::dynamics::ShapePtr sphere_shape(new dart::dynamics::SphereShape(radius));

    // Configure the BodyNode shape so that it has a collision aspect and
    // a dynamic aspect.
    sphere_pair.second->createShapeNodeWith<
        dart::dynamics::CollisionAspect,
        dart::dynamics::DynamicsAspect>(sphere_shape);

    // Inertia
    dart::dynamics::Inertia sphere_inertia;
    const double sphere_mass = density * sphere_shape->getVolume();
    sphere_inertia.setMass(sphere_mass);
    sphere_inertia.setMoment(sphere_shape->computeInertia(sphere_mass));
    sphere_pair.second->setInertia(sphere_inertia);

    // Coefficient of restitution
    sphere_pair.second->setRestitutionCoeff(restitution);

    // Set the initial position of the sphere.
    // The first three components are the orgientation using angle-axis
    // (logmap), the second three components are the translation.
    Eigen::Vector6d positions(Eigen::Vector6d::Zero());
    positions[5] = starting_height;
    sphere_pair.first->setPositions(positions);

    // Create the world object (shared_ptr) and add the sphere skeleton
    dart::simulation::WorldPtr world(new dart::simulation::World);
    world->addSkeleton(sphere);

    // Simulate the world for some amount of time
    constexpr double END_TIME = 5.0;
    while (world->getTime() < END_TIME) {

        std::cout << world->getTime() << " : ";
        for (auto i = 0u; i < sphere->getJoint(0)->getNumDofs(); ++i)
            std::cout << sphere->getJoint(0)->getPosition(i) << " ";
        std::cout << std::endl;

        world->step(false);
    }

    std::cout << world->getGravity() << std::endl;

    return EXIT_SUCCESS;
}
