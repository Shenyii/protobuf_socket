#! /usr/bin/python
#  coding=utf-8
 
import socket
import addressbook_pb2
import sys

MAX_SIZE = 128 * 1024

#创建socket通信的对象
client = socket.socket()

#连接服务器的IP地址和端口号
client.connect(("localhost", 5000))

# 手动输入消息并打印
def PromptForAddress(person):
    person.id = int(input("Enter person ID number: "))
    person.name = input("Enter name: ")

    email = input("Enter email address (blank for none): ")
    if email != "":
        person.email = email

    while True:
        number = input("Enter a phone number (or leave blank to finish): ")
        if number == "":
            break

        phone_number = person.phones.add()
        phone_number.number = number

        type = input("Is this a mobile, home, or work phone? ")
        if type == "mobile":
            phone_number.type = addressbook_pb2.Person.PhoneType.MOBILE
        elif type == "home":
            phone_number.type = addressbook_pb2.Person.PhoneType.HOME
        elif type == "work":
            phone_number.type = addressbook_pb2.Person.PhoneType.WORK
        else:
            print ("Unknown phone type; leaving as default value.")


if __name__ == "__main__":
    address_book = addressbook_pb2.AddressBook()

    PromptForAddress(address_book.people.add())

    msg = address_book.SerializeToString()
    if len(msg) >= MAX_SIZE:
        print('data is too big, server will not receive data!')

    client.send(msg)

