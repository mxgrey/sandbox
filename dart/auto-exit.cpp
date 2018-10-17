
#include <dart/collision/CollisionObject.hpp>
#include <dart/collision/CollisionResult.hpp>
#include <dart/dynamics/BoxShape.hpp>
#include <dart/dynamics/BodyNode.hpp>
#include <dart/dynamics/FreeJoint.hpp>
#include <dart/dynamics/SphereShape.hpp>
#include <dart/dynamics/WeldJoint.hpp>
#include <dart/gui/osg/Viewer.hpp>
#include <dart/gui/osg/WorldNode.hpp>
#include <dart/simulation/World.hpp>

class AutoExitWorldNode : public dart::gui::osg::WorldNode
{
public:

  AutoExitWorldNode(const dart::simulation::WorldPtr& world,
                    const double timeout = 2.0)
    : WorldNode(world),
      mTimeout(timeout)
  {
    // Do nothing
  }

  void customPostStep() override
  {
    // Leave after some amount of time
    const double current_time = getWorld()->getTime();
    if(current_time > mTimeout)
    {
      std::cout << "Reached " << mTimeout << " simulation seconds, so we are "
                << "exiting" << std::endl;

      mViewer->setDone(true);
    }

    // Leave when we detect that the ball has hit the ground
    const auto& ball = getWorld()->getSkeleton("ball");
    const auto& ground = getWorld()->getSkeleton("ground");

    const auto& collisions = getWorld()->getLastCollisionResult();
    for(const auto& contact : collisions.getContacts())
    {
      const auto& s1 = contact.collisionObject1->getShapeFrame()
          ->asShapeNode()->getSkeleton();

      const auto& s2 = contact.collisionObject2->getShapeFrame()
          ->asShapeNode()->getSkeleton();

      if(    (s1 == ball && s2 == ground)
          || (s1 == ground && s2 == ball))
      {
        std::cout << "The ball has hit the ground at " << current_time
                  << " seconds, so we will end the simulation" << std::endl;

        mViewer->setDone(true);
        return;
      }
    }
  }

  void customPostRefresh() override
  {
    mViewer->getCameraManipulator()->computeHomePosition(nullptr, true);
    mViewer->getCameraManipulator()->home(0);
  }

  const double mTimeout;

};

int main()
{
  dart::simulation::WorldPtr world = dart::simulation::World::create();

  auto ground = dart::dynamics::Skeleton::create("ground");
  auto bn = ground->createJointAndBodyNodePair<dart::dynamics::WeldJoint>()
      .second;

  bn->createShapeNodeWith<
      dart::dynamics::VisualAspect,
      dart::dynamics::CollisionAspect>(
        std::make_shared<dart::dynamics::BoxShape>(
          Eigen::Vector3d(1, 1, 0.1)));

  world->addSkeleton(ground);

  auto ball = dart::dynamics::Skeleton::create("ball");
  dart::dynamics::FreeJoint* fj;
  std::tie(fj, bn) =
      ball->createJointAndBodyNodePair<dart::dynamics::FreeJoint>();

  Eigen::Isometry3d tf = Eigen::Isometry3d::Identity();
  tf.translate(Eigen::Vector3d(0, 0, 2));
  // Uncomment the following line if you want the simulation to time out
//  tf.translate(Eigen::Vector3d(0, 0, 18));
  fj->setRelativeTransform(tf);

  bn->createShapeNodeWith<
      dart::dynamics::VisualAspect,
      dart::dynamics::CollisionAspect>(
        std::make_shared<dart::dynamics::SphereShape>(0.1))
      ->getVisualAspect()->setColor(Eigen::Vector4d(1.0, 0.0, 0.0, 1.0));

  world->addSkeleton(ball);

  dart::gui::osg::Viewer viewer;
  viewer.addWorldNode(new AutoExitWorldNode(world));

  viewer.run();
}
