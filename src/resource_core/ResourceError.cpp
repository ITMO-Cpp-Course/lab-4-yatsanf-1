#include "ResourceError.h"

namespace lab4::resource
{
ResourceError::ResourceError(const std::string& msg) : message_(msg) {}

const char* ResourceError::what() const noexcept
{
    return message_.c_str();
}
} // namespace lab4::resource