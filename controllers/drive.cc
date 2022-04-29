#include <cstdint>
#include <cstdlib>
#include <drogon/HttpController.h>
#include <drogon/HttpTypes.h>
#include <drogon/HttpViewData.h>
#include <drogon/orm/DbClient.h>
#include <drogon/orm/Exception.h>
#include <exception>
#include <filesystem>
#include <string_view>
#include <bcrypt.h>

#include "../utils.h"

using namespace drogon;
namespace fs = std::filesystem;

class Drive : public drogon::HttpController<Drive> {
public:
    METHOD_LIST_BEGIN
    ADD_METHOD_TO(Drive::homepage, "/");
    ADD_METHOD_TO(Drive::exists, "/exists", Post, "ValidDrivenameParam");
    ADD_METHOD_TO(Drive::create, "/create", Post, "ValidDrivenameParam");
    ADD_METHOD_VIA_REGEX(Drive::read, "/(?!public).*", Get, Post, "OptionalJWT");
    METHOD_LIST_END

    void homepage(CARGS) {
        auto res = HttpResponse::newRedirectionResponse("/public/home.html");
        callback(res);
    }

    void exists(CARGS) {
        DBCLI->execSqlAsync(
           "SELECT * FROM drives WHERE name = $1",
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
        auto& drivename = req->getParameter("drivename");
        auto& password = req->getParameter("password");

        DBCLI->execSqlAsync(
            "INSERT INTO drives (name, password) VALUES ($1, $2)", 
            [callback, &password, &drivename] (const orm::Result& r) {
                fs::create_directory(fs::path("./drive_data/" + drivename));
                auto res = HttpResponse::newHttpResponse();
                res->setStatusCode(k302Found);
                res->addHeader("location", "/" + drivename);
                if (!password.empty()) {
                    res->addCookie("token", JWT::Encode(signer, {{"drive_id", r.insertId()}}));
                }
                callback(res);
            },
            DBERR,
            drivename,
            (password.empty() ? "" : bcrypt::generateHash(password))
        );
    }

    void read(CARGS) {
        std::string pathstr = std::move(req->getPath());
        if (pathstr.find("../") != std::string::npos || pathstr.find("/..") != std::string::npos) {
            SENDCODE(500);
            return;
        }
        
        trimstr(pathstr, '/');
        /*
        fs::path path {"./drive_data/" + pathstr};
        if (fs::is_regular_file(path)) {
            auto res = HttpResponse::newFileResponse(path.string());
            callback(res);
            return;
        }
        */

        if (*(pathstr.cend()-1) != '/') { // add / at the end if necessary
            pathstr += '/';
        }

        std::string drivename;
        if (size_t pos = pathstr.find('/', 1); pos != std::string::npos) {
            drivename = std::string(pathstr.c_str()+1, pos - 1);
        } else {
            drivename = std::string(pathstr.c_str()+1);
        }

        DBCLI->execSqlAsync(
            "SELECT * FROM drives WHERE name = $1",
            [callback, &pathstr, &drivename, &req] (const orm::Result& r) {
                auto hash = r[0]["password"].as<std::string>();
                auto drive_id = r[0]["drive_id"].as<std::string_view>();
                bool ask_password {};
                HttpViewData data;
                if (!hash.empty()) { // if exists password, get token data
                    auto& token_drive_id = req->getParameter("drive_id");
                    ask_password = !(token_drive_id == drive_id || bcrypt::validatePassword(req->getParameter("password"), hash));
                }
                data["drivename"] = drivename;
                data["pathstr"] = data.needTranslation(pathstr) ? data.htmlTranslate(pathstr) : pathstr;
                data["path"] = fs::path{"./drive_data/" + pathstr};
                data["notfound"] = r.size() == 0;
                data["ask_password"] = ask_password;
                auto res = HttpResponse::newHttpViewResponse("Drive", data);
                if (ask_password) {
                    res->setStatusCode(k403Forbidden);
                } else {
                    res->addCookie("token", JWT::Encode(signer, {{"drive_id", drive_id}})); // Renew token
                }
                callback(res);
            },
            DBERR,
            drivename
        );
    }
};
