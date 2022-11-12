add_executable(BotTaws
    ChatClient.cpp
    ChatClient.hpp
    TwitchAuth.cpp
    TwitchAuth.hpp
    Main.cpp)

target_link_libraries(BotTaws PRIVATE
    fmt::fmt
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::WebSockets
    Qt6::NetworkAuth)

install(TARGETS BotTaws
    DESTINATION bin)
