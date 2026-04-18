#include "ResourceManager.h"

namespace lab4::resource
{

std::shared_ptr<FileHandle> ResourceManager::getFile(const std::string& filename)
{
    auto it = cache_.find(filename);

    if (it != cache_.end())
    {
        auto ptr = it->second.lock();
        if (ptr)
        {
            return ptr; // нашли! возвращаем из кеша
        }
        // Не нашли (файл уже удалился) - удаляем из кеша
        cache_.erase(it);
    }

    // Если не нашли в кеше - создаём новый файл
    auto newFile = std::make_shared<FileHandle>(filename);
    // Сохраняем в кеш
    cache_[filename] = newFile;
    return newFile;
}

void ResourceManager::cleanCache()
{
    // Проходим по всем записям в кеше
    for (auto it = cache_.begin(); it != cache_.end();)
    {
        // Если файл уже мёртвый (expired)
        if (it->second.expired())
        {
            // Удаляем запись из кеша
            it = cache_.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

} // namespace lab4::resource