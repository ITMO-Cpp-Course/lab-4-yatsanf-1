#pragma once

#include <stdexcept> //Стандартные ошибки

namespace lab4::resource
{

class ResourceError : public std::runtime_error
{
  public:
    explicit ResourceError(const std::string& message);
};
} // namespace lab4::resource
