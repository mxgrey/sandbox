
#include <iostream>

#include <ignition/math/eigen3/Conversions.hh>
#include <Eigen/Geometry>

int main()
{
  Eigen::Vector3d vecE = Eigen::Vector3d(1.0, 2.0, 3.0);
  ignition::math::Vector3d vecI = ignition::math::eigen3::convert(vecE);

  std::cout << vecI << std::endl;
}
