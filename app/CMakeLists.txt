find_package(Qt6 REQUIRED
	COMPONENTS
		Core
		Gui
		Widgets
		Quick
		Qml
		QuickControls2
		QuickLayouts
		Quick3D
)
qt_standard_project_setup(REQUIRES 6.12)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

file(GLOB_RECURSE EGP_APP_UI_QML_FILES
	CONFIGURE_DEPENDS
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	ui/*.qml
)
file(GLOB EGP_APP_UI_JAVASCRIPT_FILES
	CONFIGURE_DEPENDS
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	ui/javascript/*.js
)
file(GLOB_RECURSE EGP_APP_UI_RESOURCE_FILES
	CONFIGURE_DEPENDS
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	resources/*
)
file(GLOB_RECURSE EGP_APP_UI_CPP_FILES
	CONFIGURE_DEPENDS
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	src/features/*.cpp
)
file(GLOB_RECURSE EGP_APP_UI_HPP_FILES
	CONFIGURE_DEPENDS
	RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
	src/features/*.hpp
)
add_executable(EGP_app
	src/main.cpp
)
qt_add_qml_module(
	EGP_app
	URI
	EGP.ui
	VERSION
	1.0
	QML_FILES
	${EGP_APP_UI_QML_FILES}
	SOURCES
	${EGP_APP_UI_CPP_FILES}
	${EGP_APP_UI_HPP_FILES}
	RESOURCES
	${EGP_APP_UI_JAVASCRIPT_FILES}
)
qt_add_resources(
	EGP_app
	"resources"
	PREFIX
	"/"
	FILES
	${EGP_APP_UI_RESOURCE_FILES}
)
target_include_directories(EGP_app
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/src/features/headers
)
target_link_libraries(EGP_app
	PRIVATE
		Qt6::Core
		Qt6::Gui
		Qt6::Qml
		Qt6::Quick
		Qt6::QuickControls2
		Qt6::QuickLayouts
		Qt6::Quick3D
)
target_link_libraries(EGP_app
	PRIVATE
		EGP_common
		EGP_components
		glaze::glaze
)
qt_import_qml_plugins(EGP_app)
target_sources(EGP_app PRIVATE $<TARGET_OBJECTS:EGP_componentsplugin_init>)
target_link_libraries(EGP_app PRIVATE EGP_componentsplugin)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
	target_compile_definitions(EGP_app PRIVATE QT_QML_DEBUG)
endif()

# Deploy Qt DLLs on Windows
if(WIN32)
	if(MINGW)
		target_link_options(EGP_app PRIVATE -static-libgcc -static-libstdc++ -static)
	endif()

	get_target_property(qmake_executable Qt6::qmake IMPORTED_LOCATION)
	get_filename_component(qt_bin_dir "${qmake_executable}" DIRECTORY)
	add_custom_command(
		TARGET EGP_app
		POST_BUILD
		COMMAND
			"${qt_bin_dir}/windeployqt.exe"
			--qtpaths
			"${qt_bin_dir}/qtpaths.exe"
			--no-translations
			# --no-compiler-runtime
			--qmldir
			"${CMAKE_CURRENT_SOURCE_DIR}/ui"
			"$<TARGET_FILE_DIR:EGP_app>"
		COMMENT "Deploying Qt DLLs for EGP_app"
		VERBATIM
	)
endif()
