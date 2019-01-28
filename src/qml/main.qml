import QtQuick 2.12
import QtQuick.Controls.Material 2.12
import QtQuick.Controls 2.12 as QQC2
import Fluid.Controls 1.0 as FluidControls
import com.gecko.rr 1.0 as RR
import "rrui" as RRUi
import "common"
import "user"
import "models"

RRUi.ApplicationWindow {
    id: mainWindow

    title: qsTr("Record Rack")
    visibility: FluidControls.ApplicationWindow.Maximized
    pageStack.anchors.leftMargin: sidebar.expanded ? sidebar.width : 0
    initialPage: userProfile.isFirstTime ? onboardingPage : loginPage

    RRUi.Sidebar {
        id: sidebar
        expanded: false
        onCurrentIndexChanged: mainWindow.pageStack.replace(null, model.get(currentIndex).fileName);
    }

    RR.UserProfile { id: userProfile }
    RR.Settings { id: settings }

    Component {
        id: onboardingPage

        OnboardingPage { onFinished: mainWindow.pageStack.replace(null, loginPage); }
    }

    Component {
        id: passwordChangePage

        PasswordChangePage {
            onAccepted: mainWindow.pageStack.replace(null, sidebar.model.get(0).fileName);
            QQC2.StackView.onRemoved: {
                mainWindow.appBar.visible = true;
                sidebar.expanded = true;
            }
        }
    }

    Component {
        id: loginPage

        LoginPage {
            QQC2.StackView.onRemoved: {
                if (!userProfile.isFirstTime) {
                    mainWindow.appBar.visible = true;
                    sidebar.expanded = true;
                }
            }
            onLoggedIn: {
                if (userProfile.isFirstTime)
                    mainWindow.pageStack.replace(null, passwordChangePage);
                else
                    mainWindow.pageStack.replace(null, sidebar.model.get(0).fileName);
            }
        }
    }

    Connections {
        target: mainWindow.pageStack.currentItem !== null && mainWindow.pageStack.currentItem.objectName === "dashboardPage" ?
                    mainWindow.pageStack.currentItem : null
        onLinkActivated: {
            sidebar.currentIndex = sidebar.findIndexFromFileName(Array.isArray(link) ? link[0] : link);
            mainWindow.pageStack.replace(link, properties);
        }
    }

    Connections {
        target: mainWindow.pageStack.currentItem !== null && mainWindow.pageStack.currentItem.objectName === "settingsHomePage" ?
                    mainWindow.pageStack.currentItem : null
        onSignedOut: {
            mainWindow.appBar.visible = false;
            sidebar.expanded = false;
            sidebar.currentIndex = 0;
            mainWindow.pageStack.replace(null, loginPage);
        }
    }
}
