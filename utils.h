#include <string>
#include <regex>

#define CARGS const HttpRequestPtr &req, std::function<void (const HttpResponsePtr &)> &&callback

bool validDrivename(const std::string& s);