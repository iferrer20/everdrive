#include <string>
#include <regex>
#include <drogon/orm/Exception.h>
#include <jwt/jwt_all.h>

#ifndef JWT_PASS
#define JWT_PASS "secret"
#endif

#define CARGS const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback
#define DBCLI app().getDbClient()
#define SENDCODE(CODE) auto res = HttpResponse::newHttpResponse(); \
            res->setStatusCode((HttpStatusCode) CODE);\
            callback(res)

#define DBERR [callback] (const orm::DrogonDbException &e) { \
    std::cerr << "error:" << e.base().what() << std::endl; \
    SENDCODE(500); \
}

#define MIDDLEWARE_RESULT(KEY, RESULT) const_cast<std::unordered_map<std::string, std::string>&>(req->getParameters())[KEY] = RESULT

bool validDrivename(const std::string& s);
void trimstr(std::string& str, char trimchar);
inline HS256Validator signer(JWT_PASS);
