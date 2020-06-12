#include "Client.h"
#include <sstream>
Client* Client::_instance = new Client();
const string baseUrl = "http://mirror.bjtu.edu.cn/se_homework/";
void Client::login(string username, string password, function<void(bool, string)> _cb) {
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(baseUrl + "login/" + username + "/" + password);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([_cb,this,username](HttpClient* sender, HttpResponse* response) {
		if (!response)
		{
			return;
		}
		if (!response->isSucceed())
		{
			log("response failed");
			log("error buffer: %s", response->getErrorBuffer());
			_cb(false, "response failed");
			return;
		}
		auto* buffer = response->getResponseData();
		string s = string(buffer->data(), buffer->size());
		stringstream ss(s);
		string status;
		ss >> status;
		if (status == "success") {
			_username = username;
			ss >> _token;
			_cb(true, "Login success!");
		}
		else if (status == "failed") {
			string message;
			getline(ss, message);
			_cb(false, message);
		}
		else {
			_cb(false, "Unknown Error");
		}
	});
	HttpClient::getInstance()->sendImmediate(request);
	request->release();
}

void  Client::reg(string username, string password, function<void(bool, string)>_cb) {
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(baseUrl + "reg/" + username + "/" + password);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([_cb,this,username](HttpClient* sender, HttpResponse* response) {
		if (!response)
		{
			return;
		}
		if (!response->isSucceed())
		{
			log("response failed");
			log("error buffer: %s", response->getErrorBuffer());
			_cb(false, "response failed");
			return;
		}
		auto* buffer = response->getResponseData();
		string s = string(buffer->data(), buffer->size());
		stringstream ss(s);
		string status;
		ss >> status;
		if (status == "success") {
			_cb(true, "Register success!");
		}
		else if (status == "failed") {
			string message;
			getline(ss, message);
			_cb(false, message);
		}
		else {
			_cb(false, "Unknown Error");
		}
	});
	HttpClient::getInstance()->sendImmediate(request);
	request->release();
}
void  Client::updatePassword(string oldPassword, string newPassword, function<void(bool, string)>_cb) {
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(baseUrl + "updatePassword/" + _username + "/" + oldPassword + "/" + newPassword);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([_cb, this](HttpClient* sender, HttpResponse* response) {
		if (!response)
		{
			return;
		}
		if (!response->isSucceed())
		{
			log("response failed");
			log("error buffer: %s", response->getErrorBuffer());
			_cb(false, "response failed");
			return;
		}
		auto* buffer = response->getResponseData();
		string s = string(buffer->data(), buffer->size());
		stringstream ss(s);
		string status;
		ss >> status;
		if (status == "success") {
			_cb(true, "Update success!");
		}
		else if (status == "failed") {
			string message;
			getline(ss, message);
			_cb(false, message);
		}
		else {
			_cb(false, "Unknown Error");
		}
		});
	HttpClient::getInstance()->sendImmediate(request);
	request->release();
}
void  Client::updateScore(int newScore, function<void(bool, string)>_cb) {
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(baseUrl + "updateScore/" + _username + "/" + to_string(newScore) + "?token=" + _token);
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([_cb, this](HttpClient* sender, HttpResponse* response) {
		if (!response)
		{
			return;
		}
		if (!response->isSucceed())
		{
			log("response failed");
			log("error buffer: %s", response->getErrorBuffer());
			_cb(false, "response failed");
			return;
		}
		auto* buffer = response->getResponseData();
		string s = string(buffer->data(), buffer->size());
		stringstream ss(s);
		string status;
		ss >> status;
		if (status == "success") {
			_cb(true, "Update success!");
		}
		else if (status == "failed") {
			string message;
			getline(ss, message);
			_cb(false, message);
		}
		else {
			_cb(false, "Unknown Error");
		}
		});
	HttpClient::getInstance()->sendImmediate(request);
	request->release();
}

void Client::ranklist(function<void(bool, string, vector<pair<string, int>>)> _cb) 
{
	HttpRequest* request = new (std::nothrow) HttpRequest();
	request->setUrl(baseUrl + "users/");
	request->setRequestType(HttpRequest::Type::GET);
	request->setResponseCallback([_cb, this](HttpClient* sender, HttpResponse* response) {
		if (!response)
		{
			return;
		}
		vector<pair<string, int>> result;
		if (!response->isSucceed())
		{
			log("response failed");
			log("error buffer: %s", response->getErrorBuffer());
			_cb(false, "response failed", result);
			return;
		}
		auto* buffer = response->getResponseData();
		string s = string(buffer->data(), buffer->size());
		stringstream ss(s);
		string status;
		ss >> status;
		if (status == "success") {
			int num;
			ss >> num;
			string username; int score;
			for (int i = 0; i < num; i++) {
				ss >> username >> score;
				result.emplace_back(username, score);
			}
			_cb(true, "success!", result);
		}
		else if (status == "failed") {
			string message;
			getline(ss, message);
			_cb(false, message, result);
		}
		else {
			_cb(false, "Unknown Error", result);
		}
		});
	HttpClient::getInstance()->sendImmediate(request);
	request->release();
}