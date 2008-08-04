/*
  Copyright 2007 Google Inc.

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.
*/

#include <sys/time.h>

#include <KMessageBox>
#include <QGraphicsProxyWidget>
#include <QGraphicsLinearLayout>
#include <KInputDialog>
#include <ggadget/file_manager_interface.h>
#include <ggadget/gadget_consts.h>
#include <ggadget/logger.h>
#include <ggadget/decorated_view_host.h>
#include <ggadget/options_interface.h>
#include <ggadget/script_context_interface.h>
#include <ggadget/script_runtime_interface.h>
#include <ggadget/script_runtime_manager.h>
#include <ggadget/qt/qt_graphics.h>
#include "plasma_view_host.h"
#include "plasma_view_host_internal.h"

#include <plasma/applet.h>

using namespace ggadget::qt;
namespace ggadget {

void PlasmaViewHost::Private::OnViewMoved(int x, int y) {
  if (type_ == ViewHostInterface::VIEW_HOST_MAIN && !is_popout_)
    info->applet->moveBy(x, y);
}

void PlasmaViewHost::Private::OnOptionViewOK() {
  HandleOptionViewResponse(ViewInterface::OPTIONS_VIEW_FLAG_OK);
}

void PlasmaViewHost::Private::OnOptionViewCancel() {
  HandleOptionViewResponse(ViewInterface::OPTIONS_VIEW_FLAG_CANCEL);
}

PlasmaViewHost::PlasmaViewHost(GadgetInfo *info, ViewHostInterface::Type type, bool popout)
  : d(new Private(info, type, popout)) {
}

PlasmaViewHost::~PlasmaViewHost() {
  delete d;
}

void PlasmaViewHost::Destroy() {
  delete this;
}

void PlasmaViewHost::SetView(ViewInterface *view) {
  if (d->view_ == view) return;
  d->Detach();
  d->view_ = view;
}

void *PlasmaViewHost::GetNativeWidget() const {
  return d->widget_;
}

void PlasmaViewHost::ViewCoordToNativeWidgetCoord(
    double x, double y, double *widget_x, double *widget_y) const {
  double zoom = d->view_->GetGraphics()->GetZoom();
  if (widget_x)
    *widget_x = x * zoom;
  if (widget_y)
    *widget_y = y * zoom;
}

void PlasmaViewHost::NativeWidgetCoordToViewCoord(
    double x, double y, double *view_x, double *view_y) const {
  double zoom = d->view_->GetGraphics()->GetZoom();
  if (zoom == 0) return;
  if (view_x) *view_x = x / zoom;
  if (view_y) *view_y = y / zoom;
}

void PlasmaViewHost::QueueDraw() {
  d->QueueDraw();
}

void PlasmaViewHost::QueueResize() {
  d->QueueResize();
}

void PlasmaViewHost::EnableInputShapeMask(bool enable) {
}

void PlasmaViewHost::SetResizable(ViewInterface::ResizableMode mode) {
  // TODO:
}

void PlasmaViewHost::SetCaption(const char *caption) {
  d->caption_ = QString::fromUtf8(caption);
  if (d->parent_dialog_)
    d->parent_dialog_->setWindowTitle(d->caption_);
}

void PlasmaViewHost::SetShowCaptionAlways(bool always) {
  // TODO:
}

void PlasmaViewHost::SetCursor(int type) {
}

void PlasmaViewHost::SetTooltip(const char *tooltip) {
}

bool PlasmaViewHost::ShowView(bool modal, int flags,
                          Slot1<void, int> *feedback_handler) {
  return d->ShowView(modal, flags, feedback_handler);
}

void PlasmaViewHost::CloseView() {
  d->CloseView();
}

bool PlasmaViewHost::ShowContextMenu(int button) {
  return d->ShowContextMenu(button);
}

void PlasmaViewHost::Alert(const ViewInterface *view, const char *message) {
  KMessageBox::information(NULL,message,
                           view->GetCaption().c_str());
}

bool PlasmaViewHost::Confirm(const ViewInterface *view, const char *message) {
  int ret = KMessageBox::questionYesNo(NULL,
                                       message,
                                       view->GetCaption().c_str() );
  return ret == KMessageBox::Yes;
}

std::string PlasmaViewHost::Prompt(const ViewInterface *view,
                                   const char *message,
                                   const char *default_value) {
  QString s = KInputDialog::getText(view->GetCaption().c_str(),
                                   message);
  return s.toUtf8().data();
}

ViewHostInterface::Type PlasmaViewHost::GetType() const {
  return d->type_;
}

ViewInterface *PlasmaViewHost::GetView() const {
  return d->view_;
}

int PlasmaViewHost::GetDebugMode() const {
  return d->info->view_debug_mode;
}

} // namespace ggadget
#include "plasma_view_host_internal.moc"
