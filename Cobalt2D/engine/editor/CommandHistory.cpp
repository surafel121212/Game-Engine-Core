#include "engine/editor/CommandHistory.hpp"

namespace cobalt {

void CommandHistory::execute(std::unique_ptr<EditorCommand> command) {
  if (!command) return;
  while (commands_.size() > cursor_) commands_.pop_back();
  command->execute();
  commands_.push_back(std::move(command));
  cursor_ = commands_.size();
}

bool CommandHistory::undo() {
  if (!canUndo()) return false;
  --cursor_;
  commands_[cursor_]->undo();
  return true;
}

bool CommandHistory::redo() {
  if (!canRedo()) return false;
  commands_[cursor_]->execute();
  ++cursor_;
  return true;
}

void CommandHistory::clear() {
  commands_.clear();
  cursor_ = 0;
}

std::string CommandHistory::lastCommand() const {
  return cursor_ == 0 ? std::string{} : commands_[cursor_ - 1]->name();
}

}  // namespace cobalt