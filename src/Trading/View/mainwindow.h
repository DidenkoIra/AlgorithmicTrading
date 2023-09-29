#ifndef TRADING_VIEW_MAINWINDOW_H
#define TRADING_VIEW_MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>

#include "../Controller/controller.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  explicit MainWindow(Controller *controller, QWidget *parent = nullptr);
  ~MainWindow();

 private slots:
  void on_action_open_triggered();
  void on_action_exit_triggered();
  void on_pushButton_approximation_draw_clicked();
  void on_pushButton_approximation_clear_clicked();
  void on_pushButton_interpolation_clear_clicked();
  void on_pushButton_interpolation_spline_draw_clicked();
  void on_pushButton_interpolation_newton_draw_clicked();
  void on_pushButton_approximation_calc_on_date_clicked();
  void on_pushButton_interpolation_calc_on_date_clicked();

 private:
  Ui::MainWindow *ui;
  Controller *controller_;
  int curren_to_day_{0};
  void ResetPlotWidgets();
  void ShowFileInfo(QString filename);
  void SetEnableButtonDrawApproximation();
  void SetEnableButtonDrawInterpolation();
  void PlotApproximationUpdate();
  void PlotInterpolationUpdate();
};
#endif  // TRADING_VIEW_MAINWINDOW_H
