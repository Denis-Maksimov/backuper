# *backuper*
копирует папку из *[1 аргумент]* в *[2 аргумент]* 
если *[2 аргумент]* есть, то он переименновывается в *[2 аргумент_(дата)]*
если какая-то ошибка, то создаётся лог файл

```c++
#include "u_restorer.hpp"

/***************************************
 * --первый аргумент (argv[1]) = source
 * --второй аргумент (argv[2]) = target 
 ***************************************/
int main(int argc, char const *argv[])
{
    
    restorer::Backuper b(argc,argv);
    b.restore();
    
    restorer::Backuper c;
    c.set_source("./backup_directory");
    c.set_target("./crashed_directory");
    c.restore();
    
    return 0;
}

```

### зачем это нужно?
для здорового сна, чтобы в случае краха системы на работе не мчаться туда восстанавливать или не объяснять 1000 слов очевидцам как восстановить, а просто сказать, мол "запускайте восстановитель" и профит..
Да, можно было бы скриптом прописать, но тут есть проверки аргументов, все дела..

## todo
- создать конфиг, где прописывается путь к аргументам, ну и соответственно эти конфиги сохраняются
- какой-то мало-мальский интерфейс, чтобы понять, что всё супер
- журнал событий можно ещё.. но это не точно..
- работа с архивами

-----------------------------
# *OBSERVER*
Очередной велосипед паттерна "Наблюдатель". Попытался сделать через шаблоны.. 
Основу взял по мотивам данного ролика:
https://www.youtube.com/watch?v=8swJUrIfvIk&list=PLdhEOd5Bckb54U_bm9pS3eMQldHDyA8h_&index=26

### в чём отличия?
* изменил хранение наблюдателей в стеке 
* изменил сырые указатели на std::shared_ptr
* перевёл в шаблонный вид
* обработку onNotify() сделал в многопоточном исполнении так как обработчики могут быть ужасно долгими
-----------------------------
### короткое описание

- Имеется "субъект" наблюдения и "наблюдатели"
- субъект добавляет наблюдателей, у которых прописаны callback'и (onNotify) 
- субъект выполняяет какие-то действия и при возникновении каких-то событий вызывает notify()
- наблюдатели сделаны тупо для того, чтобы хранить кэлбеки и данные, которые меняются в кэлбеках

``На что обратить внимание``
- доступ к данным осуществлять через мьютекс или другую блокировку
- объекты наблюдателей создавать через std::shared_ptr (но это  не точно..)

*P.S. Вроде как в C++20 встроен этот паттерн, так что я точно велосипедист..*




