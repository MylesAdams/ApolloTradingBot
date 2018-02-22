#include "mainwindow.h"
#include <QApplication>

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

//     Apollo::Bot::Twitter t;
//     Apollo::Bot::FourChan f;
//     f.addSearchQuery("bitcoin", 12141241); //for now the number of results arg does nothing, lol.
//     const auto& fourchandata = f.getData();
//     for (auto& com : fourchandata)
//     {
//         std::cout << com.content << "\n\n" << std::endl;
//     }
//     t.addSearchQuery("vechain", 100);
//    // t.addTimeline("vechainofficial", 5);
//     auto comments = t.getData();
//
//     std::ofstream out("../resources/twitter_comments.json");
//     using rapidjson::Value;
//     rapidjson::Document doc;
//     doc.SetArray();
//     rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();
//     for (auto com : comments)
//     {
//         rapidjson::Value v;
//         v.SetObject();
//         v.AddMember("content", Value(com.content, allocator), allocator);
//         doc.PushBack(v, allocator);
//     }
//     rapidjson::OStreamWrapper osw(out);
//     rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
//     doc.Accept(writer);
//     out.close();
//     return 0;
// }
