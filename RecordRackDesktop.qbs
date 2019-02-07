import qbs
import qbs.Environment
import "deployment/deployment.qbs" as Deployment

Project {
    name: "RecordRackDesktop"

    property string version: Environment.getEnv("RR_VERSION") || "0.1.0";
    property bool windowsInstaller: false
    property bool withTests: false
    property bool withDocumentation: false
    property bool deploymentEnabled: true

    qbsSearchPaths: "qbs"
    minimumQbsVersion: "1.9"

    Deployment { condition: project.deploymentEnabled }

    references: [
        "3rdparty/fluid/fluid.qbs",
        "src/src.qbs",
        "src/rrcore/rrcore.qbs",
        "tests/tests.qbs"
    ]
}
