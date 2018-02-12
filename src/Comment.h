#ifndef COMMENT_H
#define COMMENT_H

#include <string>

namespace Apollo {
    struct Comment
    {
        std::string content;
        std::string ID;

        Comment()
            :   content(""),
                ID("")
        {}

        Comment(const std::string& content, const std::string& ID)
            :   content(content),
                ID(ID)
        {}
    };
}
#endif