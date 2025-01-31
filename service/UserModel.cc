//#include <common/Response.h>
#include <models/Users.h>
#include <drogon/orm/Exception.h>
#include <drogon/drogon.h>
#include "UserModel.h"
#include "../utils/cryptopp.h"

using namespace drogon;
using namespace drogon::orm;
using namespace drogon_model::koi;



void service::UserModel::login(std::string &account,
                        std::string &password) 
{ 
    auto clientDb=drogon::app().getDbClient();
    Mapper<Users> mapper(clientDb);

    auto userInDb = 
            mapper.findOne({Users::Cols::_account,account});
    
    //--------------------
    //Cryptopp cryptopp;
    // utils::Cryptopp cryptopp;
    // std::string pass = password+userInDb.getValueOfSalt();

    /*
        if(cryptopp.hashPassword(pass)!=userInfo.getValueOfPassWord())
        {
            LOG_ERROR<<"密码错误";
        }

    */

    //--------------------
    if(userInDb.getValueOfPassword() != password)
    {
        LOG_ERROR<<"密码错误";
    }
      
}



void service::UserModel::registdo(std::string account,std::string &password)
{
    auto clientPtr = drogon::app().getDbClient();
    auto res = clientPtr->execSqlSync("select account from users where account = ?",account);
    
    try{
        if(res.empty())
        {   
            clientPtr->execSqlSync("INSERT INTO users(account,password) VALUES (?,?)",account,password);
            LOG_ERROR<<"注册成功";
        }
        else
        {
            LOG_ERROR<<"用户已存在";
        }
    }
    catch(const DrogonDbException& e){
            LOG_ERROR<<"注册失败";
    }

}

void service::UserModel::registdo(std::string account,std::string &password,std::string &nickname)
{
    auto clientPtr = drogon::app().getDbClient();
    auto res = clientPtr->execSqlSync("select account from users where account = ?",account);
    
    try{
        if(res.empty())
        {   

            clientPtr->execSqlSync("INSERT INTO users(account,password,nickname) VALUES (?,?,?)",
                        account,password,nickname);
    
            LOG_ERROR<<"注册成功";
        }
        else
        {
            LOG_ERROR<<"注册失败";
        }
    }
    catch(const DrogonDbException& e){
            LOG_ERROR<<"注册失败";
    }


}




void service::UserModel::updateState(const std::string state,int id)
{
    auto clientPtr = drogon::app().getDbClient();
    LOG_ERROR<<"数据库连接成功";
    try{
        auto res = clientPtr->execSqlSync("update users set state = ? where id=? ",state,id);
        LOG_ERROR<<id<<"  "<<"更新状态成功";
    }
    catch(const DrogonDbException& e){
        LOG_ERROR<<"状态更新失败";
    }
}



void service::UserModel::resetState()
{
    auto clientPtr = drogon::app().getDbClient();
        LOG_ERROR<<"数据库连接成功";
    try{
        auto res = clientPtr->execSqlSync("update user set state = 'offline' where state = 'online'");
        LOG_ERROR<<"更新状态成功";
    }
    catch(const DrogonDbException& e){
        LOG_ERROR<<"状态更新失败";
    }
    
}


void service::FriendModel::insert(int id,int friendid)
{
    auto clientPtr = drogon::app().getDbClient();

    try{
        auto res = clientPtr->execSqlSync("insert into friends values(?, ?),(?,?)",id,friendid,friendid,id);
        LOG_INFO<<"添加成功";
    }
    catch(const DrogonDbException& e){
        LOG_INFO<<"添加失败";
    }

}

std::vector<service::User> service::FriendModel::query(int id) //返回好友列表
{
    auto clientPtr = drogon::app().getDbClient();
    std::vector<service::User> vec;
    try{
        auto res = clientPtr->execSqlSync("select users.id,users.nickname  from users inner join friends on friends.friendid = users.id where friends.id=?",id);
        for(auto row : res)
        {
            int id = row["id"].as<int>();
            std::string nickname = row["nickname"].as<std::string>();
            //std::string state = row["state"].as<std::string>();
            LOG_INFO<<id;
            LOG_INFO<<nickname;
            //LOG_INFO<<state;

            User user;
            user.setId(id);
            user.setNickName(nickname);
            
            vec.push_back(user);
            
        }
        return vec;
        LOG_ERROR<<"查询成功";
    }catch(const DrogonDbException& e){
        LOG_ERROR<<"查询失败";
    }

}

void service::GroupModel::addGroup(int id,int groupid,std::string role)
{
    auto clientPtr = drogon::app().getDbClient();
    try{
        auto res = clientPtr->execSqlSync("insert into groupuser values(?, ?, ?)",groupid,id,role);
        LOG_ERROR<<"加入成功";
    }
    catch(const DrogonDbException& e){
        LOG_ERROR<<"加入失败";
    }
}

int service::GroupModel::createGroup(std::string groupname,std::string groupdesc,int id)
{
    auto clientPtr = drogon::app().getDbClient();
    //auto transPtr=clientPtr->newTransaction();
    int groupid;
    try{
        clientPtr->execSqlSync("insert into allgroup(groupname, groupdesc) values(?, ?)",groupname,groupdesc);
        auto res = clientPtr->execSqlSync("SELECT LAST_INSERT_ID()");      
        if(!res.empty())
        {        
            for(auto it:res)
            {     
                groupid = it["LAST_INSERT_ID()"].as<int>();  
                try
                {
                clientPtr->execSqlSync("insert into groupuser(groupid,id,grouprole) values(?, ?, ?)",groupid,id,"creator");   
                }
                catch(const std::exception& e)
                {
                    std::cerr << e.what() << '\n';
                } 
            }
        }
    return groupid;
    }
    catch(const DrogonDbException& e){
        LOG_ERROR<<"创建失败,该群组已存在";
    }
}

//根据指定groupid 查询群组用户id列表 ，除userid自己，主要用户群聊业务给群组其它成员群发消息
std::vector<int> service::GroupModel::queryGroupUsers(int id, int groupid)
{
    auto clientPtr = drogon::app().getDbClient();

    std::vector<int> idVec;
    try{
        auto res = clientPtr->execSqlSync("select id from groupuser where groupid = ? and id != ?",groupid,id);
        LOG_ERROR<<"查询成功";

        for(auto row:res)
        {
            int id = row["id"].as<int>();
            LOG_ERROR<<id;
            idVec.push_back(id);
        }

        return idVec;
    }
    catch(const DrogonDbException& e){
        LOG_ERROR<<"查询失败";
    }
}



std::vector<int> service::GroupModel::queryGroupUsers( int groupid)
{
    auto clientPtr = drogon::app().getDbClient();

    std::vector<int> idVec;
    try{
        auto res = clientPtr->execSqlSync("select id from groupuser where groupid = ? ",groupid);

        for(auto row:res)
        {
            int id = row["id"].as<int>();
            idVec.push_back(id);
        }

        return idVec;
    }
    catch(const DrogonDbException& e){
        LOG_ERROR<<"查询失败";
    }
    
}


//存储
void service::offlineMessageModel::insert(int id,std::string msg)
{
    auto clientPtr = drogon::app().getDbClient();
    try{
    auto res = clientPtr->execSqlSync("insert into offlinemessage values(?, ?)",id,msg);        
        LOG_ERROR<<"插入成功";
    }
    catch(const DrogonDbException& e){
        LOG_ERROR<<"插入失败";
    }

}



//移除
void service::offlineMessageModel::remove(int id)
{
    auto clientPtr = drogon::app().getDbClient();
    try{
    auto res = clientPtr->execSqlSync("delete from offlinemessage where userid=?",id);        
        LOG_ERROR<<"删除成功";
    }
    catch(const DrogonDbException& e){
        LOG_ERROR<<"删除失败";
    }

}

//查询用户离线消息
std::vector<std::string> service::offlineMessageModel::query(int id)
{
    auto clientPtr = drogon::app().getDbClient();
    std::vector<std::string> vec;
    try{
    auto res = clientPtr->execSqlSync("select message from offlinemessage where id = ?",id);        
        LOG_ERROR<<"成功";
        for(auto row :res)
        {
            std::string message = row["message"].as<std::string>();
            LOG_ERROR<<message;
            vec.push_back(message);
        }
        return vec;
    }   
    catch(const DrogonDbException& e){
        LOG_ERROR<<"失败";
    }

}


