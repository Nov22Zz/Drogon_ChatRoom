#pragma once

#include <drogon/WebSocketController.h>
#include <drogon/HttpResponse.h>
#include <unordered_map>
#include <string>

#include "chatpage.h"


using namespace drogon;



struct JsonMessage
{
  int id;
  int toid;
  std::string message;
};

struct ChatMessage
{
  std::string sender;
  std::string receiver;
  std::string message;
};




namespace controllers
{
  
  class userChatWeb : public drogon::WebSocketController<userChatWeb>
{
  public:
    void handleNewMessage(const WebSocketConnectionPtr&,
                                  std::string &&,
                                  const WebSocketMessageType &) override;
    void handleNewConnection(const HttpRequestPtr &,
                                    const WebSocketConnectionPtr&) override;
    void handleConnectionClosed(const WebSocketConnectionPtr&) override;
    
    WS_PATH_LIST_BEGIN
    // list path definitions here;
    // WS_PATH_ADD("/path", "filter1", "filter2", ...);
    WS_PATH_ADD("/user/chat",Get,Post,Options);
    
    
    WS_PATH_LIST_END

    void setResponseHeaders(const drogon::HttpResponsePtr& response) 
    {
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
        response->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
    }

  private:

};

}




