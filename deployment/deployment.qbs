import qbs

Project {
    name: "deployment"

    references: [
        "windows.qbs",
        "tarball.qbs",
        "linux.qbs"
    ]
}
