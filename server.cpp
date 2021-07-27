#include <iostream>
#include <fstream>
#include <string>
#include <boost/asio.hpp>

#include "addressbook.pb.h"

using namespace boost::asio;
using namespace std;

#define MAX_SIZE 128 * 1024

// 打印输出
void ListPeople(const tutorial::AddressBook& address_book) {
    for (int i = 0; i < address_book.people_size(); i++) {
        const tutorial::Person& person = address_book.people(i);

        cout << "Person ID: " << person.id() << endl;
        cout << "  Name: " << person.name() << endl;
        if (person.has_email()) {
            cout << "  E-mail address: " << person.email() << endl;
        }

        for (int j = 0; j < person.phones_size(); j++) {
            const tutorial::Person::PhoneNumber& phone_number = person.phones(j);

            switch (phone_number.type()) {
                case tutorial::Person::MOBILE:
                    cout << "  Mobile phone #: ";
                    break;
                case tutorial::Person::HOME:
                    cout << "  Home phone #: ";
                    break;
                case tutorial::Person::WORK:
                    cout << "  Work phone #: ";
                    break;
            }
            cout << phone_number.number() << endl;
        }
    }
}


int main(int argc, char* argv[]) {
    // 验证库的版本与头文件编译的内容是否一致
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // 所有asio类都需要io_service对象
    io_service iosev;
    ip::tcp::acceptor acceptor(iosev, ip::tcp::endpoint(ip::tcp::v4(), 5000));
    while(1) {
        // socket对象
        ip::tcp::socket socket(iosev);
        // 等待直到客户端连接进来
        acceptor.accept(socket);
        // 显示连接进来的客户端
        std::cout << socket.remote_endpoint().address() << std::endl;
        boost::system::error_code ec;

        // 接收数据
        char buf[MAX_SIZE];
        size_t len = socket.read_some(buffer(buf), ec);
        if(len >= MAX_SIZE) {
            std::cout << "data is too big, receive failed!" << std::endl;
            continue;
        }
        // 如果出错，打印出错信息
        if(ec) {
            std::cout << "receive error: " << boost::system::system_error(ec).what() << std::endl;
            continue;
        }

        tutorial::AddressBook address_book;
        if(address_book.ParseFromArray(buf, len) != 1) {    //序列化
            std::cout << "Failed to parse address book." << std::endl;
        }

        ListPeople(address_book);
    }
    
    // 可选项：删除所有protobuf分配的全局对象
    google::protobuf::ShutdownProtobufLibrary();

    return 0;
}