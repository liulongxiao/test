#include"config_parse/ConfigParser.h"
#include"gateway/ctp/gwinclude/CtpGateway.h"
#include"main_engine/MainEngine.h"
#include"gateway/FileGateWay.h"
#include"strategy/StrategyBase.h"
#include"tools/ma.h"

#define Sleep(n) std::this_thread::sleep_for(std::chrono::seconds(n))

class PrintStrategy:public StrategyBase{
public:
    PrintStrategy():StrategyBase(),ma100(100),ma200(200){}
    void OnTick(EventPtr eventPtr)override {
        L1Tick * data =static_cast<L1Tick *>(eventPtr->getData());
        cout<<data->Symbol<<endl;
        cout<<data->lastPrice<<" MA100:"<<ma100.data()<<"MA200:"<<ma200.data()<<endl;
        ma200.push(data->lastPrice);
        ma100.push(data->lastPrice);

        if(ma100.data()>ma200.data()*1.0005)
            insertLimitOrder("ctp",'K',data->Symbol,'L',1,data->lastPrice);
        if(ma100.data()*1.0005<ma200.data())
            insertLimitOrder("ctp",'P',data->Symbol,'L',1,data->lastPrice);
    }


    void RegistAll(EventBusPtr eventBusPtr)override {
        RegistEvent(eventBusPtr, "L1TickEvent", std::bind(&PrintStrategy::OnTick, this, std::placeholders::_1));
    }

    Ma ma200;
    Ma ma100;
};


int main(){

    std::shared_ptr<StrategyBase> l(new PrintStrategy);
    std::shared_ptr<MainEngine> mengine(new MainEngine);
    std::shared_ptr<EventEngine> eengine(new EventEngine);
    l->SetMainEngine(mengine.get());
    mengine->SetEngine(eengine);
    mengine->AddEventListener(l);
    std::shared_ptr<GateWay> g(new CtpGateway("/home/liulongxiao/bt/config_file/ctpconfig"));
    mengine->AddGateWay("ctp",g);
    mengine->Start();

    mengine->SubscribeMarketData("ctp",{"rb1810"});
    //mengine->QryAvaliableCash("ctp");
    getchar();

}
