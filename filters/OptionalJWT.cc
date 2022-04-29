/**
 *
 *  OptionalJWT.cc
 *
 */

#include "OptionalJWT.h"
#include "../utils.h"
#include <unordered_map>

using namespace drogon;

void OptionalJWT::doFilter(const HttpRequestPtr &req,
                         FilterCallback &&fcb,
                         FilterChainCallback &&fccb) {
    std::string s;
    try {
        s = std::to_string(std::get<1>(JWT::Decode(req->getCookie("token"), &signer))["drive_id"].get<int>());
    } catch(...) {
        s = "invalid";
    }

    MIDDLEWARE_RESULT("drive_id", s);
    fccb();
}
