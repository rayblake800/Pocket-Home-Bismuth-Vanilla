#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

class CounterComponent : public Component {
public:
    CounterComponent(int initialValue = 0, int minimum = 0, int maximum = 9);
    virtual~CounterComponent();
    
    int getValue();
    
    void setValue(int newValue);
    
    class Listener{
    public:
        virtual ~Listener();
        
        virtual counterValueChanged(ConterComponent* counter){}
    };
    
    void addListener(Listener* listener);
    void removeListener(Listener* listener);
private:
    TextButton minusButton;
    TextButton plusButton;
    TextEditor textField;

};