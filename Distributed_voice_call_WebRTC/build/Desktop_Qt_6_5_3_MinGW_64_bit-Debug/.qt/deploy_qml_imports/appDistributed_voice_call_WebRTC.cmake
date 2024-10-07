# Auto-generated deploy QML imports script for target "appDistributed_voice_call_WebRTC".
# Do not edit, all changes will be lost.
# This file should only be included by qt_deploy_qml_imports().

set(__qt_opts )
if(arg_NO_QT_IMPORTS)
    list(APPEND __qt_opts NO_QT_IMPORTS)
endif()

_qt_internal_deploy_qml_imports_for_target(
    ${__qt_opts}
    IMPORTS_FILE "D:/UT/CN/CA1/git/CN_CA_1_/Distributed_voice_call_WebRTC/build/Desktop_Qt_6_5_3_MinGW_64_bit-Debug/.qt_plugins/Qt6_QmlPlugins_Imports_appDistributed_voice_call_WebRTC.cmake"
    PLUGINS_FOUND __qt_internal_plugins_found
    QML_DIR     "${arg_QML_DIR}"
    PLUGINS_DIR "${arg_PLUGINS_DIR}"
)

if(arg_PLUGINS_FOUND)
    set(${arg_PLUGINS_FOUND} "${__qt_internal_plugins_found}" PARENT_SCOPE)
endif()
