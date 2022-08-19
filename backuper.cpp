#include <filesystem>
#include <exception>
#include <iostream>
#include <string>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <streambuf>
/**
 * @brief 
 * @note compile with g++ ./backuper.cpp -std=c++17 -lstdc++fs
 */
namespace fs = std::filesystem;

class Custom_error : public std::exception {
    private:
        std::string message;
        bool n=false;

    public:
        Custom_error(std::string const & msg) : message(msg){
            if(msg=="destination folder is exist!")
            {
                this->n=true;
            }
        }
        const char * what() const noexcept override{
            return message.c_str();
        }
        bool ne()const{
            return this->n;
        }
};



bool m_copy(fs::path const & src,fs::path const & dst) 
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
             if(!m_copy(current,dst/current.filename()))
             {
                 //хз, наверн никогда не вызовется
                 throw Custom_error("m_copy error!");
             }
         }
         else
         {
             fs::copy_file(
                 current,
                 dst/current.filename()
             );
         }
                 
    }
    return true;
    
    
}


int main(int argc, char const *argv[])
{
    
   auto time = std::time(nullptr);
    try
    {
        
        if (argc != 3)
        {
            throw Custom_error("invalid arguments count!");
        }
        std::string source{argv[1]};
        std::string target{argv[2]};
    
    
        try
        {
            m_copy(fs::path(source),fs::path(target));
        }
        catch(const Custom_error& e)
        {
            
            if(e.ne()){
                std::stringstream ss{};
                ss<<target;
                ss.imbue(std::locale("ru_RU.utf8"));
                ss<<std::put_time(std::gmtime(&time), "%c");
                fs::rename(fs::path(target),fs::path(ss.str()));
                m_copy(fs::path(source),fs::path(target));
                // m_copy("./src","./copy_src");
            }else{
                std::cerr << e.what() << '\n';
            }
        }
        
        
    }
    catch(const std::exception& e)
    {
        

        std::ofstream f("logfile.txt",std::ofstream::app);
        f.imbue(std::locale("ru_RU.utf8"));
        f << std::put_time(std::gmtime(&time), "%c")<< "::";
        f<<e.what()<<std::endl;
        
        std::cerr << e.what() << '\n';
    }
    return 0;
}
