
#include <dart/dynamics/Skeleton.hpp>
#include <dart/dynamics/WeldJoint.hpp>
#include <dart/dynamics/RevoluteJoint.hpp>
#include <dart/dynamics/CylinderShape.hpp>

#include <dart/simulation/World.hpp>

#include <dart/gui/osg/Viewer.hpp>
#include <dart/gui/osg/WorldNode.hpp>

int main()
{
  dart::dynamics::RevoluteJoint::Properties p;
  p.mAxis = Eigen::Vector3d::UnitY();

  auto skeleton = dart::dynamics::Skeleton::create();
  auto parent = skeleton->createJointAndBodyNodePair<
      dart::dynamics::RevoluteJoint>(nullptr, p).second;

  p.mT_ParentBodyToJoint.translate(-Eigen::Vector3d::UnitZ());

  auto child = skeleton->createJointAndBodyNodePair<
      dart::dynamics::RevoluteJoint>(parent, p).second;

  const double R = 0.1;
  const double h = 0.9;
  auto rod = std::make_shared<dart::dynamics::CylinderShape>(R, h);

  for(auto * const bn : {parent, child})
  {
    const Eigen::Vector3d center = -0.5*Eigen::Vector3d::UnitZ();
    const double mass = 1.0;

    bn->createShapeNodeWith<dart::dynamics::VisualAspect>(rod)
        ->setRelativeTranslation(center);
    dart::dynamics::Inertia inertia;
    inertia.setMass(mass);
    inertia.setMoment(rod->computeInertia(mass));
    inertia.setLocalCOM(center);

    bn->setInertia(inertia);

    auto joint = bn->getParentJoint();
    joint->setPosition(0, 60.0*M_PI/180.0);
    joint->setDampingCoefficient(0, 0.3);
  }

  auto world = std::make_shared<dart::simulation::World>();
  world->addSkeleton(skeleton);

  dart::gui::osg::Viewer viewer;
  osg::ref_ptr<dart::gui::osg::WorldNode> node =
      new dart::gui::osg::WorldNode(world);
  node->setNumStepsPerCycle(100);

  viewer.addWorldNode(node);

  viewer.run();
}
