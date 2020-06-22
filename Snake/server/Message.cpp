#include "Message.h"
#include <string.h>
#include <iostream>
#include "Node.h"

Message::Message(){}

Message::Message(const uint8_t type) : _type(type){}

Message::Message(const uint8_t type, const InputInfo &info) : _type(type), _inputInfo(info){}

Message::Message(const uint8_t type, const FruitInfo &info) : _type(type), _fruitInfo(info){}

Message::Message(const uint8_t type, Node* node) : _type(type), _node(node){}

Message::Message(const InputInfo &info) : _inputInfo(info){}

Message::Message(const FruitInfo &info) : _fruitInfo(info){}


void Message::to_bin()
{
    if(_type == INPUT) //package input info
    {
        _data = new char[INPUT_SIZE];
        _size = INPUT_SIZE; 
        char* tmp = _data; 

        memcpy((void*)tmp, (void*)&_type, sizeof(uint8_t));
        tmp += sizeof(uint8_t);

        memcpy((void*)tmp, _inputInfo.toString().c_str(), sizeof(char) * (sizeof(InputInfo) + 1)); 
    }
    else if(_type == FRUIT_EATEN) //package fruit info
    {
        _size = FRUIT_SIZE;
        _data = new char[FRUIT_SIZE];
        
        char* tmp = _data;

        memcpy((void*)tmp, (void*)&_type, sizeof(uint8_t));
        tmp += sizeof(uint8_t);

        memcpy((void*)tmp, _fruitInfo.toString().c_str(), sizeof(char) * (sizeof(FruitInfo) + 2)); 
    }
    else if(_type == Message::START) //package start info
    {
        _size = sizeof(uint8_t) + sizeof(char);
        _data = new char[_size];
        
        char* tmp = _data;

        memcpy((void*)tmp, (void*)&_type, sizeof(uint8_t));
        tmp += sizeof(uint8_t);
        memcpy(tmp, &_player, sizeof(char));
    }
    else if(_type == Message::NODE)
    {
        _size = sizeof(uint8_t) + sizeof(Node);
        _data = new char[_size];

         char* tmp = _data;

        memcpy((void*)tmp, (void*)&_type, sizeof(uint8_t));
        tmp += sizeof(uint8_t);
        memcpy(tmp, _node->toString().c_str(), sizeof(char) * (sizeof(Node) + 3)); 
    }
    else //LOGIN OR LOGOUT 
    {
        _data = new char[sizeof(uint8_t)];
        _size = sizeof(uint8_t);
        memcpy((void*)_data, (void*)&_type, sizeof(uint8_t));
    }
    
}

int Message::from_bin(char * data)
{   
    //if type = LOGIN | LOGOUT only _type is read
    char* tmp = data;
    memcpy(&_type, tmp, sizeof(uint8_t)); //read type of message from data
    tmp += sizeof(uint8_t); 

    if(_type == INPUT) //save inputinfo
    {
        char* str = new char[sizeof(char) * (sizeof(InputInfo) + 1)];
        memcpy(&str[0], tmp, sizeof(char) * (sizeof(InputInfo) + 1));

        _inputInfo.fromString(str);
        delete[] str;
    }
    else if(_type == FRUIT_EATEN) //save fruit info
    {
        char* str = new char[sizeof(char) * (sizeof(FruitInfo) + 2)];
        memcpy(&str[0], tmp, sizeof(char) * (sizeof(FruitInfo) + 2));
        _fruitInfo.fromString(str);
        delete[] str; 
    }
    else if(_type == Message::START) //save start info
    {
        memcpy(&_player, tmp, sizeof(char));
    }
    else if(_type == Message::NODE)
    {
        _node = new Node();
        char* str = new char[sizeof(char) * (sizeof(Node) + 3)];
        memcpy(&str[0], tmp, sizeof(char) * (sizeof(Node) + 3));
        _node->fromString(str);
        delete[] str; 
    }

    return 0;
}

Message::~Message()
{
    if(_data != nullptr)
    {
         delete[] _data;
         _data = nullptr;
    }
    //TODO: deletes
}