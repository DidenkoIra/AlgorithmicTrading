#include "plotwidget.h"

#include "ui_plotwidget.h"

PlotWidget::PlotWidget(QWidget *parent)
    : QWidget(parent), ui(new Ui::PlotWidget) {
  ui->setupUi(this);
  ui->widget->setInteraction(QCP::iRangeDrag, true);
  ui->widget->setInteraction(QCP::iRangeZoom, true);
  ui->widget->legend->setVisible(true);

  ui->widget->axisRect()->insetLayout()->setInsetPlacement(
      0, QCPLayoutInset::ipBorderAligned);
  ui->widget->axisRect()->insetLayout()->setInsetAlignment(
      0, Qt::AlignLeft | Qt::AlignBottom);
}

PlotWidget::~PlotWidget() { delete ui; }

void PlotWidget::DrawPoints(bool approximation) {
  approximation_ = approximation;
  double max_value;
  if (approximation_) {
    max_value = controller_->GetMaxValueApproximation(true);
  } else {
    max_value = controller_->GetMaxValueInterpolation(true);
  }
  ui->widget->clearGraphs();
  ui->widget->addGraph();
  std::vector<double> std_x = controller_->GetDatesValues();
  std::vector<double> std_y = controller_->GetValues();
  ui->widget->xAxis->setRange(0, std_x.back());
  ui->widget->yAxis->setRange(0, max_value);
  QVector<double> x = QVector<double>(std_x.begin(), std_x.end());
  QVector<double> y = QVector<double>(std_y.begin(), std_y.end());
  SetTickers();
  ui->widget->graph(0)->addData(x, y);
  ui->widget->graph(0)->setName("Данные из файла");
  ui->widget->graph(0)->setPen(QPen(QColor("black")));
  ui->widget->graph(0)->setLineStyle(QCPGraph::lsNone);
  ui->widget->graph(0)->setScatterStyle(
      QCPScatterStyle(QCPScatterStyle::ssDisc, 4));
  ui->widget->replot();
}

void PlotWidget::DrawGraph(const QString name) {
  count_plots_++;
  double max_value;
  if (approximation_) {
    max_value = controller_->GetMaxValueApproximation(false);
  } else {
    max_value = controller_->GetMaxValueInterpolation(false);
  }
  ui->widget->addGraph();
  ui->widget->graph(count_plots_)->setName(name);
  std::vector<double> std_x = controller_->GetPredictDatesValues();
  std::vector<double> std_y = controller_->GetPredictValues();
  ui->widget->xAxis->setRange(0, std_x.back());
  ui->widget->yAxis->setRange(0, max_value);
  QVector<double> x = QVector<double>(std_x.begin(), std_x.end());
  QVector<double> y = QVector<double>(std_y.begin(), std_y.end());
  ui->widget->graph(count_plots_)->addData(x, y);
  QPen pen(colors_[count_plots_]);
  pen.setWidth(2);
  ui->widget->graph(count_plots_)->setPen(pen);
  ui->widget->replot();
}

void PlotWidget::Reset() {
  count_plots_ = 0;
  ui->widget->clearGraphs();
  ui->widget->replot();
}

void PlotWidget::SetTickers() {
  auto dates = controller_->GetDates();
  auto dates_values = controller_->GetDatesValues();
  auto count = dates.size();
  QSharedPointer<QCPAxisTickerText> textTicker(new QCPAxisTickerText);
  ui->widget->xAxis->setTicker(textTicker);
  ui->widget->xAxis->setTickLabelRotation(-90);
  for (auto i = 0; i < count; i++) {
    if (i % 10 == 0) {
      textTicker->addTick(dates_values[i], QString::fromStdString(dates[i]));
    }
  }
}

int PlotWidget::CountPlots() const { return count_plots_; }
