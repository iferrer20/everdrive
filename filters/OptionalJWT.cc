/**
 *
 *  OptionalJWT.cc
 *
 */

#include "OptionalJWT.h"
#include "../utils.h"
#include <jwt/jwt_all.h>
#include <unordered_map>

using namespace drogon;

void OptionalJWT::doFilter(const HttpRequestPtr &req,
                         FilterCallback &&fcb,
                         FilterChainCallback &&fccb) {
    
    try {
        //payload = std::get<1>(JWT::Decode(req->getCookie("token"), &signer));
        nlohmann::json header, payload{};
        std::tie(header, payload) = JWT::Decode(req->getCookie("token"), &signer);
        req->attributes()->insert("JWT_PAYLOAD", payload);
    } catch(InvalidTokenError &tfe) {
        req->attributes()->insert("JWT_PAYLOAD", nlohmann::json{});
    }
    
    fccb();
}
