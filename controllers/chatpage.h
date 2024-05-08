#pragma once

#include <drogon/HttpController.h>

#include <drogon/WebSocketController.h>
#include <drogon/WebSocketConnection.h>
#include <trantor/net/TcpConnection.h>


#include "../models/Users.h"
#include "../models/Friends.h"
#include "../models/Groupuser.h"
#include "../models/Allgroup.h"
#include "../models/Offlinemessages.h"
#include "../service/UserModel.h"


using namespace drogon;





class chatpage : public drogon::HttpController<chatpage>
{
  public:
    METHOD_LIST_BEGIN
    // use METHOD_ADD to add your custom processing function here;
    // METHOD_ADD(chatpage::get, "/{2}/{1}", Get); // path is /chatpage/{arg2}/{arg1}
    // METHOD_ADD(chatpage::your_method_name, "/{1}/{2}/list", Get); // path is /chatpage/{arg1}/{arg2}/list
    // ADD_METHOD_TO(chatpage::your_method_name, "/absolute/path/{1}/{2}/list", Get); // path is /absolute/path/{arg1}/{arg2}/list
    ADD_METHOD_TO(chatpage::loginPage, "/login", Get); 
    ADD_METHOD_TO(chatpage::doLogin,"/login",Post);
    ADD_METHOD_TO(chatpage::doregister,"/register",Post);
    
    
    ADD_METHOD_TO(chatpage::getchatPage,"/user/chat",Get);
    ADD_METHOD_TO(chatpage::addfriend,"/user/chat/addfriend",Post);
    ADD_METHOD_TO(chatpage::addgroup,"/user/chat/addgroup",Post);
    ADD_METHOD_TO(chatpage::oneChat,"/user/chat/oneChat",Post);

    METHOD_LIST_END
    // your declaration of processing function maybe like this:
    // void get(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, int p1, std::string p2);
    // void your_method_name(const HttpRequestPtr& req, std::function<void (const HttpResponsePtr &)> &&callback, double p1, int p2) const;


    void loginPage (const HttpRequestPtr& req,
    std::function<void (const HttpResponsePtr &)> &&callback);

    void doLogin   (const HttpRequestPtr& req, 
    std::function<void (const HttpResponsePtr &)> &&callback); //drogon_model::db::Users  //User users ,User users
    
    void doregister(const HttpRequestPtr& req, 
    std::function<void (const HttpResponsePtr &)> &&callback); //drogon_model::db::Users

    void getchatPage(const HttpRequestPtr& req, 
    std::function<void (const HttpResponsePtr &)> &&callback);

    void addfriend(const HttpRequestPtr& req, 
    std::function<void (const HttpResponsePtr &)> &&callback);
    
    void addgroup(const HttpRequestPtr& req, 
    std::function<void (const HttpResponsePtr &)> &&callback);

    void oneChat(const HttpRequestPtr& req,  
    std::function<void (const HttpResponsePtr &)> &&callback);

    void groupChat(const HttpRequestPtr& req,  
    std::function<void (const HttpResponsePtr &)> &&callback);

    
  private:

  service::UserModel _userModel;
  service::FriendModel _friendModel;
  service::GroupModel _groupModel;


};






