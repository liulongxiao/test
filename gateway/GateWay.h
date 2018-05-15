//
// Created by root on 4/26/18.
//

#ifndef BT_GATEWAY_H
#define BT_GATEWAY_H
#include<memory>
#include<thread>
#include "../engine/EventEngine.h"
#include "../events/L1TickEvent.h"
#include "../events/TradeEvent.h"
#include "../events/OrderEvent.h"
#include "../events/PositionEvent.h"

class GateWay {
public:
    GateWay():__active(false){}

    void SetEngine(EventEnginePtr eve_ptr){
        eventEnginePtr=eve_ptr;
    }
    virtual void Start(){
        __active=true;
    }
    virtual void Stop() {
        __active=false;
    }



    void PublishEvent(EventPtr p){
        eventEnginePtr->PushEvent(p);
    }


    void OnL1Tick(L1TickPtr l1TickPtr){
        EventPtr evn(new L1TickEvent(l1TickPtr));
        PublishEvent(evn);
    }

    void OnTrade(TradeDataPtr tradeDataPtr){
        EventPtr evn(new TradeEvent(tradeDataPtr));
        PublishEvent(evn);
    }

    void OnOrder(OrderDataPtr orderDataPtr){
        EventPtr evn(new OrderEvent(orderDataPtr));
        PublishEvent(evn);
    }

    void OnPosition(PositionDataPtr positionDataPtr)
    {
        EventPtr evn(new PositionEvent(positionDataPtr));
        PublishEvent(evn);
    }


    void OnAccount()
    {

    }


    virtual void SubscribeMarketData(std::vector<std::string> symbol_list){}
    virtual void UnSubscribeMarketData(std::vector<std::string> symbol_list){};
    virtual void insertMarketOrder(char offsetflag,const string& symbol,char side,float quantity){}
    virtual void insertLimitOrder(char offsetflag,const string& symbol,char side,float quantity,float price){}
    virtual void QryPosition(){};
    virtual void QryPosition(const std::string & symbol){}
    virtual void QryAvaliableCash(){}

protected:
    volatile  bool __active;
    EventEnginePtr eventEnginePtr;


};

typedef std::shared_ptr<GateWay> GateWayPtr;

#endif //BT_GATEWAY_H
