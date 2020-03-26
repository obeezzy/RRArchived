function Controller() {
    installer.autoRejectMessageBoxes();
    installer.installationFinished.connect(function() {
        gui.clickButton(buttons.NextButton);
    })
}

Controller.prototype.WelcomePageCallback = function() {
    gui.clickButton(buttons.NextButton, 3000);
}

Controller.prototype.CredentialsPageCallback = function() {
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.IntroductionPageCallback = function() {
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.TargetDirectoryPageCallback = function()
{
    var homeDir = installer.environmentVariable("HOME")
    gui.currentPageWidget().TargetDirectoryLineEdit.setText(homeDir + "/Qt");
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.ComponentSelectionPageCallback = function() {
    var widget = gui.currentPageWidget();

    var qtVersion = installer.environmentVariable("QT_VERSION_SCRIPT")
    if (qtVersion === "")
        console.warn("QT_VERSION_SCRIPT not set.");

    widget.deselectAll();
    widget.selectComponent("qt." + qtVersion + ".gcc_64")
    widget.selectComponent("qt." + qtVersion + ".doc")
    widget.selectComponent("qt." + qtVersion + ".examples")
    widget.selectComponent("qt." + qtVersion + ".qtcharts")
    widget.selectComponent("qt." + qtVersion + ".qtcharts.gcc_64")
    widget.selectComponent("qt." + qtVersion + ".qtnetworkauth")
    widget.selectComponent("qt." + qtVersion + ".qtnetworkauth.gcc_64")
    widget.selectComponent("qt." + qtVersion + ".qtremoteobjects")
    widget.selectComponent("qt." + qtVersion + ".qtspeech")
    widget.selectComponent("qt." + qtVersion + ".qtremoteobjects.gcc_64")

    gui.clickButton(buttons.NextButton);
}

Controller.prototype.LicenseAgreementPageCallback = function() {
    gui.currentPageWidget().AcceptLicenseRadioButton.setChecked(true);
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.StartMenuDirectoryPageCallback = function() {
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.ReadyForInstallationPageCallback = function()
{
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.FinishedPageCallback = function() {
var checkBoxForm = gui.currentPageWidget().LaunchQtCreatorCheckBoxForm
if (checkBoxForm && checkBoxForm.launchQtCreatorCheckBox) {
    checkBoxForm.launchQtCreatorCheckBox.checked = false;
}
    gui.clickButton(buttons.FinishButton);
}
