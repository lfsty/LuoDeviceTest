#include "filtersetting.h"
#include "ui_filtersetting.h"

FilterSetting::FilterSetting(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::FilterSetting)
{
    ui->setupUi(this);

    connect(ui->m_pushbutton_lowpass, &QPushButton::clicked, this, &FilterSetting::sig_Filter_lowpass_enabled);
    connect(ui->m_pushbutton_highpass, &QPushButton::clicked, this, &FilterSetting::sig_Filter_highpass_enabled);
    connect(ui->m_pushbutton_notch, &QPushButton::clicked, this, &FilterSetting::sig_Filter_notch_enabled);
    connect(ui->m_pushbutton_highpass_impen, &QPushButton::clicked, this, &FilterSetting::sig_Filter_highpass_impen_enabled);

    connect(ui->m_pushButton_filter_on, &QPushButton::clicked, this, &FilterSetting::sig_FilterEnabled);


}

FilterSetting::~FilterSetting()
{
    delete ui;
}

void FilterSetting::init()
{
    //初始化参数
    emit sig_FilterEnabled(ui->m_pushButton_filter_on->isChecked());
    emit sig_Filter_lowpass_enabled(ui->m_pushbutton_lowpass->isChecked());
    emit sig_Filter_highpass_enabled(ui->m_pushbutton_highpass->isChecked());
    emit sig_Filter_notch_enabled(ui->m_pushbutton_notch->isChecked());
    emit sig_Filter_highpass_impen_enabled(ui->m_pushbutton_highpass_impen->isChecked());

    on_m_listWidget_lowpass_currentTextChanged(ui->m_listWidget_lowpass->currentItem()->text());
    on_m_listWidget_highpass_currentTextChanged(ui->m_listWidget_highpass->currentItem()->text());
    on_m_listWidget_notch_currentTextChanged(ui->m_listWidget_notch->currentItem()->text());
//    on_m_listWidget_highpass_impen_currentTextChanged(ui->m_listWidget_highpass_impen->currentItem()->text());

}

void FilterSetting::on_m_pushButton_filter_on_clicked(bool checked)
{
    if(checked)
    {
        ui->m_pushButton_filter_on->setText("滤波器：开");
    }
    else
    {
        ui->m_pushButton_filter_on->setText("滤波器：关");
    }
}


void FilterSetting::on_m_listWidget_lowpass_currentTextChanged(const QString& currentText)
{
    // index = 0
    double _fc = currentText.split("Hz")[0].toDouble();
    emit sig_Filter_Changed(0, m_filter_order, QVector<double> {_fc, 0}, FilterType::Lowpass);
}


void FilterSetting::on_m_listWidget_highpass_currentTextChanged(const QString& currentText)
{
    // index = 1
    double _fc = currentText.split("Hz")[0].toDouble();
    emit sig_Filter_Changed(1, m_filter_order, QVector<double> {_fc, 0}, FilterType::Highpass);
}


void FilterSetting::on_m_listWidget_notch_currentTextChanged(const QString& currentText)
{
    // index = 2
    double _fc = currentText.split("Hz")[0].toDouble();
    emit sig_Filter_Changed(2, m_filter_order, QVector<double> {_fc - 0.1, _fc + 0.1}, FilterType::Bandstop);
}


void FilterSetting::on_m_listWidget_highpass_impen_currentTextChanged(const QString& currentText)
{
    // index = 3
    double _fc = currentText.split("Hz")[0].toDouble();
    emit sig_Filter_Changed(3, m_filter_order, QVector<double> {_fc, 0}, FilterType::Highpass);
}

