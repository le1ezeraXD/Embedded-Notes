#include<iostream>
#include<vector>
#include<memory>

class Base {
public:
    virtual void doWork() const;      // virtual function
    virtual void mf4() const;
    void mf5();

};
// the extend class need to use "override" to define the function from the Base class
class Derived: public Base{
public:
    virtual void doWork() const override;
    void mf4() const override;

};


class Widget {
public:
    using DataType = std::vector<double>;
    DataType& data() &{
        return values;
    }
    DataType data() &&{
        return std::move(values);
    }

private:
    DataType values{1.0,2.0,3.0};
};
int main(void) {
    // std::unique_ptr<Base> upb = 
    //     std::make_unique<Derived>();

    // upb->doWork();
    int a = 1;
    decltype(a) b = 2;
    // doSomething(a);
    
}