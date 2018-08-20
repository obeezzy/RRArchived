import QtQuick 2.10
import QtQuick.Controls.Material 2.3
import QtQuick.Controls 2.3 as QQC2
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

    Component {
        id: onboardingPage

        OnboardingPage { onFinished: mainWindow.pageStack.replace(loginPage); }
    }

    Component {
        id: loginPage

        LoginPage {
            QQC2.StackView.onRemoved: {
                mainWindow.appBar.visible = true;
                sidebar.expanded = true;
            }
            onLoggedIn: mainWindow.pageStack.replace(sidebar.model.get(0).fileName);
        }
    }
}
