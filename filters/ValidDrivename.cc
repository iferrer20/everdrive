/**
 *
 *  ValidDrivename.cc
 *
 */

#include "ValidDrivename.h"
#include "../utils.h"

using namespace drogon;

void ValidDrivenameParam::doFilter(const HttpRequestPtr &req,
                         FilterCallback &&fcb,
                         FilterChainCallback &&fccb)
{
    //Edit your logic here
    if (validDrivename(req->getParameter("drivename"))) {
        //Passed
        fccb();
        return;
    }
    //Check failed
    Json::Value r;
    r["error"] = "Invalid drivename";
    auto res = drogon::HttpResponse::newHttpJsonResponse(r);
    res->setStatusCode(k400BadRequest);
    fcb(res);
}
