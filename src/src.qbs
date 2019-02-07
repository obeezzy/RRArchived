import qbs

CppApplication {
    name: "src"

    cpp.cxxLanguageVersion: "c++14"
    cpp.includePaths: ["rrcore"]
    targetName: "rr"

    files: ["main.cpp", "plugins.cpp", "plugins.h"]

    Depends {
        name: "Qt"
        submodules: ["qml", "quick", "quickcontrols2", "sql", "charts"]
        versionAtLeast: "5.11"
    }

    Depends { name: "cpp" }
    Depends { name: "rrcore" }
    //Depends { name: "fluid" }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
        qbs.installDir: "bin"
    }

    Group {
        name: "Resources"
        files: [
            "qml/qml.qrc",
            "icons/icons.qrc",
            "images.qrc",
            "fonts/fonts.qrc"
        ]
    }
}
