
# OS Base includes
include(../base/CMakeLists.txt)

include_directories( 
	${CMAKE_CURRENT_SOURCE_DIR}/../base
)

########### next target ###############

set(KCM_PCI_LIBS
    KF5::KCMUtils
    KF5::I18n
    KF5::IconThemes
    KF5::KDE4Support
    ${X11_LIBRARIES}
)

set(KCM_PCI_PART_SRCS kcm_pci.cpp ../base/os_current.cpp )

if(Wayland_Client_FOUND)
    set(KCM_PCI_LIBS ${KCM_PCI_LIBS} Wayland::Client)
    set(KCM_PCI_PART_SRCS ${KCM_PCI_PART_SRCS} ../base/info_wayland.cpp)
endif()

if(PCIUTILS_FOUND)
    message(STATUS "Enabling PCI module based on pciutils library")
    add_definitions(-DHAVE_PCIUTILS)

    if(UNIX AND NOT APPLE)
        add_definitions (-fpermissive)
    endif()

    set(KCM_PCI_PART_SRCS ${KCM_PCI_PART_SRCS} kpci.cpp)
    set(KCM_PCI_LIBS ${KCM_PCI_LIBS} ${PCIUTILS_LIBRARIES} ${ZLIB_LIBRARIES})
endif()

add_library(kcm_pci MODULE ${KCM_PCI_PART_SRCS})

target_link_libraries(kcm_pci ${KCM_PCI_LIBS} )

install(TARGETS kcm_pci  DESTINATION ${PLUGIN_INSTALL_DIR})

########### install files ###############

install( FILES kcm_pci.desktop  DESTINATION  ${SERVICES_INSTALL_DIR})
