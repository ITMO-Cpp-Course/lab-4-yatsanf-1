#include "../include/FileHandle.hpp"
#include "../include/ResourceError.hpp"

namespace lab4::resource
{

FileHandle::FileHandle(const std::string& filename) : name_(filename), opened_(false)
{
    file_.open(filename, std::ios::in | std::ios::out |
                             std::ios::app); // Открывает файл в режиме: читать, писать, дописывать в конец

    if (!file_.is_open())
    {
        throw ResourceError("Не могу открыть файл: " + filename);
    }
    opened_ = true;
}

FileHandle::~FileHandle()
{
    close();
}

FileHandle::FileHandle(FileHandle&& other) noexcept // принимает временную ссылкку и обещает не выбрасывать ошибку
    : file_(std::move(other.file_)), name_(std::move(other.name_)),
      opened_(other.opened_) // забираем файл, имя и статус
{
    other.opened_ = false;
}

FileHandle& FileHandle::operator=(FileHandle&& other) noexcept
{
    if (this != &other)
    {
        close();
        file_ = std::move(other.file_);
        name_ = std::move(other.name_);
        opened_ = other.opened_;
        other.opened_ = false;
    }
    return *this;
}

void FileHandle::write(const std::string& text)
{
    if (!opened_)
    {
        throw ResourceError("Файл закрыт: " + name_);
    }

    // Сбрасываем флаги ошибок
    file_.clear();
    // Перемещаемся в конец файла (чтобы дописывать)
    file_.seekp(0, std::ios::end);
    // Пишем текст
    file_ << text;
    // Сохраняем на диск
    file_.flush();
}

std::string FileHandle::read()
{
    if (!opened_)
    {
        throw ResourceError("Файл закрыт: " + name_);
    }

    file_.clear();
    file_.seekg(0, std::ios::beg);

    std::string content;
    std::string line;

    // Читаем файл построчно
    while (std::getline(file_, line))
    {
        content += line + "\n";
    }
    return content;
}

void FileHandle::close()
{
    if (opened_)
    {
        file_.close();
        opened_ = false;
    }
}
} // namespace lab4::resource