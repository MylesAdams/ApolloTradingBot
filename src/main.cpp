
#include "mainwindow.h"
#include <QApplication>

#include "FourChan.h"
#include "Twitter.h"

#include <iostream>
#include <sstream>
#include <cctype>

#include "rapidjson/document.h"
#include "rapidjson/writer.h"

using Apollo::Comment;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}

//int main()
//{
//    Apollo::Bot::Twitter t;
//    auto comments = t.getData();

//    std::ofstream out("../resources/twitter_comments.json");
//    using rapidjson::Value;
//    rapidjson::Document doc;
//    doc.SetArray();
//    rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
//    for (auto com : comments)
//    {
//        rapidjson::Value v;
//        v.SetObject();
//        v.AddMember("content", Value(com.content, allocator), allocator);
//        doc.PushBack(v, allocator);
//    }
//    rapidjson::OStreamWrapper osw(out);
//    rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
//    doc.Accept(writer);
//    out.close();
//    return 0;
//}
