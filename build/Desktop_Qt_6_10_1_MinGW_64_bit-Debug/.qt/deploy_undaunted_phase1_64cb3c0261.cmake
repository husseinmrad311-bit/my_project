include("D:/Desktop/Undaunted-Phase1/Undaunted-Phase1/undaunted-phase1/build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/.qt/QtDeploySupport.cmake")
include("${CMAKE_CURRENT_LIST_DIR}/undaunted-phase1-plugins.cmake" OPTIONAL)
set(__QT_DEPLOY_I18N_CATALOGS "qtbase")

qt6_deploy_runtime_dependencies(
    EXECUTABLE "D:/Desktop/Undaunted-Phase1/Undaunted-Phase1/undaunted-phase1/build/Desktop_Qt_6_10_1_MinGW_64_bit-Debug/undaunted-phase1.exe"
    GENERATE_QT_CONF
)
