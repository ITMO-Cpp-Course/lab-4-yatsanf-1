#pragma once

#include <exception>
#include <string>

namespace lab4::resource
{

class ResourceError : public std::exception
{
  private:
    std::string message_;

  public:
    explicit ResourceError(const std::string& message);
    const char* what() const noexcept override;
};
} // namespace lab4::resource
