ename T>
void f(T&& param){};

template<typename T>
void fwd(T&& param) {
    f(std::forward<T>(param));
};