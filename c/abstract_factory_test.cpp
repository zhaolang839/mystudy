#include <iostream>
#include <string>
#include <memory>



class Button{
public:
    Button() = default;

    virtual ~Button() = default;
    virtual void paint() const = 0;
};