#ifndef TRADING_VIEW_PLOTWIDGET_H
#define TRADING_VIEW_PLOTWIDGET_H

#include <QVector>
#include <QWidget>

#include "../Controller/controller.h"

namespace Ui {
class PlotWidget;
}

class PlotWidget : public QWidget {
  Q_OBJECT

 public:
  explicit PlotWidget(QWidget *parent = nullptr);
  ~PlotWidget();
  void SetController(Controller *controller) { controller_ = controller; };
  void DrawPoints(bool approximation);
  void DrawGraph(const QString name);
  void Reset();
  void SetTickers();
  int CountPlots() const;

 private:
  Ui::PlotWidget *ui;
  Controller *controller_;
  int count_plots_{};
  QVector<QColor> colors_{QColor("black"), QColor("red"),  QColor("orange"),
                          QColor("green"), QColor("blue"), QColor("purple")};
  bool approximation_{true};
};

#endif  // TRADING_VIEW_PLOTWIDGET_H
