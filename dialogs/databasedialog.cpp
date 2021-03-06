#include "databasedialog.h"
#include "ui_databasedialog.h"
#include <QFileDialog>
#include <QMessageBox>

DatabaseDialog::DatabaseDialog(QWidget *parent) : QDialog(parent), ui(new Ui::DatabaseDialog)
{
    ui->setupUi(this);

    this->_databasemodel = new DatabaseModel(ui->tvDatabase);
    ui->tvDatabase->setModel(this->_databasemodel);
    ui->tvDatabase->horizontalHeader()->setSectionResizeMode(0, QHeaderView::ResizeToContents);
    ui->tvDatabase->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);
    ui->tvDatabase->horizontalHeader()->setSectionResizeMode(2, QHeaderView::ResizeToContents);
    ui->tvDatabase->horizontalHeader()->setSectionResizeMode(3, QHeaderView::ResizeToContents);

    connect(ui->leName, &QLineEdit::textChanged, [this](const QString&) { this->validateDialog(); });
}

DatabaseDialog::~DatabaseDialog()
{
    delete ui;
}

void DatabaseDialog::validateDialog()
{
    ui->tbSave->setEnabled(!ui->leName->text().isEmpty() && (this->_databasemodel->rowCount() > 0));
}

void DatabaseDialog::on_tbImport_clicked()
{
    QStringList files = QFileDialog::getOpenFileNames(this, "Import PAT file...", QString(), "PAT file (*.pat)");

    if(files.isEmpty())
        return;

    this->_databasemodel->loadPats(files);
    this->validateDialog();
}

void DatabaseDialog::on_tbSave_clicked()
{
    QString s = QFileDialog::getSaveFileName(this, "Save Signature...", QString(), "REDasm signature (*.rdb)");

    if(s.isEmpty())
        return;

    this->_databasemodel->save(ui->leName->text(), s);
}
