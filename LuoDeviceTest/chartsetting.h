#ifndef CHARTSETTING_H
#define CHARTSETTING_H

#include <QDialog>

struct ChartSettingData
{
    double YMax   = 50;
    double YMin   = -50;
    double XRange = 5;
    bool AutoAdjust = false;
};

namespace Ui
{
    class ChartSetting;
}

class ChartSetting : public QDialog
{
    Q_OBJECT
public:
    explicit ChartSetting( QWidget* parent = nullptr );
    ~ChartSetting();
    void SetCurrentChartData( ChartSettingData current_data );

private:
    ChartSettingData m_chart_data;

signals:
    void SettingChanged( ChartSettingData chart_data );

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_m_doubleSpinBox_ymax_valueChanged( double arg1 );

    void on_m_doubleSpinBox_ymin_valueChanged( double arg1 );

    void on_m_doubleSpinBox_xrange_valueChanged( double arg1 );

    void on_m_radioButton_chart_autoadjust_clicked(bool checked);

private:
    Ui::ChartSetting* ui;
};

#endif // CHARTSETTING_H
