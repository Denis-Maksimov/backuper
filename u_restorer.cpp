
#include "u_restorer.hpp"

namespace restorer
{

class Custom_error : public std::exception {
    private:
        std::string message;
    public:
        Custom_error(std::string const & msg) : message(msg){
        }
        Custom_error(const char* msg) : message(msg){
        }
        const char * what() const noexcept override{
            return message.c_str();
        }
};


//-------------------------------------------------
// (0)Деструктор со списком инициализации
Backuper::~Backuper()
{
    
}

Backuper::Backuper()
{
    this->source=0;
    this->target=0;
    // this->source=nullptr;
    // this->target=nullptr;
}
// (1)Конструктор со списком инициализации
Backuper::Backuper(const int argc, const char** argv)
{
    check_args(argc,argv);
    this->source=argv[1];
    this->target=argv[2];
    // check_paths();
}
Backuper::Backuper(fs::path& src, fs::path& dst)
:
source(src),
target(dst)
{
    check_paths();
}

// (2)Конструктор копирования
Backuper::Backuper(const Backuper& other)
{
    //copy other.data to this
    this->source=other.source;
    this->target=other.target;
}

// (3)Конструктор перемещения, noexcept - для оптимизации при использовании стандартных контейнеров
Backuper::Backuper(Backuper&& other) noexcept
{
    this->source=other.source;
    this->target=other.target;
    (void)other.source;
    (void)other.target;
    // other.source=nullptr;
    // other.target=nullptr;
    /*move other data to data data*/
    /*delete other.data*/
}
// (4)Оператор присваивания копированием (copy assignment)
Backuper& 
Backuper::operator=(const Backuper& other)
{
    if (this == &other)
        return *this;
    this->source=other.source;
    this->target=other.target;
    return *this;
}
// (5)Оператор присваивания перемещением (move assignment)
Backuper& 
Backuper::operator=(Backuper&& other) noexcept
{
    if (this == &other)
        return *this;
    this->source=other.source;
    this->target=other.target;
    (void)other.source;
    (void)other.target;
    // other.source=nullptr;
    // other.target=nullptr;
    /*delete data*/
    /*move other data to data data*/
    /*delete other.data*/
    return *this;
}

void
Backuper::check_paths() const
{
    // if(fs::path(nullptr)==source)
    // {
    //     throw Custom_error("error: bad params!");
    // }
    // if(fs::path(nullptr)==target)
    // {
    //     throw Custom_error("error: bad params!");
    // }
    if(!fs::exists(source))
    {
        throw Custom_error("error: source file isn't exist!");
    }
    if(!fs::is_directory(source))
    {
        throw Custom_error("source isn't directory!");
    }
    if(!fs::exists(target))
    {
        throw Custom_error("error: destination file isn't exist!");
    }
}

void
Backuper::check_args(const int argc, const char** argv) const
{
    if (argc != 3)
    {
        throw Custom_error("invalid arguments count!");
    }
    const char* src =argv[1];
    const char* dst =argv[2];
    
    if(!fs::exists(src))
    {
        std::cout<<src<<"\n";
        throw Custom_error("error: source file isn't exist!");
    }
    if(!fs::is_directory(src))
    {
        throw Custom_error("source isn't directory!");
    }
    if(!fs::exists(dst))
    {
        std::cout<<dst<<"\n";
        throw Custom_error("error: destination file isn't exist!");
    }
}

void
Backuper::restore() const
{
    check_paths();
    if(fs::exists(target))
    {
        std::stringstream ss{};
        ss<<target.c_str();
        // ss.imbue(std::locale("ru_RU.utf8"));
        auto t=std::time(nullptr);
        ss<<std::put_time(std::gmtime(&t), " %d-%m-%Y %R");
        fs::rename(fs::path(target),fs::path(ss.str()));
    }
    r_copy(source,target);

}



void
Backuper::set_source(const char* src) noexcept
{
    this->source=src;
}

void 
Backuper::set_target(const char* dst) noexcept
{
    this->target=dst;
    
}


void 
Backuper::r_copy(fs::path const & src,fs::path const & dst) const
{

    if(!fs::exists(src))
    {
        throw Custom_error("error: source file isn't exist!");
    }
    if(!fs::is_directory(src))
    {
        throw Custom_error("source isn't directory!");
    }
    if(fs::exists(dst))
    {
        throw Custom_error("destination folder is exist!");
    }
    if(!fs::create_directory(dst))
    {
        throw Custom_error("unable to create directory!");
    }

    for ( fs::directory_iterator file(src);   file != fs::directory_iterator(); ++file )
    {
         fs::path current(file->path());
         if (fs::is_directory(current))
         {
             r_copy(current,dst/current.filename());
         }
         else
         {
             fs::copy_file(
                 current,
                 dst/current.filename()
             );
         }
                 
    }   
    
}

//-------------------------------------------------
};
