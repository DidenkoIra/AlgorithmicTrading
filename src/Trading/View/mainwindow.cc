#include "mainwindow.h"

#include "./ui_mainwindow.h"

MainWindow::MainWindow(Controller *controller, QWidget *parent)
    : controller_(controller), QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  ui->plot_approximation->SetController(controller);
  ui->plot_interpolation->SetController(controller);
  SetEnableButtonDrawApproximation();
  SetEnableButtonDrawInterpolation();
}

MainWindow::~MainWindow() { delete ui; }

void MainWindow::on_action_open_triggered() {
  QString q_filename = QFileDialog::getOpenFileName(this, "Выберите файл:", ".",
                                                    "csv files (*.csv)");
  if (!q_filename.isEmpty()) {
    curren_to_day_ = 0;
    std::string filename = q_filename.toStdString();
    controller_->LoadFile(filename);
    ResetPlotWidgets();
    ShowFileInfo(q_filename);
    ui->plot_approximation->DrawPoints(true);
    PlotApproximationUpdate();
    ui->plot_interpolation->DrawPoints(false);
    PlotInterpolationUpdate();
  }
}

void MainWindow::ResetPlotWidgets() {
  ui->plot_approximation->Reset();
  ui->plot_interpolation->Reset();
}

void MainWindow::ShowFileInfo(QString filename) {
  ui->statusBar->showMessage("Открыт файл: " + filename);
  auto count_point = controller_->GetCountPoint();
  ui->spinBox_count_approximation_points->setMinimum(count_point);
  ui->spinBox_count_approximation_points->setValue(count_point);
  ui->spinBox_count_interpolation_points->setMinimum(count_point);
  ui->spinBox_count_interpolation_points->setValue(count_point);
  auto first_date = controller_->GetFirstDate();
  auto qdate =
      QDateTime::fromString(QString::fromStdString(first_date), "yyyy-MM-d");
  ;
  ui->dateTimeEdit_approximation->setDateTime(qdate);
  ui->dateTimeEdit_approximation->setMinimumDateTime(qdate);
  ui->dateTimeEdit_interpolation->setDateTime(qdate);
  ui->dateTimeEdit_interpolation->setMinimumDateTime(qdate);
  ui->spinBox_approximation_degree->setValue(1);
  ui->spinBox_to_day->setValue(0);
}

void MainWindow::SetEnableButtonDrawApproximation() {
  ui->pushButton_approximation_draw->setEnabled(
      ui->plot_approximation->CountPlots() != 5 &&
      controller_->GetCountPoint() != 0);
}

void MainWindow::SetEnableButtonDrawInterpolation() {
  ui->pushButton_interpolation_spline_draw->setEnabled(
      ui->plot_interpolation->CountPlots() != 5 &&
      controller_->GetCountPoint() != 0);
  ui->pushButton_interpolation_newton_draw->setEnabled(
      ui->plot_interpolation->CountPlots() != 5 &&
      controller_->GetCountPoint() != 0);
}

void MainWindow::PlotApproximationUpdate() {
  ui->plot_approximation->update();
  SetEnableButtonDrawApproximation();
}

void MainWindow::PlotInterpolationUpdate() {
  ui->plot_interpolation->update();
  SetEnableButtonDrawInterpolation();
}

void MainWindow::on_action_exit_triggered() { QApplication::quit(); }

void MainWindow::on_pushButton_approximation_draw_clicked() {
  if (ui->spinBox_to_day->value() != curren_to_day_) {
    curren_to_day_ = ui->spinBox_to_day->value();
    on_pushButton_approximation_clear_clicked();
  }
  controller_->CalcApproximation(
      ui->spinBox_approximation_degree->value(),
      ui->spinBox_count_approximation_points->value(),
      ui->spinBox_to_day->value());
  ui->plot_approximation->DrawGraph(
      "Степень: " + QString::number(ui->spinBox_approximation_degree->value()) +
      ", точек: " +
      QString::number(ui->spinBox_count_approximation_points->value()));
  PlotApproximationUpdate();
}

void MainWindow::on_pushButton_approximation_clear_clicked() {
  ui->plot_approximation->Reset();
  if (controller_->GetCountPoint()) ui->plot_approximation->DrawPoints(true);
  PlotApproximationUpdate();
}

void MainWindow::on_pushButton_interpolation_clear_clicked() {
  ui->plot_interpolation->Reset();
  if (controller_->GetCountPoint()) ui->plot_interpolation->DrawPoints(false);
  PlotInterpolationUpdate();
}

void MainWindow::on_pushButton_interpolation_spline_draw_clicked() {
  controller_->CalcInterpolationSpline(
      ui->spinBox_count_interpolation_points->value());
  ui->plot_interpolation->DrawGraph(
      "Сплайн, точек: " +
      QString::number(ui->spinBox_count_interpolation_points->value()));
  PlotInterpolationUpdate();
}

void MainWindow::on_pushButton_interpolation_newton_draw_clicked() {
  controller_->CalcInterpolationNewton(
      ui->spinBox_interpolation_degree->value(),
      ui->spinBox_count_interpolation_points->value());
  ui->plot_interpolation->DrawGraph(
      "Ньютон, степень: " +
      QString::number(ui->spinBox_interpolation_degree->value()) + ", точек: " +
      QString::number(ui->spinBox_count_interpolation_points->value()));
  PlotInterpolationUpdate();
}

void MainWindow::on_pushButton_approximation_calc_on_date_clicked() {
  if (controller_->GetCountPoint()) {
    auto date_value = ui->dateTimeEdit_approximation->dateTime()
                          .toString("yyyy-MM-d hh:mm")
                          .toStdString();
    QString str;
    str.setNum(controller_->PredictValueOnDateApproximation(
        ui->spinBox_approximation_degree->value(),
        ui->spinBox_count_approximation_points->value(), date_value));
    ui->lineEdit_exchange_rate->setText(str);
  }
}

void MainWindow::on_pushButton_interpolation_calc_on_date_clicked() {
  if (controller_->GetCountPoint()) {
    auto date_value = ui->dateTimeEdit_interpolation->dateTime()
                          .toString("yyyy-MM-d hh:mm")
                          .toStdString();
    QString str;
    str.setNum(controller_->PredictValueOnDateInterpolationSpline(date_value));
    ui->lineEdit_exchange_rate_spline->setText("Сплайн: " + str);
    str.setNum(controller_->PredictValueOnDateInterpolationNewton(
        ui->spinBox_approximation_degree->value(), date_value));
    ui->lineEdit_exchange_rate_newton->setText("Ньютон: " + str);
  }
}
