import qbs

QtApplication {
    name: "QMLPurchaseTransactionItemModel"
    targetName: "tst_qmlpurchasetransactionitemmodeltest"
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
        files: ["tst_qmlpurchasetransactionitemmodeltest.cpp", "../utils/*.cpp", "../utils/*.h"]
    }
}
