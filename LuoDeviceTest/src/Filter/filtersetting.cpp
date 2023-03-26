#include "filtersetting.h"
#include "ui_filtersetting.h"

FilterSetting::FilterSetting(QWidget* parent) :
    QDialog(parent),
    ui(new Ui::FilterSetting)
{
    ui->setupUi(this);

    connect(ui->m_listWidget_lowpass, &QListWidget::currentRowChanged, this, &FilterSetting::sig_Filter_lowpass_changed);
    connect(ui->m_listWidget_highpass, &QListWidget::currentRowChanged, this, &FilterSetting::sig_Filter_highpass_changed);
    connect(ui->m_listWidget_notch, &QListWidget::currentRowChanged, this, &FilterSetting::sig_Filter_notch_changed);
    connect(ui->m_listWidget_highpass_impen, &QListWidget::currentRowChanged, this, &FilterSetting::sig_Filter_highpass_impen_changed);

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

    emit sig_Filter_lowpass_changed(ui->m_listWidget_lowpass->currentRow());
    emit sig_Filter_highpass_changed(ui->m_listWidget_highpass->currentRow());
    emit sig_Filter_notch_changed(ui->m_listWidget_notch->currentRow());
    emit sig_Filter_highpass_impen_changed(ui->m_listWidget_highpass_impen->currentRow());
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

