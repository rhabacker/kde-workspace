########### add version number into compilation defines
add_definitions (-DAPP_VERSION=\\\"${KWIN_VERSION}\\\")
add_definitions(-DHAVE_X11=${X11_FOUND})
add_subdirectory(config)
add_subdirectory(demo)
add_subdirectory(data)

########### next target ###############
set(kwin_oxygen_SRCS
     oxygenbutton.cpp
     oxygenclient.cpp
     oxygenclientgroupitemdata.cpp
     oxygendecohelper.cpp
     oxygenexceptionlist.cpp
     oxygenfactory.cpp
     oxygensizegrip.cpp
     oxygentitleanimationdata.cpp
  )

kconfig_add_kcfg_files(kwin_oxygen_SRCS oxygenconfiguration.kcfgc )

add_library(kwin3_oxygen MODULE ${kwin_oxygen_SRCS})
target_link_libraries(kwin3_oxygen Qt5::Widgets )
target_link_libraries(kwin3_oxygen KF5::GuiAddons KF5::I18n KF5::WindowSystem KF5::Style KF5::Service)

target_link_libraries(kwin3_oxygen kdecorations)

target_link_libraries(kwin3_oxygen oxygenstyle)

kservice_desktop_to_json(kwin3_oxygen oxygenclient.desktop)

if(X11_FOUND)
  target_link_libraries(kwin3_oxygen XCB::XCB)
endif()

install(TARGETS kwin3_oxygen  DESTINATION ${PLUGIN_INSTALL_DIR}/kwin/kdecorations)
