#include <drogon/drogon.h>
#include <drogon/utils/FunctionTraits.h>
#include <functional>
#include "utils.h"

using namespace drogon;

int main() {
    //Set HTTP listener address and port
    //Load config file
    app().loadConfigFile("./config.json");
    //Run HTTP framework,the method will block in the internal event loop
    app().setExceptionHandler([] (
                const std::exception& e, 
                const HttpRequestPtr& r, 
                std::function<void (const HttpResponsePtr &)> &&callback) {
        std::cout << "ERR\n";
        SENDCODE(500);
    });
    app().run();
    
    return 0;
}
