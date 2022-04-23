#include <drogon/HttpController.h>
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
    ADD_METHOD_VIA_REGEX(Drive::read, "/(?!public).*", Get);
    METHOD_LIST_END

    void homepage(CARGS) {
        auto res = HttpResponse::newRedirectionResponse("/public/home.html");
        callback(res);
    }

    void exists(CARGS) {
        Json::Value data;
        fs::path path {"../drive_data/"};
        path.append(req->getParameter("drivename"));
        data["exists"] = fs::exists(path);
        auto res = HttpResponse::newHttpJsonResponse(data);
        callback(res);
    }

    void read(CARGS) {
        auto& pathstr = req->getPath();
        std::string drivename;
        if (size_t pos = pathstr.find('/', 1); pos != std::string::npos) {
            drivename = std::string(pathstr.c_str()+1, pos - 1);
        } else {
            drivename = std::string(pathstr.c_str()+1);
        }

        if (pathstr.find("../") != std::string::npos) {
            auto res = HttpResponse::newHttpResponse();
            res->setStatusCode(k500InternalServerError);
            callback(res);
            return;
        }

        fs::path path {"../drive_data/" + pathstr};
        HttpViewData data;
        data["path"] = path;
        data["pathstr"] = data.needTranslation(pathstr) ? data.htmlTranslate(pathstr) : pathstr;
        data["drivename"] = drivename;
        data["notfound"] = !fs::exists(path);
        auto res = HttpResponse::newHttpViewResponse("Drive", data);
        callback(res);
        
    }

};
