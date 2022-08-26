#ifndef U_RESTORER
#define U_RESTORER
#include <filesystem>
#include <exception>
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <streambuf>

namespace restorer
{
namespace fs = std::filesystem;
/****************************************************
* @name Backuper
* @author Maksimov Denis (dn.maksimow@gmail.com)
* @brief копирует рекурсивно с заменой существующей папки  резервной
* @details description
****************************************************/
class Backuper
{
public:
    // (0)Деструктор
    ~Backuper();

    // (1)Конструктор со списком инициализации
    // Backuper(/* args */)=delete;
    Backuper(/* args */);
    Backuper(const int argc, const char** argv);
    Backuper(fs::path& src, fs::path& dst);
    // (2)Конструктор копирования
    Backuper(const Backuper& other);

    // (3)Конструктор перемещения, noexcept - для оптимизации при использовании стандартных контейнеров
    Backuper(Backuper&& other) noexcept;

    // (4)Оператор присваивания копированием (copy assignment)
    Backuper& operator=(const Backuper& other);

    // (5)Оператор присваивания перемещением (move assignment)
    Backuper& operator=(Backuper&& other) noexcept;

private:
    /*data*/
    fs::path source;
    fs::path target;
    void check_args(const int argc, const char** argv) const;
    void check_paths() const;
    void r_copy(fs::path const & src,fs::path const & dst) const;
public:
    /*public methods*/
    void set_source(const char*) noexcept;
    void set_target(const char*) noexcept;
    void restore() const;
    
};







};


#endif /* U_RESTORER */
