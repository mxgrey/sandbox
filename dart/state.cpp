#include <iostream>

#include <dart/dart.hpp>

dart::dynamics::SkeletonPtr create_box(const Eigen::Vector3d& dims, double mass, const Eigen::Vector4d& color, const std::string& box_name)
{
    dart::dynamics::SkeletonPtr box_skel = dart::dynamics::Skeleton::create(box_name);

    // Give the box a body
    dart::dynamics::BodyNodePtr body;
    std::cout << "-----Creating body for box" << std::endl;
    body = box_skel->createJointAndBodyNodePair<dart::dynamics::FreeJoint>(nullptr).second;
    body->setName(box_name);

    // Give the body a shape
    auto box = std::make_shared<dart::dynamics::BoxShape>(dims);
    auto box_node = body->createShapeNodeWith<dart::dynamics::VisualAspect, dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(box);
    box_node->getVisualAspect()->setColor(color);
    // Set up inertia
    dart::dynamics::Inertia inertia;
    inertia.setMass(mass);
    inertia.setMoment(box->computeInertia(mass));
    body->setInertia(inertia);

    return box_skel;
}

dart::dynamics::SkeletonPtr create_floor()
{
    double floor_width = 10.;
    double floor_height = 0.1;

    dart::dynamics::SkeletonPtr floor_skel = dart::dynamics::Skeleton::create("floor");

    // Give the floor a body
    std::cout << "-----Creating body for floor" << std::endl;
    dart::dynamics::BodyNodePtr body = floor_skel->createJointAndBodyNodePair<dart::dynamics::WeldJoint>(nullptr).second;

    // Give the body a shape
    auto box = std::make_shared<dart::dynamics::BoxShape>(Eigen::Vector3d(floor_width, floor_width, floor_height));
    auto box_node = body->createShapeNodeWith<dart::dynamics::VisualAspect, dart::dynamics::CollisionAspect, dart::dynamics::DynamicsAspect>(box);
    box_node->getVisualAspect()->setColor(dart::Color::Gray());

    // Put the body into position
    Eigen::Isometry3d tf(Eigen::Isometry3d::Identity());
    tf.translation()[2] -= floor_height / 2.0;
    body->getParentJoint()->setTransformFromParentBodyNode(tf);

    return floor_skel;
}

void test_1()
{
  auto world = std::make_shared<dart::simulation::World>();

  auto box_skel = create_box({0.1, 0.1, 0.1}, 1., {1., 0., 0., 1.}, "box");
  auto floor_skel = create_floor();

  box_skel->setPosition(5, 0.2);

  world->addSkeleton(box_skel);
  world->addSkeleton(floor_skel);

  for (size_t i = 0; i < 20; i++)
      world->step();

  std::cout << box_skel->getRootBodyNode()->getTransform().matrix() << std::endl;
  std::cout << "====== preclone box_skel [" << box_skel->getName() << ": " << box_skel->getNumJoints() << "]" << std::endl;
  auto box_skel2 = box_skel->cloneSkeleton();
  std::cout << "====== pre box_skel [" << box_skel->getName() << ": " << box_skel->getNumJoints() << "]" << std::endl;
  auto state = box_skel->getState();
  std::cout << "========== post box_skel [" << box_skel->getName() << ": " << box_skel->getNumJoints() << "]" << std::endl;
  for(const auto& pair : state.getMap())
  {
    std::cout << pair.first.name() << ": " << typeid(*pair.second).name() << std::endl;
  }
  box_skel2->setState(state);
  std::cout << "\n" << box_skel2->getRootBodyNode()->getTransform().matrix() << std::endl;
}

void test_2()
{
  auto world = std::make_shared<dart::simulation::World>();

  std::cout << "---------------------\nCreating box" << std::endl;
  auto box_skel = create_box({0.1, 0.1, 0.1}, 1., {1., 0., 0., 1.}, "box");
  std::cout << "---------------------\nCreating floor" << std::endl;
  auto floor_skel = create_floor();

  box_skel->setPosition(5, 0.2);

  world->addSkeleton(box_skel);
  world->addSkeleton(floor_skel);

  dart::dynamics::Skeleton::State bookmark_10;
  for(std::size_t i=0; i < 20; ++i)
  {
    if(i == 10)
    {
      std::cout << "------------------------\n About to save bookmark" << std::endl;
      bookmark_10 = box_skel->getState();
      std::cout << "Transform at step 10:\n"
                << box_skel->getRootBodyNode()->getTransform().matrix()
                << std::endl;
    }
    world->step();
  }

  std::cout << "\nTransform at step 20:\n"
            << box_skel->getRootBodyNode()->getTransform().matrix()
            << std::endl;

  box_skel->setState(bookmark_10);
  std::cout << "\nTransform from bookmarked state:\n"
            << box_skel->getRootBodyNode()->getTransform().matrix()
            << std::endl;
}

int main()
{
//  test_1();

  test_2();
}
