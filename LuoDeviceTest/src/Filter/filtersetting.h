#ifndef FILTERSETTING_H
#define FILTERSETTING_H

#include <QDialog>
#include "DesignButter/filterDesign.h"
#include <QVector>

namespace Ui
{
    class FilterSetting;
}

class FilterSetting : public QDialog
{
    Q_OBJECT

public:
    explicit FilterSetting(QWidget* parent = nullptr);
    ~FilterSetting();
    void init();
private:
    int m_filter_order = 2;

private slots:

    void on_m_pushButton_filter_on_clicked(bool checked);

    void on_m_listWidget_lowpass_currentTextChanged(const QString& currentText);

    void on_m_listWidget_highpass_currentTextChanged(const QString& currentText);

    void on_m_listWidget_notch_currentTextChanged(const QString& currentText);

    void on_m_listWidget_highpass_impen_currentTextChanged(const QString& currentText);

private:
    Ui::FilterSetting* ui;

signals:
    void sig_FilterEnabled(bool enable);

    void sig_Filter_lowpass_enabled(bool enable);
    void sig_Filter_highpass_enabled(bool enable);
    void sig_Filter_notch_enabled(bool enable);
    void sig_Filter_highpass_impen_enabled(bool enable);

    void sig_Filter_Changed(const int index, const int order, const QVector<double> Fc, FilterType type);

};



#endif // FILTERSETTING_H
