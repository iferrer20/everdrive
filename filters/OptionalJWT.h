/**
 *
 *  OptionalJWT.h
 *
 */

#pragma once

#include <drogon/HttpFilter.h>
using namespace drogon;


class OptionalJWT : public HttpFilter<OptionalJWT>
{
  public:
    OptionalJWT() {}
    virtual void doFilter(const HttpRequestPtr &req,
                          FilterCallback &&fcb,
                          FilterChainCallback &&fccb) override;
};

