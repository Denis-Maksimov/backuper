#ifndef V_iObserver
#define V_iObserver
#include <thread>
#include <mutex>
#include <memory>
#include <algorithm>
#include <iostream>
#include <vector>
#include <stack>
namespace nsObserver{
//-------------------------- iObserver ------------------------------------------
/**
 * @brief наблюдатель. Объекты делать в виде std::shared_ptr!!!
 *  Наблюдателя потом можно прицепить на субъект наблюдения
 * @tparam EData  - тип любых данных, которые будут отслеживаться
 * @tparam TEvent - тип события, для фильтрации нотификации
 */
template <class EData, typename TEvent>
class iObserver
{
public:
    virtual ~iObserver()
    {
    }
    virtual void onNotify(EData &, TEvent) = 0;
};

//-------------------------- SUBJECT -------------------------------------------
/**
 * @brief субъект наблюдения. К нему подцепляют наблюдателей,
 * когда вызывается notify(), то в новых потоках обрабатываются
 * обработчики onNotify() наблюдателей
 * 
 * @tparam EData  - тип любых данных, которые будут отслеживаться
 * @tparam TEvent - тип события, для фильтрации нотификации
 */
template <class EData, typename TEvent>
class iSubject
{
public:
    void addObserver(std::shared_ptr<iObserver<EData, TEvent>> &&apiObserver)
    {
        iObservers_.push_back(std::move(apiObserver));
    }
    void removeObserver(std::shared_ptr<iObserver<EData, TEvent>> &&iObserver)
    {
        auto p = std::find(iObservers_.begin(), iObservers_.end(), iObserver);
        if (p != iObservers_.end())
        {
            iObservers_.erase(p);
        }
    }
private:
    //деструктор потоков, выполняется в основном потоке
    void (*ThreadDeleter)(std::thread *t) =
            [](std::thread *t)
            {
                t->join();
                delete t;
            };
    std::vector<std::shared_ptr<iObserver<EData, TEvent>>> iObservers_;
    using ThreadPtr = std::unique_ptr<std::thread, decltype(ThreadDeleter)>;
    using ThreadPool = std::stack<ThreadPtr>;
    
protected:
    void notify(EData &aData, TEvent aEevent)
    {
        ThreadPool tp;
        //вызываем обработчики наблюдателей в отдельных потоках
        for (auto &i : iObservers_)
        {
            ThreadPtr Thread(new std::thread(
                                 [&i, &aData, &aEevent]()
                                 { i->onNotify(aData, aEevent);
                                 }),
                             ThreadDeleter);
            tp.push(std::move(Thread));
        }
        
        //<--где-то здесь вызывается деструктор потоков из уничтожения tp
    }


};
};
#endif /* V_iObserver */
