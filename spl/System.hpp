#ifndef SPL_SYSTEM_HPP
#define SPL_SYSTEM_HPP

#include <map>
#include <unordered_map>
#include <string>
#include <vector>

namespace spl {

class Plugin;
class Interface;

struct InterfacePluginPair
{
  void* interface;
  Plugin* plugin;
};

class System
{
public:


  std::map<std::string, std::vector<void*>> _interfaces;

  std::unordered_map<void*, Plugin*> _interfaceToPlugin;

};

} // namespace spl

#endif // SPL_SYSTEM_HPP
