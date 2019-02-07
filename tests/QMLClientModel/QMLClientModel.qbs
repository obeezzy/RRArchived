import qbs

QtApplication {
    name: "QMLClientModel"
    targetName: "tst_qmlclientmodeltest"
    type: ["application", "autotest"]

    cpp.includePaths: ["../../src/rrcore", "../utils"]
    cpp.defines: ["QT_DEPRECATED_WARNINGS"]
    cpp.cxxLanguageVersion: "c++17"

    Depends {
        name: "Qt"
        submodules: ["core", "qml", "quick", "quickcontrols2", "widgets", "sql", "testlib"]
    }

    Depends { name: "rrcore" }

    Group {
        name: "Sources"
        cpp.includePaths: ["../../src/rrcore", "../utils"]
        files: ["tst_qmlclientmodeltest.cpp", "../utils/*.cpp", "../utils/*.h"]
    }
}
