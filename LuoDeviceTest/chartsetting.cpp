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
}

ChartSetting::~ChartSetting()
{
    delete ui;
}

void ChartSetting::SetCurrentChartData( ChartSettingData current_data )
{
    m_chart_data = current_data;
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
