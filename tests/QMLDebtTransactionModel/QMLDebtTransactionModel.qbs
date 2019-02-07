import qbs

QtApplication {
    name: "QMLDebtTransactionModel"
    targetName: "tst_qmldebttransactionmodeltest"
    type: ["application", "autotest"]

    cpp.includePaths: ["../../src/rrcore", "../utils"]
    cpp.defines: ["QT_DEPRECATED_WARNINGS"]
    cpp.cxxLanguageVersion: "c++14"

    Depends {
        name: "Qt"
        submodules: ["core", "qml", "quick", "quickcontrols2", "widgets", "sql", "testlib"]
    }

    Depends { name: "rrcore" }

    Group {
        name: "Sources"
        files: ["tst_qmldebttransactionmodeltest.cpp", "../utils/*.cpp", "../utils/*.h"]
    }
}
