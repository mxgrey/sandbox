
#include <iostream>
#include <set>
#include <unordered_set>
#include <random>
#include <string>
#include <chrono>

static const std::vector<char>& charset()
{
  static std::vector<char> characters(
      {'0','1','2','3','4',
      '5','6','7','8','9',
      'A','B','C','D','E','F',
      'G','H','I','J','K',
      'L','M','N','O','P',
      'Q','R','S','T','U',
      'V','W','X','Y','Z',
      'a','b','c','d','e','f',
      'g','h','i','j','k',
      'l','m','n','o','p',
      'q','r','s','t','u',
      'v','w','x','y','z'
      });

    return characters;
}

class StringGenerator
{
public:

  StringGenerator(const std::size_t L)
    : rd(),
      mt(rd()),
      c_dist(0, charset().size()-1),
      len(L)
  {
    // Do nothing
  }

  std::string getRandomString()
  {
    std::string str;
    for(std::size_t l=0; l < len; ++l)
      str.push_back(charset()[c_dist(mt)]);

    return str;
  }

protected:

  std::random_device rd;
  std::mt19937 mt;
  std::uniform_int_distribution<std::size_t> c_dist;
  const std::size_t len;

};


template<typename Container>
void testLookupForNRandomStrings(const std::vector<std::string>& database)
{
  const std::size_t N = database.size();
  const std::size_t len = database.front().size();

  std::size_t duplicateCount = 0;
  Container container;
  const auto insert_start = std::chrono::high_resolution_clock::now();
  for(std::size_t i=0; i < N; ++i)
  {
    if(!container.insert(database[i]).second)
      ++duplicateCount;
  }
  const auto insert_finish = std::chrono::high_resolution_clock::now();

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<std::size_t> n_dist(0, N-1);
  std::vector<std::size_t> indices;
  for(std::size_t i=0; i < 2*N; ++i)
    indices.push_back(n_dist(mt));

  const auto lookup_start = std::chrono::high_resolution_clock::now();
  for(std::size_t i=0; i < 2*N; ++i)
  {
    const std::string& r_string = database[indices[i]];
    if(container.find(r_string) == container.end())
      std::cout << "BUG! Could not find entry: " << r_string << std::endl;
  }
  const auto lookup_finish = std::chrono::high_resolution_clock::now();

  StringGenerator strGen(len);
  std::vector<std::string> randomStrings;
  randomStrings.reserve(10*N);
  for(std::size_t i=0; i < 10*N; ++i)
    randomStrings.push_back(strGen.getRandomString());

  // A database for strings which do not exist in the set
  std::vector<std::string> complementbase;
  for(std::size_t i=0; i < 10*N; ++i)
  {
    const std::string& str = randomStrings[i];
    if(container.find(str) == container.end())
      complementbase.push_back(str);

    if(complementbase.size() >= N)
      break;
  }

  const auto fail_start = std::chrono::high_resolution_clock::now();
  for(std::size_t i=0; i < complementbase.size(); ++i)
  {
    const std::string& c_string = complementbase[i];
    if(container.find(c_string) != container.end())
      std::cout << "BUG! Somehow found entry: " << c_string << std::endl;
  }
  const auto fail_finish = std::chrono::high_resolution_clock::now();


  const auto insert_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
        insert_finish - insert_start).count();
  const auto lookup_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
        lookup_finish - lookup_start).count();
  const auto fail_time = std::chrono::duration_cast<std::chrono::nanoseconds>(
        fail_finish - fail_start).count();

  std::cout << "with N=" << N << " and len=" << len << " | "
            << "insert: " << insert_time
            << ", lookup: " << lookup_time
            << ", fail: " << fail_time
            << " | total: " << insert_time + lookup_time + fail_time << std::endl;
}

std::vector<std::string> generateDatabase(
    const std::size_t N,
    const std::size_t len)
{
  std::vector<std::string> database;
  database.reserve(N);

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<std::size_t> n_dist(0, N-1);
  StringGenerator strGen(len);

  for(std::size_t i=0; i < N; ++i)
    database.push_back(strGen.getRandomString());

  return database;
}

void testBothLookupsForNRandomStrings(
    const std::size_t N,
    const std::size_t len)
{
  std::vector<std::string> database = generateDatabase(N, len);

  std::cout << "For set ";
  testLookupForNRandomStrings<std::set<std::string>>(database);

  std::cout << "For uno ";
  testLookupForNRandomStrings<std::unordered_set<std::string>>(database);
  std::cout << "\n";
}


int main()
{
  const std::vector<std::size_t> N = { 5, 10, 25, 50, 100, 1000 };
  const std::vector<std::size_t> len = { 5, 10, 25, 50, 100, 200, 500, 1000, 1500, 2000 };

  for(std::size_t n : N)
  {
    std::cout << " --------------------------------- \n"
              << "Beginning new set size: " << n << std::endl;
    for(std::size_t l : len)
    {
      testBothLookupsForNRandomStrings(n, l);
    }
  }
}
