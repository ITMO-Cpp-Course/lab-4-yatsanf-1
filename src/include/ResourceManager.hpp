#pragma once

#include "FileHandle.hpp"
#include <memory>
#include <string>
#include <unordered_map>

namespace lab4::resource
{

class ResourceManager
{
  private:
    std::unordered_map<std::string, std::weak_ptr<FileHandle>> cache_;

  public:
    // Получить файл (даст из кеша, если есть, или создаст новый)
    std::shared_ptr<FileHandle> getFile(const std::string& filename);
    void cleanCache();
};

} // namespace lab4::resource
