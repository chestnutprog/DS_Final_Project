/*
sudo vim helloworld.cpp
gg
dG
:set paste
i
/*
*/
#include <restinio/all.hpp>
#include <bits/stdc++.h>
#include "Users.h"
using namespace restinio;
using namespace std;

template <typename T>
std::ostream &operator<<(std::ostream &to, const optional_t<T> &v)
{
    if (v)
        to << *v;
    return to;
}

int main()
{
    Users users;
    std::cout << "Welcome to Block Crash!" << std::endl;
    // Create express router for our service.
    auto router = std::make_unique<router::express_router_t<>>();

    router->http_get(
        R"(/se_homework/)",
        [](auto req, auto params) {
            return req->create_response().set_body("Welcome to Block Crash!\n").done();
        });

    router->http_get(
        R"(/se_homework/user/:username)",
        [&](auto req, auto params) {
            const auto qp = parse_query(req->header().query());
            string username = cast_to<string>(params["username"]);
            string result;
            try
            {
                auto u = users.getUser(username);
                result = fmt::format("success\n{}\n{}\n", u.username, u.point);
            }
            catch (invalid_argument e)
            {
                result = fmt::format("failed\n{}\n", e.what());
            }
            return req->create_response()
                .set_body(result)
                .done();
        });

    router->http_get(
        R"(/se_homework/login/:username/:password)",
        [&](auto req, auto params) {
            const auto qp = parse_query(req->header().query());
            string username = cast_to<string>(params["username"]);
            string password = cast_to<string>(params["password"]);
            string result;
            auto u = users.login(username, password);
            if (u == 101)
                result = "success\n";
            else
                result = "failed\nIncorrect Username or Password\n";
            return req->create_response()
                .set_body(result)
                .done();
        });

    router->http_get(
        R"(/se_homework/reg/:username/:password)",
        [&](auto req, auto params) {
            const auto qp = parse_query(req->header().query());
            string username = cast_to<string>(params["username"]);
            string password = cast_to<string>(params["password"]);
            string result;
            auto u = users.reg(username, password, password);
            if (u == 102)
                result = "success\n";
            else
                result = "failed\nUsername has already existed.\n";
            return req->create_response()
                .set_body(result)
                .done();
        });

    router->http_get(
        R"(/se_homework/updatePassword/:username/:password/:newPassword)",
        [&](auto req, auto params) {
            const auto qp = parse_query(req->header().query());
            string username = cast_to<string>(params["username"]);
            string password = cast_to<string>(params["password"]);
            string newPassword = cast_to<string>(params["newPassword"]);
            string result;
            auto u = users.modifyPassword(username, password, newPassword, newPassword);
            if (u == 103)
                result = "success\n";
            else if (u == 204)
                result = "failed\nUsername doesn't exist.\n";
            else
                result = "failed\nWrong password.\n";
            return req->create_response()
                .set_body(result)
                .done();
        });

    router->http_get(
        R"(/se_homework/updateScore/:username/:score(\d+))",
        [&](auto req, auto params) {
            const auto qp = parse_query(req->header().query());
            string username = cast_to<string>(params["username"]);
            int score = cast_to<int>(params["score"]);
            string result;
            auto u = users.updateScore(username, score);
            if (u == 103)
                result = "success\n";
            else
                result = "failed\nUnknown Error.\n";
            return req->create_response()
                .set_body(result)
                .done();
        });

    router->http_get(
        R"(/se_homework/users)",
        [&](auto req, auto params) {
            const auto qp = parse_query(req->header().query());
            string result = users.showUsers();
            return req->create_response()
                .set_body(result)
                .done();
        });

    router->non_matched_request_handler(
        [](auto req) {
            return req->create_response(restinio::status_not_found()).connection_close().done();
        });

    // Launching a server with custom traits.
    struct my_server_traits : public default_single_thread_traits_t
    {
        using request_handler_t = restinio::router::express_router_t<>;
    };

    restinio::run(
        restinio::on_this_thread<my_server_traits>()
            .address("localhost")
            .request_handler(std::move(router)));

    return 0;
}
