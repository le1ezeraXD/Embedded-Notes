#include<iostream>
#include<unordered_map>
#include<vector>
#include<memory>
#include<algorithm>


class Widget{};
class WidgetID{};


//  使用weak_ptr实现缓存机制
//  在缓存里保存对对象的“非拥有引用”
//  让缓存可以在对象活着的时候复用它
//  又不阻止对象在没人用时自动释放

//  weak_ptr适用情况: 1.缓存    2.观察者模式    3.非严格继承谱系的数据结构(避免指针环路)
std::unordered_map<WidgetID, std::weak_ptr<const Widget>> cache;   // 使用weak_ptr作为缓存的值类型
std::shared_ptr<const Widget> fastLoadWidget(WidgetID id) {
    auto it = cache.find(id);
    if (it != cache.end()) {
        if (auto sp = it->second.lock()) { // 尝试提升为shared_ptr
            return sp; // 缓存命中 返回shared_ptr
        }
        // 缓存失效 继续加载
    }
    auto sp = std::make_shared<const Widget>();
    cache[id] = sp; // 存入缓存
    return sp;
};

//  观察者模式demo
//  观察者接口
class Observer {
public:
    virtual ~Observer() = default;  // 虚析构函数
    virtual void onNotify(int newValue) = 0;    // 纯虚函数
};

//  主题(被观察者)
class Subject {
public:
    void attach(const std::shared_ptr<Observer>& observer) {
        observers.push_back(observer); // 添加观察者
    }
    void setValue(int newValue) {
        value = newValue;
        notify();   // 通知所有观察者
    }
private:
    int value = 0;
    std::vector<std::weak_ptr<Observer>> observers;
    void notify() {
        std::cout << "[Subject] notify: value = " << value << "\n";
        // 遍历观察者列表
        observers.erase(
            std::remove_if(observers.begin(), observers.end(),
                [&](std::weak_ptr<Observer>& wobs) {
                    auto sp = wobs.lock(); // 尝试提升为shared_ptr
                    if (!sp) {
                        // 观察者已经析构，从列表中移除
                        std::cout << "[Subject] remove expired observer\n";
                        return true;
                    }
                    // 通知观察者
                    sp->onNotify(value);
                    return false;
                }),
            observers.end()
        );
    }        
};


//  具体观察者
class ConcreteObserver : public Observer {
public:
    explicit ConcreteObserver(std::string name) : name_(std::move(name)) {
        std::cout << "Observer [" << name_ << "] constructed\n";
    }

    ~ConcreteObserver() override {
        std::cout << "Observer [" << name_ << "] destructed\n";
    }
    
    void onNotify(int newValue) override {
        std::cout << "  [" << name_ << "] recrived value = " << newValue << "\n";
    }
private:
    std::string name_;  
};



int main(void) {

    // auto spw = std::make_shared<Widget>();
    // std::weak_ptr<Widget> wpw(spw); // wpw和spw指向同个Widget 引用计数保持为1 weak_ptr不影响引用计数

    // spw = nullptr;  // 引用计数变为0 Widget对象被析构 wpw空悬

    // std::shared_ptr<Widget> spw1 = wpw.lock();
    // auto spw2 = wpw.lock(); // 尝试提升为shared_ptr 失败返回空指针

    // std::shared_ptr<Widget> spw3(wpw); // 直接用weak_ptr构造shared_ptr 抛出异常

    // if (wpw.lock()) {
    //     std::cout << "spw1 is alive\n";
    // } else {
    //     std::cout << "spw1 is expired\n";
    // }
    Subject subject;

    {
        auto obs1 = std::make_shared<ConcreteObserver>("A");
        auto obs2 = std::make_shared<ConcreteObserver>("B");

        subject.attach(obs1);
        subject.attach(obs2);
        
        std::cout << "=== First notify ===\n";
        subject.setValue(10);   // A、B 都收到

        std::cout << "=== obs1 goes out of scope ===\n";
        // obs1/2 在本作用域结束后析构
    }

    std::cout << "=== Second notify ===\n";
    subject.setValue(20);       // 只通知 B，A 已经析构，weak_ptr 自动过期并在 notify 时被清理

    std::cout << "=== End ===\n";
    return 0;

}