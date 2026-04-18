#include "FileHandle.h"
#include "ResourceError.h"
#include "ResourceManager.h"
#include <catch2/catch_all.hpp>

using namespace lab4::resource;

TEST_CASE("RAII: файл автоматически закрывается при уничтожении объекта", "[raii]")
{
    {
        FileHandle f("test_raii.txt");
        f.write("Привет, мир!\n");
        REQUIRE(f.isOpen() == true);
    }
    FileHandle f2("test_raii.txt");
    REQUIRE(f2.isOpen() == true);
}

TEST_CASE("Перемещение: можно передать владение файлом другому объекту", "[move]")
{
    FileHandle f1("test_move.txt");
    f1.write("Строка от f1\n");
    REQUIRE(f1.isOpen() == true);

    FileHandle f2 = std::move(f1);

    REQUIRE(f1.isOpen() == false);
    REQUIRE(f2.isOpen() == true);

    f2.write("Строка от f2\n");
}

TEST_CASE("Ошибки: выбрасывается ResourceError", "[errors]")
{
    REQUIRE_THROWS_AS(FileHandle("///invalid///path///file.txt"), ResourceError);

    FileHandle f("test_errors.txt");
    f.write("Тестовый текст\n");
    f.close();

    REQUIRE_THROWS_AS(f.write("Эта запись не сработает"), ResourceError);
    REQUIRE_THROWS_AS(f.read(), ResourceError);
}

TEST_CASE("Работа с файлом: запись и чтение работают корректно", "[read_write]")
{
    FileHandle f("test_readwrite.txt");

    f.write("Первая строка\n");
    f.write("Вторая строка\n");

    std::string content = f.read();

    REQUIRE(content.find("Первая строка") != std::string::npos);
    REQUIRE(content.find("Вторая строка") != std::string::npos);
}

TEST_CASE("ResourceManager: кеш возвращает один и тот же файл при повторном запросе", "[cache]")
{
    ResourceManager rm;

    auto file1 = rm.getFile("test_cache.txt");
    file1->write("Запись через первый указатель\n");

    auto file2 = rm.getFile("test_cache.txt");
    file2->write("Запись через второй указатель\n");

    REQUIRE(file1.get() == file2.get());

    auto file3 = rm.getFile("test_cache.txt");
    std::string content = file3->read();

    REQUIRE(content.find("Запись через первый указатель") != std::string::npos);
    REQUIRE(content.find("Запись через второй указатель") != std::string::npos);
}

TEST_CASE("ResourceManager: cleanCache удаляет мёртвые файлы из кеша", "[clean]")
{
    ResourceManager rm;

    {
        auto temp = rm.getFile("temp.txt");
        temp->write("Временный файл\n");
    }
    rm.cleanCache();

    auto newFile = rm.getFile("temp.txt");
    REQUIRE(newFile->isOpen() == true);
}

TEST_CASE("Несколько пользователей могут работать с одним файлом одновременно", "[shared]")
{
    ResourceManager rm;

    auto user1 = rm.getFile("shared_users.txt");
    auto user2 = rm.getFile("shared_users.txt");
    auto user3 = rm.getFile("shared_users.txt");

    user1->write("Сообщение от user1\n");
    user2->write("Сообщение от user2\n");
    user3->write("Сообщение от user3\n");

    auto reader = rm.getFile("shared_users.txt");
    std::string content = reader->read();

    REQUIRE(content.find("user1") != std::string::npos);
    REQUIRE(content.find("user2") != std::string::npos);
    REQUIRE(content.find("user3") != std::string::npos);
}

TEST_CASE("ResourceManager: разные имена файлов - разные объекты", "[different]")
{
    ResourceManager rm;

    auto fileA = rm.getFile("fileA.txt");
    auto fileB = rm.getFile("fileB.txt");

    fileA->write("Это файл A");
    fileB->write("Это файл B");

    REQUIRE(fileA.get() != fileB.get());
    REQUIRE(fileA->read() != fileB->read());
}