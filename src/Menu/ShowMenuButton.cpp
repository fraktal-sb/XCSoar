// SPDX-License-Identifier: GPL-2.0-or-later
// Copyright The XCSoar Project

#include "ShowMenuButton.hpp"
#include "Renderer/ButtonRenderer.hpp"
#include "ui/canvas/Canvas.hpp"
#include "ui/canvas/Pen.hpp"
#include "Screen/Layout.hpp"
#include "Input/InputEvents.hpp"
#include "util/Macros.hpp"
#include "UISettings.hpp"
#include "Interface.hpp"

#ifdef ENABLE_OPENGL
#include "ui/canvas/opengl/Scope.hpp"
#endif

class ShowMenuButtonRenderer : public ButtonRenderer {
public:
  unsigned GetMinimumButtonWidth() const noexcept override {
    return Layout::GetMinimumControlHeight();
  }

  void DrawButton(Canvas &canvas, const PixelRect &rc,
                  ButtonState state) const noexcept override;
  UISettings settings = CommonInterface::GetUISettings();
};

void
ShowMenuButton::Create(ContainerWindow &parent, const PixelRect &rc,
                       WindowStyle style) noexcept
{
  Button::Create(parent, rc, style,
                 std::make_unique<ShowMenuButtonRenderer>());
}

bool
ShowMenuButton::OnClicked() noexcept
{
  InputEvents::ShowMenu();
  return true;
}

void
ShowMenuButtonRenderer::DrawButton(Canvas &canvas, const PixelRect &rc,
                                   ButtonState state) const noexcept
{
  if (!settings.show_menu_button) return;
  const unsigned pen_width = Layout::ScalePenWidth(2);
  const unsigned padding = Layout::GetTextPadding() + pen_width;

  canvas.Select(Pen(pen_width, COLOR_BLACK));
  canvas.DrawRoundRectangle({rc.left, rc.top, rc.right - 1, rc.bottom - 1},
                            PixelSize{Layout::VptScale(8u)});

  const BulkPixelPoint m[] = {
    BulkPixelPoint(rc.left + padding, rc.bottom - padding),
    BulkPixelPoint(rc.left + padding, rc.top + padding),
    BulkPixelPoint((rc.left + rc.right) / 2, rc.bottom - 2 * padding),
    BulkPixelPoint(rc.right - padding, rc.top + padding),
    BulkPixelPoint(rc.right - padding, rc.bottom - padding),
  };

  canvas.DrawPolyline(m, ARRAY_SIZE(m));

  if (state == ButtonState::PRESSED) {
#ifdef ENABLE_OPENGL
    const ScopeAlphaBlend alpha_blend;
    canvas.DrawFilledRectangle(rc, COLOR_YELLOW.WithAlpha(80));
#else
    canvas.InvertRectangle(rc);
#endif
  }
}
