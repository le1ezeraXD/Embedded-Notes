#include<iostream>
#include<vector>

int main(void) {
    enum class Color { black, white, red};
    auto white = false;
    // Color c = white;
    Color c = Color::white;
    // auto c = Color::white;
    class UserInfo {};
    enum UserInfoFields {uiName, uiEmail, uiReputation};

    UserInfo uInfo;
    // auto val = std::get<uiEmail>(uInfo);
}