/**
 *
 *  ValidDrivename.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;


class ValidDrivenameParam : public HttpFilter<ValidDrivenameParam>
{
  public:
    ValidDrivenameParam() {}
    virtual void doFilter(const HttpRequestPtr &req,
                          FilterCallback &&fcb,
                          FilterChainCallback &&fccb) override;
};

