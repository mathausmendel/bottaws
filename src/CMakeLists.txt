add_executable(BotTaws Main.cpp)

target_link_libraries(BotTaws PUBLIC
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets)

install(TARGETS BotTaws
    DESTINATION bin)
