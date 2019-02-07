
#include <dart/dynamics/BoxShape.hpp>
#include <dart/dynamics/FreeJoint.hpp>
#include <dart/dynamics/Skeleton.hpp>
#include <dart/dynamics/MeshShape.hpp>
#include <dart/dynamics/WeldJoint.hpp>

#include <dart/simulation/World.hpp>
#include <dart/constraint/ConstraintSolver.hpp>

#include <dart/gui/osg/RealTimeWorldNode.hpp>
#include <dart/gui/osg/Viewer.hpp>

#include <dart/collision/bullet/BulletCollisionDetector.hpp>
#include <dart/collision/ode/OdeCollisionDetector.hpp>

int main(int argc, char* argv[])
{
  if(argc < 2)
  {
    std::cout << "You need to input a mesh file path!" << std::endl;
    return 1;
  }

  const std::string mesh_path = argv[1];
  auto mesh = dart::dynamics::MeshShape::loadMesh(mesh_path);
  if(!mesh)
  {
    std::cout << "Could not parse the mesh at [" << mesh_path << "]"
              << std::endl;
    return 1;
  }

  auto world = dart::simulation::World::create();
//  world->getConstraintSolver()->setCollisionDetector(
//        dart::collision::BulletCollisionDetector::create());

  world->getConstraintSolver()->setCollisionDetector(
        dart::collision::OdeCollisionDetector::create());

  double z = 1.0;
  for(const std::string& name : {"1", "2"})
  {
    const auto skeleton = dart::dynamics::Skeleton::create(name);
    auto pair = skeleton
        ->createJointAndBodyNodePair<dart::dynamics::FreeJoint>();

    auto joint = pair.first;
    auto bn = pair.second;

    Eigen::Isometry3d tf = Eigen::Isometry3d::Identity();
    tf.translation()[2] = z;
    joint->setTransform(tf);
    z += 5.0;

    auto sn = bn->createShapeNodeWith<
        dart::dynamics::CollisionAspect,
        dart::dynamics::VisualAspect>(
          std::make_shared<dart::dynamics::MeshShape>(
            0.01*Eigen::Vector3d::Ones(), mesh, mesh_path));

    sn->setRelativeRotation(
          Eigen::Matrix3d(
            Eigen::AngleAxisd(90*M_PI/180.0, Eigen::Vector3d::UnitX())));

    world->addSkeleton(skeleton);
  }

  const auto ground = dart::dynamics::Skeleton::create("ground");
  auto bn = ground->createJointAndBodyNodePair<dart::dynamics::WeldJoint>().second;
  bn->createShapeNodeWith<
      dart::dynamics::CollisionAspect,
      dart::dynamics::VisualAspect>(
        std::make_shared<dart::dynamics::BoxShape>(
          Eigen::Vector3d(100.0, 100.0, 0.01)));

  world->addSkeleton(ground);

  dart::gui::osg::Viewer viewer;
  viewer.addWorldNode(new dart::gui::osg::RealTimeWorldNode(world));

  viewer.setUpViewInWindow(0, 0, 640, 480);

  viewer.run();
}
