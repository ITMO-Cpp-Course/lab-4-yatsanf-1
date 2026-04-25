#include "../include/ResourceManager.hpp"

namespace lab4::resource
{

std::shared_ptr<FileHandle> ResourceManager::getFile(const std::string& filename)
{
    auto it = cache_.find(filename); // ищем файл в хеш-таблице с именем filename

    if (it != cache_.end()) // если  не указывает на конец
    {
        auto ptr = it->second.lock(); // достаем из кеша слабую ссылку и превращаем в сильную
        if (ptr)                      // файл жив
        {
            return ptr; // нашли! возвращаем из кеша
        }
        // Не нашли (файл уже удалился) - удаляем из кеша
        cache_.erase(it);
    }

    auto newFile =
        std::make_shared<FileHandle>(filename); // создает новый объект в FileHandle и оборачивает его в умный указатель
    cache_[filename] = newFile;                 // создаем запись по ключу в кеше
    return newFile;
}

void ResourceManager::cleanCache()
{
    // Проходим по всем записям в кеше
    for (auto it = cache_.begin(); it != cache_.end();) // проходим с первого элемента кеша по последний
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