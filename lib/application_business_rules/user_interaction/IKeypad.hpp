#pragma once

#include "KeyIds.hpp"
#include <functional>

typedef std::function<void(KeyId)> HmiHandler;

/**
 * Interface to an human input device.
 *
 * This is used to implement the \ref plugin_architecture.
 */
class IKeypad
{
  public:
    virtual void setCallback(const HmiHandler callbackFunction) = 0;
    virtual bool isKeyPressed(KeyId keyInquiry) = 0;
    virtual ~IKeypad(){};
};
