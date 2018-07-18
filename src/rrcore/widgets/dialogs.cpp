#include "dialogs.h"
#include <QPrintDialog>

Dialogs::Dialogs(QObject *parent) :
    QObject(parent)
{

}

void Dialogs::showPrintDialog()
{
    QPrintDialog dialog;
    if (dialog.exec() == QDialog::Accepted) {
        // print ...
    }
}
