import qbs

CppApplication {
    consoleApplication: true

    Group {
        qbs.install: true
        fileTagsFilter: product.type
    }


    Depends { name: "Qt.core" }
    Depends { name: "Qt.network" }

    files: [
        "**/*.h",
        "**/*.cpp",
    ]
}
