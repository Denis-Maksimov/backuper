#include "v_observer.hpp"
using namespace nsObserver;

//-------------------------- eEvent --------------------------------------------
enum class eEvent
{
  ChangeValue,
  All
};

//-------------------------- EVENT DATA ----------------------------------------
struct MarketEventData
{
  size_t price {0};
  std::mutex mtx;
  virtual ~MarketEventData() = default;
};
//-------------------------- *TraderEventData DATA ----------------------------------------
struct TraderEventData :MarketEventData
{
  size_t price2 {0};
};


//-------------------------- MARKET -------------------------------------------
class Market : public iSubject<MarketEventData,eEvent>
{
public:
  void updatePrice(TraderEventData& aTicker)
  {
    std::unique_lock<std::mutex> lock(aTicker.mtx);
    aTicker.price+=10;
    // lock.unlock();

    if (aTicker.price > 5)
    {
      aTicker.price2 = aTicker.price * 2;
    }

    lock.unlock();
    notify(aTicker, eEvent::ChangeValue);
  }
};
//-------------------------- TRADER --------------------------------------
class RobotMaker : public iObserver<MarketEventData,eEvent>
{

  void onNotify( MarketEventData& aData, eEvent aEvent) override
  {
    switch (aEvent)
    {
      case eEvent::All:
      case eEvent::ChangeValue:
      {
         TraderEventData &r_pb = dynamic_cast< TraderEventData&>(aData);
        std::unique_lock<std::mutex> lock(r_pb.mtx);
        bye(r_pb.price);
        // lock.unlock();
        break;
      }
    }
  }
private:
  void bye(size_t p)
  {
          std::cout << "покупка за "<< p<< std::endl;
  }

};

class HamsterMaker : public iObserver<MarketEventData,eEvent>
{

  void onNotify( MarketEventData& aData, eEvent aEvent) override
  {
    switch (aEvent)
    {
      case eEvent::All:
      {
        [[fallthrough]];
      }
       
      case eEvent::ChangeValue:
      {
        TraderEventData &r_pb = dynamic_cast< TraderEventData&>(aData);
        std::unique_lock<std::mutex> lock(r_pb.mtx);
        bye(r_pb.price2);
       
        break;
      }
    }
  }
private:
  void bye(size_t p)
  {
          std::cout << "покупка за "<< p<< std::endl;
  }

};
//===================================================================================================

//using example
int main(int argc, char const *argv[])
{
    //observers:
    std::shared_ptr<RobotMaker> mm(new RobotMaker);
    std::shared_ptr<HamsterMaker> hamster(new HamsterMaker);
    
    // observed data
    TraderEventData data; 
    
    //subject, notificator
    Market SPBmarket;     

    //subscribe observers to subject
    SPBmarket.addObserver(hamster);
    SPBmarket.addObserver(mm);
    
    //process...
    SPBmarket.updatePrice(data);
    SPBmarket.updatePrice(data);
    SPBmarket.updatePrice(data);
    SPBmarket.updatePrice(data);
    //unsubscribe
    SPBmarket.removeObserver(hamster);
    std::cout<<"hamster margin call"<<std::endl;
    //.. and again process ..
    SPBmarket.updatePrice(data);
    SPBmarket.updatePrice(data);
    SPBmarket.updatePrice(data);

    return 0;
}










