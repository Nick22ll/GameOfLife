#ifndef CUSTOMINPUTDIALOG_H
#define CUSTOMINPUTDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QSpinBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include<QRegularExpressionValidator>

class QLineEdit;

class CustomInputDialog : public QDialog
{
    Q_OBJECT
    public:
        explicit CustomInputDialog(QWidget *parent = nullptr);

        static std::vector<int> getValues(QWidget *parent, bool *ok = nullptr);

    private:
        QList<QSpinBox*> fields;
};

#endif // CUSTOMINPUTDIALOG_H
