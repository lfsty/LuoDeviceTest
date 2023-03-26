#ifndef FILTERSETTING_H
#define FILTERSETTING_H

#include <QDialog>

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

private slots:

    void on_m_pushButton_filter_on_clicked(bool checked);

private:
    Ui::FilterSetting* ui;

signals:
    void sig_FilterEnabled(bool enable);

    void sig_Filter_lowpass_enabled(bool enable);
    void sig_Filter_highpass_enabled(bool enable);
    void sig_Filter_notch_enabled(bool enable);
    void sig_Filter_highpass_impen_enabled(bool enable);

    void sig_Filter_lowpass_changed(int index);
    void sig_Filter_highpass_changed(int index);
    void sig_Filter_notch_changed(int index);
    void sig_Filter_highpass_impen_changed(int index);

};



#endif // FILTERSETTING_H
