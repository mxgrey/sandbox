
#include <random> // Needed to create random number generation tools

#include <iostream> // Needed to print stuff out

int main()
{
  // First, we create a device that will pull randomness from entropy
  // in the hardware of the computer. This is a mildly expensive
  // operation, so we only do it once at startup and then use it as
  // a truly random seed for our pseudo-random number generator.
  std::random_device device;

  // This is our pseduo-random number generator. STL provides several
  // options to choose from, but the Mersenne Twister engine (mt19937)
  // is the most popular, so we will use that one.
  std::mt19937 generator(device());

  // Note above that we must pass device() as an argument to the
  // constructor of std::mt19937. Calling device() will generate the
  // random seed, and std::mt19937 requires a random seed during its
  // construction.


  // This allows us to create a uniform distribution of "real number"
  // values in the range [0.0 to 1.0) with double precision. Note that
  // the set is open on the right side.
  std::uniform_real_distribution<double> uniformRealDist(0.0, 1.0);

  // Create a bunch of random values in a for-loop
  std::vector<double> uniform_values;
  std::cout << "Uniform values: ";
  for(std::size_t i=0; i < 10; ++i)
  {
    // Pass the generator into the distribution to produce a
    // pseudo-random number.
    const double value = uniformRealDist(generator);
    std::cout << value << ", ";

    // As we collect these generated values, they will be
    // uniformly distributed.
    uniform_values.push_back(value);
  }
  std::cout << std::endl;

  // This allows us to create a normal (a.k.a. Gaussian)
  // distribution of "real number" values with double precision.
  // The mean of the values will be 0.0, and the standard deviation
  // will be 1.0.
  std::normal_distribution<double> normalRealDist(0.0, 1.0);
  std::vector<double> normal_values;
  std::cout << "Normal values: ";
  for(std::size_t i=0; i < 10; ++i)
  {
    // We can use the same generator for both our uniform
    // distribution or our normal distribution.
    const double value = normalRealDist(generator);
    std::cout << value << ", ";

    // As we collect these generated values, they will have
    // a normal distribution.
    normal_values.push_back(value);
  }
  std::cout << std::endl;

  // We can also create uniform integer distributions. This will
  // uniformly produce interger values in the range [0, 100].
  // Note that for integers this set is closed (unlike for real numbers).
  std::uniform_int_distribution<int> uniformIntDist(0, 100);
  std::vector<int> int_values;
  std::cout << "Integer values: ";
  for(std::size_t i=0; i < 10; ++i)
  {
    // We can also use the same generator for making integers.
    const int value = uniformIntDist(generator);
    std::cout << value << ", ";

    int_values.push_back(value);
  }
  std::cout << std::endl;
}
