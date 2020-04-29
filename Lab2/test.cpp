#include <iostream>

#include "request.h"
#include "httprequestparser.h"

using namespace httpparser;

int main(int, char**)
{
    const char text[] = "POST / HTTP/1.1\r\n"
                        "Content-Length: 19\r\n"
                        "Host: 127.0.0.1\r\n"
                        "\r\n"
                        "Name=HNU&ID=CS06142";

    Request request;
    HttpRequestParser parser;

    HttpRequestParser::ParseResult res = parser.parse(request, text, text + sizeof(text));

    if( res == HttpRequestParser::ParsingCompleted )
    {
        std::cout<<request.method<<std::endl;
        for(int i=0;i<request.content.size();i++)
        {
            std::cout<<request.content[i];
        }
        return EXIT_SUCCESS;
    }
    else
    {
        std::cerr << "Parsing failed" << std::endl;
        return EXIT_FAILURE;
    }
}
