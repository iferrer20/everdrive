#include <string>
#include <regex>
#include <drogon/orm/Exception.h>

#define CARGS const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback
#define DBCLI app().getDbClient()
#define SENDCODE(CODE) auto res = HttpResponse::newHttpResponse(); \
            res->setStatusCode((HttpStatusCode) CODE);\
            callback(res)

#define DBERR [callback] (const orm::DrogonDbException &e) { \
    std::cerr << "error:" << e.base().what() << std::endl; \
    SENDCODE(500); \
}

bool validDrivename(const std::string& s);
void trimstr(std::string& str, char trimchar);