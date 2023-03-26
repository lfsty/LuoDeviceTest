#include "chartsetting.h"
#include "ui_chartsetting.h"

ChartSetting::ChartSetting( QWidget* parent )
    : QDialog( parent )
    , ui( new Ui::ChartSetting )
{
    ui->setupUi( this );
    setWindowTitle( "绘图设置" );
    ui->m_doubleSpinBox_xrange->setValue( m_chart_data.XRange );
    ui->m_doubleSpinBox_ymax->setValue( m_chart_data.YMax );
    ui->m_doubleSpinBox_ymin->setValue( m_chart_data.YMin );
    ui->m_radioButton_chart_autoadjust->setChecked(m_chart_data.AutoAdjust);
}

ChartSetting::~ChartSetting()
{
    delete ui;
}

void ChartSetting::SetCurrentChartData( ChartSettingData current_data )
{
    m_chart_data = current_data;
    ui->m_doubleSpinBox_xrange->setValue( m_chart_data.XRange );
    ui->m_doubleSpinBox_ymax->setValue( m_chart_data.YMax );
    ui->m_doubleSpinBox_ymin->setValue( m_chart_data.YMin );
    ui->m_radioButton_chart_autoadjust->setChecked(m_chart_data.AutoAdjust);
    on_m_radioButton_chart_autoadjust_clicked(m_chart_data.AutoAdjust);
}

void ChartSetting::SetAllChannelEnabled(bool enable)
{
    m_chart_data.AllChannelEnable = enable;
    if(enable)
    {
        ui->m_doubleSpinBox_ymax->setEnabled(false);
        ui->m_doubleSpinBox_ymin->setEnabled(false);
        ui->m_radioButton_chart_autoadjust->setEnabled(false);
    }
    else
    {
        ui->m_doubleSpinBox_ymax->setEnabled(true);
        ui->m_doubleSpinBox_ymin->setEnabled(true);
        ui->m_radioButton_chart_autoadjust->setEnabled(true);
    }
}

void ChartSetting::on_buttonBox_accepted()
{
    emit SettingChanged( m_chart_data );
}

void ChartSetting::on_buttonBox_rejected()
{

}

void ChartSetting::on_m_doubleSpinBox_ymax_valueChanged( double arg1 )
{
    m_chart_data.YMax = arg1;
}

void ChartSetting::on_m_doubleSpinBox_ymin_valueChanged( double arg1 )
{
    m_chart_data.YMin = arg1;
}

void ChartSetting::on_m_doubleSpinBox_xrange_valueChanged( double arg1 )
{
    m_chart_data.XRange = arg1;
}

void ChartSetting::on_m_radioButton_chart_autoadjust_clicked(bool checked)
{
    if(checked)
    {
        ui->m_doubleSpinBox_ymax->setEnabled(false);
        ui->m_doubleSpinBox_ymin->setEnabled(false);
    }
    else
    {
        if(m_chart_data.AllChannelEnable == false)
        {
            ui->m_doubleSpinBox_ymax->setEnabled(true);
            ui->m_doubleSpinBox_ymin->setEnabled(true);
        }
    }
    m_chart_data.AutoAdjust = checked;
}

