#ifndef SECONDLAB_OPTION_H
#define SECONDLAB_OPTION_H

template<typename T>
class Option {
private:
    bool _hasValue;
    T _value;
public:
    Option() : _hasValue(false) {}
    Option(const T& value) : _hasValue(true), _value(value) {}
    bool HasValue() const { return _hasValue; }
    T GetValue() const {
        if (!_hasValue) throw std::logic_error("Option has no value");
        return _value;
    }
    T GetValueOrDefault(const T& defaultValue = T()) const {
        return _hasValue ? _value : defaultValue;
    }
};
#endif //SECONDLAB_OPTION_H