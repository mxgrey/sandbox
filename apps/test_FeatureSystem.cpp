
#include <cstddef>
#include <type_traits>
#include <typeinfo>

#include <iostream>

class Feature
{
  class Engine { };

  class Link { };
};

template<typename... Features>
class FeatureList
{
public:

  template<typename FeatureType>
  class Engine { };

  template<typename FeatureType>
  class Link { };

};


template<typename _Feature, typename... RemainingFeatures>
class FeatureList<_Feature, RemainingFeatures...>
    : public virtual FeatureList<RemainingFeatures...>
{
public:

  using CurrentFeature = _Feature;
  using NextFeature = FeatureList<RemainingFeatures...>;

  template<typename FeatureType>
  class Engine
      : public virtual CurrentFeature::template Engine<FeatureType>,
        public virtual NextFeature::template Engine<FeatureType> { };

  template<typename FeatureType>
  class Link
      : public virtual CurrentFeature::template Link<FeatureType>,
        public virtual NextFeature::template Link<FeatureType> { };
};

template<typename FeatureList, typename Feature>
struct ContainsFeature : std::is_base_of<Feature, FeatureList>
{
};

template<typename FeatureList, typename RequestedFeature,
         typename... ConflictingFeatures>
struct ConflictsWithFeatures
{

};

template<typename FeatureList, typename RequestedFeature,
         typename ConflictingFeature, typename... RemainingConflicts>
struct ConflictsWithFeatures<
    FeatureList, RequestedFeature,
    ConflictingFeature, RemainingConflicts...>
    : ConflictsWithFeatures<FeatureList, RequestedFeature, RemainingConflicts...>
{
  // Do something to check compatibility
};


int main()
{
  std::cout << std::is_base_of<Feature, void>::value << std::endl;
}
