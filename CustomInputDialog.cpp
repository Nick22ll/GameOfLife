#include "CustomInputDialog.h"


CustomInputDialog::CustomInputDialog(QWidget *parent) : QDialog(parent)
{
    QFormLayout *lytMain = new QFormLayout(this);

    //Setup of fields labels
    QLabel *tLabel = new QLabel(QString("Width: "), this);
    QSpinBox *spinBox = new QSpinBox(this);
    spinBox->setRange(10,1000);
    lytMain->addRow(tLabel, spinBox);
    fields << spinBox;
    tLabel = new QLabel(QString("Height: "), this);
    spinBox = new QSpinBox(this);
    spinBox->setRange(10,1000);
    lytMain->addRow(tLabel, spinBox);
    fields << spinBox;

    QDialogButtonBox *buttonBox = new QDialogButtonBox
            ( QDialogButtonBox::Ok | QDialogButtonBox::Cancel,
              Qt::Horizontal, this );
    lytMain->addWidget(buttonBox);

    bool conn = connect(buttonBox, &QDialogButtonBox::accepted,
                   this, &CustomInputDialog::accept);
    Q_ASSERT(conn);
    conn = connect(buttonBox, &QDialogButtonBox::rejected,
                   this, &CustomInputDialog::reject);
    Q_ASSERT(conn);

    setLayout(lytMain);
}

std::vector<int> CustomInputDialog::getValues(QWidget *parent, bool *ok)
{
    CustomInputDialog *dialog = new CustomInputDialog(parent);
    std::vector<int> values;

    const int ret = dialog->exec();
    if (ok)
        *ok = !!ret;
    if (ret) {
        foreach (auto field, dialog->fields) {
            values.push_back(field->value());
        }
    }
    dialog->deleteLater();
    return values;
}
