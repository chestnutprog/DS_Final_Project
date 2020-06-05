/*
vim helloworld.cpp
gg
dG
:set paste
i
/*
*/
#include <restinio/all.hpp>
#include <bits/stdc++.h>
using namespace restinio;

template<typename T>
std::ostream & operator<<(std::ostream & to, const optional_t<T> & v) {
    if(v) to << *v;
    return to;
}

int main() {
    std::cout << "Welcome to Block Crash!" << std::endl;
    // Create express router for our service.
    auto router = std::make_unique<router::express_router_t<>>();

    router->http_get(
        R"(/)",
        [](auto req,auto params) {
            return req->create_response().set_body("Welcome to Block Crash!\n").done();
        }
    );

    router->http_get(
            R"(/data/meter/:meter_id(\d+))",
            [](auto req, auto params) {
                const auto qp = parse_query(req->header().query());
                return req->create_response()
                        .set_body(
                                fmt::format("meter_id={} (year={}/mon={}/day={})",
                                        cast_to<int>(params["meter_id"]),
                                        opt_value<int>(qp, "year"),
                                        opt_value<int>(qp, "mon"),
                                        opt_value<int>(qp, "day")))
                        .done();
            });

    router->non_matched_request_handler(
            [](auto req){
                return req->create_response(restinio::status_not_found()).connection_close().done();
            });

    // Launching a server with custom traits.
    struct my_server_traits : public default_single_thread_traits_t {
        using request_handler_t = restinio::router::express_router_t<>;
    };

    restinio::run(
            restinio::on_this_thread<my_server_traits>()
                    .address("localhost")
                    .request_handler(std::move(router)));

    return 0;
}
