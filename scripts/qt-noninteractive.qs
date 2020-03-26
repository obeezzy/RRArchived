function Controller() {
console.log("Controller function");
    installer.setDefaultPageVisible(QInstaller.Credentials, false);
    installer.autoRejectMessageBoxes();
    installer.installationFinished.connect(function() {
        gui.clickButton(buttons.NextButton);
    })
}

Controller.prototype.WelcomePageCallback = function() {
console.log("WelcomePageCallback function");
    gui.clickButton(buttons.NextButton, 6000);
}

Controller.prototype.CredentialsPageCallback = function() {
console.log("CredentialsPageCallback function");
    gui.clickButton(buttons.NextButton, 6000);
    console.log("End of CredentialsPageCallback function");
}

Controller.prototype.IntroductionPageCallback = function() {
console.log("IntroductionPageCallback function");
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.TargetDirectoryPageCallback = function() {
console.log("TargetDirectoryPageCallback function");
    var homeDir = installer.environmentVariable("HOME")
    gui.currentPageWidget().TargetDirectoryLineEdit.setText(homeDir + "/Qt");
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.ComponentSelectionPageCallback = function() {
console.log("ComponentSelectionPageCallback function");

    var widget = gui.currentPageWidget();

    var qtVersion = installer.environmentVariable("QT_VERSION_SCRIPT")
    if (qtVersion === "")
        console.log("QT_VERSION_SCRIPT not set.");

    widget.deselectAll();
    widget.selectComponent("qt")
    widget.selectComponent("qt.qt5." + qtVersion + ".gcc_64")
    widget.selectComponent("qt.qt5." + qtVersion + ".qtcharts")
    widget.selectComponent("qt.qt5." + qtVersion + ".qtcharts.gcc_64")
    widget.selectComponent("qt.qt5." + qtVersion + ".lottie")
    widget.selectComponent("qt.qt5." + qtVersion + ".lottie.gcc_64")

    gui.clickButton(buttons.NextButton);
}

Controller.prototype.LicenseAgreementPageCallback = function() {
console.log("LicenseAgreementPageCallback function");
    gui.currentPageWidget().AcceptLicenseRadioButton.setChecked(true);
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.StartMenuDirectoryPageCallback = function() {
console.log("StartMenuDirectoryPageCallback function");
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.ReadyForInstallationPageCallback = function()
{
console.log("ReadyForInstallationPageCallback function");
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.FinishedPageCallback = function() {
console.log("FinishedPageCallback function");
var checkBoxForm = gui.currentPageWidget().LaunchQtCreatorCheckBoxForm
    if (checkBoxForm && checkBoxForm.launchQtCreatorCheckBox) {
        checkBoxForm.launchQtCreatorCheckBox.checked = false;
    }
    gui.clickButton(buttons.FinishButton);
}

//Controller.prototype.ObligationsPageCallback = function()
//{
//    var page = gui.pageWidgetByObjectName("ObligationsPage");
//    page.obligationsAgreement.setChecked(true);
//    page.completeChanged();
//    gui.clickButton(buttons.NextButton);
//}
