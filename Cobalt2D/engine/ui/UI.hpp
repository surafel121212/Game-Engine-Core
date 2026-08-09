#pragma once

#include "engine/math/Math.hpp"

#include <functional>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace cobalt {

class UIElement {
public:
  explicit UIElement(std::string name = "Element") : name_(std::move(name)) {}
  virtual ~UIElement() = default;
  const std::string& name() const { return name_; }
  void setRect(Rect rect) { rect_ = rect; }
  Rect rect() const { return rect_; }
  void setVisible(bool visible) { visible_ = visible; }
  bool visible() const { return visible_; }
  virtual void update(float) {}
  virtual bool onPointerDown(Vec2 point) { return visible_ && rect_.contains(point); }

private:
  std::string name_;
  Rect rect_{{0.0f, 0.0f}, {100.0f, 40.0f}};
  bool visible_{true};
};

class Button : public UIElement {
public:
  using UIElement::UIElement;
  void setOnClick(std::function<void()> callback) { onClick_ = std::move(callback); }
  bool onPointerDown(Vec2 point) override {
    if (!UIElement::onPointerDown(point)) return false;
    if (onClick_) onClick_();
    return true;
  }

private:
  std::function<void()> onClick_;
};

class UIContainer : public UIElement {
public:
  using UIElement::UIElement;
  template <typename Element, typename... Args>
  Element& add(Args&&... args) {
    auto element = std::make_unique<Element>(std::forward<Args>(args)...);
    Element& reference = *element;
    children_.push_back(std::move(element));
    return reference;
  }
  void update(float deltaSeconds) override {
    for (auto& child : children_) child->update(deltaSeconds);
  }

private:
  std::vector<std::unique_ptr<UIElement>> children_;
};

}  // namespace cobalt