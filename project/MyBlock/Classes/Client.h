#pragma once
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "network/HttpClient.h"
#include <string>
#include <functional>
using namespace std;
using namespace cocos2d;
using namespace cocos2d::network;
class Client {
private:
    static Client* _instance;
    string _username = "Anonymous";
    string _token = "0";
    Client() {};
public:
    static Client* getInstance() {
        return _instance;
    };
    void login(string username, string password, function<void(bool, string)>);
    void reg(string username, string password, function<void(bool, string)>);
    void updatePassword(string oldPassword, string newPassword, function<void(bool, string)>);
    void ranklist(function<void(bool, string, vector<pair<string, int>>)> _cb);
    void updateScore(int newScore, function<void(bool, string)>_cb);
};