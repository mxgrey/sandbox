
#include <dart/dynamics/BoxShape.hpp>
#include <dart/dynamics/FreeJoint.hpp>
#include <dart/dynamics/MeshShape.hpp>
#include <dart/dynamics/PlaneShape.hpp>
#include <dart/dynamics/Skeleton.hpp>
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

  const auto skeleton = dart::dynamics::Skeleton::create("box");
  auto pair = skeleton
      ->createJointAndBodyNodePair<dart::dynamics::FreeJoint>();


  const double z = -0.1;

  auto joint = pair.first;
  auto bn = pair.second;

  Eigen::Isometry3d tf = Eigen::Isometry3d::Identity();
  tf.translation()[2] = z;
  joint->setTransform(tf);

  bn->createShapeNodeWith<
      dart::dynamics::CollisionAspect,
      dart::dynamics::VisualAspect>(
//        std::make_shared<dart::dynamics::BoxShape>(Eigen::Vector3d::Ones()));
        std::make_shared<dart::dynamics::MeshShape>(
          Eigen::Vector3d::Ones(), mesh, mesh_path));

  world->addSkeleton(skeleton);

//  const Eigen::Matrix3d R(Eigen::AngleAxisd(45.0*M_PI/180.0, Eigen::Vector3d::UnitY()));
  const Eigen::Matrix3d R(Eigen::AngleAxisd(0.00001*M_PI/180.0, Eigen::Vector3d::UnitX()));

  const auto ground = dart::dynamics::Skeleton::create("ground");
  bn = ground->createJointAndBodyNodePair<dart::dynamics::WeldJoint>().second;
  auto sn = bn->createShapeNodeWith<
      dart::dynamics::CollisionAspect,
      dart::dynamics::VisualAspect>(
//        std::make_shared<dart::dynamics::BoxShape>(
//          Eigen::Vector3d(100.0, 100.0, 0.01)));
        std::make_shared<dart::dynamics::PlaneShape>(
//          R * -Eigen::Vector3d::UnitZ(), z));
          R * Eigen::Vector3d::UnitY(), z));

//  sn->setRelativeTranslation(0.0 * Eigen::Vector3d::UnitZ());



  world->addSkeleton(ground);

  dart::gui::osg::Viewer viewer;
  viewer.addWorldNode(new dart::gui::osg::RealTimeWorldNode(world));

  viewer.setUpViewInWindow(0, 0, 640, 480);

  viewer.run();

  world->step();

  std::cout << "contacts: " << world->getLastCollisionResult().getNumContacts()
            << std::endl;
}
