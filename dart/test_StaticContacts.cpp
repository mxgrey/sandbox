
#include <dart/dynamics/BoxShape.hpp>
#include <dart/dynamics/BodyNode.hpp>
#include <dart/dynamics/FreeJoint.hpp>
#include <dart/dynamics/Skeleton.hpp>

#include <dart/simulation/World.hpp>

#include <dart/collision/CollisionOption.hpp>
#include <dart/collision/CollisionResult.hpp>
#include <dart/collision/CollisionFilter.hpp>
#include <dart/collision/CollisionObject.hpp>

#include <dart/constraint/ConstraintSolver.hpp>

struct ImmobileContactFilter : dart::collision::CollisionFilter
{
  bool ignoresCollision(
      const dart::collision::CollisionObject* object1,
      const dart::collision::CollisionObject* object2) const override
  {
    auto bodyNode1 = object1->getShapeFrame()->asShapeNode();
    auto bodyNode2 = object2->getShapeFrame()->asShapeNode();

    if(!bodyNode1->getSkeleton()->isMobile()
       && !bodyNode2->getSkeleton()->isMobile())
    {
      return true;
    }

    return false;
  }
};

std::size_t test_world(const bool disableImmobileContacts)
{
  dart::simulation::WorldPtr world = dart::simulation::World::create();

  std::unique_ptr<dart::collision::BodyNodeCollisionFilter> bn_filter(
        new dart::collision::BodyNodeCollisionFilter);

  std::unique_ptr<ImmobileContactFilter> immobile_filter(
        new ImmobileContactFilter);


  double x = -0.25;
  for(const std::string& name : {"1", "2"})
  {
    const auto skeleton = dart::dynamics::Skeleton::create(name);
    skeleton->setMobile(false);
    auto pair = skeleton
        ->createJointAndBodyNodePair<dart::dynamics::FreeJoint>();
    auto joint = pair.first;
    auto bn = pair.second;

    Eigen::Isometry3d tf = Eigen::Isometry3d::Identity();
    tf.translation() = Eigen::Vector3d(x, x, x);
    joint->setTransform(tf);
    x += 0.5;

    bn->createShapeNodeWith<dart::dynamics::CollisionAspect>(
          std::make_shared<dart::dynamics::BoxShape>(
            Eigen::Vector3d::Constant(1.0)));

    world->addSkeleton(skeleton);

    if(disableImmobileContacts)
    {
      auto filter =
          std::make_shared<dart::collision::CompositeCollisionFilter>();
      filter->addCollisionFilter(bn_filter.get());
      filter->addCollisionFilter(immobile_filter.get());

      world->getConstraintSolver()->getCollisionOption().collisionFilter =
          filter;
    }
  }

  world->step();
  return world->getLastCollisionResult().getNumContacts();
}

int main()
{
  std::cout << "Without special filter we get [" << test_world(false)
            << "] contacts\n";

  std::cout << "With special filter we get [" << test_world(true)
            << "] contacts\n";

}
