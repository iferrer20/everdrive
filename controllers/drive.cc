#include <drogon/HttpController.h>
#include <drogon/orm/DbClient.h>
#include <filesystem>
#include <string_view>
#include "../utils.h"

using namespace drogon;
namespace fs = std::filesystem;

class Drive : public drogon::HttpController<Drive> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(Drive::homepage, "/");
    ADD_METHOD_TO(Drive::exists, "/exists", Post, "ValidDrivenameParam");
    ADD_METHOD_TO(Drive::create, "/create", Post, "ValidDrivenameParam");
    ADD_METHOD_VIA_REGEX(Drive::read, "/(?!public).*", Get);
    METHOD_LIST_END

    void homepage(CARGS) {
        auto res = HttpResponse::newRedirectionResponse("/public/home.html");
        callback(res);
    }

    void exists(CARGS) {
        DBCLI->execSqlAsync(
           "SELECT * FROM drives WHERE drivename = $1",  
            [callback] (const orm::Result &r) {
                Json::Value data;
                data["exists"] = r.size() > 0;
                auto res = HttpResponse::newHttpJsonResponse(data);
                callback(res);
            },
            DBERR,
            req->getParameter("drivename")
        );
        
    }

    void create(CARGS) {
        const auto& drivename = req->getParameter("drivename");
        const auto& password = req->getParameter("password");

        auto res = HttpResponse::newHttpResponse();
        res->setStatusCode(k302Found);
        res->addHeader("location", "/" + drivename);
        callback(res);
    }

    void read(CARGS) {
        std::string pathstr = std::move(req->getPath());
        

        if (pathstr.find("../") != std::string::npos || pathstr.find("/..") != std::string::npos) {
            SENDCODE(500);
            return;
        }
        
        trimstr(pathstr, '/');
        fs::path path {"../drive_data/" + pathstr};
        if (fs::is_regular_file(path)) {
            auto res = HttpResponse::newFileResponse(path.string());
            callback(res);
            return;
        }

        if (*(pathstr.cend()-1) != '/') { // add / at the end if necessary
            pathstr += '/';
        }

        std::string drivename;
        if (size_t pos = pathstr.find('/', 1); pos != std::string::npos) {
            drivename = std::string(pathstr.c_str()+1, pos - 1);
        } else {
            drivename = std::string(pathstr.c_str()+1);
        }

        HttpViewData data;
        data["path"] = path;
        data["pathstr"] = data.needTranslation(pathstr) ? data.htmlTranslate(pathstr) : pathstr;
        data["drivename"] = drivename;
        data["notfound"] = !validDrivename(drivename) || !fs::exists(path);
        data["goback"] = std::count(pathstr.begin(), pathstr.end(), '/') > 2;
        auto res = HttpResponse::newHttpViewResponse("Drive", data);
        callback(res);
        
    }

};
