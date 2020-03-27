function Controller() {
    console.log("Controller function");
    installer.autoRejectMessageBoxes();
    installer.installationFinished.connect(function() {
        gui.clickButton(buttons.NextButton);
    });
    installer.statusChanged.connect(onStatusChanged);
}

Controller.prototype.WelcomePageCallback = function() {
    console.log("WelcomePageCallback function");
    gui.clickButton(buttons.NextButton, 3000);
}

Controller.prototype.CredentialsPageCallback = function() {
    console.log("CredentialsPageCallback function");

    var email = installer.value("email");
    var password = installer.value("password");

    if (!email) {
        console.log("email command line argument missing.");
        gui.rejectWithoutPrompt();
    }
    if (!password) {
        console.log("password command line argument missing.");
        gui.rejectWithoutPrompt();
    }

    var widget = gui.currentPageWidget();
    var emailLineEdit = gui.findChild(widget, "EmailLineEdit");
    var passwordLineEdit = gui.findChild(widget, "PasswordLineEdit");

    if (emailLineEdit) {
        emailLineEdit.text = email;
    } else {
        console.log("Cannot find EmailLineEdit on CredentialsPage.");
        gui.rejectWithoutPrompt();
    }
    if (passwordLineEdit) {
        passwordLineEdit.text = password;
    } else {
        console.log("Cannot find PasswordLineEdit on CredentialsPage.");
        gui.rejectWithoutPrompt();
    }

    gui.clickButton(buttons.NextButton);
}

Controller.prototype.IntroductionPageCallback = function() {
    console.log("IntroductionPageCallback function");
    gui.clickButton(buttons.NextButton);
}

Controller.prototype.TargetDirectoryPageCallback = function() {
    console.log("TargetDirectoryPageCallback function");
    var homeDir = installer.environmentVariable("HOME");
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
    widget.selectComponent("qt.qt5." + qtVersion + ".gcc_64")
    widget.selectComponent("qt.qt5." + qtVersion + ".qtcharts.gcc_64")
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

// User defined
onStatusChanged = function(status) {
    switch (status) {
    case QInstaller.Failure:
    case QInstaller.Canceled:
    case QInstaller.Unfinished:
        gui.rejectWithoutPrompt();
        break;
    default:
        break;
    }
}
