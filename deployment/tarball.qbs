import qbs 1.0

InstallPackage {
    name: "rr_" + project.version
    targetName: name
    condition: project.deploymentEnabled

    archiver.type: "tar"
    archiver.outputDirectory: project.buildDirectory
}
