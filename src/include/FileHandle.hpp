#pragma once

#include <fstream>
#include <string>

namespace lab4::resource
{

class FileHandle
{
  private:
    std::fstream file_;
    std::string name_;
    bool opened_;

  public:
    explicit FileHandle(const std::string& filename);
    ~FileHandle();
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    FileHandle(FileHandle&& other) noexcept;
    FileHandle& operator=(FileHandle&& other) noexcept;
    void write(const std::string& text);
    std::string read();
    void close();
    bool isOpen() const
    {
        return opened_;
    }
};
} // namespace lab4::resource
