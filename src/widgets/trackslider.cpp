/* This file is part of Clementine.

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "trackslider.h"
#include "ui_trackslider.h"
#include "core/utilities.h"

TrackSlider::TrackSlider(QWidget* parent)
  : QWidget(parent),
    ui_(new Ui_TrackSlider),
    setting_value_(false)
{
  ui_->setupUi(this);

  QFont font("Courier");
  ui_->elapsed->setFont(font);
  ui_->remaining->setFont(font);

  UpdateLabelWidth();

  connect(ui_->slider, SIGNAL(sliderMoved(int)), SIGNAL(ValueChanged(int)));
  connect(ui_->slider, SIGNAL(valueChanged(int)), SLOT(ValueMaybeChanged(int)));
}

TrackSlider::~TrackSlider() {
  delete ui_;
}

void TrackSlider::UpdateLabelWidth() {
  // We set the label's minimum size so it won't resize itself when the user
  // is dragging the slider.
  UpdateLabelWidth(ui_->elapsed, "0:00:00");
  UpdateLabelWidth(ui_->remaining, "-0:00:00");
}

void TrackSlider::UpdateLabelWidth(QLabel* label, const QString& text) {
  QString old_text = label->text();
  label->setText(text);
  label->setMinimumWidth(0);
  int width = label->sizeHint().width();
  label->setText(old_text);

  label->setMinimumWidth(width);
}

QSize TrackSlider::sizeHint() const {
  int width = 500;
  width += ui_->elapsed->sizeHint().width();
  width += ui_->remaining->sizeHint().width();

  int height = qMax(ui_->slider->sizeHint().height(), ui_->elapsed->sizeHint().height());

  return QSize(width, height);
}

void TrackSlider::SetValue(int elapsed, int total) {
  setting_value_ = true; // This is so we don't emit from QAbstractSlider::valueChanged
  ui_->slider->setMaximum(total);
  ui_->slider->setValue(elapsed);
  setting_value_ = false;

  UpdateTimes(elapsed);
}

void TrackSlider::UpdateTimes(int elapsed) {
  ui_->elapsed->setText(Utilities::PrettyTime(elapsed));
  ui_->remaining->setText("-" + Utilities::PrettyTime(ui_->slider->maximum() - elapsed));

  setEnabled(true);
}

void TrackSlider::SetStopped() {
  setEnabled(false);
  ui_->elapsed->setText("0:00:00");
  ui_->remaining->setText("0:00:00");

  setting_value_ = true;
  ui_->slider->setValue(0);
  setting_value_ = false;
}

void TrackSlider::SetCanSeek(bool can_seek) {
  ui_->slider->setEnabled(can_seek);
}

void TrackSlider::ValueMaybeChanged(int value) {
  if (setting_value_)
    return;

  UpdateTimes(value);
  emit ValueChanged(value);
}

bool TrackSlider::event(QEvent* e) {
  switch (e->type()) {
    case QEvent::ApplicationFontChange:
    case QEvent::StyleChange:
      UpdateLabelWidth();
      break;
    default:
      break;
  }
  return false;
}