#include "config.hpp"

#include <cassert>
#include <filesystem>
#include <string>

static std::string s_application_name;

void sui::Config::set_application_name(const std::string &name)
{
  s_application_name = name;
}

const std::string_view sui::Config::get_application_name()
{
  return s_application_name;
}

const std::string sui::Config::get_config_dir()
{
  // assert, that the application name has been set before this function is
  // called
  assert(s_application_name != "");
  const auto app_name = sui::Config::get_application_name();

  const char *home = getenv("HOME");
  if (home)
  {
    std::filesystem::path p(home);
    p = p / ".config" / app_name / "theme.conf";
    return p.string();
  }

  return {};
}
